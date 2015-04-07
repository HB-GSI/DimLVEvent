// DimWrapper.c : Defines the entry point for the DLL application.
//
// author  : Dietrich Beck, GSI-Darmstadt
// changed : 29-JAN-2010
#define MYVERSION "1.04a"
// license : 
// purpose : This library provides a wrapper around the dim dll. Its main purpose is
//           to provide an interface to DIM that can be used with LabVIEW.
//
//           The interface to LabVIEW works such, that the service data received by 
//           DIM is copied to a double buffer. No difference exists between 
//           "SERV_BY_BUFFER" and "SERV_BY_ROUTINE". Data can be retrieved by using 
//           the routine dimWaitForData. 
//
// usage   : Dispatching of service data to message queues or notifiers from inside 
//           this library is no longer supported. Instead, the application needs to
//           obtain new service data from the library using the library function
//           dimWaitForData. Then, the applications must dispatch service data to 
//           message queues or notifiers. Two things are important here:
//           -- The application should call dimWaitForData only from one thread.
//				(not more than once at the same time).
//			 -- To prevent loss of data, it is important that the application calls
//              the routine dimWaitForData immediately after the service data has been
//              dispatched to message queues or notifiers.
//			 The application may register an occurrence to this library and may
//           wait on the occurrence in case new service data arrives.
//
// Required: The following is required to use this library
//           - DIM, has to be installed according to the manual (see DIM web-site)
//           - libmydim.so (Linux)
//           - myDimStd.dll (Windows)
//           - msvcrxxx.dll (Windows only)
//           - labview.lib (all platforms, required for sending occurrences)
//           - LabVIEW (all platforms, required for sending occurrences)
//           For usage with LabVIEW, it is recommended to use the libmydim.so/myDimStd.dll,
//           that is in the ../bin folder of this project. The DIM libraries in that
//           folder have been patched to ensure correct "endian" and proper size
//           for DIM internal buffers. (For compiling libmydim.so/myDimStd.dll, dim.h and 
//			 dis.c have been changed. The changed version of these files are in the 
//           same folder as DimWrapper.c.)
//           
//
// DIM     : DIM has been developed at CERN and is licensed via GPL. For more 
//			 information see http://www.cern.ch/dim. Acknowledgements to 
//           Clara Gaspar et al.
//            
//
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
// Last update: 16-DEC-2005
//           

//standard includes
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

//Visual C stuff
#ifdef WIN32
#include "stdafx.h"
#include <sys/timeb.h>
#include <time.h>
#endif

//Linux C stuff
#ifdef LINUX
#include <sys/time.h>
#endif

//DIM stuff
#include <dim.h>                  //DIM definitions
#include <dic.h>                  //client library of DIM
#include <dis.h>                  //server library of DIM

//this library stuff
#include "DimWrapper.h"
#include "DimWrapperBasics.h"
//#include "CSSystem.h"
#include <extcode.h>              //provides stuff required for LabVIEW, 
                                  //you must add .../National Instruments/LabVIEWX.xx/cintools as path for include files

//"global" variables used in the dll
volatile	callBackType	callBacks[MAXSERVICES];			//array of registered call backs. Each registered LabVIEW event is 
															//identified by one element of this array. The "ID" used by the
															//DIM call back function is identical to the index of the element.
volatile	int				lastCallBackRegistered = -1;	//index of last call back, that has been registered.
volatile	serviceType     services[MAXSERVICES];			//array of services. Each service is 
															//identified by one element of this array. The "ID" used  
															//is identical to the index of the element.
volatile	int				lastServiceCreated = -1;		//index of last service, that has been created
volatile	int             serverActive=0;					//flag, that indicates if this server is active and initialized
volatile	int				errorCode=0;					//error code of server (if any)
volatile	int				maxLocalBuffSize;               //max size for transferring services within the same task
volatile	int				securityEnabled = 0;            //flag for usage of security
volatile	char			*allowedNodes;                  //buffer containing the nodes that are allowed to access
volatile	int				executingCallback;              //flag indicating a callback is executed. This is needed when removing callbacks
volatile	int			    ignoreCallbackID;               //callback ID that is ignored when a callback is received. This is needed when moving callbacks
volatile    unsigned		occurrenceLV;					//LabVIEW occurrence typecast to unsigned
volatile	char			dimServerName[MAXLEN];			//name of DIM server, if THIS application is a DIM server
volatile	char			noLink[MAXLEN];                 //buffer used in case of "no link"
volatile	char			timeString[MAXLEN];				//buffer with string containing time

#ifdef WIN32
FILE						*myStdout;                      //file to be used for printf()
char						fullLogFileName[MAXLEN];        //full file name for logfile
struct _timeb				actTime;                        //time for printing messages
#endif
#ifdef LINUX
time_t						actTime;
#endif

