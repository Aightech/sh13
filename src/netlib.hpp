#ifndef   	NETLIB_H
# define   	NETLIB_H

#define SIZE_BUFF 256

#define SERVERPORT 4013

/*! \file netlib.h
    \brief TCP communication functions
    \author Alexis Devillard
    \version 6.2
    \date 30 janvier 2018
*/

/**
 * \struct _Buffer
 * \brief Structures of transmission and reception buffers.
 *
 * _Buffer make easier the process of transmission and emmission with a systeme of flag.
 */
typedef struct _Buffer{//the buffer
       
	char Rx[SIZE_BUFF]; /**< buffer of data received */
       char R_flag;  /**< received flag */
       int R_port; /**< The port were it received data. */
       
       char Tx[SIZE_BUFF]; /**< buffer of data sent */
       char T_flag;  /**< transmission flag */
       	
	
}Buffer;

/**
 * \struct _Server
 * \brief Structure of different data link to severs.
 */
typedef struct _Server {

	char name[16];/**< name of the server */
	char IPaddress[16];/**< IP address of the server */
	int  portNo;/**< Port of the server */
	
	int nbPlayers;/**< Number of player in the server */
}Server;


/*! \fn int sendTCP(char *IPaddress,int port, Buff *buff);
    \brief Send to the IPaddress on the port passed in argumment the buffer Tx buffer of the stuctur buff passed.
    \param IPaddress the destinaion IP address.
    \param port the port of the destinaion.
*/
int sendTCP(const char *IPaddress,int port, Buffer *buff);

/*! \fn scanServers(Server servers[5]);
    \brief Scan the different IP address and store the servers sh13 found inside the servers array passed in argument. 
    \param server[5] Arrays of servers found.
*/
int scanServers(Server servers[5]);

#endif 	  
