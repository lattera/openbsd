package File::Fetch;

use strict;
use FileHandle;
use File::Temp;
use File::Copy;
use File::Spec;
use File::Spec::Unix;
use File::Basename              qw[dirname];

use Cwd                         qw[cwd];
use Carp                        qw[carp];
use IPC::Cmd                    qw[can_run run QUOTE];
use File::Path                  qw[mkpath];
use File::Temp                  qw[tempdir];
use Params::Check               qw[check];
use Module::Load::Conditional   qw[can_load];
use Locale::Maketext::Simple    Style => 'gettext';

use vars    qw[ $VERBOSE $PREFER_BIN $FROM_EMAIL $USER_AGENT
                $BLACKLIST $METHOD_FAIL $VERSION $METHODS
                $FTP_PASSIVE $TIMEOUT $DEBUG $WARN
            ];

$VERSION        = '0.20';
$VERSION        = eval $VERSION;    # avoid warnings with development releases
$PREFER_BIN     = 0;                # XXX TODO implement
$FROM_EMAIL     = 'File-Fetch@example.com';
$USER_AGENT     = "File::Fetch/$VERSION";
$BLACKLIST      = [qw|ftp|];
$METHOD_FAIL    = { };
$FTP_PASSIVE    = 1;
$TIMEOUT        = 0;
$DEBUG          = 0;
$WARN           = 1;

### methods available to fetch the file depending on the scheme
$METHODS = {
    http    => [ qw|lwp wget curl lftp lynx| ],
    ftp     => [ qw|lwp netftp wget curl lftp ncftp ftp| ],
    file    => [ qw|lwp lftp file| ],
    rsync   => [ qw|rsync| ]
};

### silly warnings ###
local $Params::Check::VERBOSE               = 1;
local $Params::Check::VERBOSE               = 1;
local $Module::Load::Conditional::VERBOSE   = 0;
local $Module::Load::Conditional::VERBOSE   = 0;

### see what OS we are on, important for file:// uris ###
use constant ON_WIN     => ($^O eq 'MSWin32');
use constant ON_VMS     => ($^O eq 'VMS');                                
use constant ON_UNIX    => (!ON_WIN);
use constant HAS_VOL    => (ON_WIN);
use constant HAS_SHARE  => (ON_WIN);


=pod

=head1 NAME

File::Fetch - A generic file fetching mechanism

=head1 SYNOPSIS

    use File::Fetch;

    ### build a File::Fetch object ###
    my $ff = File::Fetch->new(uri => 'http://some.where.com/dir/a.txt');

    ### fetch the uri to cwd() ###
    my $where = $ff->fetch() or die $ff->error;

    ### fetch the uri to /tmp ###
    my $where = $ff->fetch( to => '/tmp' );

    ### parsed bits from the uri ###
    $ff->uri;
    $ff->scheme;
    $ff->host;
    $ff->path;
    $ff->file;

=head1 DESCRIPTION

File::Fetch is a generic file fetching mechanism.

It allows you to fetch any file pointed to by a C<ftp>, C<http>,
C<file>, or C<rsync> uri by a number of different means.

See the C<HOW IT WORKS> section further down for details.

=head1 ACCESSORS

A C<File::Fetch> object has the following accessors

=over 4

=item $ff->uri

The uri you passed to the constructor

=item $ff->scheme

The scheme from the uri (like 'file', 'http', etc)

=item $ff->host

The hostname in the uri.  Will be empty if host was originally 
'localhost' for a 'file://' url.

=item $ff->vol

On operating systems with the concept of a volume the second element
of a file:// is considered to the be volume specification for the file.
Thus on Win32 this routine returns the volume, on other operating
systems this returns nothing.

On Windows this value may be empty if the uri is to a network share, in 
which case the 'share' property will be defined. Additionally, volume 
specifications that use '|' as ':' will be converted on read to use ':'.

On VMS, which has a volume concept, this field will be empty because VMS
file specifications are converted to absolute UNIX format and the volume
information is transparently included.

=item $ff->share

On systems with the concept of a network share (currently only Windows) returns 
the sharename from a file://// url.  On other operating systems returns empty.

=item $ff->path

The path from the uri, will be at least a single '/'.

=item $ff->file

The name of the remote file. For the local file name, the
result of $ff->output_file will be used. 

=cut


##########################
### Object & Accessors ###
##########################

