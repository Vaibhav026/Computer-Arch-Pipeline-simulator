#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include "Instruction.h"
#include "Parse.h"
#include "ParseLatency.h"
// #include "Loop.h"
#include "Pipeline.h"
using namespace std;

Instruction instruction[1000];//Objects of Instruction class for storagee of instruction.

int Index=0;//index for instruction.
bool increment=false;
Loop loop[100];
int indexLoop=0;
int nop=0;

int l_add=0,l_sub=0,l_cmp=0,l_mul=0,l_bne=0,l_bge=0,l_branch=0,l_bl=0,l_mov=0;
int l_ldr=0,l_ldr_p=0,l_str=0,l_str_p=0;

Pipeline pipeline[4];
int recentComp=0;
int programCounter=0;
int counter=1;
int memory[1000];


int r[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//Variable for pipeline
// int rdnum,r1num,r2num;//For 1st stage
// int rdvalue,r1value,r2value;//For second stage


//In first stage determine which register to use.
//In second stage import value in the variable rdnum,r1num,r2num.
//In third stage Do arithmetic operations.
//In fourth stage handle memory
//Store value in the registers.


string removeSpace(string s);//Removes space and tabs from the string.
void parseInput(string s);
void printInstruction();
void parseLatency(string s,int number);
void showLatency();

void show()
{
   for(int i=0;i<16;i++)
   {
      cout<<"Register "<<i<<" "<<r[i]<<endl;
   }
   for(int i=0;i<indexLoop;i++)
   {
      cout<<"Loop "<<i<<" "<<loop[i].getName()<<" Start "<<loop[i].getStart()<<endl;
   }
}

void flush()
{
   pipeline[1].r1num=-1;
   pipeline[1].rdnum=-1;
   pipeline[1].r2num=-1;
   pipeline[1].check=0;
   pipeline[1].operation=0;
   pipeline[1].offset=0;
   pipeline[1].rdvalue=0;
   pipeline[1].r1value=0;
   pipeline[1].r2value=0;
   pipeline[1].result=0;
   pipeline[1].output=0;
   pipeline[1].loop="";
   pipeline[1].loopindex=-1;
   // pipeline[2]=pipeline[1];
   pipeline[0]=pipeline[1];

   recentComp=-2;
}

void copyP0()
{
         pipeline[1].r1num=pipeline[0].r1num;
         pipeline[1].r2num=pipeline[0].r2num;
         pipeline[1].rdnum=pipeline[0].rdnum;
         pipeline[1].rdvalue=pipeline[0].rdvalue;
         pipeline[1].operation=pipeline[0].operation;
         pipeline[1].check=pipeline[0].check;
         pipeline[1].output=pipeline[0].output;
         pipeline[1].offset=pipeline[0].offset;
         pipeline[1].loopindex=pipeline[0].loopindex;
         pipeline[1].loop=pipeline[0].loop;
         pipeline[1].result=pipeline[0].result;
         pipeline[1].comparision=pipeline[0].comparision;
}

void copyP1()
{
         pipeline[2].r1num=pipeline[1].r1num;
         pipeline[2].r2num=pipeline[1].r2num;
         pipeline[2].rdnum=pipeline[1].rdnum;
         pipeline[2].r1value=pipeline[1].r1value;
         pipeline[2].r2value=pipeline[1].r2value;
         pipeline[2].rdvalue=pipeline[1].rdvalue;
         pipeline[2].operation=pipeline[1].operation;
         pipeline[2].check=pipeline[1].check;
         pipeline[2].output=pipeline[1].output;
         pipeline[2].offset=pipeline[1].offset;
         pipeline[2].loopindex=pipeline[1].loopindex;
         pipeline[2].loop=pipeline[1].loop;
         pipeline[2].comparision=pipeline[1].comparision;
}

void copyP2()
{
         pipeline[3].r1num=pipeline[2].r1num;
         pipeline[3].r2num=pipeline[2].r2num;
         pipeline[3].rdnum=pipeline[2].rdnum;
         pipeline[3].r1value=pipeline[2].r1value;
         pipeline[3].r2value=pipeline[2].r2value;
         pipeline[3].rdvalue=pipeline[2].rdvalue;
         pipeline[3].operation=pipeline[2].operation;
         pipeline[3].check=pipeline[2].check;
         pipeline[3].result=pipeline[2].result;
         pipeline[3].offset=pipeline[2].offset;
         pipeline[3].comparision=pipeline[2].comparision;
         pipeline[3].loop=pipeline[2].loop;
         pipeline[3].loopindex=pipeline[2].loopindex;
}

void copyP3()//No Need for this write now.
{

}

void forwarding(int r1n,int r2n,int pipeline3Dest,int pipeline3Operation)
{
   if(pipeline[1].rdnum==r1n && pipeline[1].operation!=7 && pipeline[1].operation!=6)
         {  
            pipeline[1].r1value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r1n && pipeline3Operation!=7 && pipeline3Operation!=6)
            {  
               pipeline[1].r1value=r[r1n];
            }
         }
         if(pipeline[1].rdnum==r2n && pipeline[1].operation!=7 && pipeline[1].operation!=6)
         {  
            pipeline[1].r2value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r2n && pipeline3Operation!=7 && pipeline3Operation!=6)
            {  
               pipeline[1].r2value=r[r2n];
            }
         }
}

