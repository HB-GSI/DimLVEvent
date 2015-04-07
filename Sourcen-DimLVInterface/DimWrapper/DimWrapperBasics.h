#include <extcode.h>

#define MAXLEN              1024			//maximum length of standard items like names
#ifdef PHARLAP							
#define MAXSERVICES			1000  	        //maximum number of DIM services that can be published by a server
#define MAXBUFFERSIZE       300000			//maximum size of internal buffer, when using polling mode
#else
#define MAXSERVICES			100000          //maximum number of DIM services that can be published by a server
#define MAXBUFFERSIZE       3000000			//maximum size of internal buffer, when using polling mode
#endif
#define SEPARATOR			"|"				//seperates elements in a concated string
#define LOGFILENAME			"dimWrapper.log"//name of log file (for windows)
#define DIMWRAPPERPATH      "DIMWRAPPER_LOGPATH"//name of environment variable for folder of log file (for windows)

typedef struct
{
	unsigned   refNum;				//reference to LabVIEW item like a message queue or notifier
	int        refNumType;			//1: LabVIEW message queue, 2: LabVIEW noifier
	int        callBackID;			//identifies a call back event to a DIM service
	unsigned   serviceId;			//ID of a DIM service
	int        callBackType;		//command callback: 1; service callback: 2;
	char       *serviceName;		//name of service
} callBackType;

typedef struct
{
	char       *buffer;        //buffer to store the data of a DIM service
	int        size;           //size of the buffer
	int        my_serviceID;   //identifies a service (an element of this array) 
	unsigned   serviceId;      //ID of a DIM service
} serviceType;
