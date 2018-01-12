/*
** network.c
** Login : <mouret@fortaleza.lip6.fr>
** Started on  Wed Sep 10 14:56:17 2008 Jean-Baptiste MOURET
** $Id$
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>


#include "network.hpp"

#define SOCKET_ERROR        -1
#define QUEUE_SIZE          15
#define BUFFER_SIZE	    1024

static void _die(const char* str)
{
  perror(str);
  exit(1);
}

int new_server_socket(int port)
{
  int server_socket;
  struct sockaddr_in address;
  // 1. socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
  address.sin_family = AF_INET;
  // 2. bind
  if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR)
    _die("bind");
  // 3. listen
  if(listen(server_socket, QUEUE_SIZE) == SOCKET_ERROR)
    _die("listen");
  return server_socket;
}

int server_accept(int server_socket)
{
  assert(server_socket != SOCKET_ERROR);
  int client_socket;
  struct sockaddr_in address;
  int address_size = sizeof(struct sockaddr_in);
  
  client_socket = accept(server_socket, 
			 (struct sockaddr*)&address, 
			 (socklen_t *)&address_size);
  printf("client connected, socket=%d\n", client_socket);
  return client_socket;
}

void send_http_buffer(int client_socket, const char* content, int content_size,
		      int type)
{
  char res[BUFFER_SIZE];
  const char* header = "HTTP/1.0 200 OK";
  time_t t = time(0x0);
  const char* date = ctime(&t);
  memset(res, 0, BUFFER_SIZE);
  if (type == HTML)
    sprintf(res, "%s\n%sContent-Type: text/html\nContent-Length: %d\n\n", 
	    header, date, content_size);  
  else
    sprintf(res, "%s\n%sContent-Type: application/octet-stream\nContent-Length: %d\n\n",
	    header, date, content_size);  
  write(client_socket, res, strlen(res));
  write(client_socket, content, content_size);
  usleep(1000);
  close(client_socket);
}

void read_request(int client_socket, char** filename)
{
  assert(client_socket != SOCKET_ERROR);
  char buffer[BUFFER_SIZE], 
    fname[BUFFER_SIZE], 
    req[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  memset(req, 0, BUFFER_SIZE);
  memset(fname, 0, BUFFER_SIZE);
  int k = read(client_socket, buffer, BUFFER_SIZE);
  if (k == -1)
    _die("read ");
  printf("request :%d\n%s\n", k, buffer);
  sscanf(buffer, "%s %s", req, fname);
  if (*filename !=  NULL)
  {
  	*filename = (char*)calloc(strlen(fname), sizeof(char));
  	strncpy(*filename, fname + 1, strlen(fname) - 1);
  }
}
