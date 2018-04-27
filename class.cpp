#include <iostream>
#include <vector>
using namespace std;

class InstructionBlock{
public:
	int instructiontype;
	int register1;
	bool operand2type;
	int operand2;
	bool operand3type;
	bool operand3present;
	int operand3;
	InstructionBlock(int instructiontype,int register1,bool operand2type,int operand2,bool operand3type,bool operand3present,int operand3){
		this->instructiontype=instructiontype;
		this->register1=register1;
		this->operand2type=operand2type;
		this->operand2=operand2;
		this->operand3type=operand3type;
		this->operand3present=operand3present;
		this->operand3=operand3;
	}
	void printInstructionBlock(){
		cout<<"instructiontype"<<instructiontype<<endl;
		cout<<"register1"<<register1<<endl;
		cout<<"operand2type"<<operand2type<<endl;
		cout<<"operand2"<<operand2<<endl;
		cout<<"operand3type"<<operand3type<<endl;
		cout<<"operand3present"<<operand3present<<endl;
		cout<<"operand3"<<operand3<<endl;
	}
};

int main(){

	vector<InstructionBlock> v;
	for(int i=0;i<3;i++){
		InstructionBlock obj(100,1,true,2,false,true,9);
		v.push_back(obj);
		v[i].printInstructionBlock();
	}

}