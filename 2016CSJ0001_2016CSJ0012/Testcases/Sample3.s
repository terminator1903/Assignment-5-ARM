@ fibonacci program

mov r7, #0   
mov r0, #0
@ Please add a file

mov r6, r0 
mov r1, #0 
mov r2, #1 
CMP r6, #0
BNE LOOP
mov r6, #20
LOOP:
	ADD r1, r2, r1
	SUB r2, r1, r2
	mov r0, #1
	mov r0, #0
	ADD r7, r7, #1
CMP r7, r6
BNE LOOP