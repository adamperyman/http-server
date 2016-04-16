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

  int readStatus = recv(clientSocket, buffer, BUFF_SIZE - 1, 0);
  if (readStatus < 0) {
    fprintf(stderr, "Error: Reading from socket.\n");
    return;
  }

  // Temporary GET handling.
  char fileName[BUFF_SIZE] = { '\0' };
  sscanf(buffer, "GET /%s ", fileName);
  // End temporary GET handling.

  // Handle file request.
  FILE* fp = fopen(fileName, "rb");
  if (!fp) {
    fprintf(stderr, "Error: File opening.\n");
    return;
  }

  // Temporary header.
  char* header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\ncharset=UTF-8\r\n";
  if (send(clientSocket, &header, strlen(header), 0) < 0) {
    fprintf(stderr, "Error: Writing to socket.\n");
    return;
  }
  // End temporary header.

  fprintf(stdout, "Sending: %s\n", fileName);

  char fileChar = 0;
  while ((fileChar = fgetc(fp)) != EOF) {
    if (send(clientSocket, &fileChar, sizeof(char), 0) < 0) {
      fprintf(stderr, "Error: Writing to socket.\n");
      return;
    }
  }
  fclose(fp);

  fprintf(stdout, "Sent: %s\n\n", fileName);
  fprintf(stdout, "Waiting..\n");
}
