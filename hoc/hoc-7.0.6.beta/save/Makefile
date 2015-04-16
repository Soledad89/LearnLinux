### Makefile.  Generated from Makefile.in by configure.
### ====================================================================
### Makefile for the extensible high-order calculator, hoc.
###
### Current target list:
###	all
###	ansi2knr
###	asm
###	bundle
###	check
###	check-a
###	check-b
###	check-c
###	check-d
###	check-e
###	check-f
###	check-g
###	chkctype
###	chkctype.i
###	chkctype.s
###	chkgamma
###	chkgamma.i
###	chkgamma.s
###	chkieee
###	chkieee.i
###	chkieee.s
###	chklocale
###	chklocale.i
###	chklocale.s
###	chkscalb
###	chkscalb.i
###	chkscalb.s
###	chkstore.i
###	chkstore.s
###	chkstrtod.i
###	chkstrtod.s
###	clean
###	clean-dirs
###	clean-dist
###	clean-doc
###	clean-exe
###	clean-html
###	clean-pdf
###	clean-ps
###	clean-test
###	clean-txt
###	code.i
###	code.s
###	code.o
###	config.h
###	config.status
###	configure
###	convert-to-knr
###	dist
###	distclean
###	doc
###	hoc
###	hoc.c
###	hoc.html
###	hoc.i
###	hoc.s
###	hoc.o
###	hoc.pdf
###	hoc.ps
###	hoc.txt
###	ieee754.i
###	ieee754.s
###	ieee754.o
###	init.i
###	init.s
###	init.o
###	install
###	install-el
###	install-exe
###	install-ftp
###	install-hlp
###	install-lib
###	install-man
###	install-mkdirs
###	install-rc
###	install-share
###	install-show
###	io.i
###	io.s
###	io.o
###	maintainer-clean
###	Makefile
###	math.i
###	math.s
###	math.o
###	mostlyclean
###	pr
###	readline.o
###	reconfigure
###	rsync
###	random.i
###	random.s
###	random.o
###	store.i
###	store.s
###	store.o
###	string.i
###	string.s
###	string.o
###	symbol.i
###	symbol.s
###	symbol.o
###	TAGS
###	time.i
###	time.s
###	time.o
###	translations.tmp
###	uninstall
###	uninstall-el
###	uninstall-exe
###	uninstall-ftp
###	uninstall-hlp
###	uninstall-lib
###	uninstall-man
###	uninstall-rc
###	update-save
###	xtab.h
###	ytab.h
###
### [05-Jan-2002]
### ====================================================================

PACKAGE_DATE	= 09-Aug-2002
PACKAGE_NAME	= hoc
PACKAGE_VERSION	= 7.0.6.beta

PROGRAM		= $(PACKAGE_NAME)
VERSION		= $(PACKAGE_VERSION)

prefix		= /usr/local
bindir		= $(prefix)/bin
catdir		= $(prefix)/man/cat$(manext)
emacsdir	= $(prefix)/share/emacs/site-lisp
localedir	= $(shrlibdir)/locale
mandir		= $(prefix)/man/man$(manext)
manext		= 1
shrlibdir	= $(shrlibdirtop)/$(HOCVERSION)
shrlibdirtop	= $(prefix)/share/lib/$(PROGRAM)
srcdir		= .

### ====================================================================
### Compiler names and options:

CC		= CC

CFLAGS		=  -I/usr/local/include $(INCLUDES) $(DEFS) $(OPT) $(XCFLAGS)

DEFS		= -DHAVE_CONFIG_H $(XDEFS)

INCLUDES	= -I$(srcdir) $(XINCLUDES)

### For GNU compilers:				-Wl,-rpath $(prefix)/lib
### For OSF/1 and IRIX native compilers:	-rpath $(prefix)/lib
### For Sun native compilers:			-R$(prefix)/lib
### Portable (if LD_LIBRARY_PATH is set):
LDFLAGS		=  -L/usr/local/lib -R/usr/local/lib $(XLDFLAGS)

### Some systems have -lcurses, others have -lncurses, and most need -lm
LIBS		= -lreadline -lncurses  $(XLIBS)

OPT		= -g $(XOPT)

### The X* variables are for convenient command-line augmentation of
### the defaults:

XCFLAGS		=

### Array sizes that can be changed at compile time by -DMAX_xxx=nnn
### options (e.g., XDEFS="-DMAX_PROG=65536"):
###
###	-----------------------------------------------------------------------
###	Source file	Named constant	Default Description
###	-----------------------------------------------------------------------
###	code.c		MAX_PROG	32767	virtual machine instruction limit
###	-----------------------------------------------------------------------
###
### The default values shown should be sufficiently generous that increases
### should be rarely needed.  If any of these values are changed, failures of
### "make check-d" are expected, since the test files a setup to probe each
### of the above default limits.

XDEFS		=

XINCLUDES	=

XLCLINTDEFS	=

### -shiftsigned is for lclint, -bitwisesigned et al for splint
XLCLINTFLAGS	= -shiftsigned
XLCLINTFLAGS	= -bitwisesigned -shiftnegative -shiftimplementation

XLCLINTINCLUDES	=

XLDFLAGS	=

XLIBS		=

XLINTDEFS	=

XLINTFLAGS	=

XOPT		=

YACC		= yacc

### yacc -d: generate y.tab.h file
YFLAGS		= -d

### ====================================================================
### Other programs needed for this Makefile:

