#include <stdio.h>
#include <sys/socket.h>

#ifndef SOCKETS
#define SOCKETS

/**
 *  Return a valid welcome socket.
 */
int _getIPV4ServerSocket(void);

/**
 *  Return an ipv4 server address.
 */
struct sockaddr_in _getServerAddress(size_t);

/**
 *  Bind server address to server socket.
 */
void _bindHostAddress(int, struct sockaddr_in);

/**
 *  Listen on server socket for N incoming connections.
 */
void _startListening(int, size_t);

/**
 *  Start connection loop and do servery things.
 */
void _startConnectionHandler(int);

#endif // SOCKETS.
