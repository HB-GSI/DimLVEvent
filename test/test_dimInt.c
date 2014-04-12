#include <stdio.h>
#include <dic.h> 

int buffer[10]; 
int no_link = -1; 
int version; 
  
buff_received(tag, bufferp, size) 
long *tag;
char *bufferp;
 int *size; 
{ 
int i; 
  
   if(bufferp[0] == -1) 
        printf("Service SERV_BY_BUFFER not available\n"); 
   else 
   { 
        printf("received service SERV_BY_BUFFER\n\t"); 
		
   } 
   printf("\n"); 
} 
  
main() 
{ 
  
   dic_info_service("test653_pdcCounter", MONITORED, 10, 
        buffer, 40, buff_received, 0, &no_link,4); 
   while(1) 
   { 
        sleep(5); 
   } 
} 


