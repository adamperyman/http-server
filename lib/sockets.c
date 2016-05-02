#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "file-handling.h"

int _getIPV4ServerSocket (void) {
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (serverSocket < 0) {
    fprintf(stderr, "Error: Opening socket.\n");
    exit(EXIT_FAILURE);
  }

  return serverSocket;
}

struct sockaddr_in _getServerAddress (size_t _portNumber) {
  struct sockaddr_in serverAddress = {};
  bzero((char*) &serverAddress, sizeof(serverAddress));

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(_portNumber);

  return serverAddress;
}

void _bindHostAddress (int _serverSocket, struct sockaddr* _serverAddress) {
  if (bind(_serverSocket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) < 0) {
    fprintf(stderr, "Error: Binding.\n");
    exit(EXIT_FAILURE);
  }
}

void _startListening (int _serverSocket, size_t _maxConnections) {
  if (listen(_serverSocket, _maxConnections) < 0) {
    fprintf(stderr, "Error: Listening.\n");
    exit(EXIT_FAILURE);
  }
}

void _startConnectionHandler (int _serverSocket) {
  while (1) {
    struct sockaddr_in clientAddress = {};
    socklen_t clientAddressLength = sizeof(clientAddress);

    fprintf(stdout, "Waiting..\n\n");

    int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
      fprintf(stderr, "Error: Accepting connection.\n");
      exit(EXIT_FAILURE);
    }

    int pid = fork();
    if (pid < 0) {
      fprintf(stderr, "Error: Creating child process.\n");
      exit(EXIT_FAILURE);
    }

    // Handle sockets.
    if (pid == 0) {
      close(_serverSocket);
      _sendFile(clientSocket);
      exit(EXIT_SUCCESS);
    } else {
      close(clientSocket);
    }
  }
}
