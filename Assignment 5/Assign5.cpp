#include<iostream>
#include <vector>
#include <sstream>
#include<fstream>
#include<string>
#include "Instruction.h"
using namespace std;

int reg[16];
int mem[100];
int instCount=0;
int instFile=0;
int prev=0;
int cycle=0;
int l_add=0,l_sub=0,l_cmp=0,l_mul=0,l_bne=0,l_bge=0,l_branch=0,l_bl=0,l_mov=0;
int l_ldr=0,l_ldr_p=0,l_str=0,l_str_p=0;
Instruction instruction[10000];

int redspace(string s,int initial,int l)  //Function to trim unwanted space in Syntax.
{ 

   while(initial<l && (s[initial]==32 || s[initial]=='\n') )    //Incrementing position until non-space char occur in the expression.
      initial++;

   return initial;      //returning Extra spaces free Expression.
}

bool checkint(string s,int size)    //Function to check whether integer is occuring in a string.
{    
    int l=size;
    
    int i=0;
    
    if(s[i]=='-')   //Detecting for negative sign.
       i++;
         
    while(i<l)
    {

        int z= s[i]-'0';
        
        if(z<0 || z>9)    //If z<0 or Z>9 then the number can't be a integer.So returning false.
          return false;

        i++;    
    }

    return true;

}

//Function r1(...) to handle Destination resistor and first operand register for commands like add, sub, mul involing  3 values in the expression.
//l=Length of String s,j=Non space position of s,i=Instruction Number 

int r1(string s,int l,int i,int j)  
{
   
   if(s[j]!='r')    //Checking if First char of the String is r or not ,If not then the string is not representing any registor so the operand is invalid. 
   {
      cout<<"Invalid operand in line"<<i<<endl;
      return -1;
   }

   int d;
   
   int d1=s[j+1]-'0';
   
   int d2=s[j+2] - '0' ;
   
   if(0<=d2 && d2<=9)   //If we are using registor higher than 9 then assigning register index to d. 
   {
        d=10*d1 + d2;   
        
   }
   else
   {
       d=d1;  // If we are using single digit register.
   }  

      
   if(d<0 || d>15)  //Checking If register index is correct or not.
   {
      cout<<"Invalid register in line"<<i<<endl;
      return -1;
   }   
   

   return d;

}

// Function "r2(...)" for last operand in the expression.
//l=Length of the String,i=Instruction Number,j=Index of first non-space Char in s,d= Index of Destination Register,Command= ARM command.

