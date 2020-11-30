	.data
l.102:	 # 0.000000
	0x00000000
l.101:	 # 1.000000
	0x3f800000
l.100:	 # 1.500000
	0x3fc00000
l.99:	 # 400.000000
	0x43c80000
l.98:	 # 4.000000
	0x40800000
	.text
	.globl _min_caml_start
dbl.38:
	add.s	$f0, $f0, $f0
	jr	$ra
iloop.54:
	li	$t8, 0
	sub	$t9, $v0, $t8
	bne	$t9, $zero, beq_else.117
	li	$v0, 1
	j	min_caml_print_int
beq_else.117:
	sub.s	$f2, $f2, $f3
	add.s	$f2, $f2, $f4
	sw.s	$f4, 0($sp)
	sw	$v0, 8($sp)
	sw.s	$f2, 16($sp)
	sw.s	$f5, 24($sp)
	sw.s	$f1, 32($sp)
	move	$t8, $ra
	sw	$t8, 44($sp)
	addi	$sp, $sp, 48
	jal	dbl.38
	addi	$sp, $sp, -48
	lw	$t8, 44($sp)
	move	$ra, $t8
	lw.s	$f1, 32($sp)
	mul.s	$f0, $f0, $f1
	lw.s	$f5, 24($sp)
	add.s	$f1, $f0, $f5
	lw.s	$f0, 16($sp)
	mul.s	$f2, $f0, $f0
	mul.s	$f3, $f1, $f1
	add.s	$f4, $f2, $f3
	c.lt.s	0, $f6, $f4
	bt.s	0, fble_else.119
	lw	$v0, 8($sp)
	addi	$v0, $v0, -1
	lw.s	$f4, 0($sp)
	j	iloop.54
fble_else.119:
	li	$v0, 0
	j	min_caml_print_int
xloop.44:
	li	$t8, 400
	slt	$t9, $v0, $t8
	bne	$t9, $zero, ble_else.120
	jr	$ra
ble_else.120:
	sw	$v0, 0($sp)
	sw	$v1, 4($sp)
	move	$t8, $ra
	sw	$t8, 12($sp)
	addi	$sp, $sp, 16
	jal	min_caml_float_of_int
	addi	$sp, $sp, -16
	lw	$t8, 12($sp)
	move	$ra, $t8
	move	$t8, $ra
	sw	$t8, 12($sp)
	addi	$sp, $sp, 16
	jal	dbl.38
	addi	$sp, $sp, -16
	lw	$t8, 12($sp)
	move	$ra, $t8
	div.s	$f0, $f0, $f1
	sub.s	$f0, $f0, $f1
	lw	$v0, 4($sp)
	sw.s	$f0, 8($sp)
	move	$t8, $ra
	sw	$t8, 20($sp)
	addi	$sp, $sp, 24
	jal	min_caml_float_of_int
	addi	$sp, $sp, -24
	lw	$t8, 20($sp)
	move	$ra, $t8
	move	$t8, $ra
	sw	$t8, 20($sp)
	addi	$sp, $sp, 24
	jal	dbl.38
	addi	$sp, $sp, -24
	lw	$t8, 20($sp)
	move	$ra, $t8
	div.s	$f0, $f0, $f1
	sub.s	$f5, $f0, $f1
	li	$v0, 1000
	lw.s	$f4, 8($sp)
	move	$t8, $ra
	sw	$t8, 20($sp)
	addi	$sp, $sp, 24
	jal	iloop.54
	addi	$sp, $sp, -24
	lw	$t8, 20($sp)
	move	$ra, $t8
	lw	$v0, 0($sp)
	addi	$v0, $v0, 1
	lw	$v1, 4($sp)
	j	xloop.44
yloop.40:
	li	$t8, 400
	slt	$t9, $v0, $t8
	bne	$t9, $zero, ble_else.122
	jr	$ra
ble_else.122:
	li	$v1, 0
	sw	$v0, 0($sp)
	move	$t8, $ra
	move	$s6, $v1
	move	$v1, $v0
	move	$v0, $s6
	sw	$t8, 4($sp)
	addi	$sp, $sp, 8
	jal	xloop.44
	addi	$sp, $sp, -8
	lw	$t8, 4($sp)
	move	$ra, $t8
	lw	$v0, 0($sp)
	addi	$v0, $v0, 1
	j	yloop.40
_min_caml_start:
	li	$v0, 0
	move	$t8, $ra
	sw	$t8, 4($sp)
	addi	$sp, $sp, 8
	jal	yloop.40
	addi	$sp, $sp, -8
	lw	$t8, 4($sp)
	move	$ra, $t8
	j	loop
min_caml_float_of_int:
	itof	$f0, $v0
	jr	$ra
min_caml_print_int:
	out	$v0
	jr	$ra
