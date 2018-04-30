mov r1, #10
Mov r2, #1
<<<<<<< HEAD
mov r3, #0
str r2, [ r1, #2 ]
ldr r2, [ r1, #2 ]		
=======
mov r3, #156
mov r4, r3
str r1, [ r1, #2 ]
ldr r4, [ r1, #2 ]
str r3, [ r4, #7 ]		
>>>>>>> 3b25de7f83886e30b73292db713b2286a66cb594
StoreIntegers:
add r3, r3, #4
add r2, r2, #1
sub r1, r1, #1
cmp r1, #0 	
bne StoreIntegers
mov r1, #1000
mov r4, #0     
add r4, r4, r2   
add r3, r3, #4    
sub r1, r1, #1   
cmp r1, #999 
<<<<<<< HEAD
bne StoreIntegers
=======
bne LoadI
mov pc, lr
add sp, sp, #4

>>>>>>> 3b25de7f83886e30b73292db713b2286a66cb594