void r2(string s,int l,int i,int j,int d,string command) 
{
   if(s[j]=='r' || s[j]=='#')   //Possible starting char for any last operand is either r(If registor is involved) or #(If number is used directly).
   {  
      
      if(s[j]=='r') //For Register.
      {  

         int second1 = s[j+1]-'0'; //Getting the value of the first position of the index of the registor involving in the operation.   
     
         int second2 = -1 ; 

         if(j+2<l)
          second2 = s[j+2] -'0';  //Getting the value of the second position of the index of the registor involving in the operation.
      
         int second ;

         if(second2 == -1)
          second = second1 ;  //If register involved is between r0 and r9 
         else
          second =10*second1 + second2 ;  //If register involved is between r10 and r15


         if(second<0 || second>15)  // Check if register mentioned is exists or not.
         {
            cout<<"Invalid register in line"<<i<<endl;
         }
         
         if(second/10 == 0)
         {
            if(j+1 != l-1)    //Checking if String imported is Correct(If it Ends after last operand or not) or not(in case register is between r0 and r9).
            { 
    cout<<"Problem in line"<<i<<endl;  
          return;
            }  
         }
         else
         {
            if(j+2 != l-1)    //Checking if String imported is Correct(If it Ends after last operand or not) or not(in case register is not between r0 and r9).
            { 
    cout<<"Problem in line"<<i<<endl;
                return;
            }  
         }   
         
  //For 2 Operands Commands

         if(command == "mov"){          //For MOV command
          reg[d]= reg[second];
          instruction[instFile].setR1(second);
          instruction[instFile].setType("MOV");
             instruction[instFile].setDestination(d);
               instruction[instFile].setCheck(0);

     instFile++;
            //Assigning value to destination register with the value of register given in last operand
         }
         else 
    if(command == "comp") //For CMP command
                {
              if(reg[d]>reg[second])  //Return 1 if Operand1 is greater than operand 2
                prev = 1;
              else 
      if(reg[d]<reg[second])  //Return 2 if Operand1 is less than operand 2
               prev= 2;
                  else    //Return 0 if Operand1 is equal to operand 2
                 prev = 0; 
               instruction[instFile].setR1(second);
          instruction[instFile].setType("CMP");
             instruction[instFile].setDestination(d);
               instruction[instFile].setCheck(0);

     instFile++;
                }  
      }
      else  //Case when last operand starts with #
      {  
         int size=l-j-1;
         string str=s.substr(j+1,size);  //Getting the string after # 
         
         if(!checkint(str,size))  //If there is not any integer value after #
         {
            cout<<"Immediate operand not integer in line"<<i<<endl;
            return;
         } 

         stringstream conv(str);

         int add=0;

         conv>>add; // Assigning value of conv to add
         

        if(command == "mov")          // If command is MOV
        {
          reg[d]=add;

          instruction[instFile].setR1(add);
          instruction[instFile].setType("MOV");
             instruction[instFile].setDestination(d);
               instruction[instFile].setCheck(1);

     instFile++;   // Assign Destination register with value of add.
        }
        else 
    if(command == "comp") //If command is CMP
                {
              if(reg[d]>add)  //Return 1 if Operand1 is greater than operand 2
                prev = 1;
              else 
        if(reg[d]<add)  //Return 2 if Operand1 is less than operand 2
                 prev= 2;
              else    //Return 0 if Operand1 is equal to operand 2
                   prev = 0; 
                 instruction[instFile].setR1(add);
          instruction[instFile].setType("CMP");
             instruction[instFile].setDestination(d);
               instruction[instFile].setCheck(1);

     instFile++;
          }  
      


      }        
   }
   else // if last operand start with any other char then 'r' or '#'
   {
      cout<<"Problem in Command MOV in line."<<i<<endl;
      return;
   }  


}

//Function For MOV Command
//s= Line containing mov command, l= length of the string , initial= Intial Position in the String ,i=Instruction Number.

void mov(string s,int l,int initial,int i)
{  
      
   int j=redspace(s,initial,l); //Getting the index of first non-space char in s

   int d=r1(s,l,i,j); // Getting Destination Resistor

   if(d==-1)    // If Error in r1 function ie error in input
   return;   
   
    

   if(d/10 > 0)   //if Destination register number is more than 9 then inc j to check string further.
   j++;
      
   j=redspace(s,j+2,l); // Removing whitespace
   
   if(s[j]!=',')  // Checking if there is a ',' after destination register or not, if not then show error.
   {
      cout<<" ',' missing in line"<<i<<endl;
      return;
   }

   j=redspace(s,j+1,l); // Removing whitespace

   r2(s,l,i,j,d,"mov"); 
   //This line below for clock cycle
     
     
   cycle+=l_mov;
   instCount++;
}

//Function for CMP command.
//s= Line containing CMP command,l=Lenght of the string s,initial=Initial Position in the string,i=Instruction number

void comp(string s,int l,int initial,int i)
{
    int j=redspace(s,initial,l);  //Getting the index of first non-space char in s

    int d=r1(s,l,i,j);  // Getting Destination Resistor

   if(d==-1)  // If Error in r1 function ie error in input
   return;   
   
   if(d/10 > 0)   //if Destination register number is more than 9 then inc j to check string further.
   j++;

   j=redspace(s,j+2,l);   // Removing whitespace
   
   if(s[j]!=',')    // Checking if there is a ',' after destination register or not, if not then show error.
   {
      cout<<" ',' missing in line"<<i<<endl;
      return;
   }

   j=redspace(s,j+1,l);   // Removing whitespace

   r2(s,l,i,j,d,"comp");
   cycle+=l_cmp;
   instCount++;


}


