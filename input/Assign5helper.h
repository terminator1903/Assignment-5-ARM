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
int opcode[5];
int iNum[5];


bool valid(string s)                    // helper function in determining operand for register
{
    if(s.length()>8) return false;
    for(int i = 1;i<s.length();i++)
    {
        if(s[i]<'0'||s[i]>'9') return false;
    }
    return true;
}
/*

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
	cout<<"Execution Done\n";
}


void idecode()
{
	cout<<"Hadippa\n";
}


void ifetch()
{
	if(cycles[0]>0)
	{
		cycles[0]--;
	}
	else if(!(stall) &&cycles[0]==0)
	{
		iNum[0]++;
		opcode[0] = instructions[iNum[0]].opcode;
		cycles[0] = 1;
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
*/