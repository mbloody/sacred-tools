# Makefile

CC  := gcc -Wall
CXX := g++ -Wall
SED := sed
CFLAGS := -O2
LDFLAGS:=

SVNVERSION := $(shell LC_ALL=C svnversion -cn . 2>/dev/null | sed -e "s/.*://" -e "s/\([0-9]*\).*/\1/" | grep "[0-9]" || LC_ALL=C svn info . 2>/dev/null | awk '/^Revision:/ {print $$2 }' | grep "[0-9]" || LC_ALL=C git svn info . 2>/dev/null | awk '/^Revision:/ {print $$2 }' | grep "[0-9]" || echo unknown)
SVNDEV := -D'SVN_REV="$(SVNVERSION)"'

PROGS:= sndpack sndunpack

CLEANS = $(PROGS) *.o
COMPILE = $(CC) $(CFLAGS) $(SVNDEV)
LINK = $(CC) $(LDFLAGS)

.PHONY: all
all: $(PROGS)

.PHONY: clean
clean:
	@-$(RM) $(CLEANS) 2>/dev/null

%.o: ./%.c $(MKDEPS) $(INCFILES)
	$(COMPILE) -c $<

sndunpack: sndunpack.o
	$(COMPILE) -o $@ $<

sndpack: sndpack.o
	$(COMPILE) -o $@ $<

