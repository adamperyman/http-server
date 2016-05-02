#ifndef SOCKETS
#define SOCKETS

/**
 *  Return a valid welcome socket.
 */
int _getServerSocket(void);

/**
 *  Return an ipv4 server address.
 */
sockaddr_in _getServerAddress(size_t);

/**
 *  Bind server address to server socket.
 */
void _bindHostAddress(int, sockaddr*);

/**
 *  Listen on server socket for N incoming connections.
 */
void _startListening(int, size_t);

/**
 *  Start connection loop and do servery things.
 */
void _startConnectionHandler(int);

#endif // SOCKETS.