{
    ### template for autogenerated accessors ###
    my $Tmpl = {
        scheme          => { default => 'http' },
        host            => { default => 'localhost' },
        path            => { default => '/' },
        file            => { required => 1 },
        uri             => { required => 1 },
        vol             => { default => '' }, # windows for file:// uris
        share           => { default => '' }, # windows for file:// uris
        _error_msg      => { no_override => 1 },
        _error_msg_long => { no_override => 1 },
    };
    
    for my $method ( keys %$Tmpl ) {
        no strict 'refs';
        *$method = sub {
                        my $self = shift;
                        $self->{$method} = $_[0] if @_;
                        return $self->{$method};
                    }
    }
    
    sub _create {
        my $class = shift;
        my %hash  = @_;
        
        my $args = check( $Tmpl, \%hash ) or return;
        
        bless $args, $class;
    
        if( lc($args->scheme) ne 'file' and not $args->host ) {
            return File::Fetch->_error(loc(
                "Hostname required when fetching from '%1'",$args->scheme));
        }
        
        for (qw[path file]) {
            unless( $args->$_() ) { # 5.5.x needs the ()
                return File::Fetch->_error(loc("No '%1' specified",$_));
            }
        }
        
        return $args;
    }    
}

=item $ff->output_file

The name of the output file. This is the same as $ff->file,
but any query parameters are stripped off. For example:

    http://example.com/index.html?x=y

would make the output file be C<index.html> rather than 
C<index.html?x=y>.

=back

=cut

sub output_file {
    my $self = shift;
    my $file = $self->file;
    
    $file =~ s/\?.*$//g;
    
    return $file;
}

### XXX do this or just point to URI::Escape?
# =head2 $esc_uri = $ff->escaped_uri
# 
# =cut
# 
# ### most of this is stolen straight from URI::escape
# {   ### Build a char->hex map
#     my %escapes = map { chr($_) => sprintf("%%%02X", $_) } 0..255;
# 
#     sub escaped_uri {
#         my $self = shift;
#         my $uri  = $self->uri;
# 
#         ### Default unsafe characters.  RFC 2732 ^(uric - reserved)
#         $uri =~ s/([^A-Za-z0-9\-_.!~*'()])/
#                     $escapes{$1} || $self->_fail_hi($1)/ge;
# 
#         return $uri;
#     }
# 
#     sub _fail_hi {
#         my $self = shift;
#         my $char = shift;
#         
#         $self->_error(loc(
#             "Can't escape '%1', try using the '%2' module instead", 
#             sprintf("\\x{%04X}", ord($char)), 'URI::Escape'
#         ));            
#     }
# 
#     sub output_file {
#     
#     }
#     
#     
# }

=head1 METHODS

=head2 $ff = File::Fetch->new( uri => 'http://some.where.com/dir/file.txt' );

Parses the uri and creates a corresponding File::Fetch::Item object,
that is ready to be C<fetch>ed and returns it.

Returns false on failure.

=cut

sub new {
    my $class = shift;
    my %hash  = @_;

    my ($uri);
    my $tmpl = {
        uri => { required => 1, store => \$uri },
    };

    check( $tmpl, \%hash ) or return;

    ### parse the uri to usable parts ###
    my $href    = __PACKAGE__->_parse_uri( $uri ) or return;

    ### make it into a FFI object ###
    my $ff      = File::Fetch->_create( %$href ) or return;


    ### return the object ###
    return $ff;
}

### parses an uri to a hash structure:
###
### $class->_parse_uri( 'ftp://ftp.cpan.org/pub/mirror/index.txt' )
###
### becomes:
###
### $href = {
###     scheme  => 'ftp',
###     host    => 'ftp.cpan.org',
###     path    => '/pub/mirror',
###     file    => 'index.html'
### };
###
### In the case of file:// urls there maybe be additional fields
###
### For systems with volume specifications such as Win32 there will be 
### a volume specifier provided in the 'vol' field.
###
###   'vol' => 'volumename'
###
### For windows file shares there may be a 'share' key specified
###
###   'share' => 'sharename' 
###
### Note that the rules of what a file:// url means vary by the operating system 
### of the host being addressed. Thus file:///d|/foo/bar.txt means the obvious
### 'D:\foo\bar.txt' on windows, but on unix it means '/d|/foo/bar.txt' and 
### not '/foo/bar.txt'
###
### Similarly if the host interpreting the url is VMS then 
### file:///disk$user/my/notes/note12345.txt' means 
### 'DISK$USER:[MY.NOTES]NOTE123456.TXT' but will be returned the same as
### if it is unix where it means /disk$user/my/notes/note12345.txt'.
### Except for some cases in the File::Spec methods, Perl on VMS will generally
### handle UNIX format file specifications.
###
### This means it is impossible to serve certain file:// urls on certain systems.
###
### Thus are the problems with a protocol-less specification. :-(
###

