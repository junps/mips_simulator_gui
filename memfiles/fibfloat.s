	.data
l.23:	 # 30.000000
	0x41f00000
l.22:	 # 2.000000
	0x40000000
l.21:	 # 1.000000
	0x3f800000
	.text
	.globl _min_caml_start
fib.10:
	c.lt.s	0, $f1, $f0
	bt.s	0, fble_else.27
	jr	$ra
fble_else.27:
	sub.s	$f1, $f0, $f1
	sw.s	$f0, 0($sp)
	move	$t8, $ra
	mov.s	$f0, $f1
	sw	$t8, 12($sp)
	addi	$sp, $sp, 16
	jal	fib.10
	addi	$sp, $sp, -16
	lw	$t8, 12($sp)
	move	$ra, $t8
	lw.s	$f2, 0($sp)
	sub.s	$f1, $f2, $f1
	sw.s	$f0, 8($sp)
	move	$t8, $ra
	mov.s	$f0, $f1
	sw	$t8, 20($sp)
	addi	$sp, $sp, 24
	jal	fib.10
	addi	$sp, $sp, -24
	lw	$t8, 20($sp)
	move	$ra, $t8
	lw.s	$f1, 8($sp)
	add.s	$f0, $f1, $f0
	jr	$ra
_min_caml_start:
	move	$t8, $ra
	sw	$t8, 4($sp)
	addi	$sp, $sp, 8
	jal	fib.10
	addi	$sp, $sp, -8
	lw	$t8, 4($sp)
	move	$ra, $t8
	move	$t8, $ra
	sw	$t8, 4($sp)
	addi	$sp, $sp, 8
	addi	$sp, $sp, -8
	lw	$t8, 4($sp)
	move	$ra, $t8
	j	loop
