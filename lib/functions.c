#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "macros.h"

void _printHeading (void) {
  fprintf(stdout, "*************************************\n");
  fprintf(stdout, "*                                   *\n");
  fprintf(stdout, "*        A Simple Web Server        *\n");
  fprintf(stdout, "*                                   *\n");
  fprintf(stdout, "* Author: Adam Peryman              *\n");
  fprintf(stdout, "* Contact: adam.peryman@gmail.com   *\n");
  fprintf(stdout, "*************************************\n");
}

void _sendFile (int clientSocket) {
  char buffer[BUFF_SIZE] = { '\0' };
  bzero(buffer, BUFF_SIZE);

  int readStatus = read(clientSocket, buffer, BUFF_SIZE);
  if (readStatus < 0) {
    fprintf(stderr, "Error: Reading from socket.\n");
    return;
  }

  // Temporary GET handling.
  char fileName[BUFF_SIZE] = { '\0' };
  sscanf(buffer, "GET /%s ", fileName);

  // Handle file request.
  FILE* fp = fopen(fileName, "rb");
  if (!fp) {
    fprintf(stderr, "Error: File opening.\n");
    return;
  }

  fprintf(stdout, "Sending: %s\n", fileName);

  char fileChar = 0;
  int i = -1;
  bzero(buffer, BUFF_SIZE);
  while ((fileChar = fgetc(fp)) != EOF) {
    buffer[++i] = fileChar;
  }
  fclose(fp);

  // Temporary header handling.
  char header[BUFF_SIZE] = "HTTP/1.1 200 OK\r\n";
  char contentType[BUFF_SIZE] = "Content-Type: text/html\r\n";

  char response[BUFF_SIZE * 5] = { '\0' };
  strcat(response, header);
  strcat(response, contentType);
  strcat(response, buffer);
  strcat(response, "\r\n\r\n");

  fprintf(stdout, "RESP: %s\n", response);

  if (write(clientSocket, response, strlen(response) + 2) < 0) {
    fprintf(stderr, "Error: Sending.");
    return;
  };

  fprintf(stdout, "Sent: %s\n\n", fileName);
  fprintf(stdout, "Waiting..\n\n");
}
