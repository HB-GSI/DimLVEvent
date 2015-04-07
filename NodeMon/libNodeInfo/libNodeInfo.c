// libNodeInfo.c : Defines the entry point for the DLL application.
//
//
// author  : Dietrich Beck, GSI-Darmstadt
// changed : 09-MAR-2006
//           12-JUL-2006
// version : 1.0
// license : GPL
// purpose : This library provides the possibility to obtain information about CPU
//           usage of a node. 
//
//
// Required: The following is required to use this library
//           - msvcrxxx.dll (Windows only)
//
//
// Compiling: 
// - Linux: "gmake all" 
// - Windows: use MS Visual C++ (compiler options to be set in "Configuration Properties -> 
//								 C/C++ -> Preprocessor Definitions")
//
// Compiler options:
// - WIN32:   to be used for compiling on Windows.
// - LINUX:   to be used for compiling on Linux.
//
// This program is free software; you can redistribute it and/or modify it under the terms of the 
// GNU General Public License as published by the Free Software Foundation; either version 2 of 
// the license, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the GNU General License for more details (http://www.gnu.org).

// Gesellschaft für Schwerionenforschung, GSI
// Planckstr. 1, 64291 Darmstadt, Germany
// For all questions and ideas contact D.Beck@gsi.de.
// Last update: 09-MAR-2006
//           

//standard libaries
#include <stdlib.h>
#include <stdio.h>

//visual C stuff
#ifdef WIN32
#include "stdafx.h"
#endif

//this library stuff
#include "libNodeInfo.h"
#include <GetNodeInfo.h>

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : initMyLib: Initializes this shared library/dll
// returns : void
void initMyLib()
{
} //initMyLib


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : deinitMyLib: deinitializes this shared library/dll
// returns : void
void deinitMyLib()
{
} //deinitMyLib


//-----------------------------------------------------------------------------------
#ifdef WIN32
//entry point for dll. Created by MS Visual C++
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: 
	{	
		initMyLib();
		break;
	} // DLL_PROCESS_ATTACH
	case DLL_THREAD_ATTACH: 
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
	{
		deinitMyLib();
		break;
	} // DLL_PROCESS_DETACH
		break;
	}
    return TRUE;
} //DLLMain
#endif
#ifdef LINUX
//entry point for shared library (Linux)
void __attribute__ ((constructor)) my_init(void)
{
	initMyLib();
} //initMyLib

//exit point for shared library (Linux)
void __attribute__ ((destructor)) my_fini(void)
{
	deinitMyLib();
} //deinitMyLib
#endif


//-----------------------------------------------------------------------------------
LIBNODEINFO_API void getNodeName(char *nodeName)
{
	GetNodeName(nodeName);
} //getNodeName


//-----------------------------------------------------------------------------------
LIBNODEINFO_API void getNodeOS(char *OS)
{
	GetNodeOS(OS);
} //getNodeOS


//-----------------------------------------------------------------------------------
LIBNODEINFO_API void getProcessInfo(	char *pNames,	//list of processes separated by "|"
										int pNamesLen,  //length of pNames buffer
										int *nProcs,    //number of processes
										int *pId,		//ID of processes
										int *pMem,		//memory of processes
										double *pRunT,	//runtime of process [s]
										int size		//max size of previous arrays
									)
{
	PROCESSINFORMATION	procInfo[MAXPROCS];
	int					numberProcs;
	int					i;
	char                *pointer;

	//get information
	GetProcsInfo(procInfo, &numberProcs);
	
	//init variables
	if (numberProcs > size) numberProcs = size;
	sprintf(pNames, "");
	pointer = pNames;

	for (i=0; i<numberProcs; i++)
	{
		//process names
		if (strlen(procInfo[i].processName) + strlen(pNames) + 1 < pNamesLen)
		{
			strcpy(pointer,procInfo[i].processName);
			pointer += strlen(pointer);
			sprintf(pointer,"|");
			pointer ++;
		} //if

		pId[i]		= procInfo[i].processID;
		pMem[i]		= procInfo[i].processMem;
		pRunT[i]	= (double)(procInfo[i].processRunTime)/(double)TICKS_SECOND;
	} //for i

	*nProcs = numberProcs;
} //getProcessInfo
