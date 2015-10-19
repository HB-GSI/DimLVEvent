// NodeMon.c : Defines the entry point for the console application.
//
// author: Dietrich Beck, GSI-Darmstadt
//
// version: 1.00
//
// history: This program was developed as a serious exercise for the usage of DIM.
// It implements some of the features of "pcmon", which has been developed at CERN
// by Clara Gaspar (thanks).
// 15-Oct-2004 first version
// 20-OCT-2004 debugged version
// 09-MAR-2006 resize services, when list of processes changes
// 30-OCT-2008 Windows only: hide the console window completely (can only be killed via taskmanager)
//
// purpose: This program publishes actual information about processes and the node
// via DIM.
//
// License Agreement for this software:
//
// Copyright (C)
// Gesellschaft für Schwerionenforschung, GSI
// Planckstr. 1
// 64291 Darmstadt
// Germany
//
// Contact: D.Beck@gsi.de 
//
// This program is free software; you can redistribute it and/or modify it under the terms of the 
// GNU General Public License as published by the Free Software Foundation; either version 2 of 
// the license, or (at your option) any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the GNU General License for more details (http://www.gnu.org).
//
// Gesellschaft für Schwerionenforschung, GSI
// Planckstr. 1, 64291 Darmstadt, Germany
// For all questions and ideas contact: D.Beck@gsi.de or H.Brand@gsi.de
// Last update: 11-SEP-2002
//

//include standard stuff
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//include my stuff#
//extern int _imp_dis_remove_service_(unsigned);
#include <dis.h>
#include "GetNodeInfo.h"

//include win stuff
#ifdef WIN32
#include <process.h>
#include <windows.h>
#endif

#define MONITORNAME "PCMON" // prefix for DIM services
#define UPDATETIME  2       // update time [s]

//data to be published by DIM
int    pidsBuffer[MAXPROCS];		//process IDs
char*  pnamesBuffer;                //names of processes
char   info[MAXLENGTH];             //some info
char   pcos[MAXLENGTH];             //OS of node
float  pcTime;                      //total CPU usage
int    pcMem;						//total mem usage of all processes
char   nodeName[MAXLENGTH];         //node name
int    pmemsBuffer[MAXPROCS];		//memory usage of processes
int    pruntimesBuffer[MAXPROCS];   //consumed CPU time of all processes
float  ptimesBuffer[MAXPROCS];      //CPU usage of all processes
int    pcnProcs;                    //number of processes
int    oldPcnProcs;					//old number of processes
int    oldNamesLen;                 //old length of process names


