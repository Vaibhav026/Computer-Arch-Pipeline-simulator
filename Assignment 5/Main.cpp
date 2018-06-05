#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include "Instruction.h"
#include "Parse.h"
#include "ParseLatency.h"
#include "Pipeline.h"
using namespace std;

Instruction instruction[10000];//Objects of Instruction class for storagee of instruction.

int Index=0;//index for instruction.
bool increment=false;
Loop loop[1000];
int indexLoop=0;
int nop=0;

int l_add=0,l_sub=0,l_cmp=0,l_mul=0,l_bne=0,l_bge=0,l_branch=0,l_bl=0,l_mov=0;
int l_ldr=0,l_ldr_p=0,l_str=0,l_str_p=0,l_ble=2;

int instructionCount=0;
int cycle=0;

Pipeline pipeline[4];
int recentComp=0;
int programCounter=0;
int counter=1;
int printer=0;
int linkreg=0;
int memory[10000];


int r[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//Variable for pipeline
// int rdnum,r1num,r2num;//For 1st stage
// int rdvalue,r1value,r2value;//For second stage


//In first stage determine which register to use.
//In second stage import value in the variable rdnum,r1num,r2num.
//In third stage Do arithmetic operations.
//In fourth stage handle memory
//Store value in the registers.

string pipe5="";

string removeSpace(string s);//Removes space and tabs from the string.
void parseInput(string s);
void printInstruction();
void parseLatency(string s,int number);
void showLatency();

int freezerFlag=0;


void show()
{
   r[15]=programCounter;
   // r[14]=linkreg;
   if(r[15]>Index)
   {
      r[15]=Index;
   }
   // if(r[14]>Index)
   // {
   //    r[14]=Index;
   // }
   r[15]*=4;
   // r[14]*=4;
   for(int i=0;i<16;i++)
   {
      cout<<"Register "<<i<<" "<<r[i]<<endl;
   }
}

void getInfo()
{
   cout<<"Total Number of Instructions: "<<instructionCount<<endl;
   cout<<"Total Number of Cycles: "<<cycle<<endl;
   float avgCPI=cycle/(instructionCount*1.0);
   float avgIPC=instructionCount/(cycle*1.0);
   cout<<"Average CPI: "<<avgCPI<<endl;
   cout<<"Average IPC: "<<avgIPC<<endl;
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
   pipeline[1].inst="";
   pipeline[1].loopindex=-1;
   pipeline[2]=pipeline[1];
   pipeline[0]=pipeline[1];
   if(printer==0)
   cout<<"Instructions flushed in the pipeline."<<endl;

   // recentComp=0;
}

// void flush()
// {
//    pipeline[1].r1num=-1;
//    pipeline[1].rdnum=-1;
//    pipeline[1].r2num=-1;
//    pipeline[1].check=0;
//    pipeline[1].operation=0;
//    pipeline[1].offset=0;
//    pipeline[1].rdvalue=0;
//    pipeline[1].r1value=0;
//    pipeline[1].r2value=0;
//    pipeline[1].result=0;
//    pipeline[1].output=0;
//    pipeline[1].loop="";
//    pipeline[1].inst="";
//    pipeline[1].loopindex=-1;
//    // pipeline[2]=pipeline[1];
//    pipeline[0]=pipeline[1];
//    if(printer==0)
//    cout<<"Instructions flushed in the pipeline."<<endl;

//    // recentComp=0;
// }


void pipelineInfo()
{
   cout<<"**********************************************"<<endl;
   cout<<"IF Stage: "<<pipeline[0].inst<<endl;
   cout<<"ID Stage: "<<pipeline[1].inst<<endl;
   cout<<"EX Stage: "<<pipeline[2].inst<<endl;
   cout<<"MEM Stage: "<<pipeline[3].inst<<endl;
   cout<<"WB Stage: "<<pipe5<<endl;
   cout<<"**********************************************"<<endl;
}

int loaduse=1;

void freeze(int i)
{
   int N=0;
   for(int j=0;j<i;j++)
   {
      cycle++;
      if(printer==0)
      {
         
         cout<<"Enter Choice for next instruction.\n";
         cin>>printer;
         if(printer==0){
            pipelineInfo();
            show();
            cout<<"------------------------"<<endl;
         }

      }
      else
      {
         // break;
      }

   }
}

int freezer=0;

// int freezeAdd=0,freezeSub=0,freezeMul=0,freezeCmp=0;
// int freezeLdr=0,freezeStr=0,freezeB=0,freezeBl=0;
// int freezeMov=0,freezeBle=0,freezeBge=0,freezeBne=0;

void operate(int decide)
{
   freezer=0;
   Instruction inst=instruction[decide];
   Instruction inst1=instruction[decide+1];
   Instruction inst2=instruction[decide+2];
   
   cycle++;

   int pipeline3Dest=pipeline[3].rdnum;
   int pipeline3Operation=pipeline[3].operation;
   
   //Stage5
   
   int flag1=0;

   pipe5=pipeline[3].inst;
   if(pipeline[3].operation!=5){
   if(pipeline[3].rdnum>=0)
      r[pipeline[3].rdnum]=pipeline[3].output;

   }
   else
   {
      if(pipeline[3].operation==12)
      {
         r[14]=linkreg*4;
         linkreg=0;
      }
      else{
      int temps=pipeline[3].output;
      // cout<<"Temps"<<temps<<endl;
      recentComp=(temps>=0)?(temps==0?0:1):(-1);
      // cout<<"Recent COmp "<<recentComp<<endl;
   }}

   // cout<<"Recent Comp "<<recentComp<<endl;
   // pipeline[1].output=pipeline[3].output;
   // cout<<"Rd output "<<pipeline[3].output<<endl;
   //Stage4

// if(freezer!=2)
// {
   pipeline[3]=pipeline[2];
   if(pipeline[3].operation<=5)
      pipeline[3].output=pipeline[2].result;
   else
   {
      if(pipeline[3].operation==6)
      {
         int tempr=(pipeline[3].result-1000);
         // cout<<"Tempr "<<tempr<<endl;
         if(tempr%4!=0)
         {
            cerr<<"Wrong Access to memory."<<endl;
         }
         else{
         pipeline[3].output=memory[tempr/4];
         pipeline[2].inst="";
         // freeze(l_ldr-l_add-1);
         freezer+=l_ldr-l_add;

         }     
      }
      else
      {
         if(pipeline[3].operation==7)
         {
            int tempr=(pipeline[3].result-1000);
            if(tempr%4!=0)
            {
               cerr<<"Wrong Memory Address."<<endl;
            }
            else{
            memory[tempr/4]=r[pipeline[3].rdnum];
            pipeline[3].output=pipeline[2].result;
            // cout<<memory[tempr/4];
            pipeline[3].output=0;
            pipeline[3].rdnum=-1; 
            pipeline[2].inst="";
            // freeze(l_str-l_add-1);
            freezer+=l_str-l_add;

            }    
         } 
         else
         {
            if(pipeline[2].operation==8)
            {
               if(recentComp!=0)
               {
                  // cout<<"Stage 4 Happened for BNE "<<endl;
                  programCounter=pipeline[2].result;
                  decide=programCounter;
                  inst=instruction[decide];
                  pipeline[2].inst="";
                  // pipeline
                  freezer+=l_bne-1;
                  // freeze(l_bne-1);
                     flush();
                     // cout<<"IF should have something now."<<endl;
                     // pipelineInfo();
                     // freeze(1);
                  // cout<<"programCounter is "<<programCounter<<endl;
                  // operate(programCounter++);
                  // programCounter++;
                  recentComp=0;
                // if(printer==0){
                  // show();
                  // pipelineInfo();}
                  // return;
               }
               else
               {
                  // cout<<"Stage 4 Completed for BNE "<<endl;
                  // flush();
                  // programCounter=pipeline[2].result-1;
                  // cout<<"programCounter is "<<programCounter<<endl;
                  // operate(programCounter++);
                  // programCounter++;
                  // show();
               }
            }
            else
            {
               if(pipeline[2].operation==9)
               {
                  if(recentComp==1)
                  {
                     // cout<<"Stage 4 Happened for BGE "<<endl;
                     programCounter=pipeline[2].result;
                     decide=programCounter;
                     inst=instruction[decide];
                     pipeline[2].inst="";
                     // freeze(l_bge-1);
                     freezer+=l_bge-1;
                     // pipelineInfo();
                     flush();
                     // freeze(1);
                     // programCounter=pipeline[2].result;
                     // cout<<"programCounter is "<<programCounter<<endl;
                     // operate(programCounter++);
                     // programCounter++;
                     recentComp=0;
                     // if(printer==0){
                     // show();
                     // pipelineInfo();}
                     // return;
                  }
                  else
                  {
                     // cout<<"Stage 4 Completed for BGE "<<endl;
                     // flush();
                     // programCounter=pipeline[2].result-1;
                     // cout<<"programCounter is "<<programCounter<<endl;
                     // operate(programCounter++);
                     // programCounter++;
                     // show();
                  }                

               }
               else
               {
                  if(pipeline[2].operation==10)
                  {
                     if(recentComp==-1)
                     {
                        // cout<<"Stage 4 Happened for BLE "<<endl;
                        programCounter=pipeline[2].result;
                        decide=programCounter;
                        inst=instruction[decide];
                        pipeline[2].inst="";
                        // freeze(l_ble-1);
                        freezer+=l_ble-1;
                        // pipelineInfo();
                        flush();
                        // freeze(1);
                        // programCounter=pipeline[2].result;
                        // cout<<"Program Counter is "<<programCounter<<endl;
                     // operate(programCounter++);
                     // programCounter++;
                        recentComp=1;
                        // if(printer==0){
                        // show();
                        // pipelineInfo();}
                        // return;
                     }
                     else
                     {
                        // cout<<"Stage 4 Completed for BLE "<<endl;
                     // flush();
                     // programCounter=pipeline[2].result-1;
                        // cout<<"Program Counter is "<<programCounter<<endl;
                     // operate(programCounter++);
                     // programCounter++;
                        // show();
                     }

                  }
                  else
                  {
                  	if(pipeline[2].operation==11)
                  	{
                  		// programCounter=pipeline[2].result-1;
                  		// flush();
                  		// cout<<"Program Counter is from B command "<<programCounter<<endl;
                  		// show();
                  		// return;
                  	}
                     else
                     {
                        if(pipeline[2].operation==12)
                        {

                        }
                        else
                        {

                        }
                     }
                  }  
               }
            }
         } 
      }      
   }
// }
// else
// {
//    pipeline[3].inst="";
// }
   //Stage 3
   int r1n,r2n;
   // string tempinst;

   switch(pipeline[1].operation)
   {
      case 1://For Add
         // pipeline[2]=pipeline[1];
         
         r1n=pipeline[1].r1num;
         r2n=pipeline[1].r2num;
         if(pipeline[2].rdnum==r1n && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r1value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r1n && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r1value=r[r1n];
            }
         }
         if(pipeline[2].rdnum==r2n && pipeline[1].check==0 && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r2value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r2n  && pipeline[1].check==0 && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r2value=r[r2n];
            }
         }
         // if(freezer==0){
         pipeline[2].result=pipeline[1].r1value+pipeline[1].r2value;
         // cout<<"ADD 1_"<<pipeline[1].r1value<<" 2_"<<pipeline[1].r2value<<endl;
         pipeline[2].r1num=pipeline[1].r1num;
         pipeline[2].r2num=pipeline[1].r2num;
         pipeline[2].rdnum=pipeline[1].rdnum;
         pipeline[2].r1value=pipeline[1].r1value;
         pipeline[2].offset=pipeline[1].offset;
         pipeline[2].r2value=pipeline[1].r2value;
         pipeline[2].rdvalue=pipeline[1].rdvalue;
         pipeline[2].operation=pipeline[1].operation;
         pipeline[2].check=pipeline[1].check;
         pipeline[2].output=pipeline[1].output;
         pipeline[2].loopindex=pipeline[1].loopindex;
         pipeline[2].loop=pipeline[1].loop;
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         // }   
         // if(freezeAdd<l_add-1){
         //    freezer=2;
         //    freezeAdd++;
         // }
         // else
            // freezer=0;
         pipeline[1].inst="";
         // freeze(l_add-1);
         freezer+=l_add-1;


         
         // cout<<"Add Instruction in stage 3 of the pipeline."<<endl;
         
         // pipeline[2].inst=tempinst;            // cout<<pipeline[2].result<<endl;
      break;
      case 2://For MUL

         // int r1n,r2n;
         r1n=pipeline[1].r1num;
         r2n=pipeline[1].r2num;
         if(pipeline[2].rdnum==r1n && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r1value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r1n && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r1value=r[r1n];
            }
         }
         if(pipeline[2].rdnum==r2n && pipeline[1].check==0 && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r2value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r2n  && pipeline[1].check==0 && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r2value=r[r2n];
            }
         }

         // freezer=2;
         

         // cout<<"Mul Instruction in stage 3 of the pipeline."<<endl;
         // pipeline[2]=pipeline[1];
         pipeline[2].result=pipeline[1].r1value*pipeline[1].r2value;
         pipeline[2].r1num=pipeline[1].r1num;
         pipeline[2].r2num=pipeline[1].r2num;
         pipeline[2].rdnum=pipeline[1].rdnum;
         pipeline[2].r1value=pipeline[1].r1value;
         pipeline[2].r2value=pipeline[1].r2value;
         pipeline[2].offset=pipeline[1].offset;
         pipeline[2].rdvalue=pipeline[1].rdvalue;
         pipeline[2].operation=pipeline[1].operation;
         pipeline[2].check=pipeline[1].check;
         pipeline[2].output=pipeline[1].output;
         pipeline[2].loopindex=pipeline[1].loopindex;
         pipeline[2].loop=pipeline[1].loop;
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         pipeline[1].inst="";
         // freeze(l_mul-1);
         freezer+=l_mul-1;
            // cout<<pipeline[2].result<<endl;
      break;
      case 3://For SUB
         // int r1n,r2n;
         r1n=pipeline[1].r1num;
         r2n=pipeline[1].r2num;
         if(pipeline[2].rdnum==r1n && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r1value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r1n && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r1value=r[r1n];
            }
         }
         if(pipeline[2].rdnum==r2n && pipeline[1].check==0 && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r2value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r2n  && pipeline[1].check==0 && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r2value=r[r2n];
            }
         }   

         // freezer=2;
         

         // cout<<"Sub Instruction in stage 3 of the pipeline."<<endl;
         // pipeline[2]=pipeline[1];
         pipeline[2].result=pipeline[1].r1value-pipeline[1].r2value;
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
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         pipeline[1].inst="";
         // freeze(l_sub-1);
         freezer+=l_sub-1;
            // cout<<pipeline[2].result<<endl;
      break;
      case 4://For MOV
         // pipeline[2]=pipeline[1];
      	r1n=pipeline[1].r1num;
         r2n=pipeline[1].r2num;
         if(pipeline[2].rdnum==r1n && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r1value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r1n && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r1value=r[r1n];
            }
         }
         if(pipeline[2].rdnum==r2n && pipeline[1].check==0 && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r2value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r2n && pipeline[1].check==0 && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r2value=r[r2n];
            }
         }
         // freezer=2;
         
         // cout<<"Mov Instruction in stage 3 of the pipeline."<<endl;
         pipeline[2].result=pipeline[1].r2value;
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
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         pipeline[1].inst="";
         // freeze(l_mov-1)
         freezer+=l_mov-1;
            // cout<<pipeline[2].result<<endl;
      break;
      case 5://CMP
         // pipeline[2]=pipeline[1];
         r1n=pipeline[1].r1num;
         r2n=pipeline[1].r2num;
         if(pipeline[2].rdnum==r1n && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r1value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r1n && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r1value=r[r1n];
            }
         }
         if(pipeline[2].rdnum==r2n && pipeline[1].check==0 && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r2value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r2n && pipeline[1].check==0 && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r2value=r[r2n];
            }
         }


         // freezer=2;
         

         // cout<<"Cmp Instruction in stage 3 of the pipeline."<<endl;
         pipeline[2].result=pipeline[1].r1value-pipeline[1].r2value;
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
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         pipeline[1].inst="";
         // freeze(l_sub-1);
         freezer+=l_sub-1;
            break;// cout<<pipeline[2].result<<endl;
      case 6:
      // cout<<"Load Instruction in stage 3 of the pipeline."<<endl;
         // r1n=pipeline[1].r1num;
         r2n=pipeline[1].r2num;
         // if(pipeline[2].rdnum==r1n && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         // {  
         //    pipeline[1].r1value=pipeline[2].result;
         // }
         // else
         // {
         //    if(pipeline3Dest==r1n && pipeline3Operation!=7 && pipeline3Operation!=7)
         //    {  
         //       pipeline[1].r1value=r[r1n];
         //    }
         // }
         if(pipeline[2].rdnum==r2n && (pipeline[1].check==0 || pipeline[1].check==2) && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r2value=pipeline[2].result;
         }
         else
         {
            if(pipeline3Dest==r2n  && (pipeline[1].check==0 || pipeline[1].check==2) && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r2value=r[r2n];
            }
         }

         // freezer+=1;
         

         pipeline[2].result=pipeline[1].r2value+pipeline[1].offset;
         // cout<<"Pipline 2 result "<<pipeline[2].result<<endl;
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
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         pipeline[1].inst="";
         freezer+=l_add-1;
         // freeze(l_add-1);
      break;
      case 7:
      	 r1n=pipeline[1].rdnum;
         r2n=pipeline[1].r2num;
         // cout<<"r2n for store "<<pipeline[1].r1num<<endl;
         if(pipeline[2].rdnum==r1n && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r1value=pipeline[2].result;
            // cout<<"1111"<<endl;
         }
         else
         {
            if(pipeline3Dest==r1n && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r1value=r[r1n];
               // cout<<"2222"<<endl;
            }
         }
         if(pipeline[2].rdnum==r2n && (pipeline[1].check==0 || pipeline[1].check==2) && pipeline[2].operation!=7 && pipeline[2].operation!=6)
         {  
            pipeline[1].r2value=pipeline[2].result;
            // cout<<"3333"<<endl;
         }
         else
         {
            if(pipeline3Dest==r2n  && (pipeline[1].check==0 || pipeline[1].check==2) && pipeline3Operation!=7 && pipeline3Operation!=7)
            {  
               pipeline[1].r2value=r[r2n];
               // cout<<"4444"<<endl;
            }
         }

         // freezer+=1;
         // 

         // cout<<"R2 value for store "<<pipeline[1].r2value<<endl;
         // cout<<"Offset value for store "<<pipeline[1].offset<<endl;
         // cout<<"Store Instruction in stage 3 of the pipeline."<<endl;
         pipeline[2].result=pipeline[1].r2value+pipeline[1].offset;
         pipeline[2].r1num=pipeline[1].r1num;
         // cout<<"Pipline 2 result "<<pipeline[2].result<<endl;
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
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         pipeline[1].inst="";
         // freeze(l_add-1);
         freezer+=l_add-1;
      break;
      case 8:
         // loop[pipeline[1].loopindex].getStart();
         // cout<<"BNE Instruction in stage 3 of the pipeline."<<endl;
         pipeline[2].result=loop[pipeline[1].loopindex].getStart();
         // cout<<"Stage 3 Happened for BNE "<<pipeline[2].result<<" "<<loop[pipeline[1].loopindex].getStart()<<endl;
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
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         break;

      case 9:
         pipeline[2].result=loop[pipeline[1].loopindex].getStart();
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
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         break;

      case 10:
         pipeline[2].result=loop[pipeline[1].loopindex].getStart();
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
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         break;
      case 11:
         pipeline[2].result=loop[pipeline[1].loopindex].getStart();
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
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         break;
      case 12:
         pipeline[2].result=loop[pipeline[1].loopindex].getStart();
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
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
         break;
      case 0:
         pipeline[2].result=pipeline[1].result;
         // cout<<"ADD 1_"<<pipeline[1].r1value<<" 2_"<<pipeline[1].r2value<<endl;
         pipeline[2].r1num=pipeline[1].r1num;
         pipeline[2].r2num=pipeline[1].r2num;
         pipeline[2].rdnum=pipeline[1].rdnum;
         pipeline[2].r1value=pipeline[1].r1value;
         pipeline[2].offset=pipeline[1].offset;
         pipeline[2].r2value=pipeline[1].r2value;
         pipeline[2].rdvalue=pipeline[1].rdvalue;
         pipeline[2].operation=pipeline[1].operation;
         pipeline[2].check=pipeline[1].check;
         pipeline[2].output=pipeline[1].output;
         pipeline[2].loopindex=pipeline[1].loopindex;
         pipeline[2].loop=pipeline[1].loop;
         pipeline[2].instruction=pipeline[1].instruction;
         pipeline[2].inst=pipeline[1].inst;
      break;
   }
   
   //Stage2

      pipeline[1]=pipeline[0];
      // cout<<pipeline[1].operation<<" is in Stage 2"<<endl;
      if(pipeline[1].r1num>=0)
         pipeline[1].r1value=r[pipeline[1].r1num];
      if(pipeline[1].check==0 && pipeline[1].r2num>=0)
         pipeline[1].r2value=r[pipeline[1].r2num];
      else
      {     if(pipeline[1].check==1){
               pipeline[1].r2value=pipeline[1].r2num;
               if(pipeline[1].operation==6 || pipeline[1].operation==7)
               {
                  pipeline[1].r2value=0;
               }
            }else
            {
               pipeline[1].r2value=r[pipeline[1].r2num];  
               // pipeline[1].offset=
            }
      }
      if(pipeline[1].operation==6)
      {
                      bool first=false;
                        // pipeline[0].operation=6;
                        if(programCounter+1<Index)
                        {
                           if(inst.getR1()==pipeline[1].rdnum)
                           {
                              // int N;
                              if(printer==0){
                              cout<<"Enter Choice (Inside Load use hazard) for next instruction"<<endl;
                                 cin>>printer;
                              pipeline[0].rdnum=-1;
                              pipeline[0].r2num=-1;
                              pipeline[0].check=0;
                              pipeline[0].operation=0;
                              pipeline[0].offset=0;
                              pipeline[0].rdvalue=0;
                              pipeline[0].r1value=0;
                              pipeline[0].r2value=0;
                              pipeline[0].result=0;
                              pipeline[0].output=0;
                              pipeline[0].loop="";
                              pipeline[0].inst="NOP";
                              pipeline[0].loopindex=-1;
                              
                                 if(printer==0)
                                 {
                                    // cout<<(counter++)<<endl;
                                    pipelineInfo();
                                    show();
                                    cout<<"------------------------"<<endl;
                                 }
                              }
                                 nop++;
                                 first=true;
                                 loaduse=0;
                                 pipeline[0].r1num=-1;
                                 operate(9999);
                                 loaduse=1;
                           }
                           else
                           {   if(inst.getR2()==pipeline[0].rdnum && inst.getCheck()==0)
                              {
                                 // int N;
                                 if(printer==0){
                                 cout<<"Enter Choice (Inside Load use hazard)for next instruction"<<endl;
                                 cin>>printer;
                                 pipeline[0].rdnum=-1;
                                 pipeline[0].r2num=-1;
                                 pipeline[0].check=0;
                                 pipeline[0].operation=0;
                                 pipeline[0].offset=0;
                                 pipeline[0].rdvalue=0;
                                 pipeline[0].r1value=0;
                                 pipeline[0].r2value=0;
                                 pipeline[0].result=0;
                                 pipeline[0].output=0;
                                 pipeline[0].loop="";
                                 pipeline[0].inst="NOP";
                                 pipeline[0].loopindex=-1;
   // cout<<"Before 9999"<<endl;
                                 if(printer==0)
                                 {
                                    // cout<<(counter++)<<endl;
                                    pipelineInfo();
                                    show();
                                    cout<<"------------------------"<<endl;
                                 }
                              }
                              first=true;
                              nop++;
                              loaduse=0;
                              ;
                                 operate(9999);
                                 loaduse=1;
                              }
                              else
                              {
                                 if(inst.getDestination()==pipeline[0].rdnum && (inst.getType()=="STR" || inst.getType()=="CMP" ))
                                 {
                                    if(printer==0){
                                 cout<<"Enter Choice (For compare or store )for next instruction"<<endl;
                                 cin>>printer;
                                 pipeline[0].rdnum=-1;
                                 pipeline[0].r2num=-1;
                                 pipeline[0].check=0;
                                 pipeline[0].operation=0;
                                 pipeline[0].offset=0;
                                 pipeline[0].rdvalue=0;
                                 pipeline[0].r1value=0;
                                 pipeline[0].r2value=0;
                                 pipeline[0].result=0;
                                 pipeline[0].output=0;
                                 pipeline[0].loop="";
                                 pipeline[0].inst="NOP";
                                 pipeline[0].loopindex=-1;
   // cout<<"Before 9999"<<endl;
                                 if(printer==0)
                                 {
                                    // cout<<(counter++)<<endl;
                                    pipelineInfo();
                                    show();
                                    cout<<"------------------------"<<endl;
                                 }
                              }
                              first=true;
                              nop++;
                              loaduse=0;
                              ;
                                 operate(9999);
                                 loaduse=1;
                                 }
                              }
                           }
                     }
                        
      }
      if(pipeline[0].loop!="")
      {
      		bool flag=false;
         for(int lindex=0;lindex<indexLoop;lindex++)
         {
            if(loop[lindex].getName()==pipeline[0].loop)
            {
            	flag=true;
               pipeline[1].loopindex=lindex;
               break;
            }  
         }
         if(flag==false)
         {
         	cout<<pipeline[0].loop<<" doesn't exist."<<endl;
         	pipeline[1].result=0;
            pipeline[1].r1num=-1;
            pipeline[1].r2num=-1;
            pipeline[1].rdnum=-1;
            pipeline[1].r1value=0;
            pipeline[1].r2value=0;
            pipeline[1].rdvalue=0;
            pipeline[1].operation=0;
            pipeline[1].check=0;
            pipeline[1].output=0;
            pipeline[1].offset=0;
            pipeline[1].loopindex=-1;//pipeline[1].loopindex;
            pipeline[1].loop="";//pipeline[1].loop;
         // pipeline[1].instruction=pipeline[1].instruction;
         }
         else
         {
            if(pipeline[0].operation==11)
            {
                  programCounter=loop[pipeline[1].loopindex].getStart();
                  // programCounter--;
                  inst=instruction[programCounter];
                  pipeline[0].operation=0;
                  freezer+=l_branch-1;
                  // freeze(l_branch-1);
                  // if(printer==0)
                  // pipelineInfo();
                  // return;
                  
            }
            else
            {
               if(pipeline[0].operation==12)
               {
                     linkreg=programCounter;
                     programCounter=loop[pipeline[1].loopindex].getStart();
                     // programCounter--;
                     inst=instruction[programCounter];
                     pipeline[0].operation=0;
                     // freeze(l_bl-1);
                     freezer+=l_bl-1;
                     // if(printer==0)
                     // pipelineInfo();
                     // return;
                     
               }  
            }
         }
      }
   

      pipeline[0].inst="";
   freeze(freezer-1);
   freezer=0;   //Stage1

   if(decide!=9999)
   {
      instructionCount++;
      pipeline[0].rdnum=inst.getDestination();
      pipeline[0].r1num=inst.getR1();
      pipeline[0].r2num=inst.getR2();
      pipeline[0].check=inst.getCheck();
      pipeline[0].offset=inst.getOffset();
      string type=inst.getType();
      pipeline[0].r1value=0;
      pipeline[0].r2value=0;
      pipeline[0].loop=inst.getLoop();
      pipeline[0].instruction=decide;
      pipeline[0].inst=inst.getInst();
      if(type=="ADD")
      {
         pipeline[0].operation=1;
         // cout<<"Add instruction inserted in the pipeline."<<endl;
      }
      else
      {
         if (type=="MUL")
         {
            pipeline[0].operation=2; 
            // cout<<"Mul instruction inserted in the pipeline."<<endl;    
         }
         else
         {
            if (type=="SUB")
            {
               pipeline[0].operation=3;
            }
            else
            {
               if (type=="MOV")
               {
                  pipeline[0].operation=4;
                  // cout<<"Mov instruction inserted in the pipeline."<<endl;
               }
               else
               {
                  if (type=="CMP")
                  {
                     pipeline[0].operation=5;
                     pipeline[0].r1num=pipeline[0].rdnum;
                     // cout<<"Cmp instruction inserted in the pipeline."<<endl;
                  }
                  else
                  {
                     if (type=="LDR")
                     {
                        // bool first=false;
                        pipeline[0].operation=6;
                        // if(programCounter+1<Index)
                        // {
                        //    if(inst1.getR1()==pipeline[0].rdnum)
                        //    {
                        //       // int N;
                        //       if(printer==0){
                        //       cout<<"Enter Choice for next instruction"<<endl;
                        //          cin>>printer;
                        //          if(printer==0)
                        //          {
                        //             // cout<<(counter++)<<endl;
                        //             pipelineInfo();
                        //             show();
                        //             cout<<"------------------------"<<endl;
                        //          }
                        //       // // show();
                        //       }
                        //          nop++;
                        //          first=true;
                        //          loaduse=0;
                        //       operate(9999);
                        //       loaduse=1;
                        //    }
                        //    else
                        //       if(inst1.getR2()==pipeline[0].rdnum && inst1.getCheck()==0)
                        //       {
                        //          // int N;
                        //          if(printer==0){
                        //          cout<<"Enter Choice for next instruction"<<endl;
                        //          cin>>printer;
                        //          if(printer==0)
                        //          {
                        //             // cout<<(counter++)<<endl;
                        //             pipelineInfo();
                        //             show();
                        //             cout<<"------------------------"<<endl;
                        //          }
                        //       }
                        //       first=true;
                        //       nop++;
                        //       loaduse=0;
                        //          operate(9999);
                        //          loaduse=1;
                        //       }
                        // }
                        // if(programCounter+2<Index && first==false)
                        // {
                        //    if(inst2.getR1()==pipeline[0].rdnum)
                        //    {
                        //       // int N;
                        //       // if(printer==0){
                        //       // cout<<"Enter Choice for next instruction"<<endl;
                        //       //    cin>>printer;
                        //       //    if(printer==0)
                        //       //    {
                        //       //       // cout<<(counter++)<<endl;
                        //       //       pipelineInfo();
                        //       //       show();
                        //       //       cout<<"------------------------"<<endl;
                        //       //    }
                        //       // }
                        //       operate(programCounter+1);
                        //       if(printer==0){
                        //       cout<<"Enter Choice() for next instruction"<<endl;
                        //          cin>>printer;
                        //          if(printer==0)
                        //          {
                        //             // cout<<(counter++)<<endl;
                        //             pipelineInfo();
                        //             show();
                        //             cout<<"------------------------"<<endl;
                        //          }
                        //       }
                        //       loaduse=0;
                        //       operate(9999);
                        //       loaduse=1;
                        //       nop++;
                        //       increment=true;
                        //       // operate(900);
                        //    }
                        //    else
                        //       if(inst2.getR2()==pipeline[0].rdnum && inst2.getCheck()==0)
                        //       {
                        //          // int N;
                        //       //    if(printer==0){
                        //       //    cout<<"Enter Choice for next instruction"<<endl;
                        //       //    cin>>printer;
                        //       //    if(printer==0)
                        //       //    {
                        //       //       // cout<<(counter++)<<endl;
                        //       //       pipelineInfo();
                        //       //       show();
                        //       //       cout<<"------------------------"<<endl;
                        //       //    }
                        //       // }

                        //       operate(programCounter+1);
                        //       if(printer==0){
                        //       cout<<"Enter Choice for next instruction"<<endl;
                        //          cin>>printer;
                        //          if(printer==0)
                        //          {
                        //             // cout<<(counter++)<<endl;
                        //             pipelineInfo();
                        //             show();
                        //             cout<<"------------------------"<<endl;
                        //          }
                        //       }
                        //       loaduse=0;
                        //       operate(9999);
                        //       loaduse=1;
                        //       nop++;
                        //       increment=true;
                        //          // operate(900);
                        //       }
                        // }  
                        // pipeline[0].r1num=pipeline[0].rdnum;
                     }
                     else
                     {
                        if (type=="STR")
                        {
                           pipeline[0].operation=7;
                           // pipeline[0].rdnum=inst.getDestination();
      // cout<<" R1 Store "<< pipeline[0].r1num;
      // cout<<" Rd Store "<< pipeline[0].rdnum;
      
                     // cout<<"R2 Store "<<pipeline[0].r2num;//=inst.getR2();

                                                    
                              // pipeline[0].r1num=pipeline[0].rdnum;
                        }
                        else
                        {
                           if (type=="BNE")
                           {
                              pipeline[0].operation=8;
                              // cout<<"Bne instruction inserted in the pipeline."<<endl;
                              // cout<<"Stage 1 Happened for BNE"<<endl;                              // pipeline[0].r1num=pipeline[0].rdnum;
                           }
                           else
                           {   
                              if (type=="BGE")
                              {
                                 pipeline[0].operation=9;
                                 // cout<<"BGE instruction inserted in the pipeline."<<endl;                              // pipeline[0].r1num=pipeline[0].rdnum;
                              }  
                              else
                              {   
                                 if (type=="BLE")
                                 {
                                    pipeline[0].operation=10;
                                    // cout<<"BLE instruction inserted in the pipeline."<<endl;                              // pipeline[0].r1num=pipeline[0].rdnum;
                                 }
                                 else
	                              {   
	                                 if (type=="B")
	                                 {
	                                    pipeline[0].operation=11;
	                                    // cout<<"B instruction inserted in the pipeline."<<endl;                              // pipeline[0].r1num=pipeline[0].rdnum;
	                                 }
	                                 else
		                              {   
		                                 if (type=="BL")
		                                 {
		                                    pipeline[0].operation=12;                              // pipeline[0].r1num=pipeline[0].rdnum;
		                                 }
		                                 else { 
		                                 pipeline[0].operation=0;
		                                 // cout<<"Nop Instruction inserted in the pipeline."<<endl;
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
   else
   {
      pipeline[0].rdnum=-1;
      pipeline[0].r1num=-1;
      pipeline[0].r2num=-1;
      pipeline[0].r1value=0;
      pipeline[0].r2value=0;
      pipeline[0].check=0;
      pipeline[0].operation=0;
      pipeline[0].loop="";
      pipeline[0].loopindex=-1;
      pipeline[0].instruction=-1;
      pipeline[0].inst="NOP";
   }
   if(printer==0 && (decide!=9999 || (decide==9999 && loaduse==1)))
   {
         cout<<"Enter Choice for next instruction"<<endl;
         cin>>printer;
      
      if(printer==0)
      {
         // cout<<"This is IF"<<endl;
         pipelineInfo();
         show();

         cout<<"------------------------"<<endl;
      }
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
   for(programCounter=0;programCounter<Index+4+nop;programCounter++)
   {  
      if(programCounter<Index){
      operate(programCounter);
      if(increment)
         programCounter++;   
      }
      else
      operate(9999);
      // if(printer==0)
      // {
      //    cout<<"Enter Choice for next instruction"<<endl;
      //    cin>>printer;
      
      // if(printer==0)
      // {
      //    // cout<<(counter++)<<endl;
      //    show();
      //    cout<<"------------------------"<<endl;
      // }}      
   }
   if(printer==1)
   {cout<<"Final output is: "<<endl;
   show();
   }
   getInfo();
}
