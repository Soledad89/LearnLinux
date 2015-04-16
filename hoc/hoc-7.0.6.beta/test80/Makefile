### ====================================================================
### Makefile for hoc tests.
###
### Current target list:
###	all
###	check
###	check-a
###	check-b
###	check-c
###	check-d
###	check-e
###	check-f
###	check-g
###	check-end
###	check-loop
###	check-start
###	clean
###	clean-test
###	distclean
###	install
###	mostlyclean
###	maintainer-clean
###	uninstall
###	Jn.hoc
###	Yn.hoc
###
### [21-Dec-2001]
### ====================================================================

### ====================================================================
### Programs needed by this Makefile:

AWK		= awk

BASENAME	= basename

CMP		= cmp

DATE		= date

### Format string to produce, e.g., "11-Dec-2001"
DATEFLAGS	= '+%d-%h-%Y'

DIFF		= diff

### We want lettercase ignored in the diff checks, so that NaN and nan
### are considered equivalent.  This reduces the number of differences
### reported on some systems.
DIFFFLAGS	= -i

DIFFLINES	= 25

EGREP		= egrep

GREP		= grep

HEAD		= head

HEADFLAGS	= -$(DIFFLINES)

HOC		= env HOCPATH=$(HOCPATH) ../hoc

HOCPATH		= .:..:

LS		= ls

MAPLE		= maple

MV		= mv

### This filter reduces irrelevant differences across platforms from
### differing output representation of Infinity and NaN.  The
### substitutions assume output in okay/*.out from Sun Solaris native
### compilers.  We also include filtering of leading zeros in two
### dates from the strftime tests.  The ... pattern matches ###: some
### older make implementations (e.g., Hewlett-Packard HP-UX 10.01
### /usr/bin/make) misinterpret the first # as a make comment start,
### which causes the command to fail entirely.  Switching to dot to
### match any character is an acceptable subterfuge.
PREDIFF		= $(SED) \
		  -e 's/^... CreationDate:.*/... CreationDate:/' \
		  -e 's/= [Ii][Nn][Ff] *$$/= Infinity/' \
		  -e 's/= nan0x[0-9a-fA-F][0-9a-fA-F]* *$$/= NaN/' \
		  -e 's/= NaN[QS] *$$/= NaN/' \
		  -e 's/= [-+]NaN *$$/= NaN/' \
		  -e 's/= [-+][QS]NaN *$$/= NaN/' \
		  -e '/__GID__/d' \
		  -e '/__HOCRC__/d' \
		  -e '/__MAX_/d' \
		  -e '/__PID__/d' \
		  -e '/__PPID__/d' \
		  -e '/__SYSHOC/d' \
		  -e '/__UID__/d' \
		  -e 's/ 08 /  8 /' \
		  -e 's/ 09 /  9 /' \
		  -e '/[0-9]@/d'

RM		= /bin/rm -f

SED		= sed

SHELL		= /bin/sh

TEST		= test

TOUCH		= touch

TR		= tr

TRUE		= true

WC		= wc

### ====================================================================
### Program options:

### For testing, we suppress loading of all default startup files,
### both to save time, and to ensure a uniform test environment, and
### we turn off input prompts:
HOCFLAGS	= --no-banner --no-help-file --no-readline \
		  --no-site-file --no-user-file --silent

### ====================================================================
### Test file lists (these are named CHECK_xxx, instead of CHECK-xxx,
### because some make implementations export to shells that do not
### accept hyphen in environment variable names):

### NB: ack1.hoc cannot be used as a standalone test, because it needs
### input from both the terminal and the ack1.hoc file.
CHECK_A         = ack.hoc double.hoc fac.hoc fac1.hoc fac2.hoc \
		  fib.hoc fib2.hoc fibsum.hoc fibtest.hoc gamma.hoc

