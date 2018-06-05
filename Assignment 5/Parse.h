#ifndef PARSE_H
#define PARSE_H
#include<string>
#include "Loop.h"
using namespace std;
extern Instruction instruction[10000];//Objects of Instruction class for storagee of instruction.
extern int Index;
extern Loop loop[1000];
extern int indexLoop;

string removeSpace(string s)//Removes space and tabs from the string.
{
	for(int i=0;i<s.length();i++)
	{
		if(s[i]==' '  || s[i]=='\t' || s[i]==',')
		{
		}
		else
		{
			return s.substr(i);
		}
	}
}

void instructionInput(string type,string lop,int regd,int reg1,int reg2,int check,int offset,int inum,string inst)
{
	instruction[Index].setType(type);
	instruction[Index].setLoop(lop);
	instruction[Index].setR1(reg1);
	instruction[Index].setR2(reg2);
	instruction[Index].setDestination(regd);
	instruction[Index].setCheck(check);
	instruction[Index].setOffset(offset);
	instruction[Index].setNum(inum);
	instruction[Index].setInst(inst);
	Index++;
}


void parseAddSubMul(string s,int number)
{
	try
	{
		string inst=s;
		string command="";
		int destination=-1;
		int operand1=-1;
		int operand2=-1;
		int i=0;
		
		for(i=0;i<s.length();i++)
		{
			if(s[i]==' ' || s[i]==',')
				break;
			command+=s[i];
		}

		// cout<<command<<endl;	

		s=s.substr(i);
		s=removeSpace(s);

		//For Destination Register.
		
		if(s[0]!='r')
				throw "Invalid token obtained at destination at line "+number;

		string temp="";

		for(i=1;i<s.length();i++)
		{
			if(s[i]==' ' || s[i]==',')
				break;	
			temp+=s[i];
		}
		
		// s=removeSpace(s.substr(space+1));

		stringstream parse(temp);
		
		parse>>destination;
		// cout<<destination<<endl;
		
		temp="";
		
		//For Operand 1.

		s=removeSpace(s.substr(i));
		
		if(s[0]!='r')
				throw "Invalid token obtained at operand1 at line "+number;


		for(i=1;i<s.length();i++)
		{
			if(s[i]==' ' || s[i]==',')
				break;
			temp+=s[i];
		}
		
		s=removeSpace(s.substr(i));

		stringstream parse1(temp);
		parse1>>operand1;

		// cout<<operand1<<endl;
		temp="";

		//For Operand 2
		int check=0;
		if(s[0]!='r' && s[0]!='#')
				throw "Invalid token obtained at operand2 "+number;

		if(s[0]=='#')
			check=1;
		
		for(i=1;i<s.length();i++)
		{
			temp+=s[i];
		}
		
		stringstream parse2(temp);
		parse2>>operand2;

		// cout<<operand2<<endl;

		instructionInput(command,"",destination,operand1,operand2,check,0,number,inst);
	}
	catch(const char* msg)
	{
		cerr<<msg<<endl;
	}
}

void parseMovCmp(string s,int number)
{
	try
	{
		string inst=s;
		string command="";
		int operand1=-1;
		int operand2=-1;
		int i=0;
		
		for(i=0;i<s.length();i++)
		{
			if(s[i]==' ')
				break;
			command+=s[i];
		}

		// cout<<command<<endl;	

		s=s.substr(i);
		s=removeSpace(s);

		//For Destination Register.
		
		if(s[0]!='r')
				throw "Invalid Token obtained at operand1 in MOV at line "+number;

		string temp="";

		for(i=1;i<s.length();i++)
		{
			if(s[i]==' ' || s[i]==',')
				break;	
			temp+=s[i];
		}
		
		stringstream parse(temp);
		
		parse>>operand1;
		// cout<<operand1<<endl;
		
		temp="";
		
		//For Operand 1.

		int check=0;
		s=removeSpace(s.substr(i));
		
		if(s[0]!='r' && s[0]!='#')
				throw "Invalid token obtained at operand2 at line "+number;
		if(s[0]=='#')
			check=1;


		for(i=1;i<s.length();i++)
		{
			if(s[i]==' ')
				break;
			temp+=s[i];
		}

		stringstream parse1(temp);
		parse1>>operand2; 

		// cout<<operand2<<endl;
		
		instructionInput(command,"",operand1,-1,operand2,check,0,number,inst);
		
	}
	catch(const char* msg)
	{
		cerr<<msg<<endl;
	}
}

