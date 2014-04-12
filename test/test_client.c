#include <stdio.h>
#include <dic.h> 

int buffer[10];
char *buff[128]; 
int no_link = -1; 
int version; 
  
buff_received(tag, bufferp, size) 
long *tag;
char *bufferp;
 int *size; 
{ 
int conn_id; 
  
   if(bufferp[0] == -1) 
        printf("Service SERV_BY_BUFFER not available\n"); 
   else 
   { 
        printf("received service SERV_BY_BUFFER\n\t"); 
		conn_id = dic_get_conn_id();
		sprintf(buff, "conn_id = %d\n", conn_id);
		printf(buff);
		dic_get_server (buff);
		sprintf(buff, "%s\n", buff); 
		printf(buff);
		printf(dic_get_server_services(conn_id));
		printf("\n");
		dic_get_id (buff);
		sprintf(buff, "%s\n", buff); 
		printf(buff);
   } 
} 
  
main() 
{ 
  
   dic_info_service("12345test", MONITORED, 10, 
        buffer, 40, buff_received, 0, &no_link,4); 
   { 
        sleep(5); 
   } 
} 