//Function for Add,Sub,Mul command.
//s= Line containing Add or Sub or Mul command,l=Lenght of the string s,initial=Initial Position in the string,i=Instruction number


void asmul (string s,int l,int initial,int i,string command)
{  
   int j=redspace(s,initial,l); // Removing whitespace

   
   int d=r1(s,l,i,j); // Getting Destination Resistor

   if(d == -1)
    return;  

   if(d/10 > 0)
   j++;

   j=redspace(s,j+2,l);
   
   if(s[j]!=',')
   {
      cout<<" ',' missing in line"<<i<<endl;
      return;
   }

   j=redspace(s,j+1,l);

   

   int src = r1(s,l,i,j); //Getting Operand 1
   
   if(src == -1)
   return ;   
   
   if(src/10 > 0)
   j++;
      
   j=redspace(s,j+2,l);

   if(s[j]!=',')
   {
      cout<<" ',' missing in line"<<i<<endl;
      return;
   }

   j=redspace(s,j+1,l);  

   if(s[j]=='r' || s[j]=='#') 
   {  
      


      if(s[j]=='r')
      {  
          int second1 = s[j+1]-'0';
     
        int second2 = -1;

        if(j+2<l)
        second2 = s[j+2] -'0';
      
        int second ;

        if(second2 == -1 )
        second = second1 ;
        else
        second =10*second1 + second2 ;
        
        if(second<0 || second>15)
         {
            cout<<"Invalid register in line"<<i<<endl;
            return;
         }
         
         if(second/10 == 0)
         {
             if(j+1 != l-1)
             { cout<<"Problem in line"<<i<<endl;    
              return;
            }  
         }
         else
         {
             if(j+2 != l-1)
             { cout<<"Problem in line"<<i<<endl;
              return;
             }    
         }    
         
         if( command == "add") // For ADD command 
         {
          reg[d] = reg[second] + reg[src];
          cycle+=l_add;
          instruction[instFile].setR1(src);
          instruction[instFile].setR2(second);
          instruction[instFile].setType("ADD");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(0);
          instFile++;
          instCount++;
         }   
         else if(command == "mul") //For MUL Command
         {
          reg[d] = reg[second] * reg[src] ; 
          cycle+=l_mul;
          instruction[instFile].setR1(src);
          instruction[instFile].setR2(second);
          instruction[instFile].setType("MUL");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(0);
          instFile++;
          instCount++;
         }
         else if(command == "sub") //For SUB Command
         {
          reg[d] = reg[src] - reg[second] ;   
          cycle+=l_sub;
          instruction[instFile].setR1(src);
          instruction[instFile].setR2(second);
          instruction[instFile].setType("SUB");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(0);
          instFile++;
          instCount++;
          }
      }
      else
      {  
         int size=l-j-1;
         string str=s.substr(j+1,size);
         
         if(!checkint(str,size))
         {
             cout<<"Immediate operand not integer in line"<<i<<endl;
             return;
           }    
         stringstream conv(str);


      int add=0;
      conv>>add;
         
        if( command == "add") 
        {
          reg[d] = add + reg[src];
          cycle+=l_add;
          instruction[instFile].setR1(src);
          instruction[instFile].setR2(add);
          instruction[instFile].setType("ADD");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(1);
          instFile++;
          instCount++;
        }    
      else if(command == "mul")
      {
        reg[d] = add * reg[src] ; 
        cycle+=l_mul;
        instruction[instFile].setR1(src);
          instruction[instFile].setR2(add);
          instruction[instFile].setType("MUL");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(1);
          instFile++;
        instCount++;  
      }
      else if(command == "sub")
      {reg[d] = reg[src] - add ;   
        cycle+=l_sub;
        instruction[instFile].setR1(src);
          instruction[instFile].setR2(add);
          instruction[instFile].setType("SUB");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(1);
          instFile++;
        instCount++;
      }
       
      }
   }
   else
   {
      cout<<"problem in command add in line"<<i<<endl;
      return;
   }  


}


