#include<string>
using namespace std;

class Loop
{
	private:
		string name;
		int start;
	public:
		string getName()
		{
			return name;
		}
		int getStart()
		{
			return start;
		}
		void setName(string s)
		{
			name=s;
		}
		void setStart(int s)
		{
			start=s;
		}
};