AUTOCONF	= autoconf

AUTOHEADER	= autoheader

AWK		= nawk

CHMOD		= chmod

CMP		= cmp

COL		= col

CP		= rcp -p

DEROFF		= deroff

ETAGS		= etags

FIND		= find

FGREP		= fgrep

GROFF		= groff

GUNZIP		= /usr/local/bin/gunzip

GZIP		= /usr/local/bin/gzip

JAR		= /usr/bin/jar

LCLINT		= /usr/local/bin/splint

### lclint (available at http://lclint.cs.virginia.edu/) is a powerful
### extended lint implementation with a great many options.
###
### In mid February 2002, a renamed replacement, splint, was
### announced.  It can be found at http://www.splint.org/downloads/,
### and there is now a technical paper published about it:
###
###	David Evans and David Larochelle
###	Improving Security Using Extensible Lightweight Static Analysis
###	IEEE Software 19(1), 42--51, January/February 2002
###     CODEN IESOEG, ISSN 0740-7459
###
### The two programs are largely compatible, although splint has
### renamed a few old lclint options.  The companion configure script
### will try to find first splint, then lclint.
###
### The trick with lint programs is to find a set that produce useful
### output, without hiding important complaints in a mass of less
### important messages.  The set below seems to work reasonably for
### the DVI driver family on Sun Solaris 2.7, and is probably usable
### on other systems as well.  Any of these can options be turned back
### on at make time by supplying them with a plus prefix, e.g.,
###
### 	make lclint XLCLINTFLAGS='+varuse'
###
### NB: For use with other programs, these options could be installed in
### $HOME/.lclintrc, one per line (removing the backslashes), and
### optionally commented from sharp (#) to end of line.  The -nof option
### below suppresses the loading of user customization files, so that we
### do not provide any other options than the ones shown here.

LCLINTDEFS	= -DOPEN_MAX=20 \
		  -D__STDC__ \
		  -D__sparc \
		  -D'fileno(f)=0' \
		  $(SYSHOCDEFS) \
		  $(XLCLINTDEFS)

LCLINTFLAGS	= $(LCLINTDEFS) \
		  $(LCLINTINCLUDES) \
		  -boolops \
		  -booltype BOOLEAN \
		  -branchstate \
		  -castfcnptr \
		  -compdef \
		  -compmempass \
		  -dependenttrans \
		  -exitarg \
		  -exportlocal \
		  -fileextensions \
		  -fixedformalarray \
		  -formatconst \
		  -globstate \
		  -ifempty \
		  -immediatetrans \
		  -incondefs \
		  -macrovarprefixexclude \
		  -mayaliasunique \
		  -mustfree \
		  -mutrep \
		  -nof \
		  -nullassign \
		  -nullpass \
		  -nullret \
		  -nullstate \
		  -observertrans \
		  -onlytrans \
		  -paramuse \
		  -predboolint \
		  -predboolothers \
		  -realcompare \
		  -statictrans \
		  -sysunrecog \
		  -temptrans \
		  -type \
		  -unqualifiedtrans \
		  -unreachable \
		  -unsignedcompare \
		  -usedef \
		  -varuse \
		  -warnposix \
		  +voidabstract \
		  $(LCLINTOTHERFLAGS) \
		  $(XLCLINTFLAGS)

LCLINTINCLUDES	= -I$(prefix)/include

### These additional lclint flags may be only temporarily desirable:
LCLINTOTHERFLAGS = \
		  -formattype \
		  -redef

LCLINTSRCS	= $(SRCS)

LINT		= /opt/SUNWspro/bin/lint

LINTDEFS	= -Dinline= $(SYSHOCDEFS) $(XLINTDEFS)

# These values are for Sun Solaris 2.6 lint
LINTFLAGS       = -errchk=%all -errhdr=. -errtags -fd -I. -Ncheck=%all \
		  -Nlevel=4 -p -Xtransition=yes -D__cplusplus=1 \
		  $(LINTDEFS) $(LINTINCLUDES) $(XLINTFLAGS)

LINTFLAGS       = -errchk=%all -errhdr=. -errtags -fd -I. -Ncheck=%all \
		  -Nlevel=4 -p -Xtransition=yes $(LINTDEFS) \
		  $(LINTINCLUDES) $(XLINTFLAGS)

LINTFLAGS	= -bchx -I. $(LINTDEFS) $(LINTINCLUDES) $(XLINTFLAGS)

LINTINCLUDES	= -I$(prefix)/include

LINTSRCS	= $(SRCS)

LN		= ln

LS		= ls

MAN2HTML	= man2html

MAN2HTMLFLAGS	= -grammar-level 3.2

MKDIR		= mkdir 2>/dev/null

MV		= mv

PSTOPDF		= distill

PSTOPDFFLAGS	=

RM		= /usr/bin/rm -f

RMDIR		= rmdir

RSYNC		= rsync

RSYNCFLAGS	= --rsh ssh --links --times --update --ignore-times --cvs-exclude

SED		= sed

### Recent versions of autoconf override setting of SHELL in configure.in, sigh...
SHELL		= /bin/bash
SHELL		= /bin/sh

SORT		= sort

SPELL		= spell

STRIP		= strip

### During development, we don't want debug symbols stripped at
### installation time:
STRIP		= echo strip

TAR		= /usr/local/bin/tar

TEST		= test

TOUCH		= touch

TRUE		= true