void showreg() //For printing Values stored in registers.
{
   for(int i=1;i<=16;i++)
   {cout<<"r"<<(i-1)<<"="<<reg[i-1]<<"\t";
    if(i%4==0)
      cout<<endl;
   }
   cout<<"----------------------------------"<<endl;
}

void showInstructions()
{
  for(int i=0;i<instFile;i++)
  {
      cout<<i<<" "<<instruction[i].getType()<<" "<<instruction[i].getDestination()<<" "<<instruction[i].getR1()<<" ";
      if(instruction[i].getType()=="ADD"||instruction[i].getType()=="MUL"||instruction[i].getType()=="SUB")  
      {
        cout<<instruction[i].getR2();
      }
      cout<<endl;
  }
}




bool Islabel[100];

vector <string> label;
vector <int> pos ;

string find(string s)  // TO find if there is ':' in the sting ,To be useful in B,BNE type statements.
{    
    int l=s.length();

    string t="";
    for(int i=0;i<l;i++)
    {   
        if(s[i]==':')
        return t;

        t=t+s[i];    
    }    

    return "";
}

//Storing Information about any labels occuring in ARM program.
void reslabels(string a,int i)
{
   string s=find(a);
   if( s =="")
   Islabel[i]=false;
   else
   { Islabel[i]=true;
     label.push_back(s);
     pos.push_back(i); 
   } 

}

//Checking if a particular label exists or not(When Branch link is called).
int f(string a)
{ int sz=label.size();
    for(int i=0;i<sz;i++)
    {
        if(a.compare(label[i]) == 0 ) 
        return pos[i];        
    }    

    return -1;
}

//Function to handle LDR and STR commands
//where s="Instruction",i=instruction number,command=which arm command is called,l=length of the string,initial=Initial position of the string s.

