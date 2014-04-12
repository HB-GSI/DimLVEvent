#include <stdio.h>
#include <dis.h> 

int buffer[] = { 0,1,2,3,4,5,6,7,8,9 };  
int service_id;  
  
void build_service(tag, address, size)  
long *tag;  
int **address;  
int *size;  
{
  int conn_id;
  char *name[128];

  
  dis_get_client (name);
  printf(name);
  printf("\n");

  conn_id = dis_get_conn_id();
  sprintf(name, "conn_ID %d\n", conn_id );
  printf(name);

  sprintf(name, "timeout %d\n", dis_get_timeout (service_id, conn_id));
  printf(name);

  sprintf(name, "%s\n",dis_get_client_services (conn_id));
  printf(name);

  *address = buffer;  
  *size = sizeof(buffer);


}    

void execute_cmnd(tag, cmnd_buffer, size)  
long *tag;  
char *cmnd_buffer;  
int *size;  
{  
    if(*tag == 1)  
    {  
        printf("SERV_CMND: Command %s received\n",cmnd_buffer);  
        dis_update_service(service_id);  
    }  
}  
  
main()  
{  
  
    dis_add_service("SERV_BY_BUFFER", "L", buffer, 40, 0, 0);  
    service_id = dis_add_service("SERV_BY_ROUTINE", "L", 0, 0,  
        build_service, 0);  
	//    dis_add_cmnd("SERV_CMND", 0, execute_cmnd, 1);  
    dis_start_serving("DIS_TEST");  
    while(1)  
    {  
        sleep(10);  
    }  
}  

