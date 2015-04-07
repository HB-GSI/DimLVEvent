// GetLinuxInfo.c : Gets information from the Linux OS
//
// author: Dietrich Beck, GSI
// history: parts of code originate from "pcmon" by Clara Gaspar, CERN.
// 15-Oct-2004 created
// 18-Oct-2004 bug fixed
// 20-OCT-2004 another bug fixed
//
#include "GetNodeInfo.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>

#define PAGE_SIZE 4096   //page size
#define CONSTF    100    //fudge factor converting cpu time from OS into seconds
#define NODEOS "Linux"   //OS name


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
	sleep((int)((double)milliSeconds/1000.0));
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
        gets the identifiers of all processes
*/
void GetIdentifiers(int *Pids, int *nProcs)
{
        char name[1024];
	glob_t globbuf;
	int i, id, ret, nrprocs;
	char search[64];

	strcpy(search,"/proc/*/stat");
	glob(search, GLOB_NOSORT, NULL, &globbuf);
	if (globbuf.gl_pathc <= MAXPROCS) nrprocs = globbuf.gl_pathc;
	else nrprocs = MAXPROCS;
	for (i = 0; i < nrprocs; i++) 
	{
		strcpy(name,globbuf.gl_pathv[globbuf.gl_pathc - i - 1]);
		sscanf(&name[6], "%d", &id);
		Pids[i] = id;
	}
	globfree(&globbuf);

	*nProcs = i; 
}

void GetName(int pid, char *name)
{
        FILE *f;
        int i;
        char fname[1024], c;

	strcpy(name,"N/A");

	sprintf(fname,"/proc/%d/cmdline",pid);
	f = fopen(fname, "r");
	if (!f) 
	{
		return;
	}
	i = 0;
	while ((i < (MAXLENGTH - 1)) && ((c = fgetc(f)) != EOF)) 
	{
		name[i++] = (c == '\0') ? ' ' : c;
	}
	if (i > 0) name[i] = '\0';
	fclose(f);
}

/*
        gets cpu time and memory
*/
void GetCPUandMem(int pid, unsigned int *runtime, int *mem)
{
  char   fname[MAXLENGTH];
  FILE   *f;
  int    cputime;

  // dummies for reading
  char pname[MAXLENGTH];
  char state[MAXLENGTH];
  int ppid, pgid, session, tty, tty_pgid, flags, min_flt, cmin_flt, maj_flt, cmaj_fl,
    tms_cutime, tms_cstime, priority, nice, nothing, it_real_value, start_time, vsiz;      
  // now comes the interesting stuff
  int tms_utime, tms_stime;
  int rss;

  // read stuff from file
  sprintf(fname,"/proc/%d/stat",pid);
  f = fopen(fname, "r");
  if (!f) 
  {
    return;
  }
  fscanf(f,"%d",&pid);
  fscanf(f,"%s %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 pname,state,&ppid,
	 &pgid,&session,&tty,&tty_pgid,
	 &flags,&min_flt,&cmin_flt,&maj_flt,
	 &cmaj_fl,&tms_utime,&tms_stime,&tms_cutime,
	 &tms_cstime,&priority,&nice,&nothing,
	 &it_real_value,&start_time,&vsiz,&rss);
  fclose(f);

  // calculate memory
  *mem = rss * PAGE_SIZE / 1024;
  
  // calculate runtime
  cputime  = (int)((float)(tms_stime + tms_utime)/(float)CONSTF);
  *runtime = cputime * (float)TICKS_SECOND;
}

/*
	gets the actual information about processes
 */
void GetProcsInfo(PROCESSINFORMATION *procInfo, int *numberProcs)
{
		
	int pid[MAXPROCS];
	unsigned int i;
	char processName[MAXLENGTH];
	int mem;
	unsigned int runtime;
	
	// Get the list of process identifiers.
	GetIdentifiers(pid, numberProcs);

        // Calculate how many process identifiers were returned.
	if (*numberProcs > MAXPROCS) *numberProcs = MAXPROCS;

	//populate procInfo array
	for (i=0; i < *numberProcs; i++)
	{
	  // Get the process name.
	  GetName(pid[i], processName);
	  strcpy(procInfo[i].processName, processName);
			
      	  //PID
	  procInfo[i].processID = pid[i];

	  //Runtime and memory
	  GetCPUandMem(pid[i], &runtime, &mem);
	  procInfo[i].processMem     = mem;
	  procInfo[i].processRunTime = runtime;
	} //for i
} //GetProcessInfo
