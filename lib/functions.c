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
  // Temporary header.
  char* header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  write(clientSocket, header, strlen(header));
  // End temporary header.

  char buffer[BUFF_SIZE] = { '\0' };
  bzero(buffer, BUFF_SIZE);

  int readStatus = read(clientSocket, buffer, BUFF_SIZE - 1);
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

  fprintf(stdout, "Sending: %s\n\n", fileName);

  int fileChar = 0;
  while ((fileChar = fgetc(fp)) != EOF) {
    int writeStatus = write(clientSocket, &fileChar, sizeof(char));

    if (writeStatus < 0) {
      fprintf(stderr, "Error: Writing to socket.\n");
      return;
    }
  }
  fclose(fp);
}