void parseLdrStr(string s,int number)
{
	try
	{
		string inst=s;
		string command="";
		int operand1=-1;
		int operand2=-1;
		int i=0;
		
		for(i=0;i<s.length();i++)
		{
			if(s[i]==' ')
				break;
			command+=s[i];
		}

		// cout<<command<<endl;	

		s=s.substr(i);
		s=removeSpace(s);

		//For Destination Register.
		
		if(s[0]!='r')
				throw "Invalid Token obtained at operand1 at line "+number;

		string temp="";

		for(i=1;i<s.length();i++)
		{
			if(s[i]==' ' || s[i]==',')
				break;	
			temp+=s[i];
		}
		
		stringstream parse(temp);
		
		parse>>operand1;
		// cout<<operand1<<endl;
		
		temp="";
		
		//For Operand 1.

		int check=0;
		s=removeSpace(s.substr(i));
		
		if(s[0]!='[')
				throw "Invalid token obtained at operand2 at line "+number;
		
		s=removeSpace(s.substr(1));
		int offset=0;
		if(s[0]!='r' && s[0]!='#')
				throw "Invalid token obtained at operand2 at line "+number;
		if(s[0]=='#')
			check=1;
		


		for(i=1;i<s.length();i++)
		{
			if(s[i]==' ' || s[i]==',' || s[i]==']')
				break;
			temp+=s[i];
		}

		
		stringstream parse1(temp);
		if(check==1)
		parse1>>offset;
		else
		parse1>>operand2;
		// cout<<operand2<<endl;
		s=removeSpace(s.substr(i));
		if(s[0]=='#')
		{
			check=2;
			temp="";
			for(i=1;i<s.length();i++)
			{	
				if(s[i]==' ' || s[i]==']')
					break;
				temp+=s[i];

			}
			stringstream parse3(temp);
				parse3>>offset;
				// cout<<offset<<endl;
			s=removeSpace(s.substr(i));

		}
		if(s[0]!=']')
				throw "Invalid Token obtained at operand2 at line "+number;
		
		instructionInput(command,"",operand1,-1,operand2,check,offset,number,inst);
	}
	catch(const char* msg)
	{
		cerr<<msg<<endl;
	}
}

void parseBranch(string s,int number)
{
	try
	{
		string inst=s;
		string command="";
		string loopname;
		int i=0;
		for(i=0;i<s.length();i++)
		{
			if(s[i]==' ' || s[i]=='\t')
			{
				break;
			}
			command+=s[i];
		}
		s=removeSpace(s.substr(i));
		loopname=s;
		instructionInput(command,loopname,-1,-1,-1,3,-1,number,inst);
	}
	catch(const char* msg)
	{
		cerr<<msg<<endl;
	}
}

void printInstruction()
{
	for(int i=0;i<Index;i++)
	{
		string command=instruction[i].getType();
		cout<<instruction[i].getNum()<<endl;
		if(command=="ADD" || command=="MUL" || command=="SUB")
		{
			cout<<"Command: "<<command<<" Destination: "<<instruction[i].getDestination()<<" ";
			cout<<"Operand1: "<<instruction[i].getR1()<<" Operand2: "<<instruction[i].getR2()<<" "<<endl;
		}
		else
		{
			if(command=="LDR" || command=="STR")
			{
				cout<<"Command: "<<command<<" Destination: "<<instruction[i].getDestination()<<" ";
				cout<<"Operand1: "<<instruction[i].getR1()<<" Operand2: "<<instruction[i].getR2()<<" ";
				cout<<"offset: "<<instruction[i].getOffset()<<endl;
			}
			else
			{	
				if(command=="MOV" || command=="CMP")
				{
					cout<<"Command: "<<command<<" ";
					cout<<"Destination: "<<instruction[i].getDestination()<<" Operand2: "<<instruction[i].getR2()<<" Check is "<<instruction[i].getCheck()<<endl;
				}
				else
				{	
					if(command=="BL" || command=="B" || command=="BNE" || command=="BGE" || command=="BLE")
					{
						cout<<"Command: "<<command<<" Loop Name: "<<instruction[i].getLoop()<<endl;
					}	
				}	
			}	
		}
	}
	for(int i=0;i<indexLoop;i++)
	{
		cout<<"Loop Name: "<<loop[i].getName()<<" Starting Instruction Number: "<<loop[i].getStart()<<endl;
	}
}

void parseInput(string s,int number)
{
	s=removeSpace(s);

	bool flag;
	int pos=0;
	for(int i=0;i<s.length();i++)
	{
		if(s[i]==':')
		{
			flag=true;
			pos=i;
		}
		else
		{
			if(s[i]!=' ')
				flag=false;
		}
	}
	
	if(flag)
	{	
		loop[indexLoop].setName(s.substr(0,pos));
		loop[indexLoop].setStart(Index);
		indexLoop++;
	}
	else
	{

		if((s[0]=='M' && s[1]=='U') || s[0]=='A' || (s[0]=='S' && s[1]=='U') || (s[0]=='m' && s[1]=='u') || s[0]=='a' || (s[0]=='s' && s[1]=='u'))
		{
			parseAddSubMul(s,number);
		}
		else
		{
			if((s[0]=='M'|| s[0]=='m') || (s[0]=='C'|| s[0]=='c'))
			{
				parseMovCmp(s,number);
			}
			else
			{
				if((s[0]=='L'|| s[0]=='l') || (s[0]=='S'|| s[0]=='s'))
				{
					parseLdrStr(s,number);
				}
				else
				{
					if((s[0]=='B'|| s[0]=='b'))
					{
						parseBranch(s,number);
					}
					else
					{
						cout<<"Error in "<<number<<endl;
						
					}	
				}	
			}
		}

	}


}
#endif