/**
 *  Adam Peryman (adam.peryman@gmail.com)
 *  10/04/2016
 *
 *  A multi-threaded HTTP server written in C.
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include "functions.h"
#include "macros.h"

int main(void)
{
    PrintHeading();

    // Initialize Windows sockets.
    #if _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(1, 1), &wsaData);
    #endif

    // Construct address information.
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(MY_PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));

    // Create and bind socket.
    int s = socket(PF_INET, SOCK_STREAM, 0);
    bind(s, (struct sockaddr*)&address, sizeof(address));

    // Max 10 incoming connections.
    listen(s, 10);
    fprintf(stdout, "> Waiting..\n\n");

    // Send requested file(s).
    char* header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    while (!PIGS_FLY)
    {
        pthread_t threadBuffer;
        int fd = accept(s, NULL, NULL);
        send(fd, header, strlen(header), 0);
        pthread_create(&threadBuffer, NULL, (void*)&SendFile, NULL);
    }

    return 0;
}
