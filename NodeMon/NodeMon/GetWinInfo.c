// GetWinInfo.c : Gets information from the Windows OS
//
//#include "stdafx.h"
#include "GetNodeInfo.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

#define NODEOS "Windows" //name of OS


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
	Sleep((DWORD)milliSeconds);
}


/* 
	gets the name of a node
 */
void GetNodeName(char *nodeName)
{
	TCHAR  infoBuf[MAXLENGTH];
	DWORD  bufCharCount;
	bufCharCount = MAXLENGTH;

	GetComputerName( infoBuf, &bufCharCount );
	strcpy(nodeName, infoBuf);
}


/*
	gets the actual information about processes
 */
void GetProcsInfo(PROCESSINFORMATION *procInfo, int *numberProcs)
{
		
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
} //GetProcessInfo
