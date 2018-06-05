#ifndef PARSELATENCY_H
#define PARSELATENCY_H
#include<string>
using namespace std;

extern int l_add,l_sub,l_cmp,l_mul,l_bne,l_bge,l_branch,l_bl,l_mov;
extern int l_ldr,l_ldr_p,l_str,l_str_p;

void parseLatency(string s,int index)
{
  int len=s.length();
  int i=0;
  while(s[i]==' ')
  {
    i++;
  }
  s=s.substr(i);
  if(s[0]=='#')
  {
    return;
  }
  len=s.length();
  string comm="";
  for(i=0;i<len;i++)
  {
    if(s[i]==' '||s[i]=='=')
    {
      break;
    }
    comm+=s[i];
  }
  int checkEqual=0;
  for(;i<len;i++)
  {
    if(s[i]=='=')
    {
      checkEqual=1;
      break;
    }
    // comm+=s[i];
  }
  if(checkEqual!=1)
  {
    cout<<"%% Syntax error in Latency.txt file at line "<<index<<endl;
    return; 
  }

  s=s.substr(i+1);
  len=s.length();
  i=0;
  while(s[i]==' ')
  {
    i++;
  }
  s=s.substr(i);
  len=s.length();
  int j=0;
  for(j=0;j<len;j++)
  {
    if(s[j]==';')
    {
      break;
    }
  }
  if(j==len && s[j-1]!=';')
  {
    cout<<"%%Syntax error in Latency.txt file at line "<<index<<" ';' might be missing there.";
    return;
  }
  if(comm.compare("")==0)
  {
    return;
  }
  
  if(comm.compare("ldr")==0 || comm.compare("LDR")==0)
  {
    stringstream tt(s.substr(0,j));
    tt>>l_ldr;
  }
  else
  {
    if(comm.compare("str")==0 || comm.compare("STR")==0)
    {
      stringstream tt(s.substr(0,j));
      tt>>l_str;
    }
    else
    {
      if(comm.compare("add")==0 || comm.compare("ADD")==0)
      {
        stringstream tt(s.substr(0,j));
    	tt>>l_add;
      }
      else
      {
        if(comm.compare("sub")==0 || comm.compare("SUB")==0)
        {
          stringstream tt(s.substr(0,j));
    	  tt>>l_sub;
    	}
        else
        {
          if(comm.compare("mul")==0 || comm.compare("MUL")==0)
          {
            stringstream tt(s.substr(0,j));
    		tt>>l_mul;
          }
          else
          {
            if(comm.compare("cmp")==0 || comm.compare("CMP")==0)
            {
              stringstream tt(s.substr(0,j));
    		  tt>>l_cmp;
            }
            else
            {
              if(comm.compare("bne")==0 || comm.compare("BNE")==0)
              {
                stringstream tt(s.substr(0,j));
    			tt>>l_bne;
			  }
              else
              {
                if(comm.compare("bge")==0 || comm.compare("BGE")==0)
                {
                  stringstream tt(s.substr(0,j));
    			  tt>>l_bge;
    			}
                else
                {
                  if(comm.compare("b")==0 || comm.compare("B")==0)
                  {
                    stringstream tt(s.substr(0,j));
    			    tt>>l_branch;
                  }
                  else
                  {
                    if(comm.compare("bl")==0 || comm.compare("BL")==0)
                    {
                      stringstream tt(s.substr(0,j));
    				  tt>>l_bl;
                    }
                    else
                    {
                      if(comm.compare("mov")==0 || comm.compare("MOV")==0)
                      {
                        stringstream tt(s.substr(0,j));
                        tt>>l_mov;
                      }
                      else
                      {
                        if(comm.compare("ldr_pseudo")==0 || comm.compare("LDR_PSEUDO")==0)
                        {
                          stringstream tt(s.substr(0,j));
                          tt>>l_ldr_p;
                        }
                        else
                        {
                          if(comm.compare("str_pseudo")==0 || comm.compare("STR_PSEUDO")==0)
                          {
                            stringstream tt(s.substr(0,j));
    						tt>>l_str_p;
                          }
                          else
                          {
                            cout<<"Error in Latency File at line "<<index<<endl;
                          }
                        }
                      } 
                    }
                  }
                }
              }
            }
          }
        } 
      }
    }
  }  
}


void showLatency()
{
	cout<<"LDR "<<l_ldr<<endl;
	cout<<"STR "<<l_str<<endl;
	cout<<"MOV "<<l_mov<<endl;
	cout<<"MUL "<<l_mul<<endl;
	cout<<"PseudoSTR "<<l_str_p<<endl;
	cout<<"PseudoLDR "<<l_ldr_p<<endl;
	cout<<"SUB "<<l_sub<<endl;
	cout<<"ADD "<<l_add<<endl;
	cout<<"CMP "<<l_cmp<<endl;
	cout<<"B "<<l_branch<<endl;
	cout<<"BL "<<l_bl<<endl;
	cout<<"BNE "<<l_bne<<endl;
	cout<<"BGE "<<l_bge<<endl;
}
#endif