sub _parse_uri {
    my $self = shift;
    my $uri  = shift or return;

    my $href = { uri => $uri };

    ### find the scheme ###
    $uri            =~ s|^(\w+)://||;
    $href->{scheme} = $1;

    ### See rfc 1738 section 3.10
    ### http://www.faqs.org/rfcs/rfc1738.html
    ### And wikipedia for more on windows file:// urls
    ### http://en.wikipedia.org/wiki/File://
    if( $href->{scheme} eq 'file' ) {
        
        my @parts = split '/',$uri;

        ### file://hostname/...
        ### file://hostname/...
        ### normalize file://localhost with file:///
        $href->{host} = $parts[0] || '';

        ### index in @parts where the path components begin;
        my $index = 1;  

        ### file:////hostname/sharename/blah.txt        
        if ( HAS_SHARE and not length $parts[0] and not length $parts[1] ) {
            
            $href->{host}   = $parts[2] || '';  # avoid warnings
            $href->{share}  = $parts[3] || '';  # avoid warnings        

            $index          = 4         # index after the share

        ### file:///D|/blah.txt
        ### file:///D:/blah.txt
        } elsif (HAS_VOL) {
        
            ### this code comes from dmq's patch, but:
            ### XXX if volume is empty, wouldn't that be an error? --kane
            ### if so, our file://localhost test needs to be fixed as wel            
            $href->{vol}    = $parts[1] || '';

            ### correct D| style colume descriptors
            $href->{vol}    =~ s/\A([A-Z])\|\z/$1:/i if ON_WIN;

            $index          = 2;        # index after the volume
        } 

        ### rebuild the path from the leftover parts;
        $href->{path} = join '/', '', splice( @parts, $index, $#parts );

    } else {
        ### using anything but qw() in hash slices may produce warnings 
        ### in older perls :-(
        @{$href}{ qw(host path) } = $uri =~ m|([^/]*)(/.*)$|s;
    }

    ### split the path into file + dir ###
    {   my @parts = File::Spec::Unix->splitpath( delete $href->{path} );
        $href->{path} = $parts[1];
        $href->{file} = $parts[2];
    }

    ### host will be empty if the target was 'localhost' and the 
    ### scheme was 'file'
    $href->{host} = '' if   ($href->{host}      eq 'localhost') and
                            ($href->{scheme}    eq 'file');

    return $href;
}

=head2 $where = $ff->fetch( [to => /my/output/dir/ | \$scalar] )

Fetches the file you requested and returns the full path to the file.

By default it writes to C<cwd()>, but you can override that by specifying 
the C<to> argument:

    ### file fetch to /tmp, full path to the file in $where
    $where = $ff->fetch( to => '/tmp' );

    ### file slurped into $scalar, full path to the file in $where
    ### file is downloaded to a temp directory and cleaned up at exit time
    $where = $ff->fetch( to => \$scalar );

Returns the full path to the downloaded file on success, and false
on failure.

=cut

sub fetch {
    my $self = shift or return;
    my %hash = @_;

    my $target;
    my $tmpl = {
        to  => { default => cwd(), store => \$target },
    };

    check( $tmpl, \%hash ) or return;

    my ($to, $fh);
    ### you want us to slurp the contents
    if( ref $target and UNIVERSAL::isa( $target, 'SCALAR' ) ) {
        $to = tempdir( 'FileFetch.XXXXXX', CLEANUP => 1 );

    ### plain old fetch
    } else {
        $to = $target;

        ### On VMS force to VMS format so File::Spec will work.
        $to = VMS::Filespec::vmspath($to) if ON_VMS;

        ### create the path if it doesn't exist yet ###
        unless( -d $to ) {
            eval { mkpath( $to ) };
    
            return $self->_error(loc("Could not create path '%1'",$to)) if $@;
        }
    }

    ### set passive ftp if required ###
    local $ENV{FTP_PASSIVE} = $FTP_PASSIVE;

    ### we dont use catfile on win32 because if we are using a cygwin tool
    ### under cmd.exe they wont understand windows style separators.
    my $out_to = ON_WIN ? $to.'/'.$self->output_file 
                        : File::Spec->catfile( $to, $self->output_file );
    
    for my $method ( @{ $METHODS->{$self->scheme} } ) {
        my $sub =  '_'.$method.'_fetch';

        unless( __PACKAGE__->can($sub) ) {
            $self->_error(loc("Cannot call method for '%1' -- WEIRD!",
                        $method));
            next;
        }

        ### method is blacklisted ###
        next if grep { lc $_ eq $method } @$BLACKLIST;

        ### method is known to fail ###
        next if $METHOD_FAIL->{$method};

        ### there's serious issues with IPC::Run and quoting of command
        ### line arguments. using quotes in the wrong place breaks things,
        ### and in the case of say, 
        ### C:\cygwin\bin\wget.EXE --quiet --passive-ftp --output-document
        ### "index.html" "http://www.cpan.org/index.html?q=1&y=2"
        ### it doesn't matter how you quote, it always fails.
        local $IPC::Cmd::USE_IPC_RUN = 0;
        
        if( my $file = $self->$sub( 
                        to => $out_to
        )){

            unless( -e $file && -s _ ) {
                $self->_error(loc("'%1' said it fetched '%2', ".
                     "but it was not created",$method,$file));

                ### mark the failure ###
                $METHOD_FAIL->{$method} = 1;

                next;

            } else {

                ### slurp mode?
                if( ref $target and UNIVERSAL::isa( $target, 'SCALAR' ) ) {
                    
                    ### open the file
                    open my $fh, $file or do {
                        $self->_error(
                            loc("Could not open '%1': %2", $file, $!));
                        return;                            
                    };
                    
                    ### slurp
                    $$target = do { local $/; <$fh> };
                
                } 

                my $abs = File::Spec->rel2abs( $file );
                return $abs;

            }
        }
    }


    ### if we got here, we looped over all methods, but we weren't able
    ### to fetch it.
    return;
}

########################
### _*_fetch methods ###
########################

### LWP fetching ###
sub _lwp_fetch {
    my $self = shift;
    my %hash = @_;

    my ($to);
    my $tmpl = {
        to  => { required => 1, store => \$to }
    };
    check( $tmpl, \%hash ) or return;

    ### modules required to download with lwp ###
    my $use_list = {
        LWP                 => '0.0',
        'LWP::UserAgent'    => '0.0',
        'HTTP::Request'     => '0.0',
        'HTTP::Status'      => '0.0',
        URI                 => '0.0',

    };

    if( can_load(modules => $use_list) ) {

        ### setup the uri object
        my $uri = URI->new( File::Spec::Unix->catfile(
                                    $self->path, $self->file
                        ) );

        ### special rules apply for file:// uris ###
        $uri->scheme( $self->scheme );
        $uri->host( $self->scheme eq 'file' ? '' : $self->host );
        $uri->userinfo("anonymous:$FROM_EMAIL") if $self->scheme ne 'file';

        ### set up the useragent object
        my $ua = LWP::UserAgent->new();
        $ua->timeout( $TIMEOUT ) if $TIMEOUT;
        $ua->agent( $USER_AGENT );
        $ua->from( $FROM_EMAIL );
        $ua->env_proxy;

        my $res = $ua->mirror($uri, $to) or return;

        ### uptodate or fetched ok ###
        if ( $res->code == 304 or $res->code == 200 ) {
            return $to;

        } else {
            return $self->_error(loc("Fetch failed! HTTP response: %1 %2 [%3]",
                        $res->code, HTTP::Status::status_message($res->code),
                        $res->status_line));
        }

    } else {
        $METHOD_FAIL->{'lwp'} = 1;
        return;
    }
}

### Net::FTP fetching
sub _netftp_fetch {
    my $self = shift;
    my %hash = @_;

    my ($to);
    my $tmpl = {
        to  => { required => 1, store => \$to }
    };
    check( $tmpl, \%hash ) or return;

    ### required modules ###
    my $use_list = { 'Net::FTP' => 0 };

    if( can_load( modules => $use_list ) ) {

        ### make connection ###
        my $ftp;
        my @options = ($self->host);
        push(@options, Timeout => $TIMEOUT) if $TIMEOUT;
        unless( $ftp = Net::FTP->new( @options ) ) {
            return $self->_error(loc("Ftp creation failed: %1",$@));
        }

        ### login ###
        unless( $ftp->login( anonymous => $FROM_EMAIL ) ) {
            return $self->_error(loc("Could not login to '%1'",$self->host));
        }

        ### set binary mode, just in case ###
        $ftp->binary;

        ### create the remote path 
        ### remember remote paths are unix paths! [#11483]
        my $remote = File::Spec::Unix->catfile( $self->path, $self->file );

        ### fetch the file ###
        my $target;
        unless( $target = $ftp->get( $remote, $to ) ) {
            return $self->_error(loc("Could not fetch '%1' from '%2'",
                        $remote, $self->host));
        }

        ### log out ###
        $ftp->quit;

        return $target;

    } else {
        $METHOD_FAIL->{'netftp'} = 1;
        return;
    }
}

### /bin/wget fetch ###
sub _wget_fetch {
    my $self = shift;
    my %hash = @_;

    my ($to);
    my $tmpl = {
        to  => { required => 1, store => \$to }
    };
    check( $tmpl, \%hash ) or return;

    ### see if we have a wget binary ###
    if( my $wget = can_run('wget') ) {

        ### no verboseness, thanks ###
        my $cmd = [ $wget, '--quiet' ];

        ### if a timeout is set, add it ###
        push(@$cmd, '--timeout=' . $TIMEOUT) if $TIMEOUT;

        ### run passive if specified ###
        push @$cmd, '--passive-ftp' if $FTP_PASSIVE;

        ### set the output document, add the uri ###
        push @$cmd, '--output-document', $to, $self->uri;

        ### with IPC::Cmd > 0.41, this is fixed in teh library,
        ### and there's no need for special casing any more.
        ### DO NOT quote things for IPC::Run, it breaks stuff.
        # $IPC::Cmd::USE_IPC_RUN
        #    ? ($to, $self->uri)
        #    : (QUOTE. $to .QUOTE, QUOTE. $self->uri .QUOTE);

        ### shell out ###
        my $captured;
        unless(run( command => $cmd, 
                    buffer  => \$captured, 
                    verbose => $DEBUG  
        )) {
            ### wget creates the output document always, even if the fetch
            ### fails.. so unlink it in that case
            1 while unlink $to;
            
            return $self->_error(loc( "Command failed: %1", $captured || '' ));
        }

        return $to;

    } else {
        $METHOD_FAIL->{'wget'} = 1;
        return;
    }
}

### /bin/lftp fetch ###
sub _lftp_fetch {
    my $self = shift;
    my %hash = @_;

    my ($to);
    my $tmpl = {
        to  => { required => 1, store => \$to }
    };
    check( $tmpl, \%hash ) or return;

    ### see if we have a wget binary ###
    if( my $lftp = can_run('lftp') ) {

        ### no verboseness, thanks ###
        my $cmd = [ $lftp, '-f' ];

        my $fh = File::Temp->new;
        
        my $str;
        
        ### if a timeout is set, add it ###
        $str .= "set net:timeout $TIMEOUT;\n" if $TIMEOUT;

        ### run passive if specified ###
        $str .= "set ftp:passive-mode 1;\n" if $FTP_PASSIVE;

        ### set the output document, add the uri ###
        ### quote the URI, because lftp supports certain shell
        ### expansions, most notably & for backgrounding.
        ### ' quote does nto work, must be "
        $str .= q[get ']. $self->uri .q[' -o ]. $to . $/;

        if( $DEBUG ) {
            my $pp_str = join ' ', split $/, $str;
            print "# lftp command: $pp_str\n";
        }              

        ### write straight to the file.
        $fh->autoflush(1);
        print $fh $str;

        ### the command needs to be 1 string to be executed
        push @$cmd, $fh->filename;

        ### with IPC::Cmd > 0.41, this is fixed in teh library,
        ### and there's no need for special casing any more.
        ### DO NOT quote things for IPC::Run, it breaks stuff.
        # $IPC::Cmd::USE_IPC_RUN
        #    ? ($to, $self->uri)
        #    : (QUOTE. $to .QUOTE, QUOTE. $self->uri .QUOTE);


        ### shell out ###
        my $captured;
        unless(run( command => $cmd,
                    buffer  => \$captured,
                    verbose => $DEBUG
        )) {
            ### wget creates the output document always, even if the fetch
            ### fails.. so unlink it in that case
            1 while unlink $to;

            return $self->_error(loc( "Command failed: %1", $captured || '' ));
        }

        return $to;

    } else {
        $METHOD_FAIL->{'lftp'} = 1;
        return;
    }
}



### /bin/ftp fetch ###
sub _ftp_fetch {
    my $self = shift;
    my %hash = @_;

    my ($to);
    my $tmpl = {
        to  => { required => 1, store => \$to }
    };
    check( $tmpl, \%hash ) or return;

    ### see if we have a ftp binary ###
    if( my $ftp = can_run('ftp') ) {

        my $fh = FileHandle->new;

        local $SIG{CHLD} = 'IGNORE';

        unless ($fh->open("|$ftp -n")) {
            return $self->_error(loc("%1 creation failed: %2", $ftp, $!));
        }

        my @dialog = (
            "lcd " . dirname($to),
            "open " . $self->host,
            "user anonymous $FROM_EMAIL",
            "cd /",
            "cd " . $self->path,
            "binary",
            "get " . $self->file . " " . $self->output_file,
            "quit",
        );

        foreach (@dialog) { $fh->print($_, "\n") }
        $fh->close or return;

        return $to;
    }
}

### lynx is stupid - it decompresses any .gz file it finds to be text
### use /bin/lynx to fetch files
sub _lynx_fetch {
    my $self = shift;
    my %hash = @_;

    my ($to);
    my $tmpl = {
        to  => { required => 1, store => \$to }
    };
    check( $tmpl, \%hash ) or return;

    ### see if we have a lynx binary ###
    if( my $lynx = can_run('lynx') ) {

        unless( IPC::Cmd->can_capture_buffer ) {
            $METHOD_FAIL->{'lynx'} = 1;

            return $self->_error(loc( 
                "Can not capture buffers. Can not use '%1' to fetch files",
                'lynx' ));
        }            

        ### check if the HTTP resource exists ###
        if ($self->uri =~ /^https?:\/\//i) {
            my $cmd = [
                $lynx,
                '-head',
                '-source',
                "-auth=anonymous:$FROM_EMAIL",
            ];

            push @$cmd, "-connect_timeout=$TIMEOUT" if $TIMEOUT;

            push @$cmd, $self->uri;

            ### shell out ###
            my $head;
            unless(run( command => $cmd,
                        buffer  => \$head,
                        verbose => $DEBUG )
            ) {
                return $self->_error(loc("Command failed: %1", $head || ''));
            }

            unless($head =~ /^HTTP\/\d+\.\d+ 200\b/) {
                return $self->_error(loc("Command failed: %1", $head || ''));
            }
        }

        ### write to the output file ourselves, since lynx ass_u_mes to much
        my $local = FileHandle->new(">$to")
                        or return $self->_error(loc(
                            "Could not open '%1' for writing: %2",$to,$!));

        ### dump to stdout ###
        my $cmd = [
            $lynx,
            '-source',
            "-auth=anonymous:$FROM_EMAIL",
        ];

        push @$cmd, "-connect_timeout=$TIMEOUT" if $TIMEOUT;

        ### DO NOT quote things for IPC::Run, it breaks stuff.
        push @$cmd, $self->uri;
        
        ### with IPC::Cmd > 0.41, this is fixed in teh library,
        ### and there's no need for special casing any more.
        ### DO NOT quote things for IPC::Run, it breaks stuff.
        # $IPC::Cmd::USE_IPC_RUN
        #    ? $self->uri
        #    : QUOTE. $self->uri .QUOTE;


        ### shell out ###
        my $captured;
        unless(run( command => $cmd,
                    buffer  => \$captured,
                    verbose => $DEBUG )
        ) {
            return $self->_error(loc("Command failed: %1", $captured || ''));
        }

        ### print to local file ###
        ### XXX on a 404 with a special error page, $captured will actually
        ### hold the contents of that page, and make it *appear* like the
        ### request was a success, when really it wasn't :(
        ### there doesn't seem to be an option for lynx to change the exit
        ### code based on a 4XX status or so.
        ### the closest we can come is using --error_file and parsing that,
        ### which is very unreliable ;(
        $local->print( $captured );
        $local->close or return;

        return $to;

    } else {
        $METHOD_FAIL->{'lynx'} = 1;
        return;
    }
}

### use /bin/ncftp to fetch files
sub _ncftp_fetch {
    my $self = shift;
    my %hash = @_;

    my ($to);
    my $tmpl = {
        to  => { required => 1, store => \$to }
    };
    check( $tmpl, \%hash ) or return;

    ### we can only set passive mode in interactive sesssions, so bail out
    ### if $FTP_PASSIVE is set
    return if $FTP_PASSIVE;

    ### see if we have a ncftp binary ###
    if( my $ncftp = can_run('ncftp') ) {

        my $cmd = [
            $ncftp,
            '-V',                   # do not be verbose
            '-p', $FROM_EMAIL,      # email as password
            $self->host,            # hostname
            dirname($to),           # local dir for the file
                                    # remote path to the file
            ### DO NOT quote things for IPC::Run, it breaks stuff.
            $IPC::Cmd::USE_IPC_RUN
                        ? File::Spec::Unix->catdir( $self->path, $self->file )
                        : QUOTE. File::Spec::Unix->catdir( 
                                        $self->path, $self->file ) .QUOTE
            
        ];

        ### shell out ###
        my $captured;
        unless(run( command => $cmd,
                    buffer  => \$captured,
                    verbose => $DEBUG )
        ) {
            return $self->_error(loc("Command failed: %1", $captured || ''));
        }

        return $to;

    } else {
        $METHOD_FAIL->{'ncftp'} = 1;
        return;
    }
}

### use /bin/curl to fetch files
sub _curl_fetch {
    my $self = shift;
    my %hash = @_;

    my ($to);
    my $tmpl = {
        to  => { required => 1, store => \$to }
    };
    check( $tmpl, \%hash ) or return;

    if (my $curl = can_run('curl')) {

        ### these long opts are self explanatory - I like that -jmb
	    my $cmd = [ $curl, '-q' ];

	    push(@$cmd, '--connect-timeout', $TIMEOUT) if $TIMEOUT;

	    push(@$cmd, '--silent') unless $DEBUG;

        ### curl does the right thing with passive, regardless ###
    	if ($self->scheme eq 'ftp') {
    		push(@$cmd, '--user', "anonymous:$FROM_EMAIL");
    	}

        ### curl doesn't follow 302 (temporarily moved) etc automatically
        ### so we add --location to enable that.
        push @$cmd, '--fail', '--location', '--output', $to, $self->uri;

        ### with IPC::Cmd > 0.41, this is fixed in teh library,
        ### and there's no need for special casing any more.
        ### DO NOT quote things for IPC::Run, it breaks stuff.
        # $IPC::Cmd::USE_IPC_RUN
        #    ? ($to, $self->uri)
        #    : (QUOTE. $to .QUOTE, QUOTE. $self->uri .QUOTE);


        my $captured;
        unless(run( command => $cmd,
                    buffer  => \$captured,
                    verbose => $DEBUG )
        ) {

            return $self->_error(loc("Command failed: %1", $captured || ''));
        }

        return $to;

    } else {
        $METHOD_FAIL->{'curl'} = 1;
        return;
    }
}


### use File::Copy for fetching file:// urls ###
###
### See section 3.10 of RFC 1738 (http://www.faqs.org/rfcs/rfc1738.html)
### Also see wikipedia on file:// (http://en.wikipedia.org/wiki/File://)
###
    
sub _file_fetch {
    my $self = shift;
    my %hash = @_;

    my ($to);
    my $tmpl = {
        to  => { required => 1, store => \$to }
    };
    check( $tmpl, \%hash ) or return;

    
    
    ### prefix a / on unix systems with a file uri, since it would
    ### look somewhat like this:
    ###     file:///home/kane/file
    ### wheras windows file uris for 'c:\some\dir\file' might look like:
    ###     file:///C:/some/dir/file
    ###     file:///C|/some/dir/file
    ### or for a network share '\\host\share\some\dir\file':
    ###     file:////host/share/some/dir/file
    ###    
    ### VMS file uri's for 'DISK$USER:[MY.NOTES]NOTE123456.TXT' might look like:
    ###     file://vms.host.edu/disk$user/my/notes/note12345.txt
    ###
    
    my $path    = $self->path;
    my $vol     = $self->vol;
    my $share   = $self->share;

    my $remote;
    if (!$share and $self->host) {
        return $self->_error(loc( 
            "Currently %1 cannot handle hosts in %2 urls",
            'File::Fetch', 'file://'
        ));            
    }
    
    if( $vol ) {
        $path   = File::Spec->catdir( split /\//, $path );
        $remote = File::Spec->catpath( $vol, $path, $self->file);

    } elsif( $share ) {
        ### win32 specific, and a share name, so we wont bother with File::Spec
        $path   =~ s|/+|\\|g;
        $remote = "\\\\".$self->host."\\$share\\$path";

    } else {
        ### File::Spec on VMS can not currently handle UNIX syntax.
        my $file_class = ON_VMS
            ? 'File::Spec::Unix'
            : 'File::Spec';

        $remote  = $file_class->catfile( $path, $self->file );
    }

    ### File::Copy is littered with 'die' statements :( ###
    my $rv = eval { File::Copy::copy( $remote, $to ) };

    ### something went wrong ###
    if( !$rv or $@ ) {
        return $self->_error(loc("Could not copy '%1' to '%2': %3 %4",
                             $remote, $to, $!, $@));
    }

    return $to;
}

### use /usr/bin/rsync to fetch files
sub _rsync_fetch {
    my $self = shift;
    my %hash = @_;

    my ($to);
    my $tmpl = {
        to  => { required => 1, store => \$to }
    };
    check( $tmpl, \%hash ) or return;

    if (my $rsync = can_run('rsync')) {

        my $cmd = [ $rsync ];

        ### XXX: rsync has no I/O timeouts at all, by default
        push(@$cmd, '--timeout=' . $TIMEOUT) if $TIMEOUT;

        push(@$cmd, '--quiet') unless $DEBUG;

        ### DO NOT quote things for IPC::Run, it breaks stuff.
        push @$cmd, $self->uri, $to;

        ### with IPC::Cmd > 0.41, this is fixed in teh library,
        ### and there's no need for special casing any more.
        ### DO NOT quote things for IPC::Run, it breaks stuff.
        # $IPC::Cmd::USE_IPC_RUN
        #    ? ($to, $self->uri)
        #    : (QUOTE. $to .QUOTE, QUOTE. $self->uri .QUOTE);

        my $captured;
        unless(run( command => $cmd,
                    buffer  => \$captured,
                    verbose => $DEBUG )
        ) {

            return $self->_error(loc("Command %1 failed: %2", 
                "@$cmd" || '', $captured || ''));
        }

        return $to;

    } else {
        $METHOD_FAIL->{'rsync'} = 1;
        return;
    }
}

#################################
#
# Error code
#
#################################

=pod

=head2 $ff->error([BOOL])

Returns the last encountered error as string.
Pass it a true value to get the C<Carp::longmess()> output instead.

=cut

### error handling the way Archive::Extract does it
sub _error {
    my $self    = shift;
    my $error   = shift;
    
    $self->_error_msg( $error );
    $self->_error_msg_long( Carp::longmess($error) );
    
    if( $WARN ) {
        carp $DEBUG ? $self->_error_msg_long : $self->_error_msg;
    }

    return;
}

sub error {
    my $self = shift;
    return shift() ? $self->_error_msg_long : $self->_error_msg;
}


1;

=pod

=head1 HOW IT WORKS

File::Fetch is able to fetch a variety of uris, by using several
external programs and modules.

Below is a mapping of what utilities will be used in what order
for what schemes, if available:

    file    => LWP, lftp, file
    http    => LWP, wget, curl, lftp, lynx
    ftp     => LWP, Net::FTP, wget, curl, lftp, ncftp, ftp
    rsync   => rsync

If you'd like to disable the use of one or more of these utilities
and/or modules, see the C<$BLACKLIST> variable further down.

If a utility or module isn't available, it will be marked in a cache
(see the C<$METHOD_FAIL> variable further down), so it will not be
tried again. The C<fetch> method will only fail when all options are
exhausted, and it was not able to retrieve the file.

A special note about fetching files from an ftp uri:

By default, all ftp connections are done in passive mode. To change
that, see the C<$FTP_PASSIVE> variable further down.

Furthermore, ftp uris only support anonymous connections, so no
named user/password pair can be passed along.

C</bin/ftp> is blacklisted by default; see the C<$BLACKLIST> variable
further down.

=head1 GLOBAL VARIABLES

The behaviour of File::Fetch can be altered by changing the following
global variables:

=head2 $File::Fetch::FROM_EMAIL

This is the email address that will be sent as your anonymous ftp
password.

Default is C<File-Fetch@example.com>.

=head2 $File::Fetch::USER_AGENT

This is the useragent as C<LWP> will report it.

Default is C<File::Fetch/$VERSION>.

=head2 $File::Fetch::FTP_PASSIVE

This variable controls whether the environment variable C<FTP_PASSIVE>
and any passive switches to commandline tools will be set to true.

Default value is 1.

Note: When $FTP_PASSIVE is true, C<ncftp> will not be used to fetch
files, since passive mode can only be set interactively for this binary

=head2 $File::Fetch::TIMEOUT

When set, controls the network timeout (counted in seconds).

Default value is 0.

=head2 $File::Fetch::WARN

This variable controls whether errors encountered internally by
C<File::Fetch> should be C<carp>'d or not.

Set to false to silence warnings. Inspect the output of the C<error()>
method manually to see what went wrong.

Defaults to C<true>.

=head2 $File::Fetch::DEBUG

This enables debugging output when calling commandline utilities to
fetch files.
This also enables C<Carp::longmess> errors, instead of the regular
C<carp> errors.

Good for tracking down why things don't work with your particular
setup.

Default is 0.

=head2 $File::Fetch::BLACKLIST

This is an array ref holding blacklisted modules/utilities for fetching
files with.

To disallow the use of, for example, C<LWP> and C<Net::FTP>, you could
set $File::Fetch::BLACKLIST to:

    $File::Fetch::BLACKLIST = [qw|lwp netftp|]

The default blacklist is [qw|ftp|], as C</bin/ftp> is rather unreliable.

See the note on C<MAPPING> below.

=head2 $File::Fetch::METHOD_FAIL

This is a hashref registering what modules/utilities were known to fail
for fetching files (mostly because they weren't installed).

You can reset this cache by assigning an empty hashref to it, or
individually remove keys.

See the note on C<MAPPING> below.

=head1 MAPPING


Here's a quick mapping for the utilities/modules, and their names for
the $BLACKLIST, $METHOD_FAIL and other internal functions.

    LWP         => lwp
    Net::FTP    => netftp
    wget        => wget
    lynx        => lynx
    ncftp       => ncftp
    ftp         => ftp
    curl        => curl
    rsync       => rsync
    lftp        => lftp

=head1 FREQUENTLY ASKED QUESTIONS

=head2 So how do I use a proxy with File::Fetch?

C<File::Fetch> currently only supports proxies with LWP::UserAgent.
You will need to set your environment variables accordingly. For
example, to use an ftp proxy:

    $ENV{ftp_proxy} = 'foo.com';

Refer to the LWP::UserAgent manpage for more details.

=head2 I used 'lynx' to fetch a file, but its contents is all wrong!

C<lynx> can only fetch remote files by dumping its contents to C<STDOUT>,
which we in turn capture. If that content is a 'custom' error file
(like, say, a C<404 handler>), you will get that contents instead.

Sadly, C<lynx> doesn't support any options to return a different exit
code on non-C<200 OK> status, giving us no way to tell the difference
between a 'successfull' fetch and a custom error page.

Therefor, we recommend to only use C<lynx> as a last resort. This is 
why it is at the back of our list of methods to try as well.

=head2 Files I'm trying to fetch have reserved characters or non-ASCII characters in them. What do I do?

C<File::Fetch> is relatively smart about things. When trying to write 
a file to disk, it removes the C<query parameters> (see the 
C<output_file> method for details) from the file name before creating
it. In most cases this suffices.

If you have any other characters you need to escape, please install 
the C<URI::Escape> module from CPAN, and pre-encode your URI before
passing it to C<File::Fetch>. You can read about the details of URIs 
and URI encoding here:

  http://www.faqs.org/rfcs/rfc2396.html

=head1 TODO

=over 4

=item Implement $PREFER_BIN

To indicate to rather use commandline tools than modules

=back

=head1 BUG REPORTS

Please report bugs or other issues to E<lt>bug-file-fetch@rt.cpan.org<gt>.

=head1 AUTHOR

This module by Jos Boumans E<lt>kane@cpan.orgE<gt>.

=head1 COPYRIGHT

This library is free software; you may redistribute and/or modify it 
under the same terms as Perl itself.


=cut

# Local variables:
# c-indentation-style: bsd
# c-basic-offset: 4
# indent-tabs-mode: nil
# End:
# vim: expandtab shiftwidth=4:




