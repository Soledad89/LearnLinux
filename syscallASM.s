	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 10
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	$1, %edi
	movl	$6, %eax
	movl	%eax, %edx
	leaq	-9(%rbp), %rsi
	movl	$0, -4(%rbp)
	movl	l_main.string(%rip), %eax
	movl	%eax, -9(%rbp)
	movb	l_main.string+4(%rip), %cl
	movb	%cl, -5(%rbp)
	callq	_write
	xorl	%edi, %edi
	movq	%rax, -24(%rbp)         ## 8-byte Spill
	movl	%edi, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc

	.section	__TEXT,__const
l_main.string:                          ## @main.string
	.ascii	"HELLO"


.subsections_via_symbols
