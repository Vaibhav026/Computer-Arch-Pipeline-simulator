#include<string.h>
using namespace std;
class Pipeline
{
	public:
		int rdnum=-1,r1num=-1,r2num=-1,check=0;
		int instruction=-1; //stage1
		string inst="";
		int operation=0,loopindex=-1;
		string loop="";
		int offset=0,rdvalue=0,r1value=0,r2value=0;//stage2

		int result=0;//stage3

		int output=0;
		//stage4


		
};
