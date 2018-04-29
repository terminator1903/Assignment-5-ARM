#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
using namespace std;


class InstructionBlock{
public:
	string instructiontype;
	int operand1;
	bool operand2type;
	int operand2;
	bool operand3type;
	bool operand3present;
	int operand3;
	InstructionBlock(string instructiontype,int operand1,bool operand2type,int operand2,bool operand3present,bool operand3type,int operand3){
		this->instructiontype=instructiontype;
		this->operand1=operand1;
		this->operand2type=operand2type;
		this->operand2=operand2;
		this->operand3type=operand3type;
		this->operand3present=operand3present;
		this->operand3=operand3;
	}
	void printInstructionBlock(){
		cout<<"instructiontype"<<instructiontype<<endl;
		cout<<"operand1"<<operand1<<endl;
		cout<<"operand2type"<<operand2type<<endl;
		cout<<"operand2"<<operand2<<endl;
		cout<<"operand3type"<<operand3type<<endl;
		cout<<"operand3present"<<operand3present<<endl;
		cout<<"operand3"<<operand3<<endl;
	}
};

vector<InstructionBlock> instructions;
map<string,int> labels;
map<string,int> latency;

bool pEnd = false;
bool stall;
int cycles[5];
int memory[1000];
string opcode[5];
int iNum[5];
int op1[5],op2[5],op3[5],r[16];
bool noins[5] = {true,true,true,true,true};

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
	stall = false;
	for(int i=4;i>0;i--)
	{
		if(cycles[i]==0&&cycles[i-1]==0)
		{
			iNum[i] = iNum[i-1];
			opcode[i] = opcode[i-1];
			op1[i] = op1[i-1];
			op2[i] = op2[i-1];
			op3[i] = op3[i-1];
			noins[i] = noins[i-1];
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
	if(!stall)
	{
		if(opcode[4]!="str")
		{
			r[op1[4]] = op2[4];
		}
	}
}


void memoryAccess()
{
	if(cycles[3]>0) cycles[3]--;
	if(cycles[3]==0)
	{
		if(!stall)
		{
			if(opcode[3]=="ldr")
			{
				cycles[3] = latency["ldr"] - 1;
				op2[3] = memory[op2[3]]; 	
			}
			else if(opcode[3]=="str")
			{
				cycles[3] = latency["str"] - 1;
				memory[op2[3]] = op1[3];
			}
			else
			{
				cycles[3] = 0;
			}
		}
	}
}


void execute()
{
	if(cycles[2]>0) cycles[2]--;
	if(cycles[2]==0)
	{
		if(!stall)
		{
			if(opcode[2]=="ldr")
			{
				if(op3[2]==0)
				{
					cycles[2] = 0;
				}
				else
				{
					cycles[2] = latency["add"] - 1;
					op2[2] += op3[2];	
				}
			}
			else if(opcode[2]=="str")
			{
				if(op3[2]==0)
				{
					cycles[2] = 0;
				}
				else
				{
					cycles[2] = latency["add"] - 1;
					op2[2] += op3[2];	
				}
			}
			else if(opcode[2]=="mov")
			{
				cycles[2] = latency["mov"] - 1;
			}
			else
			{
				if(opcode[2]=="add")
				{
					op2[2] = op2[2] + op3[2];
					cycles[2] = latency["add"] - 1;
				}
				else if(opcode[2]=="sub")
				{
					op2[2] -=op3[2];
					cycles[2] = latency["sub"] - 1;
				}
				else if(opcode[2]=="mul")
				{
					op2[2] *= op3[2];
					cycles[2] = latency["mul"] - 1;
				}
				else if (opcode[2]=="div")
				{
					op2[2] /=op3[2];
					cycles[2] = latency["div"] - 1;
				}
			}
		}
	}
}


void idecode()
{
	if(!stall)
	{
		cycles[1] = 0;
		if(opcode[1]=="str")
		{
			op1[1] = r[instructions[iNum[1]].operand1];
			op2[1] = r[instructions[iNum[1]].operand2];
			if(instructions[iNum[1]].operand3present)
			{
				if(instructions[iNum[1]].operand3type == true)
				{
					op3[1] = r[instructions[iNum[1]].operand3];
				}
				else op3[1] = r[instructions[iNum[1]].operand3]; 
			}
			else
			{
				op3[1] = 0;
			}
		}
		else
		{
			op1[1] = instructions[iNum[1]].operand1;
			if(instructions[iNum[1]].operand2type == true)
			{
				op2[1] = r[instructions[iNum[1]].operand2];
			}
			else
			{
				op2[1] = instructions[iNum[1]].operand2;	
			}
			if(instructions[iNum[1]].operand3present==true)
			{
				if(instructions[iNum[1]].operand3type == true)
				{
					op3[1] = r[instructions[iNum[1]].operand3];
				}
				else
				{
					op3[1] = instructions[iNum[1]].operand2;	
				}	
			}
			else op3[1] = 0; 
		}
	}
}


void ifetch()
{
	if(!stall)
	{
		if(iNum[0]>=instructions.size()) noins[0] = true;
		else
		{
			noins[0] = false;
			iNum[0]++;
			opcode[0] = instructions[iNum[0]].instructiontype;
			cycles[0] = 0;
		}
	}
}

bool checkEnd()
{
	for(int i=0;i<5;i++)
	{
		if(noins[i]==false) return false;
	}
	return true;
}

void nextCycle()
{
	nextStage();
	writeback();
	memoryAccess();
	execute();
	idecode();
	ifetch();
	pEnd = checkEnd();
}