CHECK_B         = author.hoc ceil.hoc class.hoc copysign.hoc \
		  cos-sin.hoc cpulimit.hoc eqne.hoc exp.hoc \
		  exponent.hoc expsig.hoc floor.hoc hypot.hoc \
		  hypot-alt.hoc infinity.hoc Jn.hoc macheps.hoc \
		  nan.hoc news.hoc nextafter.hoc relop.hoc scalb.hoc \
		  significand.hoc sqrt.hoc tan.hoc trunc.hoc \
		  unordered.hoc Yn.hoc zerosign.hoc

### Temporarily suppress Jn.hoc and Yn.hoc during development:
CHECK_B         = ceil.hoc class.hoc copysign.hoc cos-sin.hoc \
		  eqne.hoc exp.hoc exponent.hoc expsig.hoc floor.hoc \
		  hypot.hoc hypot-alt.hoc infinity.hoc macheps.hoc \
		  nan.hoc nextafter.hoc relop.hoc scalb.hoc \
		  significand.hoc sqrt.hoc tan.hoc trunc.hoc \
		  unordered.hoc zerosign.hoc

CHECK_C         = opt-author.hoc opt-help.hoc opt-silent.hoc \
		  opt-version.hoc

CHECK_D         = deep-calls.hoc long-name.hoc long-numtoken.hoc \
		  long-operand.hoc long-string.hoc many-symbols.hoc

CHECK_E         = getenv.hoc hexfp.hoc index.hoc length.hoc \
		  strftime.hoc substr.hoc tolower.hoc toupper.hoc

### Temporarily suppress strftime.hoc until local Solaris O/S upgrades
### are complete:
CHECK_E         = exponent.hoc expsig.hoc getenv.hoc hexfp.hoc \
		  index.hoc length.hoc significand.hoc substr.hoc \
		  tolower.hoc toupper.hoc

CHECK_F         = endinput.hoc load01.hoc load02.hoc load03.hoc \
		  load04.hoc load05.hoc logf01.hoc logf02.hoc \
		  save01.hoc save02.hoc

CHECK_G         = author.hoc break.hoc char.hoc continue.hoc cpulimit.hoc \
		  cryptodemo.hoc cryptotest.hoc eol-cr.hoc eol-crlf.hoc \
		  eol-lf.hoc ftoh-htof.hoc lineno.hoc news.hoc printf.hoc

CHECK_LOOP	= $(CHECK_A)

### ====================================================================
### Additional rules:

.SUFFIXES:	.hoc .out .maple

.maple.out:
	$(MAPLE) $< | \
		$(GREP) -v bytes | \
			$(SED) -e 's/^>/### >/' -e 's/  *[)]/)/g' -e 's/  *"$$/"/g' -e 's/  *$$//g' | \
				$(AWK) '{if (FNR < 6) printf("### "); print }' > $@

### ====================================================================
### Makefile targets:

all:

check:	clean-test check-start check-a check-b check-c check-d check-e \
	check-f check-g check-end
	@exit `$(LS) -1 *.out *.err 2>/dev/null | $(WC) -l`

check-a:
	@echo
	@echo "Check group A (original hoc 6 tests) ..."
	@echo
	@$(MAKE) check-loop CHECK_LOOP="$(CHECK_A)" BEGIN="" END=""

check-b:
	@echo
	@echo "Check group B (numeric hoc-7 tests) ..."
	@echo
	@$(MAKE) check-loop CHECK_LOOP="$(CHECK_B)" BEGIN="../hoc.rc begin.hoc" END="end.hoc"

check-c:
	@echo
	@echo "Check group C (command-line options hoc-7 tests) ..."
	@echo
	@$(MAKE) check-loop CHECK_LOOP="$(CHECK_C)" BEGIN="../hoc.rc begin.hoc" END="end.hoc"

check-d:
	@echo
	@echo "Check group D (internal array size limits hoc-7 tests) ..."
	@echo
	@$(MAKE) check-loop CHECK_LOOP="$(CHECK_D)" BEGIN="../hoc.rc begin.hoc" END="end.hoc"

