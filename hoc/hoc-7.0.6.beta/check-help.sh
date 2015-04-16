#! /bin/sh
### ====================================================================
### Scan the hoc help file output for lines longer than the 72-character
### standard.  The output files are named foo.out.LANG.
###
### Usage:
###	./check-help.sh
###
### [01-Jan-2002]
### ====================================================================
for f in `ls -d ??`
do
	./hoc -q help.hoc $f/help.hoc - > foo.out.$f <<EOF
	print "\n\n========================================================================\n\n"
	help_all()
	print "\n\n========================================================================\n\n"
	help_help()
	print "\n\n========================================================================\n\n"
	help_ieee_754()
	print "\n\n========================================================================\n\n"
EOF

	ls -l foo.out.$f

	echo "Check of long lines in $f hoc help text..."
	expand foo.out.$f | awk 'length($0) > 72'
done