UNZIP		= /usr/local/bin/unzip

ZIP		= /usr/local/bin/zip

ZOO		= /usr/local/bin/zoo

### ====================================================================
### File names in this package:

FILELIST	= `$(FIND) $(HOCVERSION) -type f | $(SORT)`

### This applies only to the hoc master archive site, as do
### the install-ftp and uninstall-ftp targets:
FTPDIR		= /u/ftp/pub/hoc

FTPFILES        = index.html INSTALL $(HTMLFILES) $(PDFFILES) \
		  $(PSFILES) $(PSGZFILES) $(SSHFILES) $(TXTFILES) \
		  $(HOCVERSION).jar $(HOCVERSION).tar.gz \
		  $(HOCVERSION).zip $(HOCVERSION).zoo

### Where hoc is found when we are in subdirectory test: the
### -quick option is important: it hides any local customizations
### from the testing!
HOC		= env HOCPATH=$(HOCPATH) ../$(PROGRAM) -quick

HOCPATH		= .:..:

HOCSUFFIX	= 64

### Basename of temporary hoc distribution directory:
HOCVERSION	= $(PROGRAM)-$(VERSION)

### These should be two-letter ISO 3166 country codes, corresponding
### to subdirectories with language-specific help.hoc, hoc.rc, and
### translations.hoc files.
LANGUAGES	= da de en es fr hr pt tr

SUBDIRS		= test32 test64 test80 test128

SUBDIRLOOP	= for f in $(SUBDIRS); do ( cd $$f ; $(MAKE) $@) ; done

### This needs to be one long line, to work around a bug in HP-UX make, sigh...:
SYSHOCDEFS	= -DSYSHOCDIR=\"$(SYSHOCDIR)\" -DSYSHOCHLPBASE=\"$(SYSHOCHLPBASE)\" -DSYSHOCRCBASE=\"$(SYSHOCRCBASE)\" -DSYSHOCXLTBASE=\"$(SYSHOCXLTBASE)\"

### Where the system-wide installed hoc startup files reside.  The
### filenames "help.hoc", "hoc.rc", and "translations.hoc" cannot be
### changed without further adjustments in the install-rc and
### uninstall-rc targets below.  The hoc source code itself does not
### require changes.

SYSHOCDIR	= $(shrlibdir)

SYSHOCHLP	= $(shrlibdir)/$(SYSHOCHLPBASE)

SYSHOCHLPBASE	= help.hoc

SYSHOCRC	= $(shrlibdir)/$(SYSHOCRCBASE)

SYSHOCRCBASE	= hoc.rc

SYSHOCHLPLANGALL = $(localedir)/*/$(SYSHOCHLPBASE)

SYSHOCRCLANGALL	= $(localedir)/*/$(SYSHOCRCBASE)

SYSHOCXLT	= $(shrlibdir)/$(SYSHOCXLTBASE)

SYSHOCXLTBASE	= translations.hoc

