#!./perl -w

BEGIN {
    chdir 't' if -d 't';
    @INC = '../lib';
    require './test.pl';
}

# use strict;

plan tests => 206;

my @comma = ("key", "value");

# The peephole optimiser already knows that it should convert the string in
# $foo{string} into a shared hash key scalar. It might be worth making the
# tokeniser build the LHS of => as a shared hash key scalar too.
# And so there's the possiblility of it going wrong
# And going right on 8 bit but wrong on utf8 keys.
# And really we should also try utf8 literals in {} and => in utf8.t

# Some of these tests are (effectively) duplicated in each.t
my %comma = @comma;
ok (keys %comma == 1, 'keys on comma hash');
ok (values %comma == 1, 'values on comma hash');
# defeat any tokeniser or optimiser cunning
my $key = 'ey';
is ($comma{"k" . $key}, "value", 'is key present? (unoptimised)');
# now with cunning:
is ($comma{key}, "value", 'is key present? (maybe optimised)');
#tokeniser may treat => differently.
my @temp = (key=>undef);
is ($comma{$temp[0]}, "value", 'is key present? (using LHS of =>)');

@temp = %comma;
ok (eq_array (\@comma, \@temp), 'list from comma hash');

@temp = each %comma;
ok (eq_array (\@comma, \@temp), 'first each from comma hash');
@temp = each %comma;
ok (eq_array ([], \@temp), 'last each from comma hash');

my %temp = %comma;

ok (keys %temp == 1, 'keys on copy of comma hash');
ok (values %temp == 1, 'values on copy of comma hash');
is ($temp{'k' . $key}, "value", 'is key present? (unoptimised)');
# now with cunning:
is ($temp{key}, "value", 'is key present? (maybe optimised)');
@temp = (key=>undef);
is ($comma{$temp[0]}, "value", 'is key present? (using LHS of =>)');

@temp = %temp;
ok (eq_array (\@temp, \@temp), 'list from copy of comma hash');

@temp = each %temp;
ok (eq_array (\@temp, \@temp), 'first each from copy of comma hash');
@temp = each %temp;
ok (eq_array ([], \@temp), 'last each from copy of comma hash');

my @arrow = (Key =>"Value");

my %arrow = @arrow;
ok (keys %arrow == 1, 'keys on arrow hash');
ok (values %arrow == 1, 'values on arrow hash');
# defeat any tokeniser or optimiser cunning
$key = 'ey';
is ($arrow{"K" . $key}, "Value", 'is key present? (unoptimised)');
# now with cunning:
is ($arrow{Key}, "Value", 'is key present? (maybe optimised)');
#tokeniser may treat => differently.
@temp = ('Key', undef);
is ($arrow{$temp[0]}, "Value", 'is key present? (using LHS of =>)');

@temp = %arrow;
ok (eq_array (\@arrow, \@temp), 'list from arrow hash');

@temp = each %arrow;
ok (eq_array (\@arrow, \@temp), 'first each from arrow hash');
@temp = each %arrow;
ok (eq_array ([], \@temp), 'last each from arrow hash');

%temp = %arrow;

ok (keys %temp == 1, 'keys on copy of arrow hash');
ok (values %temp == 1, 'values on copy of arrow hash');
is ($temp{'K' . $key}, "Value", 'is key present? (unoptimised)');
# now with cunning:
is ($temp{Key}, "Value", 'is key present? (maybe optimised)');
@temp = ('Key', undef);
is ($arrow{$temp[0]}, "Value", 'is key present? (using LHS of =>)');

@temp = %temp;
ok (eq_array (\@temp, \@temp), 'list from copy of arrow hash');

@temp = each %temp;
ok (eq_array (\@temp, \@temp), 'first each from copy of arrow hash');
@temp = each %temp;
ok (eq_array ([], \@temp), 'last each from copy of arrow hash');

my %direct = ('Camel', 2, 'Dromedary', 1);
my %slow;
$slow{Dromedary} = 1;
$slow{Camel} = 2;

ok (eq_hash (\%slow, \%direct), "direct list assignment to hash");
%direct = (Camel => 2, 'Dromedary' => 1);
ok (eq_hash (\%slow, \%direct), "direct list assignment to hash using =>");

$slow{Llama} = 0; # A llama is not a camel :-)
ok (!eq_hash (\%direct, \%slow), "different hashes should not be equal!");

my (%names, %names_copy);
%names = ('$' => 'Scalar', '@' => 'Array', # Grr '
          '%', 'Hash', '&', 'Code');
%names_copy = %names;
ok (eq_hash (\%names, \%names_copy), "check we can copy our hash");

sub in {
  my %args = @_;
  return eq_hash (\%names, \%args);
}

ok (in (%names), "pass hash into a method");

sub in_method {
  my $self = shift;
  my %args = @_;
  return eq_hash (\%names, \%args);
}

ok (main->in_method (%names), "pass hash into a method");

sub out {
  return %names;
}
%names_copy = out ();

ok (eq_hash (\%names, \%names_copy), "pass hash from a subroutine");

sub out_method {
  my $self = shift;
  return %names;
}
%names_copy = main->out_method ();

ok (eq_hash (\%names, \%names_copy), "pass hash from a method");

sub in_out {
  my %args = @_;
  return %args;
}
%names_copy = in_out (%names);

ok (eq_hash (\%names, \%names_copy), "pass hash to and from a subroutine");

sub in_out_method {
  my $self = shift;
  my %args = @_;
  return %args;
}
%names_copy = main->in_out_method (%names);

