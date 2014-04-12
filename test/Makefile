@usage: gmake all
DIMDIR = /misc/beck/bin/dim
INCLUDE = -I. -I$(DIMDIR)/dim
LIBS	= -L$(DIMDIR)/linux -ldim -lpthread

all: test_server test_client schrott

test_server: 
	 @echo Making target test_server
	 gcc -g $(INCLUDE) -o test_server test_server.c $(LIBS)

test_client: 
	 @echo Making target test_client
	 gcc -g $(INCLUDE) -o test_client test_client.c $(LIBS)

schrott:
	 @echo Making target db_dim_server_test
	 gcc -g $(INCLUDE) -o db_dim_server_test db_dim_server_test.c $(LIBS)