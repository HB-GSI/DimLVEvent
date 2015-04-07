// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DIMWRAPPER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DIMWRAPPER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef WIN32
#ifdef LIBNODEINFO_EXPORTS
#define LIBNODEINFO_API __declspec(dllexport)
#else
#define LIBNODEINFO_API __declspec(dllimport)
#endif
#endif
#ifdef LINUX
#define LIBNODEINFO_API
#endif

#ifdef LINUX
void __attribute__ ((constructor)) my_init(void);
void __attribute__ ((destructor)) my_fini(void);
#endif

#define MAXLEN 128
//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : getNodeName: gets name of this node
// returns : void
//-----------------------------------------------------------------------------------
LIBNODEINFO_API void getNodeName(char *nodeName);

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : getNodeOS: obtains a description string of the operating system 
// returns : void
LIBNODEINFO_API void getNodeOS(char *OS);

//-----------------------------------------------------------------------------------
// author  : Dietrich Beck, GSI-Darmstadt
// purpose : getProcessInfo: gets information about process
// returns : void
//-----------------------------------------------------------------------------------
LIBNODEINFO_API void getProcessInfo(	char *pNames,	//list of processes separated by "|"
										int pNamesLen,  //length of pNames buffer
										int *nProcs,    //number of processes
										int *pId,		//ID of processes
										int *pMem,		//memory of processes
										double *pRunT,		//runtime of process [s]
										int size		//max size of previous arrays
									);