//service IDs for DIM
int    service_myInfo;
int    service_pcmem;
int    service_pctime;
int    service_pids;
int    service_pmem;
int    service_pnames;
int    service_pruntimes;
int    service_ptimes;
int    service_pcnprocs;

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : resizeServices: Resizes the services that are not of fixed size
// returns : void
void resizeServices()
{
	char *cbuffer[MAXLENGTH];
	char *dbuffer[MAXLENGTH];
	char *monitorName[MAXLENGTH];

	if (strlen(pnamesBuffer) != oldNamesLen)
	{
		if (oldNamesLen != 0) dis_remove_service(service_pnames);
		oldNamesLen = strlen(pnamesBuffer);

		//names of processes
		sprintf(cbuffer, "%s/%s/PNAMES", MONITORNAME, nodeName);
		service_pnames = dis_add_service(cbuffer, "C", pnamesBuffer, strlen(pnamesBuffer+1), 0, 0);

		//update dns
		dis_start_serving("huhu");
	} //if strlen (pnamesBuffer)
	
	if (pcnProcs != oldPcnProcs)
	{
		if (oldPcnProcs != 0)
		{
			dis_remove_service(service_pids);
			dis_remove_service(service_pmem);
			dis_remove_service(service_pruntimes);
			dis_remove_service(service_ptimes);
		} // if oldPncProcs != 0

	oldPcnProcs = pcnProcs;

	//PID of processes
	sprintf(cbuffer, "%s/%s/PIDS", MONITORNAME, nodeName);
	sprintf(dbuffer, "I:%d", pcnProcs);
	service_pids = dis_add_service(cbuffer, dbuffer, pidsBuffer, pcnProcs*sizeof(int), 0, 0);

	//memory usage of processes
	sprintf(cbuffer, "%s/%s/PMEM", MONITORNAME, nodeName);
	sprintf(dbuffer, "I:%d", pcnProcs);
	service_pmem = dis_add_service(cbuffer, dbuffer, pmemsBuffer, pcnProcs*sizeof(int), 0, 0);

	//summed CPU time of processes [s]
	sprintf(cbuffer, "%s/%s/PRUNTIMES", MONITORNAME, nodeName);
	sprintf(dbuffer, "I:%d", pcnProcs);
	service_pruntimes = dis_add_service(cbuffer, dbuffer, pruntimesBuffer, pcnProcs*sizeof(int), 0, 0);

	//actual CPU usage of processes [%]
	sprintf(cbuffer, "%s/%s/PTIMES", MONITORNAME, nodeName);
	sprintf(dbuffer, "F:%d", pcnProcs);
	service_ptimes = dis_add_service(cbuffer, dbuffer, &ptimesBuffer, pcnProcs*sizeof(float), 0, 0);

	//update dns
	dis_start_serving("huhu");
	} //if pcnProcs != oldPcnProcs
	
} //resizeServices


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : CalcSummaryData: calculates the total cpu usage and memory consumption  
// returns : void
void CalcSummaryData (int *pmemsBuffer, float *ptimesBuffer, int numberProcs, float *cpu, int *memory)
{
	int i;

	*cpu = 0;
	*memory = 0;

	for (i=0; i<numberProcs; i++)
	{
		(*cpu)    += ptimesBuffer[i];
		(*memory) += pmemsBuffer[i];
	} // for i
} //CalcSummaryData


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : CalcResult: determines the cpu usage for the individual processes by 
//			 comparing process information that has been acquired at different times 
//			 the given time interval
// returns : void
void CalcResult(PROCESSINFORMATION *procInfoStart, 
			    PROCESSINFORMATION *procInfoStop,
			    int numberProcsStart,
			    int numberProcsStop,
				float deltaTime,                       //time [s]
			    int *numberProcs,
				int *pids, 
				int *pmems, 
				char *pnames, 
				int *pruntimes, 
				float *ptimes
			   )
{
	int i,j;
	char *pointer;
	
	*numberProcs = -1;
	pointer = pnames;
	
	for (i=0; i < numberProcsStart; i++)
	{
		for (j=0; j < numberProcsStop; j++)
		{
			if (procInfoStart[i].processID == procInfoStop[j].processID)
			{
				(*numberProcs)++;
				strcpy(pointer,procInfoStart[i].processName);
				pointer += strlen(pointer);
				sprintf(pointer,"|");
				pointer ++;
				pids[*numberProcs]      = procInfoStart[i].processID;
				pmems[*numberProcs]     = procInfoStart[i].processMem;
				pruntimes[*numberProcs] = procInfoStart[i].processRunTime/TICKS_SECOND;
				//calc: (used cpu time[s])/(deltaTime[s])*(1000%)
				ptimes[*numberProcs]    = (float)(procInfoStop[j].processRunTime - procInfoStart[i].processRunTime)
					                      /(float)TICKS_SECOND / deltaTime * 100.0;
			} // if procInfoStart
		} // for j
	} // for i
	(*numberProcs)++;

	//write 0 to rest of PID array
	for (i=*numberProcs; i<MAXPROCS; i++) pids[i] = -3;

	//convert process names to small case letters
	for (i=0;i < MAXPROCS*MAXLENGTH; i++) pnames[i] = tolower(pnames[i]);

} //CalcResult


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : Main: main of this program
// returns : void
void main(int argc, char* argv[])
{
	//my information about processes
 	PROCESSINFORMATION processInfo1[MAXPROCS];
	PROCESSINFORMATION processInfo2[MAXPROCS];
	PROCESSINFORMATION *processInfoStart;
	PROCESSINFORMATION *processInfoStop;
	PROCESSINFORMATION *processInfoTemp;

	//variables
	int		i;
	char	cbuffer[MAXLENGTH];
	int		numberProcsStart, numberProcsStop;
	float	deltaT = UPDATETIME;

	//hide command window
#ifdef WIN32
	HWND hWnd;
	SetConsoleTitle("NodeMon");
	sleep(1);
	hWnd = FindWindow(NULL, "NodeMon");
	if (hWnd == NULL) printf("can't get window handle for hiding console\n");
	else ShowWindow(hWnd, 0);
#endif

	// init pointer stuff
	pnamesBuffer = (char*)malloc(MAXPROCS*MAXLENGTH);
	processInfoStart = processInfo1;
	processInfoStop = processInfo2;

	oldPcnProcs		= 0;
	oldNamesLen     = 0;

	//get node name
	GetNodeName(nodeName);
	for (i=0; i < (int)strlen(nodeName); i++) nodeName[i] = tolower(nodeName[i]);

	//total CPU usage on node
	sprintf(cbuffer, "%s/%s/PCTIME", MONITORNAME, nodeName);
	pcTime = -2;
	service_pctime = dis_add_service(cbuffer, "F:1", &pcTime, sizeof(float), 0, 0);

	//number of processes
	sprintf(cbuffer, "%s/%s/PCNPROCS", MONITORNAME, nodeName);
	pcnProcs = -7;
	service_pcnprocs = dis_add_service(cbuffer, "I:1", &pcnProcs, sizeof(int), 0, 0);
	
	//info
	sprintf(info, "Hello world :-)");
	sprintf(cbuffer, "%s/%s/INFO", MONITORNAME, nodeName);
	service_myInfo = dis_add_service(cbuffer, "C", info, MAXLENGTH, 0, 0);
	
	//total memory usage on node
	sprintf(cbuffer, "%s/%s/PCMEM", MONITORNAME, nodeName);
	pcMem = -1;
	service_pcmem = dis_add_service(cbuffer, "L:1", &pcMem, sizeof(int), 0, 0);

	//node OS
	sprintf(cbuffer, "%s/%s/PCOS", MONITORNAME, nodeName);
	GetNodeOS(pcos);
	dis_add_service(cbuffer, "C", pcos, strlen(pcos), 0, 0);

	//start DIM server
	sprintf(cbuffer,"%s/%s", MONITORNAME, nodeName);
	dis_start_serving(cbuffer);

	//start
	while (1)
	{
		//swap pointers
		processInfoTemp  = processInfoStart;
		processInfoStart = processInfoStop;
		processInfoStop  = processInfoTemp;

		//acquire data
		GetProcsInfo(processInfoStart, &numberProcsStart);
		NodeSleep((unsigned int)(deltaT*1000.0));
		GetProcsInfo(processInfoStop, &numberProcsStop);

		//calculate info
		CalcResult(processInfoStart,processInfoStop, numberProcsStart,numberProcsStop, deltaT,
			       &pcnProcs, pidsBuffer, pmemsBuffer, pnamesBuffer, pruntimesBuffer, ptimesBuffer);
		CalcSummaryData(pmemsBuffer, ptimesBuffer, pcnProcs, &pcTime, &pcMem);

		resizeServices();
	
		//update DIM services
		dis_update_service(service_myInfo);
		dis_update_service(service_pcmem);
		dis_update_service(service_pctime);
		dis_update_service(service_pids);
		dis_update_service(service_pmem);
		dis_update_service(service_pnames);
		dis_update_service(service_pruntimes);
		dis_update_service(service_ptimes);
		dis_update_service(service_pcnprocs);

		//entertain the user of the console
	    printf("CPU %d%%, memory %dk, processes %d\n", (int)pcTime, pcMem, pcnProcs);
	} //while (1)

	//In principle we should free allocated memory and release the dim services
	//However, the only way to end this program is to kill it. So let's skip this.
} //main