void operate(int decide)
{
   Instruction inst=instruction[decide];
   Instruction inst1=instruction[decide+1];//For Forwarding(To know destination register number)
   Instruction inst2=instruction[decide+2];//For Forwarding(To know destination register number)
   int pipeline3Dest=pipeline[3].rdnum;
   int pipeline3Operation=pipeline[3].operation;
   
   //Stage5
   
   if(pipeline[3].operation!=5)//Checking if the instruction is not CMP.
   {
      if(pipeline[3].rdnum>=0 && pipeline[3].operation!=0)   //If the state in not nop
         r[pipeline[3].rdnum]=pipeline[3].output;

   }
   else
   {
      int temps=pipeline[3].output;    
      recentComp=(temps>=0)?(temps==0?0:1):(-1);
      // }   //For CMP storing its result.
   }
   
   //Stage4


   if(pipeline[2].operation<=5)
      pipeline[3].output=pipeline[2].result;
   else
   {
      if(pipeline[2].operation==6)
      {
         int tempr=(pipeline[2].result-1000);
         if(tempr%4!=0)
         {
            cerr<<"Wrong Access to memory."<<endl;
         }
         else
            pipeline[3].output=memory[tempr/4];     
      }
      else
      {
         if(pipeline[2].operation==7)
         {
            int tempr=(pipeline[2].result-1000);
            if(tempr%4!=0)
            {
               cerr<<"Wrong Memory Address."<<endl;
            }
            memory[tempr/4]=r[pipeline[2].rdnum];
            pipeline[3].output=pipeline[2].result;
            cout<<memory[tempr/4];
            pipeline[3].output=0;
            pipeline[3].rdnum=-1;     
         }  
         else
         {
            if(pipeline[2].operation==8)
            {
               if(recentComp==0)
               {
                  cout<<"Stage 4 Happened for BNE "<<endl;
                  flush();
                  programCounter=pipeline[2].result;
                  cout<<"programCounter is "<<programCounter<<endl;
                  // operate(programCounter++);
                  // programCounter++;
                  show();
               }
            }
            else
            {
               if(pipeline[2].operation==9)
               {
                  if(recentComp==1)
                  {
                     flush();
                     programCounter=pipeline[2].result;
                     operate(programCounter);
                     show();
                  }                

               }
               else
               {
                  if(pipeline[2].operation==10)
                  {
                     if(recentComp==-1)
                     {
                        flush();
                        programCounter=pipeline[2].result;
                        operate(programCounter);
                        show();
                     }

                  }
               }
            }
         }   
      }      
   }
   copyP2();
   

   //Stage 3

   int r1n,r2n;
   switch(pipeline[1].operation)
   {
      case 1://For Add
         
         r1n=pipeline[1].r1num;
         r2n=pipeline[1].r2num;
         cout<<"r1n"<<r1n<<"r2n"<<r2n<<endl;
         cout<<"r1value"<<pipeline[1].r1value<<"r2value"<<pipeline[1].r2value<<endl;
         forwarding(r1n,r2n,pipeline3Dest,pipeline3Operation);  
         pipeline[2].result=pipeline[1].r1value+pipeline[1].r2value;
         copyP1();
         break;
      
      case 2://For MUL

         r1n=pipeline[1].r1num;
         r2n=pipeline[1].r2num;
         forwarding(r1n,r2n,pipeline3Dest,pipeline3Operation);
         pipeline[2].result=pipeline[1].r1value*pipeline[1].r2value;
         copyP1();
         break;

      case 3://For SUB
         
         r1n=pipeline[1].r1num;
         r2n=pipeline[1].r2num;
         forwarding(r1n,r2n,pipeline3Dest,pipeline3Operation);
         pipeline[2].result=pipeline[1].r1value-pipeline[1].r2value;
         copyP1();
         break;
      
      case 4://For MOV
      
         pipeline[2].result=pipeline[1].r2value;
         copyP1();
         break;
      
      case 5://CMP
         
         pipeline[2].result=pipeline[1].r1value-pipeline[1].r2value;
         copyP1();
         break;
      
      case 6://LDR
         pipeline[2].result=pipeline[1].r2value+pipeline[1].offset;
         copyP1();
         break;
      
      case 7://STR
         pipeline[2].result=pipeline[1].r2value+pipeline[1].offset;
         copyP1();
         break;

      case 8:
         // loop[pipeline[1].loopindex].getStart();
         pipeline[2].result=1;//loop[pipeline[1].loopindex].getStart();
         cout<<"Stage 3 Happened for BNE "<<pipeline[2].result<<" "<<loop[pipeline[1].loopindex].getStart()<<endl;
         copyP1();
         break;

      case 9:
         pipeline[2].result=loop[pipeline[1].loopindex].getStart();
         copyP1();
         break;

      case 10:
         pipeline[2].result=loop[pipeline[1].loopindex].getStart();
         copyP1();
         break;

      case 0://NOP
         pipeline[2].result=pipeline[1].result;
         copyP1();
         break;
   }
   
   //Stage2

      if(pipeline[0].r1num>=0)   //For Operand 1
         pipeline[1].r1value=r[pipeline[0].r1num];
      

      if(pipeline[0].check==0 && pipeline[0].r2num>=0) //For Operand 2
         pipeline[1].r2value=r[pipeline[0].r2num];
      else
      {     if(pipeline[0].check==1)
               pipeline[1].r2value=pipeline[0].r2num;
            else
            {
               pipeline[1].r2value=r[pipeline[0].r2num];  
            }
      }
      if(pipeline[0].loop!="")
      {
         for(int lindex=0;lindex<indexLoop;lindex++)
         {
            if(loop[lindex].getName()==pipeline[0].loop)
            {
               pipeline[1].loopindex=lindex;
               cout<<"Stage 2 Happened for BNE "<<pipeline[1].loopindex<<endl;
               break;
            }  
         }
      }
      copyP0();
   
   //Stage1

   if(decide!=900)
   {
      pipeline[0].rdnum=inst.getDestination();
      pipeline[0].r1num=inst.getR1();
      pipeline[0].r2num=inst.getR2();
      pipeline[0].check=inst.getCheck();
      pipeline[0].offset=inst.getOffset();
      string type=inst.getType();
      pipeline[0].loop=inst.getLoop();
      
      if(type=="ADD")
      {
         pipeline[0].operation=1;
         cout<<"ADD instruction is inserted in pipeline(0)"<<endl;
      }
      else
      {
         if (type=="MUL")
         {
            pipeline[0].operation=2;
            cout<<"MUL instruction is inserted in pipeline(0)"<<endl;     
         }
         else
         {
            if (type=="SUB")
            {
               pipeline[0].operation=3;
               cout<<"SUB instruction is inserted in pipeline(0)"<<endl;
            }
            else
            {
               if (type=="MOV")
               {
                  pipeline[0].operation=4;
               }
               else
               {
                  if (type=="CMP")
                  {
                     pipeline[0].operation=5;
                     pipeline[0].r1num=pipeline[0].rdnum;
                  }
                  else
                  {
                     if (type=="LDR")
                     {
                        bool first=false;
                        pipeline[0].operation=6;
                        if(decide+1<Index)
                        {
                           if(inst1.getR1()==pipeline[0].rdnum)
                           {
                                 int N;
                                 cout<<"Enter Choice for next instruction--Now NOP will be inserted."<<endl;
                                 cin>>N;
                                 if(N==0)
                                 {
                                    cout<<(counter++)<<endl;
                                    show();
                                    cout<<"------------------------"<<endl;
                                 }
                                 // show();
                                 nop++;
                                 first=true;
                                 operate(900);
                                 cout<<"NOP is completed."<<pipeline[0].r1num<<" "<<pipeline[0].operation<<endl;
                           }
                           else
                              if(inst1.getR2()==pipeline[0].rdnum && inst1.getCheck()==0)
                              {
                                 int N;
                                 cout<<"Enter Choice for next instruction"<<endl;
                                 cin>>N;
                                 if(N==0)
                                 {
                                    cout<<(counter++)<<endl;
                                    show();
                                    cout<<"------------------------"<<endl;
                                 }
                                 first=true;
                                 nop++;
                                 operate(900);
                              }
                        }
                        if(decide+2<Index && first==false)
                        {
                           if(inst2.getR1()==pipeline[0].rdnum)
                           {
                                 int N;
                                 cout<<"Enter Choice for next instruction"<<endl;
                                 cin>>N;
                                 if(N==0)
                                 {
                                    cout<<(counter++)<<endl;
                                    show();
                                    cout<<"------------------------"<<endl;
                                 }
                                 operate(programCounter++);
                                 programCounter=decide+1;
                                 cout<<"Enter Choice for next instruction"<<endl;
                                 cin>>N;
                                 
                                 if(N==0)
                                 {
                                    cout<<(counter++)<<endl;
                                    show();
                                    cout<<"------------------------"<<endl;
                                 }

                                 operate(900);
                                 nop++;
                                 increment=true;
                                 // operate(900);
                           }
                           else
                              if(inst2.getR2()==pipeline[0].rdnum && inst2.getCheck()==0)
                              {
                                 int N;
                                 cout<<"Enter Choice for next instruction"<<endl;
                                 cin>>N;
                                 if(N==0)
                                 {
                                    cout<<(counter++)<<endl;
                                    show();
                                    cout<<"------------------------"<<endl;
                                 }

                                 operate(programCounter++);
                                 // programCounter=decide+1;
                                 cout<<"Enter Choice for next instruction"<<endl;
                                 cin>>N;
                                 if(N==0)
                                 {
                                    cout<<(counter++)<<endl;
                                    show();
                                    cout<<"------------------------"<<endl;
                                 }
                                 operate(900);
                                 nop++;
                                 increment=true;
                                 // operate(900);
                              }
                        }  
                        // pipeline[0].r1num=pipeline[0].rdnum;
                     }
                     else
                     {
                        if (type=="STR")
                        {
                           pipeline[0].operation=7;                              // pipeline[0].r1num=pipeline[0].rdnum;
                        }
                        else
                        {   
                           if (type=="BNE")
                           {
                              pipeline[0].operation=8;
                              cout<<"Stage 1 Happened for BNE"<<endl;                              // pipeline[0].r1num=pipeline[0].rdnum;
                           }
                           else
                           {   
                              if (type=="BGE")
                              {
                                 pipeline[0].operation=9;                              // pipeline[0].r1num=pipeline[0].rdnum;
                              }  
                              else
                              {   
                                 if (type=="BLE")
                                 {
                                    pipeline[0].operation=10;                              // pipeline[0].r1num=pipeline[0].rdnum;
                                 }
                                 else  
                                 pipeline[0].operation=0;
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
   else
   {
      pipeline[0].rdnum=-1;
      pipeline[0].r1num=-1;
      pipeline[0].r2num=-1;
      pipeline[0].check=0;
      pipeline[0].operation=0;
      pipeline[0].comparision=-2;
      pipeline[0].loop="";
      pipeline[0].loopindex=-1;
   }
}



int main() 
{     

   char data[100];                     // char array for storing line by line from file 
   ifstream ifile;                         // file pointer may be have to check
   ifile.open("input.txt");                
   int i=1;

   while (!ifile.eof()) {  
      ifile.getline(data, 100);
      string temp=(string)data;
      if(temp.length()>2) 
      	parseInput(temp,i++);
   }
   ifile.close(); 
   
   memory[6]=1000;
   ifstream lfile;                         // file pointer may be have to check
   lfile.open("latency.txt");
   int j=0; 
   while (!lfile.eof()) {  
      lfile.getline(data, 100);
      string temp=(string)data;
      if(temp.length()>2) 
      	parseLatency(temp,j++);
   }
   lfile.close();               
   
   // printInstruction();
   // showLatency();
   cout<<"Press 1 to See final Result and 0 to See Instructions one by one"<<endl;
   int N=0;

   for(int programCounter=0;programCounter<Index+4+nop;programCounter++)
   {  
      // programCounter=i;
      if(programCounter<Index){
         operate(programCounter);
      // if(increment)
      //    i++;
      //    // programCounter++;   
      }
      else
      operate(900);
      if(N==0){
      cout<<"Enter Choice for next instruction"<<endl;
      cin>>N;}
      if(N==0)
      {
         cout<<(counter++)<<endl;
         show();
         cout<<"------------------------"<<"programCounter is "<<programCounter <<endl;
      }
      // programCounter++;      
   }
   if(N==1)
   {cout<<"Final output is: "<<endl;
   show();
   }
}