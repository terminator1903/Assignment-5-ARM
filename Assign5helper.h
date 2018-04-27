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

bool valid(string s)                    // helper function in determining operand for register
{
    if(s.length()>8) return false;
    for(int i = 1;i<s.length();i++)
    {
        if(s[i]<'0'||s[i]>'9') return false;
    }
    return true;
}


void forward()
{
	cout<<"forwarded\n";
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
	cout<<"Burrah\n";
}

void nextCycle()
{
	forward();
	writeback();
	memoryAccess();
	execute();
	idecode();
	ifetch();
}