SYSHOCXLTLANGALL = $(localedir)/*/$(SYSHOCXLTBASE)

TESTFP		= test64

TMPDIR		= /tmp

### Where the distribution files are built (must be temporary!)
TMPHOCDIR	= $(TMPDIR)/$(HOCVERSION)

### ====================================================================
### Distribution files for this package:

ANSI2KNRFILES	= ansi2knr.1 ansi2knr.c
AWKFILES	= ascii-to-msg.awk find-missing-help.awk
CONFIGFILES     = Makefile.in config.hin configure configure.in \
		  configure.sed
CHKFILES        = chkctype.c chkgamma.c chkieee.c chklocale.c \
		  chkscalb.c chkstore.c chkstrtod.c
CHKEXEFILES	= $(CHKFILES:.c=)
DISTFILES       = $(CONFIGFILES) $(DOCFILES) $(ANSI2KNRFILES) $(AWKFILES) \
		  $(ELFILES) $(HOCFILES) $(HTMLFILES) $(INITFILES) \
		  $(MANFILES) $(CHKFILES) $(PRSRCS) $(PDFFILES) $(PSFILES) \
		  $(SHFILES) $(SOKFILES) $(SSHFILES) $(TXTFILES) index.html
DISTDIRS	= locale save $(SUBDIRS) $(RCSDIRS)
DOCFILES        = BUG-REPORTS ChangeLog COPYRIGHT INSTALL PROBLEMS \
		  PROJECTS README README.NaN README.UTAH SYSBUGS TO-DO
ELFILES		= hoc.el
ELCFILES	= $(ELFILES:.el=.elc)
HOCFILES        = annuity.hoc crypto.hoc decrypt.hoc encrypt.hoc \
		  expm1.hoc fortune.hoc log1pa.hoc log1pb.hoc \
		  log1pc.hoc primes.hoc pushd.hoc require.hoc \
		  show-strftime.hoc sunmath.hoc
HTMLFILES               = $(MANFILES:.1=.html)
INITFILES       = $(SYSHOCHLPBASE) $(SYSHOCRCBASE) translations.hoc
MANFILES	= hoc.1
OBJS		= $(SRCS:.c=.o)
PDFFILES	= $(MANFILES:.1=.pdf)
PSFILES		= $(MANFILES:.1=.ps)
PSGZFILES	= $(MANFILES:.1=.ps.gz)

### NB: This intentionally excludes subdirectory RCS directories,
### because they will be handled by recursive copies of DISTDIRS:
RCSDIRS		= RCS

SHFILES		= check-help.sh find-missing-help.sh
SRCS            = code.c hoc.c ieee754.c init.c io.c match.c math.c random.c \
		  readline.c store.c string.c strlcat.c strlcpy.c symbol.c \
		  time.c
PRSRCS          = code.c hoc.y fp_arch.h fp_t.h hoc.h ieee754.c \
		  init.c io.c match.c math.c random.c readline.c readline.h \
		  stdc.h store.c string.c strlcat.c strlcpy.c symbol.c \
		  time.c
TAGSRCS		= $(PRSRCS)
ASMSRCS		= $(SRCS:.c=.s)
SERFILES	= $(MANFILES:.1=.ser)
SOKFILES	= $(MANFILES:.1=.sok)
SSHDIR		= $(prefix)/share/a2ps/sheets
SSHFILES	= hoc.ssh
TXTFILES	= $(MANFILES:.1=.txt)

### ====================================================================
### Command abbreviations:

MANEDIT		= $(SED) \
			-e s=\@PACKAGE_DATE\@=$(PACKAGE_DATE)=g \
			-e s=\@PACKAGE_NAME\@=$(PACKAGE_NAME)=g \
			-e s=\@PACKAGE_VERSION\@=$(PACKAGE_VERSION)=g \
			-e s=\@SYSHOCDIR\@=$(SYSHOCDIR)=g \
			-e s=\@SYSHOCHLP\@=$(SYSHOCHLP)=g \
			-e s=\@SYSHOCHLPBASE\@=$(SYSHOCHLPBASE)=g \
			-e s=\@SYSHOCRC\@=$(SYSHOCRC)=g \
			-e s=\@SYSHOCRCBASE\@=$(SYSHOCRCBASE)=g \
			-e s=\@SYSHOCXLT\@=$(SYSHOCXLT)=g \
			-e s=\@SYSHOCXLTBASE\@=$(SYSHOCXLTBASE)=g

### ====================================================================
### Additional compilation rules:

.PRECIOUS:	hoc.c

.SUFFIXES:	.ser .s .i .c .pdf .ps .html .txt .1

.c.i:
	$(CC) $(CFLAGS) -E $< > $@

.c.s:
	$(CC) $(CFLAGS) -S $<

.1.html:
	$(MANEDIT) < $*.1 > $*.tmp
	$(MAN2HTML) $(MAN2HTMLFLAGS) $*.tmp
	-$(RM) $*.tmp

.1.pdf:
	$(MANEDIT) < $< | $(GROFF) -Tps -man > $*.ps
	$(PSTOPDF) $(PSTOPDFFLAGS) $*.ps

.1.ps:
	$(MANEDIT) < $< | $(GROFF) -Tps -man > $@

.1.ser:
	$(DEROFF) < $< | \
		$(SPELL) +$*.sok | \
			$(SED) -e "/[0-9.,']/d" | \
				$(SORT) -u >$@

.1.txt:
	$(MANEDIT) < $< | $(GROFF) -Tascii -man | $(COL) -b -x > $@

.ps.pdf:
	$(PSTOPDF) $(PSTOPDFFLAGS) $<

### ====================================================================
### Makefile targets:

all:	hoc

ansi2knr:	ansi2knr.c
	$(CC) $(CFLAGS) -o $@ $?

asm:	$(ASMSRCS)

bundle:
	@bundle $(DISTFILES) $(DISTDIRS)

check check-a check-b check-c check-d check-e check-f check-g:	all clean-test
	(cd $(TESTFP); $(MAKE) -s HOC="$(HOC)" SHELL="$(SHELL)" $@)

chkctype:	chkctype.o
	$(CC) $(CFLAGS) -o $@ chkctype.o $(LDFLAGS) $(LIBS)

chkieee:	chkieee.o chkstore.o
	$(CC) $(CFLAGS) -o $@ chkieee.o chkstore.o $(LDFLAGS) $(LIBS)

chkgamma:	chkgamma.o
	$(CC) $(CFLAGS) -o $@ chkgamma.o $(LDFLAGS) $(LIBS)

chklocale:	chklocale.o
	$(CC) $(CFLAGS) -o $@ chklocale.o $(LDFLAGS) $(LIBS)

chkscalb:	chkscalb.o
	$(CC) $(CFLAGS) -o $@ chkscalb.o $(LDFLAGS) $(LIBS)

chkstrtod:	chkstrtod.o
	$(CC) $(CFLAGS) -o $@ chkstrtod.o $(LDFLAGS) $(LIBS)

clean:
	-$(RM) $(OBJS) *.i *.ln *.o *.s *.ser *.u *~ \#* a.lst a.out core
	-$(SUBDIRLOOP)

clean-dirs:
	-$(RM) *~ ./*/*~

clean-dist:
	-$(RM) -rf $(TMPHOCDIR)
	-$(RM) $(HOCVERSION).jar $(HOCVERSION).tar.gz \
		$(HOCVERSION).zip $(HOCVERSION).zoo
	-$(RM) $(HOCVERSION).jar-lst $(HOCVERSION).tar-lst \
		$(HOCVERSION).zip-lst $(HOCVERSION).zoo-lst

clean-doc:	clean-html clean-pdf clean-ps clean-txt

clean-exe:
	-$(RM) a.out $(CHKEXEFILES) hoc

clean-html:
	-$(RM) $(HTMLFILES)

