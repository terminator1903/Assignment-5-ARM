In order to compile and execute the code run the following commands

1. g++ main.cpp
2. ./a.out inputfile.s latency.txt

Note :-
	
	replace inputfile.s with any file you have to check upon
	latency.txt represents the latency file. The format is same as in assignment 3 and 4.

	Our program run in 2 modes 
	1. step by step mode
		shows the working of pipeline in step by step manner(press 's' for running this mode). It runs one cycle per time.

	2. final mode
		Here, the full program is run, and we get to see the final output only, i.e, the final values present in each of the registers.

Algorithm
	
	As stated in design doc, we have a function for forwarding, and seperate functions for each of the stages of the pipeline, which run in reverse. We assume R-type instructions latency in the exec 		stage, and the latency for memory instructions in the memory stage. We have a variable cycles for each of the stages, which tells how many cycles are left for the completion of that stage. These 		are also displayed in our step by step mode. Whenever the cycles of all the stages are zero, forwarding takes place, and the value is refreshed. This continues until we reach the end of file.

	Also, we do bubble the pipeline whenever we have dependencies that cannot be solved simply by forwarding(such as ldr 
	followed by add, where the loaded value is one of the add operands). As write back clearly occurs before registers fetch, there is clearly no structural hazard. Forwarding takes care of data 		hazard. We also flush the pipeline and start again in case the branch change is true, hence, taking care of control hazard.

Latency.txt

	This file contains the latencies of various instructions, and it has to provided as input. In case any instruction is missing from the file, the latency of each part of that instruction is assumed 	to be 1. The latency is considered as such:

	1). For instructions such as ldr and str, without any offset, the latency is considred of the memory stage. In this case, the latencies of other stages is 1.
	2).For ldr and str instructions with offset, the latency of add operation is also taken into account(in the execution stage). The total latency includes latency of add, and the latency of ldr/str.
	3).For all the other instructions, the latency is considered for the execution stage. The total latency is l+4, where l is given in the input latency file.
    
	(Sample latency file:
	
		add = 1;
		ldr = 20;
		str = 20;
	) 
