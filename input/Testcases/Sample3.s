MOV  R0,#1            
MOV  R1,#6            
MOV  R3,#1            
BL loop

loop:                                      
MUL R4,R3,R0
ADD R0,R0,#1
CMP R0,R1
BLE loop
MOV pc,lr