volatile	char			tempBuff[2][MAXBUFFERSIZE];		//used for temporary storage of DIM service data
volatile	int				tempBuffSize[2];				//size of tempBuffer 
volatile	int				newDataFlag[2];					//flag indicating, we have new data. -3: polling of data inactive, 
															// -2: polling of data active, -1: data being received, 0..N: new data available
volatile	int				nextBuffCB;						//flag for the next buffer to be used by "receivedCB"
volatile	int				nextBuffWait;					//flag for the next buffer to be used by "DimWaitForData"
volatile	double			timePerIteration;				//used by my_usleep

volatile	int				libInitialized = 0;             //check, if library has been initialized. At the moment, this is experimental


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : getTimeString: obtain a string containing the current time
// returns : string containing the time
char *getTimeString()
{
	char *timeline;

#ifdef WIN32
	_ftime(&actTime);
	timeline = ctime( &(actTime.time));
#endif
#ifdef LINUX
	actTime = time(NULL);
	timeline = ctime(&actTime);
#endif
	sprintf(timeString, "%s", timeline);

	return timeString;
} //getTimeString


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dw_printf: my own version of printf
// returns : 
void dw_printf(char *myString)
{
	printf(myString);
#ifdef WIN32
	if (myStdout != NULL) fclose(myStdout);
	myStdout = freopen(fullLogFileName, "a+", stdout);
#endif
} //dw_printf


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : exit_user_routine: provides an exit handler for DIM applications using this dll.
//           The exit handler prevents sudden death of applications, in case they receive
//           an EXIT command.
// returns : void
void exit_user_routine (exitCode)
int *exitCode;
{
	char my_message[MAXLEN];

	serverActive = 0;
	errorCode    = *exitCode * -1;

	sprintf(my_message, "%.19s, PID %d, server %s: ", getTimeString(), getpid(), dimServerName);
	sprintf(my_message, "%s Dim: received exit command %d\n", my_message, errorCode);
	dw_printf(my_message);
}


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dic_error_user_routine: provides an error handler for DIM applications using this dll.
// returns : void
void dic_error_user_routine (int severity, int error_code, char *message)
{
	char my_message[MAXLEN];

	sprintf(my_message, "%.19s, PID %d, server %s: ", getTimeString(), getpid(), dimServerName);
	sprintf(my_message, "%s Dim client: error_user_routine - severity (0 - info, 1 - warning, 2 - error, 3 - fatal): %d, error_code %d, %s\n",
		my_message, severity, error_code, message);
	dw_printf(my_message);
}


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dis_error_user_routine: provides an error handler for DIM applications using this dll.
// returns : void
void dis_error_user_routine (int severity, int error_code, char *message)
{
	char my_message[MAXLEN];
	
	sprintf(my_message, "%.19s, PID %d, server %s: ", getTimeString(), getpid(), dimServerName);
	sprintf(my_message, "%s DIM server: error_user_routine - severity (0 - info, 1 - warning, 2 - error, 3 - fatal): %d, error_code %d, %s\n",
		my_message, severity, error_code, message);
	dw_printf(my_message);
}


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : my_usleep: provides an usleep that allows sleeping below 10ms on Linux by eating CPU resources,
//           provides a workaround to sleep below (or larger than) 1ms on Windows
// returns : void
void my_usleep(int usec)
{
	int             dummy;          //dummy variable;
	double          i;       
	double          nOfLoops;
#ifndef WIN32
	volatile int    delayRead;      //volatile variable used for reading
#endif
	if ((usec < 0) || (usec > 1000000)) return;
	nOfLoops = (double)usec / timePerIteration;
#ifdef WIN32
	for (i = 0; i < nOfLoops; i++) dummy = (int)sqrt((double)i);
#endif
#ifdef LINUX
	for (i = 0; i < nOfLoops; i++) dummy = delayRead;
#endif
} //my_usleep


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : waitOneTick: wait as short as possible without consuming CPU time
// returns : void
void waitOneTick()
{
#ifdef WIN32
	usleep(1000);         //this is the shortest wait that is available on Windows without tricks: usleep(1000) = 2ms
#endif
#ifdef LINUX
	usleep(0);            //this is the shortest wait that is available on Linux without tricks: usleep(0) = 10ms
#endif
} //waitOneTick()


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : initMyLib: Initializes this shared library/dll
// returns : void
void initMyLib()
{
	//variables
	int				i;
	char			my_message[MAXLEN];
#ifdef WIN32
	struct _timeb startTime;
	struct _timeb stopTime;
	int           iterations = 1000000;
#endif
#ifdef PHARLAP
	iterations = 1000;
#endif
#ifdef LINUX
	struct timeval  startTime;
	struct timeval  stopTime;
	struct timezone dummyZone;
	int             iterations = 1000000;
#endif

	if (!libInitialized)
	{

#ifdef WIN32
		//initialize file name for log file

		if (getenv(DIMWRAPPERPATH)) 
		{
			sprintf(fullLogFileName, "%s", getenv(DIMWRAPPERPATH));
			if (strlen(fullLogFileName) > 0)	sprintf(fullLogFileName, "%s\\", fullLogFileName);
			//else								sprintf(fullLogFileName, "");
		} //if getenv
		sprintf(fullLogFileName, "%s%s", fullLogFileName, LOGFILENAME);

		//open logfile
		myStdout = freopen(fullLogFileName, "a+", stdout);
#endif

		//initialize serviceIDs
		for (i=0;i<MAXSERVICES;i++) services[i].my_serviceID = -1;
		//block first service, so that it can not be used and accidental calling with index 0 does not work
		services[0].my_serviceID = 1;
		for (i=0;i<MAXSERVICES;i++) callBacks[i].callBackID = -1; 
		//block first callback, so that it can not be used and accidental calling with index 0 does not work
		callBacks[0].callBackID = 1; 
	
		//initialization
		serverActive		= 1; 
		maxLocalBuffSize	= TCP_RCV_BUF_SIZE - 4096;	//substract 4k to be on the safe side...
		executingCallback	= 0;						//flag indicating a callback is executed
		ignoreCallbackID	= -1;						//callback ID that is ignored when a callback is received
		occurrenceLV		= 0;                        //occurrence to be fired when new service data is available
		sprintf(dimServerName, "");						//name of DIM server

		for (i=0; i < 2; i++) 
		{
			tempBuffSize[i] = 0;
			newDataFlag[i]  =  -3;
			//newDataFlag[i]  =  -2;
		} //for i
		nextBuffCB      = 0;
		nextBuffWait    = 0;
		sprintf(noLink, "NOLINK");
		
		//determine CPU speed for CPU consuming "sleep"
#ifdef WIN32
		timePerIteration = 1.0;    //just a wild guess ...
		_ftime(&startTime);
		my_usleep(iterations);
		_ftime(&stopTime);
		timePerIteration = (stopTime.time  - startTime.time)    * 1000000.0;
		timePerIteration = (stopTime.millitm - startTime.millitm) * 1000.0 + timePerIteration;
		timePerIteration = (double)timePerIteration / (double)iterations;
#endif
#ifdef LINUX
		timePerIteration = 1.0;    //initialize speed determination
		gettimeofday(&startTime, &dummyZone);
		my_usleep(iterations);
		gettimeofday(&stopTime, &dummyZone);
		timePerIteration = (stopTime.tv_sec - startTime.tv_sec) * 1000000;
		timePerIteration = (stopTime.tv_usec - startTime.tv_usec) + timePerIteration;
		timePerIteration = (double)timePerIteration / (double)iterations;
#endif
		if (timePerIteration < 0.001) 
		{
			timePerIteration = 1.0;
			sprintf(my_message, "%.19s, PID %d: determination of CPU speed failed, using default value for time consuming wait\n", getTimeString(), getpid());
			dw_printf(my_message);
		}
		sprintf(my_message, "%.19s, PID %d: time per iteration for time consuming wait is %f us\n", getTimeString(), getpid(), timePerIteration);
		dw_printf(my_message);
		sprintf(my_message, "%.19s, PID %d: DimWrapper library version %s initialized\n", getTimeString(), getpid(), MYVERSION);
		dw_printf(my_message);

#ifdef WIN32
		if (!getenv(DIMWRAPPERPATH))
		{
			sprintf(my_message, "%.19s, PID %d: environment variable %s undefined - no defined folder for file %s \n", getTimeString(), getpid(), DIMWRAPPERPATH, LOGFILENAME);
			dw_printf(my_message);
		} //if not getenv 
#endif
	
		//define exit handler
		dis_add_exit_handler(exit_user_routine);
		dic_add_error_handler(dic_error_user_routine);
		dis_add_error_handler(dis_error_user_routine);

		libInitialized	= 1;
	} //if !libInitialized
} //initMyLib


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : deinitMyLib: deinitializes this shared library/dll
// returns : void
void deinitMyLib()
{
	int		i;
	char    my_message[MAXLEN];

	if (libInitialized)
	{
		for (i=0;i<MAXSERVICES;i++) if (callBacks[i].callBackID != -1)	dicReleaseService(callBacks[i].callBackID);
		dic_stop();
		for (i=0;i<MAXSERVICES;i++) if (services[i].my_serviceID != -1) disRemoveService(services[i].my_serviceID);
		sleep(1); //hm..., we need this sleep, otherwise something (we receive an exit signal?) happens
		dis_stop_serving();
		
		sprintf(my_message, "%.19s, PID %d, server %s: ", getTimeString(), getpid(), dimServerName);
		sprintf(my_message, "%s DimWrapper library deinitialized\n", my_message);
		dw_printf(my_message);
		//dw_printf("Bye Bye....\n");
#ifdef WIN32
		if (myStdout != NULL) fclose(myStdout);
#endif
		libInitialized = 0;
	} //if libInitialized
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
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : removeMyService: removes a command service (dis) or normal service (dic)
// returns : void
void removeMyService(int callBackID //identifies the service to be removed
					)
{
	//check range
	if ((callBackID < 0) || (callBackID >= MAXSERVICES)) return;
	if (callBacks[callBackID].callBackID != callBackID) return;

	//avoid execution of callback for this callback ID;
	ignoreCallbackID = callBackID;
	my_usleep(100); 
    while (executingCallback == callBackID) waitOneTick();

	//release service
	if (callBacks[callBackID].callBackType == 2) dic_release_service(callBacks[callBackID].serviceId);
	if (callBacks[callBackID].callBackType == 1) dis_remove_service(callBacks[callBackID].serviceId);

    //set element to default values
	callBacks[callBackID].refNum		=  0;
	callBacks[callBackID].refNumType	= -1;
	callBacks[callBackID].serviceId		= -1;
	callBacks[callBackID].callBackType	= -1;
	free(callBacks[callBackID].serviceName);
	callBacks[callBackID].callBackID	= -1;

	//take care that next service to be created will use the array element of this service
	lastCallBackRegistered = callBackID - 1;

	//reset ignoreCallbackID
	ignoreCallbackID = -1;
} //removeMyService

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : receivedCB: call back function to be called when a service changes.
//           This can be either a command service received by a DIM server, or an 
//           updated service received by the DIM client.
//           This function is given the data received by DIM. The data are then 
//           inserted into a LabVIEW message queue or a LabVIEW notifier. Callbacks
//           are identifier via the callBackID.
// returns : void
void receivedCB(callBackID,address,size)
int *callBackID;        //identifies a callback
char *address;          //address where the data is available
int *size;              //length of available data
{
    int       buffIndex;		 //index for tempBuffer;
	char      myNode[MAXLEN];    //my node name
	char      myMessage[MAXLEN]; //message buffer
	int       numberToPoint;    
	int       i;

	//check ranges and callBackID
	if (*size <= 0) return; 
	if ((*callBackID < 0) || (*callBackID >= MAXSERVICES)) return;  //check range of callBackID
	if (callBacks[*callBackID].callBackID != *callBackID ) return;  //check callBackID
	if (*callBackID == ignoreCallbackID) return;                    //callback of a service that is being removed

	//set flag, which callBack is executed (needed by removeMyService)
	executingCallback = *callBackID;

	//check security
	if ((securityEnabled) && (allowedNodes != NULL))
	{
		sprintf(myNode, "");
		if (callBacks[*callBackID].callBackType == 1) dis_get_client(myNode);
		if (callBacks[*callBackID].callBackType == 2) dic_get_server(myNode);
		
		if (strlen(myNode) > 1) //don't care about undefined node names, strlen <1 is probably a "NOLINK"
		{
			//format node
			for (i=0; i<(int)strlen(myNode); i++) myNode[i] = tolower(myNode[i]);
			numberToPoint = (int)strcspn(myNode, ".");
			if (numberToPoint > 0) myNode[numberToPoint] = '\0';
			sprintf(myNode, "%s", strstr(myNode, "@") + 1);

			if (strstr(allowedNodes, myNode) == NULL) 
			{
				sprintf(myMessage, "%.19s, PID %d, server %s: illegal service received from node %s\n", getTimeString(), getpid(), dimServerName, myNode);
				dw_printf(myMessage);
				executingCallback = 0;
				return;
			} //node has no right to send service
		} // if strlen(myNode)
	} //if securityEnabled

	//get index of buffer to be used
	buffIndex  = nextBuffCB;
	
	//wait until tempBuffer is available. This wait is not very fancy.
	i = 0;
	while ((newDataFlag[buffIndex] != -2) && (i<200000))  //wait maximum 200000 * 10us = 2.0s
	{
	  i++;
	  my_usleep(10);                      //wait 10us
	}
	if (newDataFlag[buffIndex] != -2)     //give up, if tempBuffer is not available
	{
		sprintf(myMessage, "%.19s, PID %d, server %s: DIMWrapper: receivedCB can't get rid of data!\n", getTimeString(), getpid(), dimServerName);
		dw_printf(myMessage);
		executingCallback = 0;
		return;
	}

	//check buffer size
	if (&(tempBuff[buffIndex][0]) == NULL) //something is really wrong
	{
		executingCallback = 0;
		return;
	}
	if (*size > MAXBUFFERSIZE)             //data larger than tempBuffer
	{
		sprintf(myMessage, "%.19s, PID %d, server %s: DIMWrapper: receivedCB, data to large!\n", getTimeString(), getpid(), dimServerName);
		dw_printf(myMessage); 
		executingCallback = 0;
		return;
	}

	//copy data and indicate callback ID
	newDataFlag[buffIndex] = -1;          //indicate that buffer is being filled
	tempBuffSize[buffIndex] = *size;
	memcpy(&(tempBuff[buffIndex][0]), address, tempBuffSize[buffIndex]);
	newDataFlag[buffIndex]    = *callBackID;

    //switch index for next buffer
	nextBuffCB = !nextBuffCB;
	
    //fire occurrence in case application program is waiting for it
	dimFireOccurrence();

	//indicate that no callback is being executed
	executingCallback = 0;
} //serv_receivedCB

//-----------------------------------------------------------------------------------
DIMWRAPPER_API void dimSetOccurrenceRef(unsigned occurrenceRef	//reference of occurrence typecast to unsigned
									   )
{
	char	myMessage[MAXLEN]; //message buffer
	
	occurrenceLV = occurrenceRef;
	
	if (occurrenceLV == 0)	sprintf(myMessage, "%.19s, PID %d: DIMWrapper: occurrence disabled, using pure polling mode.\n", getTimeString(), getpid());
	else					sprintf(myMessage, "%.19s, PID %d: DIMWrapper: occurrence enabled, using callback via occurrence.\n", getTimeString(), getpid());
	dw_printf(myMessage);
} //dimSetOccurrenceRef

//-----------------------------------------------------------------------------------
DIMWRAPPER_API void dimGetOccurrenceRef(unsigned *occurrenceRef	//reference of occurrence typecast to unsigned
									   )
{
	*occurrenceRef = occurrenceLV;
} //dimSetOccurrenceRef

//-----------------------------------------------------------------------------------
DIMWRAPPER_API void dimFireOccurrence()
{
	if (occurrenceLV == 0) return;
	Occur((LVRefNum)occurrenceLV);
}

//-----------------------------------------------------------------------------------
DIMWRAPPER_API void dimWaitForData(	int timeout,                //timeout in ms
									int *timedOut,              //TRUE: timed out
									int *callBackID,            //callBackID
									unsigned *refNum,			//(pointer to) LVRefNum (typecast to unsigned)
									int *refNumType,            //type of refnum
									int *recSize,               //size of received data
									char *copyBuffer,           //pointer to copy buffer
									int  copyBufferSize         //max size of copy buffer
								   )
{
	int t;                   //used polling time in ms
	int deltaT;				 //poll interval in ms
	int maxT;                //maximum polling time in ms
	int buffIndex;           //index of temp buffer

#ifdef WIN32
	deltaT = 2;           //shortest wait in WIN32 is 2ms
#endif
#ifdef LINUX
	deltaT = 10;          //shortest wait in Linux is 10ms
#endif

	//set buffer index
	buffIndex    = nextBuffWait;
	
	//init copy buffer and return data
	sprintf(copyBuffer, "%s", noLink);
	*timedOut              = 1;
	*callBackID            = -1;
	*recSize               = -1;
	*refNum                = 0;
	*refNumType            = 0;
	
	//check, if we are executing this for the first time
	if (newDataFlag[buffIndex] == -3) newDataFlag[buffIndex] = -2;

	if (timeout != 0)
	{
		if (newDataFlag[buffIndex] < 0) my_usleep(200); //tempBuffer not yet filled, do 200us of CPU consuming wait
		if (newDataFlag[buffIndex] < 0) my_usleep(200); //tempBuffer not yet filled, do 200us of CPU consuming wait
		if (newDataFlag[buffIndex] < 0)                 //tempBuffer not yet filled, do "slow" waiting
		{
			maxT = timeout; 
			t    = 0;
			while ((newDataFlag[buffIndex] < 0) && (t < maxT)) //no new data yet
			{
				t = t + deltaT;
				waitOneTick();
			} // while no new data yet
		} //if tempBuffer not yet filled
	} //if timeout != 0

	//check if we have new data
	if (newDataFlag[buffIndex] >= 0)    //we got new data
	{
		if (tempBuffSize[buffIndex] <= copyBufferSize) //copyBuffer is large enough
		{
			//fill return data
			copyBuffer             = memcpy(copyBuffer,&(tempBuff[buffIndex][0]), tempBuffSize[buffIndex]);
			*timedOut              = 0;
			*callBackID            = newDataFlag[buffIndex];
			*recSize               = tempBuffSize[buffIndex];
			*refNum                = callBacks[*callBackID].refNum;
			*refNumType            = callBacks[*callBackID].refNumType;
		} //if copy buffer large enough
		newDataFlag[buffIndex] = -2;	//reset new data flag
	    nextBuffWait = !nextBuffWait;	//prepare buffer switch
	} //if we got new data
} //dimWaitForData


//-----------------------------------------------------------------------------------
DIMWRAPPER_API void dimGetInfo(	int *versionNumber,		//DIM version against which this library has been built
								char *dimDnsNode,       //node of DIM name server
								int *maxLocalDataSize,	//maximum size for application internal DIM transfer
								int *pollingMode,       //0: LabVIEW occurrence enabled; 1: need to poll
								int *maxBuffSize,       //max size of buffe
								char *nodes             //list of allowed nodes
                              )
{
	*versionNumber		= DIM_VERSION_NUMBER;
	dim_get_dns_node(dimDnsNode);
	*maxLocalDataSize	= maxLocalBuffSize;
	if (securityEnabled)	sprintf(nodes, "%s", allowedNodes);
	else					sprintf(nodes, "");
	if (!occurrenceLV)	*pollingMode = 1;
	else				*pollingMode = 0;
	*maxBuffSize = MAXBUFFERSIZE;
} //dimGetInfo


//-----------------------------------------------------------------------------------
DIMWRAPPER_API int dimGetServiceInfo(	int callBackID,			//callback ID of service
										int *refNumType,		//type of LV refnum (message queue, notifier, ...)
										unsigned *refNum,       //(pointer to) refnum typecast to unsigned
										unsigned *serviceID,	//DIM service ID
										int *callBackType,		//type of callback (service, command service, ...)
										char *typeDesc,			//type descriptor for data (DIM format)
										char *serviceName,		//name of DIM service
										int  *quality,          //quality of service
										double *timeStamp       //time stamp in seconds
									 )
{
	int secs;
	int milisecs;

	//check range
	if ((callBackID < 0) || (callBackID >= MAXSERVICES)) return -1;
	if (callBacks[callBackID].callBackID != callBackID)  return -1;
	if (strlen(callBacks[callBackID].serviceName) > MAXLEN) return -1;
	
	*serviceID		= callBacks[callBackID].serviceId;
	*refNumType		= callBacks[callBackID].refNumType;
	*refNum			= callBacks[callBackID].refNum;
	*callBackType	= callBacks[callBackID].callBackType;
	sprintf(serviceName, "%s", callBacks[callBackID].serviceName);

	if (callBacks[callBackID].callBackType == 2)	
	{
		if (strlen(dic_get_format(*serviceID)) > MAXLEN) return -1;
		sprintf(typeDesc, "%s", dic_get_format(*serviceID));
		dic_get_timestamp(*serviceID, &secs, &milisecs);
		if (secs > 0)	
		{
			*timeStamp = (double)secs + (double)milisecs/1000.0 + 2082844800.0; //from 1970 (DIM) to 1904 (LabVIEW)
			*quality   = dic_get_quality(*serviceID);
		} //time stamp valid
		else
		{
			*timeStamp  = 0;
			*quality    = -1;
		}
	} // if: normal service
	else
	{
		sprintf(typeDesc, "N/A");
		*quality		= -1;
		*timeStamp		= -1;
	} //else: command service
        
	return 0;
} //dimGetServiceInfo


//-----------------------------------------------------------------------------------
DIMWRAPPER_API int dimEnableSecurity(	char *nodes	//list of allowed nodes
									)
{
	int i;

	//memory management
	if ((allowedNodes != NULL) && securityEnabled) free(allowedNodes);
	allowedNodes = (char *)malloc(strlen(nodes)+1);
	if (allowedNodes == NULL) 
	{
		securityEnabled = 0;
		return 0;
	} //if allowedNodes == NULL

	sprintf(allowedNodes, "%s", nodes);
	for (i=0; i<(int)strlen(allowedNodes); i++) allowedNodes[i] = tolower(allowedNodes[i]);
	securityEnabled = 1;

	return 1;
} //dimEnableSecurity


//-----------------------------------------------------------------------------------
DIMWRAPPER_API void dimDisableSecurity()
{
	securityEnabled = 0;
	free(allowedNodes);
} //dimDisableSecurity


//-----------------------------------------------------------------------------------
DIMWRAPPER_API int dimSetDNSNode(char *node_name)
{
	return (dim_set_dns_node (node_name));
}


//-----------------------------------------------------------------------------------
DIMWRAPPER_API int dicInfoService(char *serviceName,    //name of DIM service
								  unsigned refNum,      //LabVIEW refnum typecast to int
								  int refNumType,       //1: message queue, 2: notifier
								  int serviceType,      //1: once only, 2: timed, 4: monitored
								  int timeout,          //timeout (intervall) when to receive new data
								  int stamped			//1: get timestamp and quality
								 )
{	
	int callBackID    = -1; 
	int restartSearch = 1;
	int i;

	// get index of of element in the array "services". First search from
    // index of last service that has been created to MAXSERVICES
	i = lastCallBackRegistered;
	if ((i < 0) || (i >= MAXSERVICES)) i = 0;
	while ((i < MAXSERVICES) && (callBackID == -1))
	{
		i++;
		if ((i == MAXSERVICES) && (restartSearch)) {i = 0; restartSearch = 0;}
		if (callBacks[i].callBackID == -1) callBackID = i;
	}

			
	if ((callBackID >= 0) && (callBackID < MAXSERVICES)) //callBackID is valid?
	{
		//allocate buffer for name
		callBacks[callBackID].serviceName	= malloc(strlen(serviceName)+1);
		if (callBacks[callBackID].serviceName == NULL) return -1;

		//remember last (this) callBack that has been registered
		lastCallBackRegistered = callBackID;

		callBacks[callBackID].callBackID	= callBackID;
		callBacks[callBackID].refNum		= refNum; 
		callBacks[callBackID].refNumType	= refNumType;
		callBacks[callBackID].callBackType	= 2;
		sprintf(callBacks[callBackID].serviceName, "%s", serviceName);
		if (stamped)	callBacks[callBackID].serviceId	= dic_info_service_stamped(serviceName, serviceType, timeout, 0, 0, receivedCB, callBackID, noLink, strlen(noLink));
		else			callBacks[callBackID].serviceId	= dic_info_service(serviceName, serviceType, timeout, 0, 0, receivedCB, callBackID, noLink, strlen(noLink));
				//                LVEvent_MessageBox(serviceName, "dicInfoService");
	}
	else callBackID = -1;

	return callBackID; 
} //dicInfoService


//-----------------------------------------------------------------------------------
DIMWRAPPER_API void dicReleaseService(int callBackID //identifies the call back event to be unregistered
									 )
{	
	removeMyService(callBackID);
} //dicReleaseService


//-----------------------------------------------------------------------------------
DIMWRAPPER_API int dicCommandService(char *name,	 //identifies the command to be called
									 char *buffer,   //data
									 int  size,      //size of data
									 int  safeMode   //1: only send command, if size < maxLocalSize
								    )
{
	if (!serverActive) return 0;
	if (safeMode && (size > maxLocalBuffSize)) return -1;

	return dic_cmnd_service(name, (int *)buffer, size);
} //dicCommandService


//-----------------------------------------------------------------------------------
DIMWRAPPER_API int disAddService(	char *serviceName,    //name of DIM service
									char *typeDesc,       //type descriptor of service
									char *data,           //data of service
									int	 size,            //size of service in bytes
									int  safeMode         //1: only declare service, if size < maxLocalSize
								 )
{	
	int my_serviceID  = -1;
    int restartSearch = 1;
	int i;

	if (!serverActive) return errorCode;
	if (safeMode && (size > maxLocalBuffSize)) return -2;

	// get index of of element in the array "services". First search from
    // index of last service that has been created to MAXSERVICES
	i = lastServiceCreated;
	if ((i < 0) || (i >= MAXSERVICES)) i = 0;
	while ((i < MAXSERVICES) && (my_serviceID == -1))
	{
		i++;
		if ((i == MAXSERVICES) && (restartSearch)) {i = 0; restartSearch = 0;}
		if (services[i].my_serviceID == -1) my_serviceID = i;
	}
	
	//index valid? create service
	if ((my_serviceID >= 0) && (my_serviceID < MAXSERVICES))
	{
		//remember last (this) service that has been created
		lastServiceCreated = my_serviceID;

		//create service
		services[my_serviceID].buffer		= (char *)malloc(size);
		if (services[my_serviceID].buffer == NULL) return -1;
		memcpy((services[my_serviceID].buffer), data, size);
		services[my_serviceID].size         = size;
		services[my_serviceID].my_serviceID = my_serviceID;
		services[my_serviceID].serviceId    = dis_add_service(serviceName, typeDesc, (services[my_serviceID].buffer), services[my_serviceID].size, 0, 0);
	}
	else my_serviceID = -1;

	return my_serviceID; 
} //disAddService


//-----------------------------------------------------------------------------------
DIMWRAPPER_API int disUpdateService(int my_serviceID, //identifies the service to be removed
									char *data,       //data to be updated
									int  len          //size of data
								   )
{	
	if (!serverActive) return errorCode;

	//check range of my_serviceID
	if ((my_serviceID < 0) || (my_serviceID >= MAXSERVICES)) return -1;
	//check if my_serviceID is still valid
	if (services[my_serviceID].my_serviceID != my_serviceID) return -1;
	
	//check length
	if (len > services[my_serviceID].size) len = services[my_serviceID].size;
	if (len <= 0) return -1;
    
	//check addresses
    if (services[my_serviceID].buffer == NULL) return -1;
	if (data == NULL)						   return -1;

	//copy data and update service
	if (services[my_serviceID].my_serviceID != my_serviceID) return -1; //disRemoveService being executed?
	memcpy(services[my_serviceID].buffer, data, len);
	if (services[my_serviceID].my_serviceID != my_serviceID) return -1; //disRemoveService being executed?
	return dis_update_service(services[my_serviceID].serviceId);
} //disUpdateService


//-----------------------------------------------------------------------------------
DIMWRAPPER_API void disRemoveService(int my_serviceID //identifies the service to be removed
									 )
{	
	//check range
	if ((my_serviceID < 0) || (my_serviceID >= MAXSERVICES)) return;
	if (services[my_serviceID].my_serviceID != my_serviceID) return;

	//disable service to avoid servers may call disUpdateService
	services[my_serviceID].my_serviceID = -1;

	//wait a bit to ensure that a currently progressing disUpdateService does not go wrong
#ifdef WIN32
			usleep(1000);         //this is the shortest wait that is available on Windows without tricks: usleep(1000) = 2ms
#endif
#ifdef LINUX
			usleep(0);            //this is the shortest wait that is available on Linux without tricks: usleep(0) = 10ms
#endif

	//remove service
	dis_remove_service(services[my_serviceID].serviceId);

    //set element to default values
	free(services[my_serviceID].buffer);
	services[my_serviceID].size         = -1;
	services[my_serviceID].serviceId    = 0;

	//take care that next service to be created will use the array element of this service
	lastServiceCreated = my_serviceID - 1;
} //disRemoveService


//-----------------------------------------------------------------------------------
DIMWRAPPER_API int disSetQuality(int my_serviceID, //identifies the service
								 int quality	   //quality of service
								)		
{
	if (!serverActive) return errorCode;

	//check range of my_serviceID
	if ((my_serviceID < 0) || (my_serviceID >= MAXSERVICES)) return -1;
	//check if my_serviceID is still valid
	if (services[my_serviceID].my_serviceID == -1) return -1;

	dis_set_quality(services[my_serviceID].serviceId, quality);
	
    return 0;
} //disSetQuality


//-----------------------------------------------------------------------------------
DIMWRAPPER_API int disSetTimeStamp(	int		my_serviceID,	//identifies the service
									double	timeStamp		//timeStamp for service
								  )		
{
	int secs;		//time in seconds
	int millisecs;	//time in millisecons

	if (!serverActive) return errorCode;

	//check range of my_serviceID
	if ((my_serviceID < 0) || (my_serviceID >= MAXSERVICES)) return -1;
	//check if my_serviceID is still valid
	if (services[my_serviceID].my_serviceID == -1) return -1;

	secs		= (int)floor(timeStamp);
	millisecs	= (int)((timeStamp - (double)secs) * 1000.0);
	secs		= secs - 2082844800; //from 1904 (LabVIEW) to 1940 (DIM)
	dis_set_timestamp(services[my_serviceID].serviceId, secs, millisecs);
	
    return 0;
} //disSetTimeStamp


//-----------------------------------------------------------------------------------
DIMWRAPPER_API int disStartServing(char *serverName //name of server
				                  )
{	
	char my_message[MAXLEN];
	
    if (!serverActive) return errorCode;
	if (strlen(dimServerName) == 0)
	{
		sprintf(dimServerName, "%s", serverName);
		sprintf(my_message, "%.19s, PID %d, server %s: ", getTimeString(), getpid(), dimServerName);
		sprintf(my_message, "%s DIM server starting. \n", my_message);
		dw_printf(my_message);
	} //if strlen
	
    return dis_start_serving(dimServerName);
} //disStartServing


//-----------------------------------------------------------------------------------
DIMWRAPPER_API void disStopServing()
{	
	char my_message[MAXLEN];
	
    if (!serverActive) return;

	sprintf(my_message, "%.19s, PID %d, server %s: ", getTimeString(), getpid(), dimServerName);
	sprintf(my_message, "%s DIM server stopping. \n", my_message);
	dw_printf(my_message);

	dis_stop_serving();
} //disStartServing


//-----------------------------------------------------------------------------------
DIMWRAPPER_API int disAddCommand(char *commandName,    //name of DIM command
								 char *typeDesc,       //type description
								 unsigned refNum,      //LabVIEW refnum typecast to int
								 int refNumType        //0: message queue, 1: notifier
								)
{	
	int commandID     = -1; 
	int restartSearch = 1;
	int i;

	if (!serverActive) return errorCode;
	
	// get index of of element in the array "commands". First search from
    // index of last command that has been created to MAXSERVICES
	i = lastCallBackRegistered;
	if ((i < 0) && (i >= MAXSERVICES)) i = 0;
	while ((i < MAXSERVICES) && (commandID == -1))
	{
		i++;
		if (i == MAXSERVICES)
		{
			if (restartSearch) {i = 0; restartSearch = 0;}
			else return -1;
		}
		if (callBacks[i].callBackID == -1) commandID = i;
	}
			
	if ((commandID >= 0) && (commandID < MAXSERVICES)) //commandID is valid?
	{
		//allocate buffer for command name
		callBacks[commandID].serviceName    = malloc(strlen(commandName)+1);
		if (callBacks[commandID].serviceName == NULL) return -1;

		//remember ID of last callback registered
		lastCallBackRegistered = commandID;

		//register callback
		callBacks[commandID].callBackID		= commandID;
		callBacks[commandID].refNum			= refNum;
		callBacks[commandID].refNumType		= refNumType;
		callBacks[commandID].callBackType	= 1;
		sprintf(callBacks[commandID].serviceName, "%s", commandName);
		callBacks[commandID].serviceId		= dis_add_cmnd(commandName, typeDesc, receivedCB, commandID);
	} //array index o.k.?
	else commandID = -1;

	return commandID; 
} //disAddCommand


//-----------------------------------------------------------------------------------
DIMWRAPPER_API void disRemoveCommand(int my_commandID //identifies the command to be removed
									)
{	
	removeMyService(my_commandID);
} //disRemoveCommand
