#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
using namespace std;
int nCycles = 0, nInstructions = 0;

class InstructionBlock{
public:
	string instructiontype;
	bool isBranch;
	int BranchNum;
	int operand1;
	bool operand2type;
	int operand2;
	bool operand3type;
	bool operand3present;
	int operand3;
	InstructionBlock(string instructiontype,bool isBranch,int BranchNum,int operand1,bool operand2type,int operand2,bool operand3present,bool operand3type,int operand3){
		this->instructiontype=instructiontype;
		this-> isBranch=isBranch;
		this->BranchNum=BranchNum;
		this->operand1=operand1;
		this->operand2type=operand2type;
		this->operand2=operand2;
		this->operand3type=operand3type;
		this->operand3present=operand3present;
		this->operand3=operand3;
	}
	void printInstructionBlock(){
		cout<<"instructiontype"<<instructiontype<<endl;
		cout<<"branch present"<<isBranch<<endl;
		cout<<"branch link number"<<BranchNum<<endl;
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
bool stall = false,loadstall = false,changeBranch = false;
int cycles[5];
int cmp = 0,bnum = 0;
int memory[1000];
int stack[10];
string opcode[5];
bool stackop[5];
int iNum[5];
int op1[5],op2[5],op3[5],r[16];
bool noins[5] = {true,true,true,true,true};
bool branch[5] = {false,false,false,false,false};

bool valid(string s)                    // helper function in determining operand for register
{
    if(s.length()>8) return false;
    for(int i = 1;i<s.length();i++)
    {
        if(s[i]<'0'||s[i]>'9') return false;
    }
    return true;
}


bool checkLoadStall()
{
	int j = iNum[0]+1;
	if(instructions[j].instructiontype=="str")
	{
		if((instructions[j].operand1==instructions[iNum[0]].operand1)||(instructions[j].operand2type==true&&instructions[j].operand2==instructions[iNum[0]].operand1)||(instructions[j].operand3present==true&&instructions[j].operand3type==true&&instructions[j].operand3==instructions[iNum[0]].operand1))
		{
			return true;
		}
	}
	else if(!instructions[2].isBranch)
	{
		if((instructions[j].operand2type==true&&instructions[j].operand2==instructions[iNum[0]].operand1)||(instructions[j].operand3present==true&&instructions[j].operand3type==true&&instructions[j].operand3==instructions[iNum[0]].operand1))
		{
			return true;
		}	
	}
	return false;
}

void forwardData()
{
	if(opcode[3]!="str"&&opcode[3]!="cmp"&&!noins[3]&&!noins[1]&&!branch[3]&&!branch[1])
	{
		if(opcode[1]=="str")
		{
			if(instructions[iNum[3]].operand1==instructions[iNum[1]].operand1)
			{
				op1[1] = op2[3];
				//cout<<"The value "<<op2[3]<<"is being forwarded. The ins is "<<opcode[3]<<endl;
			}
		}
		if(opcode[1]=="cmp")
		{
			if(instructions[iNum[1]].operand1==instructions[iNum[3]].operand1) op2[1] = op2[3];
			else if(instructions[iNum[1]].operand2type==true&&instructions[iNum[1]].operand2==instructions[iNum[3]].operand1)
			{
				op3[1] = op2[3];
				//cout<<"FD - op5\n";
			} 
		
		}
		else
		{
			if(instructions[iNum[1]].operand2type==true && instructions[iNum[1]].operand2==instructions[iNum[3]].operand1)
			{
				op2[1] = op2[3];
				//cout<<"FD - op\n";
			}
			if(instructions[iNum[1]].operand3present==true&&instructions[iNum[1]].operand3type==true&&instructions[iNum[1]].operand3==instructions[iNum[3]].operand1)
			{
				op3[1] = op2[3];
				//cout<<"FD - op1\n";
			}
		}
	}
	if(opcode[2]!="str"&&opcode[2]!="cmp"&&!noins[2]&&!noins[1]&&!branch[2]&&!branch[1])
	{
		if(opcode[1]=="str")
		{
			if(instructions[iNum[2]].operand1==instructions[iNum[1]].operand1)
			{
				op1[1] = op2[2];
				//cout<<"FD - op3\n";
			}
		}
		if(opcode[1]=="cmp")
		{
			if(instructions[iNum[1]].operand1==instructions[iNum[2]].operand1) op2[1] = op2[2];
			else if(instructions[iNum[1]].operand2type==true&&instructions[iNum[1]].operand2==instructions[iNum[2]].operand1)
			{
				op3[1] = op2[2];
				//cout<<"FD - op5\n";
			} 
		
		}
		else
		{
			if(instructions[iNum[1]].operand2type==true && instructions[iNum[1]].operand2==instructions[iNum[2]].operand1)
			{
				op2[1] = op2[2];
				//cout<<"FD - op4\n"<<opcode[1]<<" "<<opcode[2]<<" "<<op2[2]<<" "<<op1[1];
			}
			if(instructions[iNum[1]].operand3present==true&&instructions[iNum[1]].operand3type==true&&instructions[iNum[1]].operand3==instructions[iNum[2]].operand1)
			{
				op3[1] = op2[2];
				//cout<<"FD - op5\n";
			}
		}
	}
}

void flushandRestart(int n)
{
	for(int i = 0;i<2;i++)
	{
		opcode[i]="null";
		noins[i] = true;
	}
	iNum[0] = n-1;
}


void nextStage()
{
	stall = false;
	for(int i=4;i>-1;i--)
	{
		if(cycles[i]>0&&!noins[i])
		{
			stall = true;
			return;
		}
		if(noins[i]) opcode[i] = "null";
	}
	if(!stall)
	{
		if(changeBranch==true)
		{
			flushandRestart(bnum);
			changeBranch = false;
		}
		if(loadstall==true) loadstall = false;
		if(noins[0]==false&&opcode[0]=="ldr") loadstall = checkLoadStall();
	}
	if(!stall) forwardData();
	if(!stall)
	{
		for(int i=4;i>0;i--)
		{
			iNum[i] = iNum[i-1];
			opcode[i] = opcode[i-1];
			op1[i] = op1[i-1];
			op2[i] = op2[i-1];
			op3[i] = op3[i-1];
			noins[i] = noins[i-1];
			branch[i] = branch[i-1];
			stackop[i] = stackop[i-1];
		}
	}
}


void writeback()
{
	if(!noins[4]&&!stall && !branch[4] && opcode[4]!="cmp")
	{
		if(opcode[4]!="str")
		{
			r[op1[4]] = op2[4];
		}
		//if(opcode[4]!="null") cout<<"Doing WriteBack\n";
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
				if(stackop[3]) op2[3] = stack[op2[3]];
				else op2[3] = memory[op2[3]];
			}
			else if(opcode[3]=="str")
			{
				cycles[3] = latency["str"] - 1;
				if(stackop[3]) stack[op2[3]] = op1[3];
				else memory[op2[3]] = op1[3];
				//cout<<"Store in memoryAccess"<<op2[3]<<endl;
				//cout<<op1[3]<<endl;

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
			if(branch[2])
			{
				if(opcode[2]=="b")
				{
					changeBranch = true;
					cycles[2] = latency["b"] - 1;
					bnum = op1[2] - 1;
				}
				else if(opcode[2]=="bl")
				{
					changeBranch = true;
					cycles[2] = latency["bl"] - 1;
					bnum = op1[2] - 1;
					r[14] = iNum[2];
				}
				else if(opcode[2]=="bne")
				{
					cycles[2] = latency["bne"] - 1;
					if(cmp!=3)
					{
						changeBranch = true;
						bnum = op1[2] - 1;
					}
				}
				else if(opcode[2]=="bge")
				{
					cycles[2] = latency["bge"] - 1;
					if(cmp!=2)
					{
						changeBranch = true;
						bnum = op1[2] - 1;
					}
				}
				else if(opcode[2]=="bne")
				{
					cycles[2] = latency["bne"] - 1;
					if(cmp!=3)
					{
						changeBranch = true;
						bnum = op1[2] - 1;
					}
				}
			}
			else if(opcode[2]=="ldr")
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
				//cout<<"Getting value from memory location "<<op2[3]<<endl;
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
				else if(opcode[2]=="mov")
				{
					cycles[2] = latency["mov"] - 1;
					if(op1[2]==15)
					{
						changeBranch = true;
						bnum = op2[2] + op3[2];
					}
					else
					{
						op2[2] += op3[3];
					}
				}
				else if(opcode[2]=="cmp")
				{
					cycles[2] = latency["cmp"] - 1;
					//cout<<op2[2]<<" with "<<op3[2]<<endl;
					if(op2[2]>op3[2])
					{
						cmp = 1;
					}
					else if (op2[2]<op3[2])
					{
						cmp = 2;
					}
					else if (op2[2] == op2[2])
					{
						cmp = 3;
					}
				}
			}
			if(opcode[2]!="null") nInstructions++;
		}
	}
}


