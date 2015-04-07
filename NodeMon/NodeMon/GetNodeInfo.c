// GetNodeInfo.c : Gets information from the Operating system
// author: Dietrich Beck, GSI
// history: parts of code originate from "pcmon" by Clara Gaspar, CERN.
// 15-Oct-2004 created
// 18-Oct-2004 bug fixed
// 20-OCT-2004 another bug fixed
// 12-JUL-2006 makefile (Linux) fixed: code does of course not require DIM

//include standard stuff
#include <stdio.h>

//include my stuff
#include "GetNodeInfo.h"

//include OS specific stuff
#ifdef WIN32
#include <tchar.h>
#include <windows.h>
#include <psapi.h>
#endif
#ifdef LINUX
#include <unistd.h>
#include <string.h>
#include <glob.h>
#endif

//define constants
#ifdef WIN32
#define NODEOS "WIN32" //name of OS
#endif
#ifdef LINUX
#define PAGE_SIZE 4096   //page size
#define CONSTF    100    //fudge factor converting cpu time from OS into seconds
#define NODEOS "LINUX"   //OS name
#endif


//-----------------------------------------------------------------------------------
void GetNodeOS(char *OS)
{
  sprintf(OS, "%s", NODEOS);
} //GetNodeOS


//-----------------------------------------------------------------------------------
void NodeSleep(unsigned int  milliSeconds)
{
#ifdef WIN32
	Sleep((DWORD)milliSeconds);
#endif
#ifdef LINUX
	sleep((int)((double)milliSeconds/1000.0));
#endif
} //NodeSleep


//-----------------------------------------------------------------------------------
void GetNodeName(char *nodeName)
{
#ifdef WIN32
	TCHAR  infoBuf[MAXLENGTH];
	DWORD  bufCharCount;
	bufCharCount = MAXLENGTH;
#endif
#ifdef LINUX
	char  infoBuf[MAXLENGTH];
	char  *pointer;
#endif

#ifdef WIN32
	GetComputerName( infoBuf, &bufCharCount );
#endif
#ifdef LINUX
	gethostname(infoBuf, MAXLENGTH);
	if (pointer = strchr(infoBuf,'.')) *pointer = '\0';
#endif

	strcpy(nodeName, infoBuf);
} //GetNodeName


#ifdef LINUX
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
} //GetIdentifiers
#endif

#ifdef LINUX
/*
        gets the name of a process
*/
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
} //GetName
#endif

#ifdef LINUX
/*
        gets cpu time and memory
*/
void GetCPUandMem(int pid, unsigned int *runtime, int *mem)
{
  char   fname[MAXLENGTH];
  FILE   *f;
  float  cputime;

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
  cputime  = (float)(tms_stime + tms_utime)/(float)CONSTF;
  *runtime = cputime * (float)TICKS_SECOND;
} //GetCPUandMem
#endif

//-----------------------------------------------------------------------------------
void GetProcsInfo(PROCESSINFORMATION *procInfo, int *numberProcs)
{
#ifdef WIN32
	DWORD aProcesses[MAXPROCS], cbNeeded, cProcesses;
	unsigned int i;
	HMODULE hMod;
	TCHAR szProcessName[MAXLENGTH];
	PROCESS_MEMORY_COUNTERS pmc;
	FILETIME CreationTime, ExitTime, KernelTime, UserTime;
	LARGE_INTEGER userT, kernelT;
	
	// Get the list of process identifiers.
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
	if (cProcesses > MAXPROCS) cProcesses = MAXPROCS;

	*numberProcs = -1;
	//populate procInfo array
	for (i=0; i < cProcesses; i++)
	{
		// Get a handle to the process.
	    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
		                               PROCESS_VM_READ,
			                           FALSE, aProcesses[i] );
		
		if (NULL != hProcess ) 
		{
			(*numberProcs)++;
			
			// Get the process name.
			strcpy(szProcessName, "<unknown>");
	        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
			{
			    GetModuleBaseName( hProcess, hMod, szProcessName, 
				                   sizeof(szProcessName)/sizeof(TCHAR) );	
			}
			strcpy(procInfo[*numberProcs].processName, szProcessName);
			
			
			//PID
			procInfo[*numberProcs].processID = (int)aProcesses[i];

			//Get process memory usage
			if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
			{
				procInfo[*numberProcs].processMem = (int)((double)pmc.WorkingSetSize/1024.0);
			}
			else procInfo[*numberProcs].processMem = -1;

			//GetProcessTime	
			if ( GetProcessTimes(hProcess, &CreationTime, &ExitTime, 
				                 &KernelTime, &UserTime) )
			{
				userT.LowPart    = UserTime.dwLowDateTime;
				userT.HighPart   = UserTime.dwHighDateTime; 
				kernelT.LowPart  = KernelTime.dwLowDateTime;
				kernelT.HighPart = KernelTime.dwHighDateTime;
				procInfo[*numberProcs].processRunTime = 
					(unsigned int)(((double)(userT.QuadPart)+(double)(kernelT.QuadPart))/10000000.0*(double)TICKS_SECOND);
			}
			else procInfo[*numberProcs].processRunTime = -1;

			//close handle to process
			CloseHandle( hProcess );
		} // if NULL
	} //for i
	(*numberProcs)++;
#endif
#ifdef LINUX
	int pid[MAXPROCS];
	unsigned int i;
	char processName[MAXLENGTH];
	int mem;
	unsigned int runtime;
	char message[128];
	
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
	  //sprintf(message, "hallo %d\n", runtime);
	  //printf(message);
	  //printf("hallo\n");
	  procInfo[i].processRunTime = runtime;
	} //for i
#endif
} //GetProcessInfo
