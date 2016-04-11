#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "macros.h"

void _printHeading (void) {
  fprintf(stdout, "**************************************\n");
  fprintf(stdout, "*                                    *\n");
  fprintf(stdout, "*        A simple HTTP Server        *\n");
  fprintf(stdout, "*                                    *\n");
  fprintf(stdout, "* Author: Adam Peryman               *\n");
  fprintf(stdout, "* Contact: adam.peryman@gmail.com    *\n");
  fprintf(stdout, "**************************************\n");
}

void _sendFile (int clientSocket) {
  char buffer[BUFF_SIZE] = { '\0' };
  bzero(buffer, BUFF_SIZE);

  int readStatus = read(clientSocket, buffer, BUFF_SIZE - 1);
  if (readStatus < 0) {
    fprintf(stderr, "Error: Reading from socket.\n");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Buffer: %s\n", buffer);

  int writeStatus = write(clientSocket, "Hello, world!", strlen("Hello, world!"));
  if (writeStatus < 0) {
    fprintf(stderr, "Error: Writing to socket.\n");
    exit(EXIT_FAILURE);
  }
}