void idecode()
{
	if(!stall&&!noins[1])
	{
		cycles[1] = 0;
		if(opcode[1]=="str")
		{
			op2[1] = r[instructions[iNum[1]].operand1];
			op3[1] = r[instructions[iNum[1]].operand2];
			if(instructions[iNum[1]].operand3present)
			{
				if(instructions[iNum[1]].operand3type == true)
				{
					op3[1] = r[instructions[iNum[1]].operand3];
					if(instructions[iNum[1]].operand3==13)
					{
						stackop[1] = true;
					}
					else
					{
						stackop[1] = false;
					}
				}
				else op3[1] = instructions[iNum[1]].operand3; 
			}
			else
			{
				op3[1] = 0;
			}
		}
		else if(branch[1])
		{
			op1[1] = instructions[iNum[1]].BranchNum;
		}
		else if(opcode[1]=="cmp")
		{
			op2[1] = r[instructions[iNum[1]].operand1];
			if(instructions[iNum[1]].operand2type==true) op3[1] = r[instructions[iNum[1]].operand2];
			else op3[1] = instructions[iNum[1]].operand2;
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
					op3[1] = instructions[iNum[1]].operand3;	
				}
				if(instructions[iNum[1]].operand3==13)
				{
					stackop[1] = true;
				}
				else
				{
					stackop[1] = false;
				}	
			}
			else op3[1] = 0; 
		}
		//cout<<op2[1]<<" is op2 \n";
		//cout<<op3[1]<<" is op3 \n";
	}
}


void ifetch()
{
	//cout<<stall<<endl;
	if(!stall)
	{
		//cout<<(iNum[0]>=instructions.size()-1)<<endl;
		if((iNum[0]>(long long)(instructions.size()-1))||loadstall==true)
		{
			noins[0] = true;
		}
		else
		{
			noins[0] = false;
			branch[0] = false;
			iNum[0]++;
			if(instructions[iNum[0]].isBranch) branch[0] = true;
			opcode[0] = instructions[iNum[0]].instructiontype;
			cycles[0] = 0;
		}
		r[14] = iNum[0];
	}
}

bool checkEnd()
{
	for(int i=0;i<5;i++)
	{
		if(noins[i]==false) return false;
		if(noins[0]==true) opcode[i]="null";
	}
	return true;
}

void nextCycle()
{
	nCycles++;
	nextStage();
	writeback();
	memoryAccess();
	execute();
	idecode();
	ifetch();
	pEnd = checkEnd();
}