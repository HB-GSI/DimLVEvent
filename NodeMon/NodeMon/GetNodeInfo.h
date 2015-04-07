// GetInfo.h : include file for procedures that obtain system information
#if !defined(GETINFO_H)
#define GETINFO_H

#define MAXPROCS     1000  //max number of processes to be published
#define MAXLENGTH    128   //max number of characters for a name

#ifdef WIN32
#define TICKS_SECOND 1000  //unit of time in ticks per second
#endif
#ifdef LINUX
#define TICKS_SECOND 1000  //unit of time in ticks per second
#endif


typedef struct _PROCESSINFORMATION
{
	char			processName[MAXLENGTH];	// name of process (image)
	int				processID;				// PID
	int				processMem;				// used memory
	unsigned int	processRunTime;			// total CPU usage in TICKS_SECOND
	double			processTime;			// CPU usage in %

} PROCESSINFORMATION;

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : GetNodeOS: obtains a description string of the operating system 
// returns : void
void GetNodeOS(char *OS);

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : NodeSleep: causes a thread to sleep 
// returns : void
void NodeSleep(unsigned int milliSeconds);

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : GetNodeName: gets the name of a node 
// returns : void
void GetNodeName(char *nodeName);

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : GetProcsInfo: gets the actual information about processes
// returns : void
void GetProcsInfo(PROCESSINFORMATION *procInfo, int *numberProcs);

#endif // !defined(GETINFO_H)