void loadstr(string s,int l,int initial,int i,string command )
{

  //Getting the index of destination resistor.

   int j=redspace(s,initial,l);

   
   int d=r1(s,l,i,j);

   if(d == -1)
   return;  

   if(d/10 > 0)
   j++;
   
   j=redspace(s,j+2,l);

   if(s[j]!=',')
   {
      cout<<" ',' missing in line"<<i<<endl;
      return;
   }

   j=redspace(s,j+1,l);


   if(s[j]=='[')
   {
      j=redspace(s,j+1,l);

      int two=r1(s,l,i,j);//Getting Operand 1

      if(two == -1)
      return ;
         
      if(two/10 > 0)
      j++;

      j=redspace(s,j+2,l);
 
      if(s[j]==']')
      {   

         if(j==l-1)
         {  int x= reg[two]-1000;
            int y = x/4;
            if(command == "ldr")
            {  
               if( x%4 == 0 && y>=0 && y<=99 )
               {
                reg[d]=mem[y];
                instruction[instFile].setR1(two);
          // instruction[instFile].setR1(add);
          instruction[instFile].setType("LDR");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(0);
          instFile++;
                cycle+=l_ldr;
                instCount++; 
               }  
               else 
               {
                cout<<"Wrongly accessed memory in line "<<i<<endl;
                // instCount++;
                return;
               } 
            }
            else
            { 
               if( x%4 == 0 && y>=0 && y<=99 )  
               {
                mem[y]=reg[d];
                cycle+=l_str;
                instruction[instFile].setR1(two);
          // instruction[instFile].setR1(add);
          instruction[instFile].setType("STR");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(0);
          instFile++;
                instCount++;
               }
               else 
               {
                cout<<"Wrongly accessed memory in line "<<i<<endl;
                // instCount++;
                return;
               }
               
            }   

         }
         else
         {
            j=redspace(s,j+1,l);

            if(s[j] == ',')
            {
                  j=redspace(s,j+1,l);

                  if(s[j] == '#')
                  {  
                     

                     int size=l-j-1;
                     string str=s.substr(j+1,size);
         
                     if(!checkint(str,size))
                     {
                        cout<<"Immediate operand not integer in line"<<i<<endl;
                        return;
                     } 
                     stringstream conv(str);


                     int offset=0;
                     conv>>offset;


                     int x= reg[two]-1000;
                     int y = x/4;
                     if(command == "ldr")
                     {  
                        if( x%4 == 0 && y>=0 && y<=99 )  
                        {
                        reg[d] = mem[y];
                        instruction[instFile].setR1(two);
          // instruction[instFile].setR1(add);
          instruction[instFile].setType("LDR");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(1);
          instFile++;
                          cycle+=l_ldr;
                          instCount++;
                        }
                        else 
                        {
                           cout<<"Wrongly accessed memory in line "<<i<<endl;
                           return;  
               
                        }
                        
                        reg[two]=reg[two] + offset ;
                     }
                     else
                     {
                        if( x%4 == 0 && y>=0 && y<=99 )  
                        {
                        mem[y] = reg[d];
                        cycle+=l_str;
                        instruction[instFile].setR1(two);
          // instruction[instFile].setR1(add);
          instruction[instFile].setType("STR");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(1);
          instFile++;
                        instCount++;
                        }
                        else 
                        {
                           cout<<"Wrongly accessed memory in line "<<i<<endl;
                           return;  
               
                        }
                        
                        reg[two]=reg[two] + offset ;
                     }   



                  }
                  else
                  {
                     cout<<"missing # in line "<<i<<endl;
                     return;
                  }   

            }
            else
            {
               cout<<" ',' missing in line "<<i<<endl;
               return;
            }   

         }   

      }
      else if(s[j] == ',' )
      {  
         j = redspace(s,j+1,l);

                  if(s[j] == '#')
                  {  

                     int size=l-j-1;
                     string str=s.substr(j+1,size-1);    // one less because we have ']' at the end also i do not allow spaces after number i.e it should be like #4] not #4 ]
                     
                     cout<<str<<endl;
                     if(!checkint(str,size-1))
                     {
                        cout<<"Immediate operand not integer in line "<<i<<endl;
                        return;
                     } 
                     stringstream conv(str);


                     int offset=0;
                     conv>>offset;

                     int x = reg[two] - 1000 + offset ;
                     int y = x/4; 
                     
                     if(command == "ldr")
                     {  
                        

                        if( x%4 == 0 && y>=0 && y<=99 )  
                        {
                        reg[d]=mem[y];
                        cycle+=l_ldr;
                        instCount++;
                        instruction[instFile].setR1(two);
          // instruction[instFile].setR1(add);
          instruction[instFile].setType("LDR");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(2);
          instFile++;
                        }
                        else 
                        {
                           cout<<"Wrongly accessed memory in line "<<i<<endl;
                           return;  
               
                        }


                     }
                     else
                     {
                        if( x%4 == 0 && y>=0 && y<=99 )  
                        {
                          mem[y] = reg[d];
                          cycle+=l_str;
                          instruction[instFile].setR1(two);
          // instruction[instFile].setR1(add);
          instruction[instFile].setType("STR");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(2);
          instFile++;
                          instCount++;
                        }
                        else 
                        {
                           cout<<"Wrongly accessed memory in line "<<i<<endl;
                           return;  
               
                        }                        
                     }   



                  }
                  else
                  {
                     cout<<"missing # in line "<<i<<endl;
                     return;
                  } 

         
      }  
      else
      {
         cout<<"Error in line"<<i<<endl;

      }   
   }
   else if(s[j]=='=' && command == "ldr")
   {  
      j=redspace(s,j+1,l);
      instruction[instFile].setR1(1000);
          // instruction[instFile].setR1(add);
          instruction[instFile].setType("LDR");
          instruction[instFile].setDestination(d);
          instruction[instFile].setCheck(3);
          instFile++;

      instCount++;
      cycle+=l_ldr_p;
      string t="";
      
      while(j<l)
      { t+=s[j] ;  
        j++; 
      }

      if(t=="")
      {
         cout<<"No label after = in line "<<i<<endl;
      }
      else
      {
         int x=f(t);

         reg[d]=1000;  
      }   

   }


   else
   {
      cout<<"Error in line "<<i<<" instead use [ or ="<<endl;
   }   

}