check-e:
	@echo
	@echo "Check group E (string functions hoc-7 tests) ..."
	@echo
	@$(MAKE) check-loop CHECK_LOOP="$(CHECK_E)" BEGIN="../hoc.rc begin.hoc" END="end.hoc"

check-f:
	@echo
	@echo "Check group F (file utility functions hoc-7 tests) ..."
	@echo
	@$(MAKE) check-loop CHECK_LOOP="$(CHECK_F)" BEGIN="../hoc.rc begin.hoc" END="end.hoc"

check-g:
	@echo
	@echo "Check group G (miscellaneous hoc-7 tests) ..."
	@echo
	@$(MAKE) check-loop CHECK_LOOP="$(CHECK_G)" BEGIN="../hoc.rc begin.hoc" END="end.hoc"

check-end:
	@echo "End tests at   `$(DATE)`"
	@echo ========================================================================
	@echo

### NB: This target is for internal use only by the check-? targets
### above.  We exclude __DATE__, __FILE__[n], __NATIVE_xxx, and
### __TIME__ output from who() to ensure identical results across
### platforms.  Because cmp lacks an ignore-case flag, comparison of
### the *.out files is complex: we have to create lowercase versions
### of each pair, and run cmp on them to decide whether to list the
### mismatch, sigh...
check-loop:
	@-for f in $(CHECK_LOOP) ; \
	do \
		$(RM) core ; \
		b=`$(BASENAME) $$f .hoc` ; \
		echo $$b ; \
		$(HOC) $(HOCFLAGS) `cat $$b.opt 2>/dev/null || $(TRUE)` \
			$(BEGIN) $$f $(END) 2>$$b.err | \
				$(EGREP) -v '^[ 	]*(__DATE__|__FILE__.[0-9][0-9]*.|__NATIVE_|__TIME__)' >$$b.out ; \
		$(CMP) -s $$b.err okay/$$b.err || $(LS) -l $$b.err && \
			$(DIFF) $(DIFFFLAGS) $$b.err okay/$$b.err | $(HEAD) $(HEADFLAGS) ; \
		$(CMP) -s $$b.err okay/$$b.err && $(RM) $$b.err ; \
		$(PREDIFF)      $$b.out | $(TR) A-Z a-z >$$b.out.1 ; \
		$(PREDIFF) okay/$$b.out | $(TR) A-Z a-z >$$b.out.2 ; \
		$(PREDIFF)      $$b.out >$$b.out.3 ; \
		$(PREDIFF) okay/$$b.out >$$b.out.4 ; \
		$(CMP) -s $$b.out.[12] || $(LS) -l $$b.out ; \
		$(DIFF) $(DIFFFLAGS) $$b.out.[34] | $(HEAD) $(HEADFLAGS) ; \
		$(CMP) -s $$b.out.[12] && $(RM) $$b.out ; \
		$(RM) $$b.out.[1234] ; \
		if $(TEST) -f $$b.dat ; \
		then \
			$(PREDIFF)      $$b.dat | $(TR) A-Z a-z >$$b.dat.1 ; \
			$(PREDIFF) okay/$$b.dat | $(TR) A-Z a-z >$$b.dat.2 ; \
			$(PREDIFF)      $$b.dat >$$b.dat.3 ; \
			$(PREDIFF) okay/$$b.dat >$$b.dat.4 ; \
			$(CMP) -s $$b.dat.[12] || $(LS) -l $$b.dat ; \
			$(DIFF) $(DIFFFLAGS) $$b.dat.[34] | $(HEAD) $(HEADFLAGS) ; \
			$(CMP) -s $$b.dat.[12] && $(RM) $$b.dat ; \
			$(RM) $$b.dat.[1234] ; \
		fi ; \
		$(TEST) -f core && $(MV) core $$b.core && echo ERROR: $$b.core dump file created ; \
	done
	@echo

