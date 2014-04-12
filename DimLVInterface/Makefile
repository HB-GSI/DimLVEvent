@usage: gmake all
ODIR  	= ../bin
IDIR   	= DimWrapper
LVDIR   = /usr/local/lv82/cintools
DIMDIR	= ../../../../../../dim
INCLUDE =  -I$(LVDIR) -I$(ODIR) -I$(IDIR) -I$(DIMDIR)/dim
LIBS	= -L$(ODIR) -lmydim 
MY_OS   = LINUX

all: libDimWrapper
clean: cleanLib

libDimWrapper: $(IDIR)/DimWrapper.c
	@echo Making target libDimWrapper
	gcc  -D$(MY_OS) -DLABVIEWGSI -fPIC -g -c -Wall $(INCLUDE)  $(IDIR)/DimWrapper.c
	gcc -shared  -o $(ODIR)/libDimWrapper.so DimWrapper.o $(LIBS)

cleanLib:
	@echo removing shared object libraries
	rm $(ODIR)/libDimWrapper.so 
	rm DimWrapper.o
