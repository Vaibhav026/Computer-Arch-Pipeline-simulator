#include<string.h>
#include<bits/stdc++.h>
using namespace std;
class Instruction
{
	private:
		string type;
		string loop;
		int reg_d;
		int reg1;
		int reg2;
		int instructionNumber;
		int check;//When Check=0 => 2nd operand in a resistor, Check=1 2nd operator is # , check=2 Offset is there.
		int offset;
		string instruct;
		
		public:
		


		void setOffset(int n)
		{
			offset=n;
		}
		int getOffset()
		{
			return offset;
		}
		void setInst(string n)
		{
			instruct=n;
		}
		string getInst()
		{
			return instruct;
		}
		void setNum(int n)
		{
			instructionNumber=n;
		}
		int getNum()
		{
			return instructionNumber;
		}
		void setType(string s)
		{
			type=s;
		}
		void setLoop(string s)
		{
			loop=s;
		}
		void setDestination(int d)
		{
			reg_d=d;
		}
		void setR1(int r1)
		{
			reg1=r1;
		}
		void setCheck(int a)
		{
			check=a;
		}
		int getCheck()
		{
			return check;
		}
		void setR2(int r2)
		{
			// checkr2=b;
			reg2=r2;
		}

		string getType()
		{
			transform(type.begin(), type.end(), type.begin(), ::toupper);
			return type;
		}
		string getLoop()
		{
			return loop;
		}
		int getDestination()
		{
			return reg_d;
		}
		int getR1()
		{
			return reg1;
		}
		int getR2()
		{
			return reg2;
		}

};
