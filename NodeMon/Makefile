@usage: gmake all
ODIR    = ../../../../binaries/DomainManagementSystem
IDIR    = NodeMon/
DIMDIR  = /home/kstest/dim
INCLUDE = -I$(IDIR) -I$(DIMDIR)/dim
LIBS	= -L$(DIMDIR)/linux -ldim -lpthread
MY_OS   =  LINUX

IDIRLIB = libNodeInfo/
ODIRLIB = bin

all: 		nodemon libInfo

mon: 		nodemon

libInfo: 	libNodeInfo


nodemon: $(IDIR)NodeMon.c $(IDIR)GetNodeInfo.c
	@echo Making target nodemon
	gcc -g -Wall -D$(MY_OS) $(INCLUDE) -o $(ODIR)/NodeMon $(IDIR)NodeMon.c $(IDIR)GetNodeInfo.c $(LIBS)

libNodeInfo: $(IDIRLIB)libNodeInfo.c $(IDIR)GetNodeInfo.c
	@echo Making target libNodeInfo
	gcc -shared -D$(MY_OS) $(INCLUDE) -fPIC -g -Wall -o $(ODIRLIB)/libNodeInfo.so $(IDIRLIB)libNodeInfo.c $(IDIR)GetNodeInfo.c