int getLatency(string s,int index)
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
    return -1;
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
    return -1; 
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
    return -1;
  }
  if(comm.compare("")==0)
  {
    return -1;
  }
  // cout<<"Index of Line "<<index<<endl;

  if(comm.compare("ldr")==0 || comm.compare("LDR")==0)
  {
    stringstream tt(s.substr(0,j));
    tt>>l_ldr;
    // cout<<" LDR "<<l_ldr<<endl;
    // l_ldr=stoi();
  }
  else
  {
    if(comm.compare("str")==0 || comm.compare("STR")==0)
    {
      stringstream tt(s.substr(0,j));
    tt>>l_str;
    // cout<<"STR "<<l_str<<endl; //=stoi(s.substr(0,j));
    }
    else
    {
      if(comm.compare("add")==0 || comm.compare("ADD")==0)
      {
        stringstream tt(s.substr(0,j));
    tt>>l_add;
    // cout<<"ADD "<<l_add<<endl;
    //=stoi(s.substr(0,j));
      }
      else
      {
        if(comm.compare("sub")==0 || comm.compare("SUB")==0)
        {
          stringstream tt(s.substr(0,j));
    tt>>l_sub;
    // cout<<"SUB "<<l_sub<<endl;
    //=stoi(s.substr(0,j));
        }
        else
        {
          if(comm.compare("mul")==0 || comm.compare("MUL")==0)
          {
            stringstream tt(s.substr(0,j));
    tt>>l_mul;
    // cout<<"MUL "<<l_mul<<endl;
    //=stoi(s.substr(0,j));
          }
          else
          {
            if(comm.compare("cmp")==0 || comm.compare("CMP")==0)
            {
              stringstream tt(s.substr(0,j));
    tt>>l_cmp;
    // cout<<"CMP "<<l_cmp<<endl;//=stoi(s.substr(0,j));
            }
            else
            {
              if(comm.compare("bne")==0 || comm.compare("BNE")==0)
              {
                stringstream tt(s.substr(0,j));
    tt>>l_bne;//=stoi(s.substr(0,j));
              // cout<<"BNE "<<l_bne<<endl;
              }
              else
              {
                if(comm.compare("bge")==0 || comm.compare("BGE")==0)
                {
                  stringstream tt(s.substr(0,j));
    tt>>l_bge;//=stoi(s.substr(0,j));
                // cout<<"BGE "<<l_bge<<endl;
                }
                else
                {
                  if(comm.compare("b")==0 || comm.compare("B")==0)
                  {
                    stringstream tt(s.substr(0,j));
    tt>>l_branch;//=stoi(s.substr(0,j));
                  // cout<<"B "<<l_branch<<endl;
                  }
                  else
                  {
                    if(comm.compare("bl")==0 || comm.compare("BL")==0)
                    {
                      stringstream tt(s.substr(0,j));
    tt>>l_bl;
    // cout<<"BL "<<l_bl<<endl;
    //=stoi(s.substr(0,j));
    //=stoi(s.substr(0,j));
                    }
                    else
                    {
                      if(comm.compare("mov")==0 || comm.compare("MOV")==0)
                      {
                        stringstream tt(s.substr(0,j));
                        tt>>l_mov;//=stoi(s.substr(0,j));
                     // cout<<"MOV "<<l_mov<<endl;
                      }
                      else
                      {
                        if(comm.compare("ldr_pseudo")==0 || comm.compare("LDR_PSEUDO")==0)
                        {
                          stringstream tt(s.substr(0,j));
                          tt>>l_ldr_p;
                          // cout<<"LDR_PSEUDO "<<l_ldr_p<<endl;
                            }
                        else
                        {
                          if(comm.compare("str_pseudo")==0 || comm.compare("STR_PSEUDO")==0)
                          {
                            stringstream tt(s.substr(0,j));
    tt>>l_str_p;//=stoi(s.substr(0,j));
    // cout<<"STR_PSEUDO "<<l_str_p<<endl;
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
  return 1;  
}

void getInfo()
{
  cout<<"Total Number of Clock Cycle: "<<cycle<<endl;
  cout<<"Instruction Count: "<<instCount<<endl;
  float avgCPI=cycle/(instCount*1.0);
  float avgIPC=instCount/(cycle*1.0);
  cout<<"Average CPI: "<<avgCPI<<endl;
  cout<<"Average IPC: "<<avgIPC<<endl;
}



string remspace(string a)
{
   int l=a.length();

   int i=l-1;

   while(a[i] == ' ')
   {
      i--;
   }

   string s=a.substr(0,i+1);

   return s;   
}

int main() {


   for(int i=0;i<=15;i++)
   mem[i];   

   for(int i=0;i<=15;i++)
   reg[i]=0;
       
   char data[100];                     // char array for storing line by line from file 
   ifstream ifile;                         // file pointer may be have to check
   ifile.open("input.txt");                
   ifstream latencyfile;
   latencyfile.open("latency.txt");
   string a[100],b[100];                            // stores every line after having read from file    
   int i=0;
   string l_a[100],l_b[100];

   int link[100];

   for(int j=0;j<100;j++)
   {
        link[j]=-1;
   }
   i=0;

   while (!ifile.eof()) {  
      ifile.getline(data, 100); 
      b[i]=data;                                        // reading from file            

      a[i]=remspace(b[i]);
      
      reslabels(a[i],i);
      i++;
   }


   ifile.close();

   int l_i=0;
   while(!latencyfile.eof())
   {
      latencyfile.getline(data,100);
      
      l_b[l_i]=data;
      l_a[l_i]=remspace(l_b[l_i]);
      if(l_a[l_i].compare("")==0 ||l_a[l_i][0]=='#')
      {

      } 
      else // break;
      { 
        // cout<<l_a[l_i]<<endl; 
        int kc=getLatency(l_a[l_i],l_i+1);
      l_i++;
      }
   }
   // cout<<"latencyfile reading completed."<<endl;
   latencyfile.close();

   int n=i-1;                                            // no of instructions including spaces
   
   
   
   i=0;

   int m=1;
   
   i=reg[15];
 while(reg[15]<=n){                         //  executing each instruction one by one 

   
    
   while(a[reg[15]].length()==0 && reg[15]<=n)
   {
    reg[15]++;          
                        // takes care of spaces b/w two instructions if any
   }


   int l=a[reg[15]].length();

   int j=redspace(a[reg[15]],0,l);                    // takes care of spaces within a given instruction

   
   string temp="";
   string lb;
   while(a[reg[15]][j]!=32 && j<l)
   {
      temp+=a[reg[15]][j];                      // reads first part of instruction such as MOV,ADD etc      
      j++;  

   }                                
   
   

   if(temp=="MOV" || temp=="mov")
   {  
      mov(a[reg[15]],l,j,reg[15]);
   }
   else if(temp=="ADD" || temp=="add")
   {
        asmul(a[reg[15]],l,j,reg[15],"add");      
   }
   else if(temp=="cmp" || temp=="CMP")
   {
        comp(a[reg[15]],l,j,reg[15]);
        // cout<<prev;
   }
   else if(temp == "SUB" || temp == "sub")
   {
      asmul(a[reg[15]],l,j,reg[15],"sub");
   }
   else if(temp== "MUL" || temp == "mul")
   {
     asmul(a[reg[15]],l,j,reg[15],"mul");
   }
   else if(temp == "BNE" || temp == "bne")
   {
      cycle+=l_bne;
      instCount++;
      if(prev != 0)
      {
        j=redspace(a[reg[15]],j,l);
        
        while(j<l)
        {
          lb+=a[reg[15]][j] ;
          j++;
        }

        

        int v;

        if(link[reg[15]]== -1){
          
          v=f(lb);

        if(v==-1)
        cout<<"No such label found in line"<<reg[15]<<endl;
        
        else
        {
          link[reg[15]]=v;
        } 

       }

       if(link[reg[15]]!=-1)  
       reg[15]=link[reg[15]]-1; 
      
      } 

   }
   else if (temp=="BGE" || temp=="bge")
   {
      cycle+=l_bge;
      instCount++;
      
      if(prev == 1)
      {
        j=redspace(a[reg[15]],j,l);
        
        while(j<l)
        {
          lb+=a[reg[15]][j] ;
          j++;
        }

        

        int v;

        if(link[reg[15]]== -1){
          
          v=f(lb);

        if(v==-1)
        cout<<"No such label found in line"<<reg[15]<<endl;
        
        else
        {
          link[reg[15]]=v;
        } 

       }

       if(link[reg[15]]!=-1)  
       reg[15]=link[reg[15]]-1; 
      
      } 



   }
   else if(temp=="B" || temp=="b")
   {

      cycle+=l_branch;
      instCount++;
      
      j=redspace(a[reg[15]],j,l);
        
        while(j<l)
        {
          lb+=a[reg[15]][j] ;
          j++;
        }

        

        int v;

        if(link[reg[15]]== -1){
          
          v=f(lb);

        if(v==-1)
        cout<<"No such label found in line"<<reg[15]<<endl;
        
        else
        {
          link[reg[15]]=v;
        } 

       }

       if(link[reg[15]]!=-1)  
       reg[15]=link[reg[15]]-1; 



   }
   else if(temp=="BL" || temp=="bl")
   {      
      reg[14] = reg[15];
      cycle+=l_bl ;
      instCount++;
      
      j=redspace(a[reg[15]],j,l);
        
        while(j<l)
        {
          lb+=a[reg[15]][j] ;
          j++;
        }

        

        int v;

        if(link[reg[15]]== -1){
          
          v=f(lb);

        if(v==-1)
        cout<<"No such label found in line"<<reg[15]<<endl;
        
        else
        {
          link[reg[15]]=v;
        } 

       }

       if(link[reg[15]]!=-1)  
       reg[15]=link[reg[15]]-1; 

      


   } 
   else if(temp=="LDR" || temp=="ldr")
   {
     loadstr(a[reg[15]],l,j,reg[15],"ldr");
   }
   else if(temp=="STR" || temp=="str")
   {
     
     loadstr(a[reg[15]],l,j,reg[15],"str");  
   }  
   if(temp=="exit")
   {
    instCount++;
    break;
   }
     
      if(m!=2)
      {
        cout<<"To continue to next statement press 1 else to see end result press 2,to break press any other integer\n";
 
        cin>>m;
        
      }
      if(m==1)
      {
        reg[15]++;
        showreg();

        int zx;
        cout<<"To see ith element of memory press i from 0 to 99 else press -1\n";
        cin>>zx;

        if(zx>=0 && zx<=99)   
        cout<<mem[zx]<<endl;
      }
      else if(m==2)
      {
          reg[15]++;       
      }  
      else
      {
        break;
      } 
      
                                                                                               
   
}
  if(m==2)
    
    showreg();
    getInfo();
    showInstructions();

   return 0;
}



