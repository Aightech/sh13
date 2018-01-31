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

#include <err.h>
#include <errno.h>

#include "vector"
#include <iostream>


int sendTCP(const char *IPaddress, int port, Buffer *buff)
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
              printf("Couldn't connect to [%s] on port : %d\n\n", IPaddress,port);
       
       if(buff->T_flag)// if there is something to send  
       {
              if(write(sockfd,buff->Tx,strlen(buff->Tx))==strlen(buff->Tx))
              {
                     buff->T_flag=0;//the data has been sent
                     if(!buff->R_flag)//no data un read
                     {
                            if(read(sockfd,buff->Rx,255)>0)
                            {
                                   buff->R_flag=1;//data received
                                   flag =1;
                            }
                     }
                     else
                         printf("Couldn't read because of unprocess data in the buffer.\n\n");   
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

int scanServers(Server servers[5])
{
             
       // --- Find the default IP interface --- //
       FILE *f;
       char line[100] , *iface , *c;
       f = fopen("/proc/net/route" , "r");
       while(fgets(line , 100 , f))
       {
              iface = strtok(line , " \t");
              c = strtok(NULL , " \t");
              if(iface!=NULL && c!=NULL && strcmp(c , "00000000") == 0)
                     break;
       } 
       fclose(f);
       // ------------------------------------- //
       
       
       // --- Find the IP address of the default interface --- //
       int fd;
       struct ifreq ifr;

       fd = socket(AF_INET, SOCK_DGRAM, 0);
       ifr.ifr_addr.sa_family = AF_INET;           //Type of address to retrieve - IPv4 IP address
       strncpy(ifr.ifr_name , iface , IFNAMSIZ-1); //Copy the interface name in the ifreq structure
       ioctl(fd, SIOCGIFADDR, &ifr);               //Get the ip address
       close(fd);
       char addressIP[16];
       strcpy(addressIP,inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) );
       // --------------------------------------------------- //
       
       //display result
       printf("Default interface is : %s \n" , iface);
       printf("IP address : %s\n" , addressIP );
       
       // --- Get The pc ID --- //
        //look for the last point of the ip address
       int i=0,pt=0;
       while(i<16&&pt<3)
              if(addressIP[i++]=='.')
                     pt++;
                     
       int pc = atoi(addressIP+i); //get the host id of the computer  
       addressIP[i]='\0';
       // ------------------------ //
       
       
       char addr[16];
       int sfd;
       int sockfd, portno;
       struct sockaddr_in serv_addr;
       int res, valopt; 
       long arg; 
       fd_set myset; 
       struct timeval tv; 
       socklen_t lon;
       
       char serv_IPaddr[255][16];
       int h=0,s=0;
       int testingPort=SERVERPORT;
       
       
       printf("scanning...\n[");
       fflush(stdout);
       for(int i=0;i<255;i++)
       {
              if(i%5==0)
              {
                     printf("#");
                     fflush(stdout);
              }
              sprintf(addr,"%s%d" ,addressIP,i);
              
              sfd = socket(AF_INET, SOCK_STREAM, 0);
              
              if (sfd < 0) printf("ERROR opening socket");
              
              // Set non-blocking 
              arg = fcntl(sfd, F_GETFL, NULL); 
              arg |= O_NONBLOCK; 
              fcntl(sfd, F_SETFL, arg);
              
              serv_addr.sin_family = AF_INET;
              serv_addr.sin_addr.s_addr=inet_addr(addr);
              serv_addr.sin_port = htons(testingPort);
              if (connect(sfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
              { 
                     if (errno == EINPROGRESS) 
                     {
		              //printf("connect EINPROGRESS OK (expected)   ");
		              tv.tv_sec = 0; 
                            tv.tv_usec = 10000; 
                            FD_ZERO(&myset); 
                            FD_SET(sfd, &myset); 
                            if (select(sfd+1, NULL, &myset, NULL, &tv) > 0) 
                            { 
                                   lon = sizeof(int); 
                                   getsockopt(sfd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon); 
                                   if(valopt==0)
                                   { 
                                          if(h<255)
                                                 strcpy(serv_IPaddr[h++],addr);
                                   }
                                   
                            }  
                     }
			
              }
              else
                     printf("somebody on %s\n\n" , addr);
                     
              arg = fcntl(sfd, F_GETFL, NULL); 
              arg &= (~O_NONBLOCK); 
              fcntl(sfd, F_SETFL, arg);       
                     
              close(sfd);
       }
       printf("]\n");
       
       
       Buffer B;
       strcpy(B.Tx,"G0");
       for(i=0;i<h;i++)
       {
              printf("- Somebody on: %s : ",serv_IPaddr[i]);
              B.T_flag=1;
              sendTCP(serv_IPaddr[i],testingPort,&B);
              if(atoi(strchr(B.Rx,'G')+1)==0)
              {
                     printf("\tsh13 server\n");
                     strcpy(servers[s].IPaddress,serv_IPaddr[i]);
                     strcpy(servers[s].name, strtok((strchr(B.Rx,'N')+1),";"));
                     servers[s++].portNo=testingPort;
              }
              else
                     printf("not a sh13 server\n");
              
       }
       return s;
       
}
       