ok (eq_hash (\%names, \%names_copy), "pass hash to and from a method");

my %names_copy2 = %names;
ok (eq_hash (\%names, \%names_copy2), "check copy worked");

# This should get ignored.
%names_copy = ('%', 'Associative Array', %names);

ok (eq_hash (\%names, \%names_copy), "duplicates at the start of a list");

# This should not
%names_copy = ('*', 'Typeglob', %names);

$names_copy2{'*'} = 'Typeglob';
ok (eq_hash (\%names_copy, \%names_copy2), "duplicates at the end of a list");

%names_copy = ('%', 'Associative Array', '*', 'Endangered species', %names,
              '*', 'Typeglob',);

ok (eq_hash (\%names_copy, \%names_copy2), "duplicates at both ends");

# And now UTF8

foreach my $chr (60, 200, 600, 6000, 60000) {
  # This little game may set a UTF8 flag internally. Or it may not. :-)
  my ($key, $value) = (chr ($chr) . "\x{ABCD}", "$chr\x{ABCD}");
  chop ($key, $value);
  my @utf8c = ($key, $value);
  my %utf8c = @utf8c;

  ok (keys %utf8c == 1, 'keys on utf8 comma hash');
  ok (values %utf8c == 1, 'values on utf8 comma hash');
  # defeat any tokeniser or optimiser cunning
  is ($utf8c{"" . $key}, $value, 'is key present? (unoptimised)');
  my $tempval = sprintf '$utf8c{"\x{%x}"}', $chr;
  is (eval $tempval, $value, "is key present? (maybe $tempval is optimised)");
  $tempval = sprintf '@temp = ("\x{%x}" => undef)', $chr;
  eval $tempval or die "'$tempval' gave $@";
  is ($utf8c{$temp[0]}, $value, 'is key present? (using LHS of $tempval)');

  @temp = %utf8c;
  ok (eq_array (\@utf8c, \@temp), 'list from utf8 comma hash');

  @temp = each %utf8c;
  ok (eq_array (\@utf8c, \@temp), 'first each from utf8 comma hash');
  @temp = each %utf8c;
  ok (eq_array ([], \@temp), 'last each from utf8 comma hash');

  %temp = %utf8c;

  ok (keys %temp == 1, 'keys on copy of utf8 comma hash');
  ok (values %temp == 1, 'values on copy of utf8 comma hash');
  is ($temp{"" . $key}, $value, 'is key present? (unoptimised)');
  $tempval = sprintf '$temp{"\x{%x}"}', $chr;
  is (eval $tempval, $value, "is key present? (maybe $tempval is optimised)");
  $tempval = sprintf '@temp = ("\x{%x}" => undef)', $chr;
  eval $tempval or die "'$tempval' gave $@";
  is ($temp{$temp[0]}, $value, "is key present? (using LHS of $tempval)");

  @temp = %temp;
  ok (eq_array (\@temp, \@temp), 'list from copy of utf8 comma hash');

  @temp = each %temp;
  ok (eq_array (\@temp, \@temp), 'first each from copy of utf8 comma hash');
  @temp = each %temp;
  ok (eq_array ([], \@temp), 'last each from copy of utf8 comma hash');

  my $assign = sprintf '("\x{%x}" => "%d")', $chr, $chr;
  print "# $assign\n";
  my (@utf8a) = eval $assign;

  my %utf8a = @utf8a;
  ok (keys %utf8a == 1, 'keys on utf8 arrow hash');
  ok (values %utf8a == 1, 'values on utf8 arrow hash');
  # defeat any tokeniser or optimiser cunning
  is ($utf8a{$key . ""}, $value, 'is key present? (unoptimised)');
  $tempval = sprintf '$utf8a{"\x{%x}"}', $chr;
  is (eval $tempval, $value, "is key present? (maybe $tempval is optimised)");
  $tempval = sprintf '@temp = ("\x{%x}" => undef)', $chr;
  eval $tempval or die "'$tempval' gave $@";
  is ($utf8a{$temp[0]}, $value, "is key present? (using LHS of $tempval)");

  @temp = %utf8a;
  ok (eq_array (\@utf8a, \@temp), 'list from utf8 arrow hash');

  @temp = each %utf8a;
  ok (eq_array (\@utf8a, \@temp), 'first each from utf8 arrow hash');
  @temp = each %utf8a;
  ok (eq_array ([], \@temp), 'last each from utf8 arrow hash');

  %temp = %utf8a;

  ok (keys %temp == 1, 'keys on copy of utf8 arrow hash');
  ok (values %temp == 1, 'values on copy of utf8 arrow hash');
  is ($temp{'' . $key}, $value, 'is key present? (unoptimised)');
  $tempval = sprintf '$temp{"\x{%x}"}', $chr;
  is (eval $tempval, $value, "is key present? (maybe $tempval is optimised)");
  $tempval = sprintf '@temp = ("\x{%x}" => undef)', $chr;
  eval $tempval or die "'$tempval' gave $@";
  is ($temp{$temp[0]}, $value, "is key present? (using LHS of $tempval)");

  @temp = %temp;
  ok (eq_array (\@temp, \@temp), 'list from copy of utf8 arrow hash');

  @temp = each %temp;
  ok (eq_array (\@temp, \@temp), 'first each from copy of utf8 arrow hash');
  @temp = each %temp;
  ok (eq_array ([], \@temp), 'last each from copy of utf8 arrow hash');

}


