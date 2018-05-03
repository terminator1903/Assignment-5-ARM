#include "Assign5helper.h"
using namespace std;
void printStatus()
{
    if(stall) cout<<"Pipelined is stalled\n";
    for(int i=0;i<5;i++)
    {
        if(!noins[i]) cout<<opcode[i]<<" ("<<cycles[i]<<") | ";
        else cout<<"null | ";
    }
    cout<<endl;
    //for(int i=0;i<5;i++)
    //{
    //    cout<<op1[i]<<" "<<op2[i]<<endl;
    //}
    cout<<"Number of Instructions executed: "<<nInstructions;
    cout<<"\nTotal number of clock cycles: "<<nCycles<<endl;
    cout<<"Average IPC: "<<(double)nInstructions/nCycles<<endl<<endl<<endl;
    for(int i=0;i<13;i++)
    {
        cout<<"R"<<i<<" : "<<r[i]<<endl;
    }
    cout<<endl;
    //cout<<branch[4]<<endl;
    cout<<"Memory Changed Locations:\n";
    for(int i=0;i<1000;i++)
    {
        if(memory[i]!=0)
        {
            cout<<"Location -  "<<i<<" : "<<memory[i]<<endl;
        }
    }
    cout<<endl;
}
int main (int argc,char *argv[]) 
{
    ifstream lfile(argv[2]);
    string line,num,eq;    
    pair<string,int> label1;
    if(argc>2)
    {
        if(lfile.is_open())
        {
            while ( lfile>>line )
            {
                if(line[0]=='#')
                {
                    string leaveline;
                    getline(lfile,leaveline);
                    continue;
                }
                for(int i=0;i<line.length();i++)
                {
                    if(line[i]>='A'&&line[i]<='Z')
                    {
                        line[i] = line[i] - 'A' + 'a';
                    }
                }
                lfile>>eq;
                if(eq=="=")
                {
                    lfile>>num;
                    if(num[num.length()-1]==';') num = num.substr(0,num.length()-1);
                    if(!valid(num))
                    {
                        cout<<"Latency of "<<line<<"not Valid in latency file\n";
                        return 0;
                    }
                }
                else
                {
                    cout<<"Error in latency File\n";
                    return 0;
                }
                latency[line] = stoi(num);
            }  
        }
    }

    ifstream myfile(argv[1]);
    if (myfile.is_open())
    {
        int i=0;
        string instructiontype;
        int operand1,operand2,operand3,BranchNum=-1;
        bool operand3type,operand3present,operand2type,isBranch=false;
        while ( myfile>>line )
        {   
            if(line==",") continue;
            if(line[line.length()-1]==',') line = line.substr(0,line.length()-1);
            for(int i=0;i<line.length();i++)
            {
                if(line[i]>='A'&&line[i]<='Z')
                {
                    line[i] = line[i] - 'A' + 'a';
                }
            }
            if(line[line.length() - 1]==':')
            {
                label1.second = instructions.size()+1;
                label1.first = line.substr(0,line.length()-1);
                labels[label1.first] = label1.second;
                continue;
            }
            if(line[0]=='@'){
                string leaveline;
                getline(myfile,leaveline);
                continue;
            }
            
            i++;
            if(line=="pc"){
                line="r15";
            }
            if(line=="lr"){
                line="r14";
            }
            if(line=="sp"){
                line="r13";
            }
            if(i==1){
            	if(line=="bne"||line=="bge"||line=="b"||line=="beq"||line=="ble"){
            		isBranch=true;
            	}
                instructiontype=line;
            }
            if(i==2){
            	if(isBranch){
            		BranchNum=labels[line];
            		i=0;
            	}
            	else{
            		operand1=stoi(line.substr(1,line.length()-1));
            	}
            }
            if(i==3){
                if(line[0]=='#'){
                    operand2type=false;
                    operand2=stoi(line.substr(1,line.length()-1));
                    operand3present=false;
                    i=0;
                }
                if(line[0]=='['){
                    i--;
                }
                if(line[0]=='r'){
                    if(instructiontype=="mov"){
                        i=0;
                    }
                    operand2type=true;
                    operand2=stoi(line.substr(1,line.length()-1));
                }
            }
            if(i==4){
                operand3present=true;
                if(line[0]==']'){
                    i=0;
                }
                if(line[0]=='#'){
                    operand3type=false;
                    operand3=stoi(line.substr(1,line.length()-1));
                    if(instructiontype=="ldr"||instructiontype=="str"){
                        i--;
                    }
                    else{
                        i=0;
                    }
                }
                if(line[0]=='r'){
                    operand3type=true;
                    operand3=stoi(line.substr(1,line.length()-1));
                    i=0;
                }
            }
            if(i==0){
                InstructionBlock obj(instructiontype,isBranch,BranchNum,operand1,operand2type,operand2,operand3present,operand3type,operand3);
                instructions.push_back(obj);
                isBranch=false;
                BranchNum=-1;
            }
            
        }

        myfile.close();
    }
    else
    {
    	cout << "Unable to open file\n";
    	return 0;
    }
    cout<<"Welcome to ARM-Sim(Pipelined Version)\n";
    cout<<"The program operates in different modes.\n";
	cout<<"1). For step by step execution, type\'s\' \n2). For full execution at any stage, press\'f\'(default mode).\n\n";
	char mode = 'k';
    iNum[0] = -1;
    r[13] = 4999;
	while(!pEnd)
	{
		if(mode!='f')
		{
			cout<<"Mode : ";
			cin>>mode;
			if(mode !='s') mode = 'f';
		}
		else mode = 'f';
        nextCycle();
        if(mode=='s')
        {
            printStatus();
        }
	}
    if(mode=='f')
    {
        cout<<"The program is terminated. The status of the pipeline, various registers and memory is:\n\n";
        printStatus();
    }
    else cout<<"\n\nThe program has been terminated\n";
    /*for(int i=0;i<instructions.size();i++){
        cout<<instructions[i].instructiontype<<" "<<instructions[i].isBranch<<" "<<instructions[i].BranchNum<<" "<<instructions[i].operand1<<" "<<instructions[i].operand2type<<" "<<instructions[i].operand2<<" "<<instructions[i].operand3present<<" "<<instructions[i].operand3type<<" "<<instructions[i].operand3<<endl;
    }
    map<string,int> ::iterator it;
    for(it=labels.begin();it!=labels.end();it++){
    	cout<<it->first<<" "<<it->second<<endl;
    }*/
    return 0;
}