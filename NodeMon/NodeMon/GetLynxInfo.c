// GetLynxInfo.c : Gets information from LynxOS
//
// author: Dietrich Beck, GSI
// 20-Oct-2004 created 
//
// "ps" gets its information from /dev/mem. Unfortunately, the format
// of /dev/mem differs between different lynx versions. So we decided
// to parse the output of the "ps" command to acquire the information
// required. Since we compiled DIM as single threaded we can not use
// the "system" command since this resulted in lots of zombie shells.
// Therefore, the information of ps is created via the shell script
// "lynxProcsInfo". This shell script should be started together 
// with NodeMon by using the "lynxStartMon" script. The script must
// be started from the directory ../NodeMon/lynxNNY (NNY = 86x or 
// 68m for the different architectures). The script "lynxProcsInfo"
// will periodically execute the command "ps axf > TEMPFILE", where
// TEMPFILE should be replaced with the filename as defined below.
//
//
#include "GetNodeInfo.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define NODEOS "LynxOS       "//OS name
#define TEMPFILE "procs.txt"  //name for tempfile


/*
        gets the OS
*/
void GetNodeOS(char *OS)
{
  strcpy(OS, NODEOS);
}


/* 
	causes a thread to sleep
 */
void NodeSleep(int milliSeconds)
{
  int sleepTime;

  sleepTime = (int)((double)milliSeconds/1000.0);
  sleep(sleepTime);
}


/* 
	gets the name of a node
 */
void GetNodeName(char *nodeName)
{
	char  infoBuf[MAXLENGTH];
	char  *pointer;

	gethostname(infoBuf, MAXLENGTH);
	if (pointer = strchr(infoBuf,'.')) *pointer = '\0';
	strcpy(nodeName, infoBuf);
}


/*
	gets the actual information about processes
 */
void GetProcsInfo(PROCESSINFORMATION *procInfo, int *numberProcs)
{
  FILE *f;
  char  fname[MAXLENGTH];
  char  cmd[MAXLENGTH];
  char  buffer[1024];
  int   nfields = 12;
  int   i;
  float num1;
  unsigned int num2, num3;
  int   plusflag, collonflag, periodflag;
  
  int   pid, ppid, pgrp, pri, stk, data;
  char  text[1024], time[1024], flags[1024], user[1024];
  char  S[1024], name[1024];

  sprintf(fname, TEMPFILE);
// write file periodically in separate bash script "lynxProcsInfo" as described
// on top of this file.

  f = fopen(fname, "r");
  if (!f) return;
  
  //skip first two lines, header and null process
  for (i=0; i<nfields;i++) fscanf(f, "%s", buffer);
  for (i=0; i<nfields;i++) fscanf(f, "%s", buffer);

  *numberProcs = 0;
  while (*numberProcs < MAXPROCS - 1) 
  {
 
    if (nfields == fscanf(f, "%d %d %d %d %s %d %d %s %s %s %s %s",
	        &pid, &ppid, &pgrp, &pri, &text, &stk, &data,
	        &time, &flags, &user, &S, &name)) 
    {
      //name
      strcpy(procInfo[*numberProcs].processName, name);
      
      //pid
      procInfo[*numberProcs].processID = pid;
      
      //mem
      sscanf(text, "%d", &(procInfo[*numberProcs].processMem));
      procInfo[*numberProcs].processMem += data + stk;

      //runtime
      plusflag   = 0;
      collonflag = 0;
      periodflag = 0;
      for (i=0;i<strlen(time);i++)
      {
	if (ispunct(time[i])) 
	{
	  if (time[i] == '+') plusflag++; 
	  if (time[i] == ':') collonflag++;
          if (time[i] == '.') periodflag++;
	} //if ispunct
      } // for i
      if (periodflag) 
      {
        sscanf(time,"%f", &num1);
        num1 = num1 * TICKS_SECOND;
        num2 = 0;
        num3 = 0;
      }
      if (collonflag == 1)
      {
        sscanf(time,"%d:%d", &num2, &num3);
        num1 = 0;
        num2 = num2 * 60 * TICKS_SECOND;
        num3 = num3 * TICKS_SECOND;
      }
      if (collonflag == 2)
      {
         sscanf(time,"%f:%d:%d", &num1, &num2, &num3);
	 num1 = num1 * 3600 * TICKS_SECOND;
	 num2 = num2 * 60 * TICKS_SECOND;
	 num3 = num3 * TICKS_SECOND;
      }
      if (plusflag)
      {
         sscanf(time,"%f %d:%d", &num1, &num2, &num3);
	 if (num1 > 48) num1 = 48;
         num1 = num1 * 24 * 3600 * TICKS_SECOND;
         num2 = num2 * 3600 * TICKS_SECOND;
         if (collonflag) num3 = num3 * 60 * TICKS_SECOND;
         else num3 = 0;
      }
      procInfo[*numberProcs].processRunTime = num1 + num2 + num3;
      (*numberProcs)++;
    } //if !fscanf
    else break;
  } // while
  fclose(f);      

} //GetProcessInfo




