#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
using namespace std;


vector<string> instructions;
map<string,int> labels;
map<string,int> latency;
bool pEnd = false;
bool stall;
int cycles[5];
int opcode[5];
int op1[5],op2[5],op3[5];
int iNum[5];
int r[13],lr,sp;


bool valid(string s)                    // helper function in determining operand for register
{
    if(s.length()>8) return false;
    for(int i = 1;i<s.length();i++)
    {
        if(s[i]<'0'||s[i]>'9') return false;
    }
    return true;
}


void nextStage()
{
	for(int i=4;i>0;i--)
	{
		if(cycles[i]==0&&cycles[i-1]==0)
		{
			iNum[i] = iNum[i-1];
			opcode[i] = opcode[i-1];
		}
		else if(cycles[i-1]>0&&cycles[i]==0)
		{
			stall = true;
			break;
		}
	}
}

void writeback()
{
	cout<<"Oye Mundea\n";
}


void memoryAccess()
{
	cout<<"Memory Chalao\n";
}


void execute()
{

}


void idecode()
{
	if(!stall)
	{
		op1[1] = opcode[] 
	}
}


void ifetch()
{
	if(!stall)
	{
		opcode[0] = instructions[iNum[0]].instructiontype;
		iNum[0]++;
	}
}


void nextCycle()
{
	nextStage();
	writeback();
	memoryAccess();
	execute();
	idecode();
	ifetch();
}