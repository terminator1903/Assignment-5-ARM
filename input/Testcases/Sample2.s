MOV r0, #0    
@initialise loop index to 0

MOV r1, #100   @number of iterations

Loop:
ADD r0, r0, #1  
@increment loop index

CMP r0, r1
BLE Loop