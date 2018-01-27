#ifndef   	NETLIB_H
# define   	NETLIB_H

#define SIZE_BUFF 256

/*! \file offline.h
    \brief offline related functions descriptions.
    \author Maeva Arlandis et Alexis Devillard
    \version 6.2
    \date 10 janvier 2017
*/


typedef struct _Buffer{//the buffer
       
	char Rx[SIZE_BUFF]; /**< buffer of data received */
       char R_flag;  /**< received flag */
       int R_port; /**< The port were it received data. */
       
       char Tx[SIZE_BUFF]; /**< buffer of data sent */
       char T_flag;  /**< sent flag */
       	
	
}Buffer;


/*! \fn int sendTCP(char *IPaddress,int port, Buff *buff);
    \brief Send to the IPaddress on the port passed in argumment the buffer Tx buffer of the stuctur buff passed.
    \param IPaddress the destinaion IP address.
    \param port the port of the destinaion.
*/
int sendTCP(char *IPaddress,int port, Buffer *buff);

#endif 	  
