// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DIMWRAPPER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DIMWRAPPER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef WIN32
#ifdef DIMWRAPPER_EXPORTS
#define DIMWRAPPER_API __declspec(dllexport)
#else
#define DIMWRAPPER_API __declspec(dllimport)
#endif
#endif
#ifdef LINUX
#define DIMWRAPPER_API
#endif

#include <extcode.h>

#ifdef LINUX
void __attribute__ ((constructor)) my_init(void);
void __attribute__ ((destructor)) my_fini(void);
#endif

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dimSetOccurrenceRef: sets the reference of the LabVIEW occurrence, that will
//           be fired when new service data is being received
// returns : void
//-----------------------------------------------------------------------------------
DIMWRAPPER_API void dimSetOccurrenceRef(unsigned occurrenceRef	//reference of occurrence typecast to unsigned
									   );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dimGetOccurrenceRef: gets the reference of the LabVIEW occurrence, that will
//           be fired when new service data is being received
// returns : void
//-----------------------------------------------------------------------------------
DIMWRAPPER_API void dimGetOccurrenceRef(unsigned *occurrenceRef	//reference of occurrence typecast to unsigned
									   );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dimFireOccurrence: fires (sets) the occurrence in LabVIEW. Before calling this
//           routine, the reference of the occurrence must be set using the routine dimSetOccurrenceRef.
// returns : void
//-----------------------------------------------------------------------------------
DIMWRAPPER_API void dimFireOccurrence();

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dimGetInfo: Gets information about DIM
// returns : void
//-----------------------------------------------------------------------------------
DIMWRAPPER_API void dimGetInfo(	int *versionNumber,		//DIM version against which this library has been built
								char *dimDnsNode,       //node of DIM name server
								int *maxLocalDataSize,	//maximum size for application internal DIM transfer
								int *pollingMode,       //0: LabVIEW occurrence enabled; 1: need to poll
								int *maxBuffSize,       //max size of buffer in polling mode
								char *nodes				//list of allowed nodes separeted by "|"
                              );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dimGetServiceInfo: Gets information about a service (service (dic) or command service (dis))
// returns : int: 0; -1 if error
DIMWRAPPER_API int dimGetServiceInfo(	int callBackID,			//callback ID of service
										int *refNumType,		//type of LV refnum (message queue, notifier, ...)
										unsigned *refNum,       //(pointer to) refnum typecast to unsigned
										unsigned *serviceID,	//DIM service ID
										int *callBackType,		//type of callback (service, command service, ...)
										char *typeDesc,			//type descriptor for data (DIM format)
										char *serviceName,		//name of DIM service
										int  *quality,          //quality of service
										double *timeStamp       //time stamp in seconds
									 );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dimWaitForData: Waits for new Dim data (dic or dis). This routine waits for new data by polling
//           Use this routine only, if the library has been compiled with compiler option "POLLING".
// returns : int: 0; -1 if error
DIMWRAPPER_API void dimWaitForData(	int timeout,                //timeout in ms
									int *timedOut,              //TRUE: timed out
									int *callBackID,            //callBackID
									unsigned *refNum,			//(pointer to) LVRefNum (typecast to unsigned)
									int *refNumType,            //type of refnum
									int *recSize,               //size of received data
									char *copyBuffer,           //pointer to copy buffer
									int copyBufferSize          //max size of copy buffer
								  );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dimEnableSecurity: Enables security feature. Only services from the
//           list of allowed nodes are executed. The list of nodes contains
//           the node names (without ".institution.org"), separated by "|".
//           The list is terminated by "|".
// returns : int: 1: success
DIMWRAPPER_API int dimEnableSecurity(	char *nodes	//list of allowed nodes
									 );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dimDisableSecurity: Disables security feature. Only services from the
//           list of allowed nodes are executed
// returns : void
DIMWRAPPER_API void dimDisableSecurity();

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dimSetDNSNode: Sets the node where DNS is running. This routine should be
//           called prior to declaring or subscribing to any services. If this routine
//           is not used, the environment variable DIM_DNS_NODE is used instead.
// returns : 1: if function executed successfully
DIMWRAPPER_API int dimSetDNSNode(char *node_name);

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dicInfoService: Register a call back function to a DIM service.
// returns : int callBackTag: the callback tag used by the callback function. -1: error
DIMWRAPPER_API int dicInfoService(char *serviceName,    //name of DIM service
								  unsigned refNum,      //LabVIEW refnum typecast to int
								  int refNumType,       //1: message queue, 2: notifier
								  int serviceType,      //1: once only, 2: timed, 4: monitored
								  int timeout,          //timeout (intervall) when to receive new data
								  int stamped			//1: get timestamp and quality
								 );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dicReleaseService: Unregister a call back function from a DIM service.
// returns : void
DIMWRAPPER_API void dicReleaseService(int callBackID //identifies the call back event to be unregistered
									 );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : dicCommandService: Requests the execution of a command at a server
// returns : "1": if command was successfully executed
//           "0": error
//           "-1": service size > max local size although safeMode was requested
DIMWRAPPER_API int dicCommandService(char *name,	 //identifies the command to be called
									 char *buffer,   //data
									 int  size,      //size of data
									 int  safeMode   //1: only send command, if size < maxLocalSize
								    );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : disAddService: Adds a DIM service to the list of provided services
// returns : int callBackTag: the callback tag used by the callback function. 
//           -1: error, 
//           -2: size of data larger than maxLocalSize although safe mode was requested
DIMWRAPPER_API int disAddService(	char *serviceName,    //name of DIM service
									char *typeDesc,       //type descriptor of service
									char *data,           //data of service
									int	 size,            //size of service in bytes
									int  safeMode         //1: only declare service, if size < maxLocalSize
								 );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : disUpdateService: change the service data and update clients
// returns : number of connected clients
DIMWRAPPER_API int disUpdateService(int my_serviceID, //identifies the service to be removed
									char *data,       //data to be updated
									int  size         //size of data
								    );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : disRemoveService: removes a service fromt the list of provided services
// returns : void
DIMWRAPPER_API void disRemoveService(int my_serviceID //identifies the service to be removed
									 );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : disSetQuality: sets the quality of a service
// returns : int: -1, if error; 0, if no error
DIMWRAPPER_API int disSetQuality(int my_serviceID, //identifies the service
								 int quality	   //quality of service
								);

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : disSetTimeStamp: sets the time stamp of a service
// returns : int: -1, if error; 0, if no error
DIMWRAPPER_API int disSetTimeStamp(	int		my_serviceID,	//identifies the service
									double	timeStamp		//timeStamp for service
								  );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : disStartServing: starts a DIM server
// returns : Returns 1 if the list of services and commands were succesfully sent to the Name Server, 0 otherwise.
DIMWRAPPER_API int disStartServing(char *serverName //name of server
				                   );

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : disStopServing: stops a DIM server
// returns : void
DIMWRAPPER_API void disStopServing();


//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : disAddCommand: Registers a call back function to a DIM command.
// returns : int callBackTag: the callback tag used by the callback function. -1: error
DIMWRAPPER_API int disAddCommand(char *commandName,    //name of DIM command
								 char *typeDesc,       //type description
								 unsigned refNum,      //LabVIEW refnum typecast to int 
								 int refNumType        //0: message queue, 1: notifier
								);

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : disRemoveCommand: removes a command from the list of provided command
// returns : void
DIMWRAPPER_API void disRemoveCommand(int my_commandID //identifies the command to be removed
									);
