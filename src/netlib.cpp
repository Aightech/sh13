#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>
#include <arpa/inet.h>

#include <sys/ioctl.h>


#include <net/if.h>
#include <sys/wait.h>

#include <fcntl.h>

#include "netlib.hpp"


int sendTCP(char *IPaddress, int port, Buffer *buff)
{
       struct sockaddr_in serv_addr;
       
       int sockfd = socket(AF_INET, SOCK_STREAM, 0);
       if (sockfd < 0) 
              printf("ERROR opening socket\n");
       
       serv_addr.sin_family = AF_INET;
       serv_addr.sin_addr.s_addr=inet_addr(IPaddress);
       serv_addr.sin_port = htons(port);
       
       int flag=0;
       
       if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) //try to connect
              printf("Couldn't connect to %s\n\n", "add");
       
       if(buff->T_flag)// if there is something to send  
       {
              if(write(sockfd,buff->Rx,strlen(buff->Rx))==strlen(buff->Rx))
              {
                     buff->T_flag=0;//the data has been sent
                     if(read(sockfd,buff->Rx,255)>0)
                     {
                            buff->R_flag=1;//data received
                            flag =1;
                     }
              }
              else
              {
                     printf("Couldn't send everything.\n\n");
                     flag=-1;
              }
       }
       else
       {
                printf("Transfer flag down, supposely nothing to send.\n\n");
                flag=-1;
       }
       
       close(sockfd);//closing the socket.
       
       //flag value:
       // -1 : problem occured
       //  0 : everthing was sent but nothing was replied
       //  1 : everthing was sent and something was replied  
       
       return flag;
       
}