check-start:
	@echo
	@echo ========================================================================
	@echo "The only output should be the names of the tests (any differences shown"
	@echo "are limited to at most" $(DIFFLINES) "lines):"
	@echo ""
	@echo "Caveats: (1) The Jn and Yn Bessel function tests expose serious"
	@echo "             inaccuracies on some platforms:"
	@echo ""
	@echo "                 IBM Power, PowerPC AIX:     Jn: 16 bits, Yn: 11 bits"
	@echo "                 SGI MIPS IRIX 6.5:          Jn: 17 bits, Yn: 10 bits"
	@echo "                 Sun SPARC Solaris 2.7, 2.8: Jn: 13 bits, Yn: 12 bits"
	@echo ""
	@echo "             The Jn and Yn reporting tolerance is thus set high: 9 bits"
	@echo ""
	@echo "         (2) The many-symbols and hexfp tests will show differences for"
	@echo "             subnormal values on systems that lack support for them."
	@echo ""
	@echo "         (3) The strftime test will show differences on many systems,"
	@echo "             either because of lack of internationalization, or because"
	@echo "             of permitted implementation choices for certain format"
	@echo "             items."
	@echo ""
	@echo "         (4) In the unlikely event that your system lacks resource"
	@echo "             limits, instead of the usual 1 second, the cpulimit test in"
	@echo "             group B could take up to 5 minutes, depending on your CPU"
	@echo "             speed."
	@echo ""
	@echo "         (5) The class test will show failures for class(+/-SNaN) on"
	@echo "             systems where only one type of NaN is supported (e.g.,"
	@echo "             Intel IA-32 and MIPS Rnnnn), and for class(+/-subnormal)"
	@echo "             on systems where subnormals are not supported (e.g., MIPS"
	@echo "             Rnnnn)."
	@echo ""
	@echo "         (6) Most IEEE 754 implementations negate without special tests"
	@echo "             for NaN.  MIPS Rnnnn ones raise invalid on negation, so"
	@echo "             -NaN produces NaN; thus, some copysign() tests will fail."
	@echo ""
	@echo "         (7) For builds --with-long-double, there will be differences"
	@echo "             in the number of figures printed on systems where the type"
	@echo "             long double is implemented as double."
	@echo ""
	@echo "Begin tests at `$(DATE)`"

clean:	clean-test
	-$(RM) *.i *.ln *.o *.u *~ \#* a.lst a.out

clean-test:
	@-$(RM) *.core *.dat *.err *.out core

distclean:	mostlyclean

install:

mostlyclean:	clean

maintainer-clean: distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	-$(RM) Jn.hoc Yn.hoc

uninstall:

Jn.hoc:	Jn.out
	echo '### -*-hoc-*-' >$@
	echo '### ====================================================================' >>$@
	echo '### Compute test values of the Bessel function J(n,x), matching against' >>$@
	echo '### high-precision values from Maple.' >>$@
	echo '###' >>$@
	$(GREP) '^#' $? >> $@
	echo '###' >>$@
	echo "### [`$(DATE) $(DATEFLAGS)`]" >> $@
	echo '### ====================================================================' >>$@
	echo '' >>$@
	$(GREP) -v '^#' $? >> $@

Yn.hoc:	Yn.out
	echo '### -*-hoc-*-' >$@
	echo '### ====================================================================' >>$@
	echo '### Compute test values of the Bessel function Y(n,x), matching against' >>$@
	echo '### high-precision values from Maple.' >>$@
	echo '###' >>$@
	$(GREP) '^#' $? >> $@
	echo '###' >>$@
	echo "### [`$(DATE) $(DATEFLAGS)`]" >> $@
	echo '### ====================================================================' >>$@
	echo '' >>$@
	$(GREP) -v '^#' $? >> $@
