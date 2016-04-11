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
  PrintHeading();

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

  if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    fprintf(stderr, "Error: Binding.\n");
    exit(EXIT_FAILURE);
  }

  listen(serverSocket, MAX_CONNS);
  struct sockaddr_in clientAddress = {};
  socklen_t clientAddressLength = sizeof(clientAddress);

  int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
  if (clientSocket < 0) {
    fprintf(stderr, "Error: Accepting connection.\n");
    exit(EXIT_FAILURE);
  }

  char buffer[BUFF_SIZE] = { '\0' };
  bzero(buffer, BUFF_SIZE);

  int n = read(clientSocket, buffer, BUFF_SIZE - 1);
  if (n < 0) {
    fprintf(stderr, "Error: Writing to socket.\n");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Buffer: %s\n", buffer);
}