clean-pdf:
	-$(RM) $(PDFFILES)

clean-ps:
	-$(RM) $(PSFILES)

clean-s:
	-$(RM) $(ASMSRCS)

clean-test:
	-$(SUBDIRLOOP)

clean-txt:
	-$(RM) $(TXTFILES)

clobber:	distclean

convert-to-knr:	ansi2knr hoc.c
	if $(TEST) -d original ; \
	then \
		echo 'Source code has already been converted to old-style K&R C.' ; \
	else \
		$(MKDIR) original ; \
		for f in $(SRCS) ; \
		do \
			$(MV) $$f original/ ; \
			./ansi2knr < original/$$f > $$f ; \
		done ; \
	fi

### Make the archive files outside this directory, to avoid mutual
### inclusion.  zoo creates funny time stamps in the .zoo archive, so we
### touch it to make it current.
dist:	clean-dist clean-dirs clean-test update-save $(DISTFILES)
	$(MKDIR) $(TMPHOCDIR)
	$(CP) $(DISTFILES) $(TMPHOCDIR)
	$(CP) -r $(DISTDIRS) $(TMPHOCDIR)
	(hocsrcdir=$$PWD ; \
		cd $(TMPHOCDIR)/.. ; \
		$(TEST) -f "$(JAR)" && \
			$(JAR) cf $$hocsrcdir/$(HOCVERSION).jar $(HOCVERSION) ; \
		$(TEST) -f "$(TAR)" -a -f $(GZIP) && \
			$(TAR) cf - $(HOCVERSION) | \
				$(GZIP) -9 > $$hocsrcdir/$(HOCVERSION).tar.gz ; \
		$(TEST) -f "$(ZIP)" && \
			$(ZIP) $$hocsrcdir/$(HOCVERSION).zip $(FILELIST) ; \
		$(TEST) -f "$(ZOO)" && \
			$(ZOO) a $$hocsrcdir/$(HOCVERSION).zoo $(FILELIST) )
	$(TOUCH) $(HOCVERSION).zoo
	$(TEST) -f "$(JAR)" && \
		$(JAR) tvf $(HOCVERSION).jar > $(HOCVERSION).jar-lst 2>&1
	$(TEST) -f "$(TAR)" -a -f $(GZIP) && \
		$(GUNZIP) < $(HOCVERSION).tar.gz | $(TAR) tvf - > $(HOCVERSION).tar-lst
	$(TEST) -f "$(UNZIP)" && \
		$(UNZIP) -v  $(HOCVERSION).zip > $(HOCVERSION).zip-lst
	$(TEST) -f "$(ZOO)" && \
		$(ZOO) -list $(HOCVERSION).zoo > $(HOCVERSION).zoo-lst
### During development, do not delete build directory
###	-$(RM) -rf $(TMPHOCDIR)

distclean:	mostlyclean clean-dist clean-exe
	-$(SUBDIRLOOP)
	-$(RM) [xy].tab.[ch] hoc.c translations.tmp
	-$(RM) confdefs.h config.cache config.h config.log config.status
	-$(RM) Makefile

doc:	$(HTMLFILES) $(PDFFILES) $(PSFILES) $(TXTFILES)

foreign:	foreign.c ieee754.o store.o
	$(CC) $(CFLAGS) -DTEST -o $@ foreign.c ieee754.o store.o $(LIBS)
	-$(RM) foreign.o

foreign.lst:	foreign
	./$? > $@

hoc:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o hoc $(LDFLAGS) $(LIBS)

### The UNIX yacc's output header file, y.tab.h, is not a portable
### file name, and yacc implementations on other systems may call it
### ytab.h instead, so we do too.
hoc.c:	hoc.y
	$(YACC) $(YFLAGS) hoc.y
	-$(TEST) -f ytab.c && $(MV) ytab.c $@
	-$(TEST) -f y.tab.c && $(MV) y.tab.c $@
	-$(TEST) -f y.tab.h && $(MV) y.tab.h ytab.h

hoc.html:	hoc.1

hoc.pdf:	hoc.1

hoc.ps:		hoc.1

hoc.ps.gz:	hoc.ps
	$(GZIP) -9 < $? > $@

hoc.txt:	hoc.1

install:	hoc install-mkdirs install-el install-exe install-man \
		install-share install-ssh install-show

install-el:	uninstall-el
	-$(TEST) -d $(emacsdir) && $(CP) $(ELFILES) $(emacsdir)
	-$(TEST) -d $(emacsdir) && (cd $(emacsdir); $(CHMOD) 664 $(ELFILES))

install-exe:	uninstall-exe
	$(CP) hoc $(bindir)/$(PROGRAM)$(HOCSUFFIX)
	$(STRIP) $(bindir)/$(PROGRAM)$(HOCSUFFIX)
	$(CHMOD) 775 $(bindir)/$(PROGRAM)$(HOCSUFFIX)
	$(LN) $(bindir)/$(PROGRAM)$(HOCSUFFIX) $(bindir)/$(PROGRAM)$(HOCSUFFIX)-$(VERSION)
	-$(TEST) "$(HOCSUFFIX)" = "64" && $(LN) $(bindir)/$(PROGRAM)$(HOCSUFFIX) $(bindir)/$(PROGRAM)
	-$(TEST) "$(HOCSUFFIX)" = "64" && $(LN) $(bindir)/$(PROGRAM)$(HOCSUFFIX) $(bindir)/$(PROGRAM)-$(VERSION)

