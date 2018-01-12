/*
** network.h
** Login : <mouret@fortaleza.lip6.fr>
** Started on  Wed Sep 10 14:47:57 2008 Jean-Baptiste MOURET
*/

#ifndef   	NETWORK_H_
# define   	NETWORK_H_

#define HTML		    1
#define BIN		    0

int new_server_socket(int port);
int server_accept(int server_socket);
void send_http_buffer(int client_socket, const char* content, int content_size,
		      int type);
void read_request(int client_socket, char** filename);

#endif 	    /* !NETWORK_H_ */
