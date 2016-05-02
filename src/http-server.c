/**
 * Adam Peryman (adam.peryman@gmail.com)
 * 11/04/2016
 *
 * A simple multi-threaded HTTP server written in C.
 */

#include <arpa/inet.h>
#include <unistd.h>
#include "output.h"
#include "sockets.h"
#include "macros.h"

int main(void) {
  _printWelcomeMessage();

  int serverSocket = _getIPV4ServerSocket();
  struct sockaddr_in serverAddress = _getServerAddress(PORT_NO);

  _bindHostAddress(serverSocket, serverAddress);
  _startListening(serverSocket, MAX_CONNS);

  _startConnectionHandler(serverSocket);

  close(serverSocket);
  return 0;
}