install-ftp:	install-ftp-precheck dist uninstall-ftp $(FTPFILES)
	for f in $(FTPFILES) ; \
	do \
		$(CP) $$f $(FTPDIR)/ ; \
		$(CHMOD) 664 $(FTPDIR)/$$f ; \
	done

install-ftp-precheck:
	@$(TEST) `$(FGREP) -c $(PACKAGE_VERSION) index.html` = "5" || \
		(echo ERROR: You must first update the distribution table in index.html; exit 1)

install-hlp:	uninstall-hlp install-mkdirs
	$(CP) $(SYSHOCHLPBASE) $(shrlibdir)/
	$(CHMOD) 664 $(shrlibdir)/$(SYSHOCHLPBASE)
	$(TEST) -d $(localedir) || $(MKDIR) $(localedir)
	-for d in $(LANGUAGES) ; \
	do  \
		$(MKDIR) $(localedir)/$$d || $(TRUE) ; \
		$(CP) locale/$$d/$(SYSHOCHLPBASE) $(localedir)/$$d/ ; \
		$(CHMOD) 664 $(localedir)/$$d/$(SYSHOCHLPBASE) ; \
	done

install-lib:	uninstall-lib
	$(TEST) -d $(shrlibdir) || $(MKDIR) -p $(shrlibdirtop) || $(MKDIR) $(shrlibdirtop) || $(TRUE)
	$(TEST) -d $(shrlibdir) || $(MKDIR) -p $(shrlibdir) || $(MKDIR) $(shrlibdir) || $(TRUE)
	$(TEST) -d $(shrlibdir) && $(CHMOD) 775 $(shrlibdir)
	$(TEST) -d $(shrlibdir) && $(CP) $(HOCFILES) $(shrlibdir)/
	$(TEST) -d $(shrlibdir) && $(CHMOD) 664 $(shrlibdir)/*.hoc

install-man:	uninstall-man
	$(MANEDIT) < hoc.1 > $(mandir)/$(PROGRAM).$(manext)
	$(CHMOD) 664 $(mandir)/$(PROGRAM).$(manext)

install-mkdirs:
	$(TEST) -d $(shrlibdir) || \
		$(MKDIR) -p $(shrlibdirtop) || $(MKDIR) $(shrlibdirtop) || $(TRUE)
	$(CHMOD) 775 $(shrlibdirtop)
	$(TEST) -d $(shrlibdir) || $(MKDIR) -p $(shrlibdir) || $(MKDIR) $(shrlibdir) || $(TRUE)
	$(TEST) -d $(localedir) || $(MKDIR) -p $(localedir) || $(MKDIR) $(localedir) || $(TRUE)
	$(CHMOD) 775 $(shrlibdir) $(localedir)
	-for d in $(LANGUAGES) ; \
	do \
		$(MKDIR) -p $(localedir)/$$d || $(MKDIR) $(localedir)/$$d || $(TRUE) ; \
	 	$(CHMOD) 775 $(localedir)/$$d ; \
	done

install-rc:	uninstall-rc install-mkdirs
	$(CP) $(SYSHOCRCBASE) $(shrlibdir)/
	$(CHMOD) 664 $(shrlibdir)/$(SYSHOCRCBASE)
	$(TEST) -d $(localedir) || $(MKDIR) $(localedir)
	-for d in $(LANGUAGES) ; \
	do  \
		$(MKDIR) $(localedir)/$$d || $(TRUE) ; \
		$(CP) locale/$$d/$(SYSHOCRCBASE) $(localedir)/$$d/ ; \
		$(CHMOD) 664 $(localedir)/$$d/$(SYSHOCRCBASE) ; \
	done

install-share:	install-hlp install-lib install-rc install-xlt

install-show:
	@echo
	@echo Installed files...
	@-$(TEST) -d $(shrlibdir) && $(LS) -l `$(FIND) $(shrlibdir) -type f | $(SORT)`
	@-$(TEST) -d $(bindir) && $(LS) -l $(bindir)/$(PROGRAM)$(HOCSUFFIX) \
		$(bindir)/$(PROGRAM)$(HOCSUFFIX)-$(VERSION)
	@-$(TEST) -d $(mandir) && $(LS) -l $(mandir)/$(PROGRAM).$(manext)
	@-$(TEST) -d $(emacsdir) && (for f in $(ELFILES); do $(LS) -l $(emacsdir)/$$f ; done)
	@-$(TEST) -f $(SSHDIR)/hoc.ssh && $(LS) -l $(SSHDIR)/hoc.ssh
	@echo

install-ssh:
	$(TEST) -d $(SSHDIR) && $(CP) hoc.ssh $(SSHDIR)/ && $(CHMOD) 664 $(SSHDIR)/hoc.ssh
	$(TEST) -d $(SSHDIR) || \
		echo "a2ps seems not to be installed: skipping installation of hoc.ssh"
	$(TEST) -d $(SSHDIR) && \
		( $(FGREP) -s 'hoc:' $(SSHDIR)/hoc.ssh /dev/null > /dev/null || \
			echo "You may wish to add a line 'hoc: /*.hoc/' to the end of $(SSHDIR)/sheets.map" )

install-xlt:	uninstall-xlt install-mkdirs
	$(CP) $(SYSHOCXLTBASE) $(shrlibdir)/
	$(CHMOD) 664 $(shrlibdir)/$(SYSHOCXLTBASE)
	$(TEST) -d $(localedir) || $(MKDIR) $(localedir)
	-for d in $(LANGUAGES) ; \
	do  \
		$(MKDIR) $(localedir)/$$d || $(TRUE) ; \
		if $(TEST) -f locale/$$d/$(SYSHOCXLTBASE) ; \
		then \
			$(CP) locale/$$d/$(SYSHOCXLTBASE) $(localedir)/$$d/ ; \
			$(CHMOD) 664 $(localedir)/$$d/$(SYSHOCXLTBASE) ; \
		fi ; \
	done

### lclint gives lots of duplicate-definition warnings if it is given
### all of the source files in one pass, so we normally run it once per
### file.  However, to permit cross-module checking, we also provide a
### target to do it in one pass.
lclint:	$(LCLINTSRCS)
	@-for f in $(LCLINTSRCS) ; \
	do \
		echo ==================== $$f ; \
		$(LCLINT) $(LCLINTFLAGS) $$f ; \
	done

lclint-one-pass:	$(LCLINTSRCS)
	$(LCLINT) $(LCLINTFLAGS) $(LCLINTSRCS)

### Although it usually is not necessary to run lint once per file,
### for consistency with the lclint and lclint-one-pass targets, we
### do the same for lint and lint-one-pass:

lint:	$(LINTSRCS)
	@-for f in $(LINTSRCS) ; \
	do \
		echo ==================== $$f ; \
		$(LINT) $(LINTFLAGS) $$f ; \
	done

lint-one-pass:	$(LINTSRCS)
	$(LINT) $(LINTFLAGS) $(LINTSRCS)

maintainer-clean:
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	-$(RM) TAGS
	$(MAKE) clean-html clean-pdf clean-ps clean-txt distclean

mostlyclean: clean
	-$(SUBDIRLOOP)

pr:	$(PRSRCS)
	@prcan $?
	@$(TOUCH) pr

### Private target for keeping home system up-to-date; this assumes an
### identical directory path at home as at the master site!
rsync:
	$(RSYNC) $(RSYNCFLAGS) $(DISTFILES) $(REMOTEHOST):$(PWD)
	$(RSYNC) $(RSYNCFLAGS) -r $(DISTDIRS) $(REMOTEHOST):$(PWD)

spell:	$(SERFILES)

TAGS:	$(TAGSRCS)
	$(ETAGS) $(TAGSRCS)

### This temporary file contains code that can be merged into
### translations.hoc, after discarding strings that do not require
### translation, and after dealing with embedded warning messages.  On
### 20-Dec-2001, this step found 306 strings, but after culling those
### that do not require translation, only 81 remained.  This is not a
### perfect solution to the text extraction problem, because strings
### inside conditional branches that are not selected on this platform
### will be missed.  translations.hoc on that date had 105 strings, so
### up to 25 or so strings can be missed that way.  The translations
### intentionally exclude the --copyright output, because that
### contains legal text that could only be translated to another
### language with agreement from AT&T legal staff.
translations.tmp:	ascii-to-msg.awk $(ASMSRCS)
	$(AWK) -f ascii-to-msg.awk $(ASMSRCS) >$@

uninstall:	uninstall-el uninstall-exe uninstall-man uninstall-share \
		uninstall-ssh

uninstall-el:
	$(TEST) -d $(emacsdir) && (cd $(emacsdir) ; $(RM) $(ELFILES) $(ELCFILES))

uninstall-dirs:
	-$(RMDIR) $(localedir) $(shrlibdir) 2>/dev/null || $(TRUE)
	-$(TEST) -z "`$(LS) $(shrlibdirtop)`" && $(RMDIR) $(shrlibdirtop)

uninstall-exe:
	-$(RM) $(bindir)/$(PROGRAM)$(HOCSUFFIX) $(bindir)/$(PROGRAM)$(HOCSUFFIX)-$(VERSION)
	-$(TEST) "$(HOCSUFFIX)" = "64" && $(RM) $(bindir)/$(PROGRAM) $(bindir)/$(PROGRAM)-$(VERSION)

uninstall-ftp:	$(FTPFILES)
	( cd $(FTPDIR) ; $(RM) $(FTPFILES) )

uninstall-hlp:
	-$(RM) $(SYSHOCHLP) $(SYSHOCHLPLANGALL) || $(TRUE)
	-for d in $(LANGUAGES) ; do $(RMDIR) $(localedir)/$$d/ 2>/dev/null || $(TRUE) ; done

uninstall-lib:
	-(cd $(shrlibdir); $(RM) $(HOCFILES)) || $(TRUE)

uninstall-man:
	-$(RM) $(mandir)/$(PROGRAM).$(manext) || $(TRUE)
	-$(RM) $(catdir)/$(PROGRAM).$(manext) || $(TRUE)

uninstall-rc:
	-$(RM) $(SYSHOCRC) $(SYSHOCRCLANGALL) || $(TRUE)
	-for d in $(LANGUAGES) ; do $(RMDIR) $(localedir)/$$d/ 2>/dev/null || $(TRUE) ; done

uninstall-share: uninstall-hlp uninstall-lib uninstall-rc uninstall-xlt uninstall-dirs

uninstall-ssh:
	-$(RM) $(SSHDIR)/hoc.ssh

uninstall-xlt:
	-$(RM) $(SYSHOCXLT) $(SYSHOCXLTLANGALL) || $(TRUE)
	-for d in $(LANGUAGES) ; \
	do \
		$(RMDIR) $(localedir)/$$d/ 2>/dev/null || $(TRUE) ; \
	done

update-save:	hoc.c xtab.h
	$(RM) save/*
	$(CP) Makefile config.h configure hoc.c ytab.h save/
	$(CHMOD) 444 save/*

### xtab.h is a copy of ytab.h, but with the oldest possible
### timestamp, to avoid unnecessary recompilations:
xtab.h:	ytab.h
	@$(TEST) -f $@ || $(TOUCH) $@
	-$(CMP) -s $@ ytab.h || $(CP) ytab.h $@

ytab.h:	hoc.c

### ====================================================================
### Object file dependencies:

CONFIG_H	= config.h stdc.h

HOC_H		= $(CONFIG_H) fp_arch.h fp_t.h hoc.h

READLINE_H	= fp_t.h readline.h

XTAB_H		= xtab.h

chkieee.o:	chkieee.c stdc.h

chkstore.o:	chkstore.c stdc.h

code.o:		code.c $(HOC_H) $(READLINE_H) xtab.h

hoc.o:		hoc.c $(HOC_H) $(READLINE_H) Makefile
	$(CC) $(CFLAGS) $(SYSHOCDEFS) -c hoc.c

ieee754.o:	ieee754.c $(HOC_H)

init.o:		init.c $(HOC_H) xtab.h

io.o:		io.c $(HOC_H) $(READLINE_H) xtab.h

math.o:		math.c $(HOC_H)

random.o:	random.c $(HOC_H)

readline.o:	readline.c $(CONFIG_H) $(READLINE_H)

store.o:	store.c $(HOC_H)

string.o:	string.c $(HOC_H)

symbol.o:	symbol.c $(HOC_H) xtab.h

time.o:		time.c $(HOC_H)

### ====================================================================
### Preprocessed file dependencies:

chkctype.i:	chkctype.c

chkgamma.i:	chkgamma.c

chkieee.i:	chkieee.c stdc.h

chklocale.i:	chklocale.c

chkscalb.i:	chkscalb.c

chkstore.i:	chkstore.c stdc.h

code.i:		code.c $(HOC_H) $(READLINE_H) xtab.h

hoc.i:		hoc.c $(HOC_H) $(READLINE_H) Makefile
	$(CC) $(CFLAGS) $(SYSHOCDEFS) -E hoc.c >$@

ieee754.i:	ieee754.c $(HOC_H)

init.i:		init.c $(HOC_H) $(READLINE_H) xtab.h

io.i:		io.c $(HOC_H) $(READLINE_H) xtab.h

math.i:		math.c $(HOC_H)

random.i:	random.c $(HOC_H)

readline.i:	readline.c $(CONFIG_H) $(READLINE_H)

store.i:	store.c $(HOC_H)

string.i: 	string.c $(HOC_H)

symbol.i:	symbol.c $(HOC_H) xtab.h

time.i:		time.c $(HOC_H)

### ====================================================================
### Assembly file dependencies:

chkctype.s:	chkctype.c

chkgamma.s:	chkgamma.c

chkieee.s:	chkieee.c stdc.h

chklocale.s:	chklocale.c

chkscalb.s:	chkscalb.c

chkstore.s:	chkstore.c stdc.h

code.s:		code.c $(HOC_H) $(READLINE_H) xtab.h

hoc.s:		hoc.c $(HOC_H) $(READLINE_H) Makefile
	$(CC) $(CFLAGS) $(SYSHOCDEFS) -S hoc.c

ieee754.s:	ieee754.c $(HOC_H)

init.s:		init.c $(HOC_H) $(READLINE_H) xtab.h

io.s:		io.c $(HOC_H) $(READLINE_H) xtab.h

math.s:		math.c $(HOC_H)

random.s:	random.c $(HOC_H)

readline.s:	readline.c $(CONFIG_H) $(READLINE_H)

store.s:	store.c $(HOC_H)

string.s: 	string.c $(HOC_H)

symbol.s:	symbol.c $(HOC_H) xtab.h

time.s:		time.c $(HOC_H)

### ====================================================================
### Additional targets required by GNU Coding standards

Makefile config.h: Makefile.in config.status
	CC="$(CC)" ./config.status

config.status: configure
	if test "$(TESTFP)" = "test128" ; \
	then \
		CC="$(CC)" $(srcdir)/configure --srcdir=$(srcdir) --no-create --config-cache --with-long-double ; \
	elif test "$(TESTFP)" = "test80" ; \
	then \
		CC="$(CC)" $(srcdir)/configure --srcdir=$(srcdir) --no-create --config-cache --with-long-double ; \
	elif test "$(TESTFP)" = "test32" ; \
	then \
		CC="$(CC)" $(srcdir)/configure --srcdir=$(srcdir) --no-create --config-cache --with-float ; \
	else \
		CC="$(CC)" $(srcdir)/configure --srcdir=$(srcdir) --no-create --config-cache ; \
	fi

configure reconfigure: configure.in configure.sed
	-$(RM) config.h
	cd $(srcdir); $(AUTOCONF) ; $(AUTOHEADER)
### Apply needed fixup for C++ and CFLAGS with configure
	$(MV) configure configure.tmp
	$(SED) -f configure.sed <configure.tmp >configure
	-$(RM) configure.tmp
	$(CHMOD) 775 configure
