## Makefile.am for texinfo.
## $Id: Makefile.am,v 1.6 1997/07/20 11:52:25 karl Exp $
## Process this file with automake to produce Makefile.in in all directories.

## Be sure we're using the right version of Automake.
AUTOMAKE_OPTIONS = 1.1p

# Additional files to distribute.
EXTRA_DIST = INTRODUCTION dir-example README-alpha

# All subdirectories.
# Do intl/ and lib/ first since the C programs depend on them.
# Do doc/ last so makeinfo will be built when we get there.
# Others are alphabetical.
SUBDIRS = intl lib emacs info makeinfo po util doc
