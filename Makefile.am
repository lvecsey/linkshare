
AM_LDFLAGS=-L$(HOME)/src/libgxkit -L$(prefix)/lib $(PRIMARY_LDFLAGS)

AM_CFLAGS=-O3 -Wall -static -std=c99

bin_PROGRAMS = FastCGI-linkshare linkshare-viewer linkshare-add

FastCGI_linkshare_CFLAGS = $(AM_CFLAGS) -I$(HOME)/src/version-control/git/uriparser/include -I$(HOME)/src/version-control/git/json-c
FastCGI_linkshare_LDFLAGS = -L$(HOME)/src/version-control/git/uriparser/.libs -L$(HOME)/src/version-control/git/json-c/.libs
FastCGI_linkshare_SOURCES = FastCGI-linkshare.c critbit.c mcast_listener.c fill_entries.c json_conv.c fill_sortval.c vote.c
FastCGI_linkshare_LDADD=-luriparser -ljson-c -lfcgi

linkshare_viewer_CFLAGS = $(AM_CFLAGS) -I$(HOME)/src/version-control/git/uriparser/include
linkshare_viewer_LDFLAGS = -L$(HOME)/src/version-control/git/uriparser/.libs
linkshare_viewer_SOURCES = linkshare-viewer.c
linkshare_viewer_LDADD=-luriparser

linkshare_add_CFLAGS = $(AM_CFLAGS) -I$(HOME)/src/version-control/git/uriparser/include
linkshare_add_LDFLAGS = -L$(HOME)/src/version-control/git/uriparser/.libs
linkshare_add_SOURCES = linkshare-add.c replace_str.c
linkshare_add_LDADD=-luriparser
