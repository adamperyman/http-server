/**
 * Adam Peryman (adam.peryman@gmail.com)
 * 11/04/2016
 *
 * A simple multi-threaded HTTP server written in C.
 */

/**
 * TODO:
 * 1. Make IP Agnostic.
 * 2. Add encryption.
 * 3. Make multi-threaded.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "functions.h"
#include "macros.h"

int main(void) {
  _printHeading();

  // Setup welcome socket.
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket < 0) {
    fprintf(stderr, "Error: Opening socket.\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serverAddress = {};
  bzero((char*) &serverAddress, sizeof(serverAddress));

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(PORT_NO);

  // Bind host address.
  if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    fprintf(stderr, "Error: Binding.\n");
    exit(EXIT_FAILURE);
  }

  // Wait for requests.
  listen(serverSocket, MAX_CONNS);
  struct sockaddr_in clientAddress = {};
  socklen_t clientAddressLength = sizeof(clientAddress);

  while (1) {
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
      fprintf(stderr, "Error: Accepting connection.\n");
      exit(EXIT_FAILURE);
    }

    int pid = fork();
    if (pid < 0) {
      fprintf(stderr, "Error: Creating child process.\n");
      exit(EXIT_FAILURE);
    }

    // Got the client process.
    if (pid == 0) {
      close(serverSocket);
      _sendFile(clientSocket);
      exit(EXIT_SUCCESS);
    } else {
      close(clientSocket);
    }
  }
}
