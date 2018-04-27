#include "Assign5helper.h"
using namespace std;
int r[13];
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
                label1.second = instructions.size();
                label1.first = line.substr(0,line.length()-1);
                labels[label1.first] = label1.second;
                continue;
            }
            if(line[0]=='@'){
                string leaveline;
                getline(myfile,leaveline);
                continue;
            }
            instructions.push_back(line);
        }

        myfile.close();
    }
    else
    {
    	cout << "Unable to open file\n";
    	return 0;
    }
    for(int i=0;i<instructions.size();i++)
    {
    	cout<<instructions[i]<<endl;
    }
    cout<<"Welcome to ARM-Sim(Pipelined Version)\n";
	cout<<"The program operates in different modes.\n";
	cout<<"1). For step by step execution, type\'s\' \n2). For full execution at any stage, press\'f\'(default mode).\n\n";
	char mode = 'k';
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
	}
    return 0;
}