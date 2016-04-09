/**
 *  Adam Peryman (adam.peryman@gmail.com)
 *  10/04/2016
 *
 *  A multi-threaded HTTP server written in C.
 */

#include <stdio.h>

#if _WIN32
#include <winsock.h>
// Fix for linker issue in VS2013.
#pragma comment(lib, "Ws2_32.lib")
#endif

// Port number for process.
#ifndef MY_PORT
#define MY_PORT 3490
#endif

// Sorry, it's not often I get to use infinite loops.
#ifndef PIGS_FLY
#define PIGS_FLY 0
#endif

// Print a heading with basic info.
void PrintHeading(void);

// Send requested file in its own thread.
unsigned long CALLBACK SendFile(void*);

int main(void)
{
    PrintHeading();

    // Initialize Windows sockets.
    #if _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(1, 1), &wsaData);
    #endif

    // Construct address information.
    struct sockaddr_in address = {};
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
        SOCKET fd = accept(s, NULL, NULL);
        send(fd, header, strlen(header), 0);
        CreateThread(NULL, 0, SendFile, fd, 0, 0);
    }

    return 0;
}

void PrintHeading(void)
{
    fprintf(stdout, "*********************************\n");
    fprintf(stdout, "*     159.335 Assignment 01     *\n");
    fprintf(stdout, "*    Adam Peryman (09079122)    *\n");
    fprintf(stdout, "*                               *\n");
    fprintf(stdout, "*   A simple web server in C.   *\n");
    fprintf(stdout, "*********************************\n");
}

unsigned long CALLBACK SendFile(void* fd)
{
    // Get request.
    char data[512] = { '\0' };
    int request = recv(fd, data, 512, 0);
    data[request] = '\0';

    // Get filename from request.
    char fileName[256] = { '\0' };
    sscanf(data, "GET /%s ", fileName);

    // Check filename.
    FILE* fp = fopen(fileName, "rb");
    if (!fp)
    {
        fprintf(stderr, "Error! Can't find %s.\n\n", fileName);
        fprintf(stdout, "> Waiting..\n\n");
        return 0;
    }

    // Get and send file.
    fprintf(stdout, "Sending %s.\n\n", fileName);
    int tempChar = 0;
    while ((tempChar = fgetc(fp)) != EOF)
    {
        send(fd, tempChar, sizeof(tempChar), 0);
    }

    // We're done here.
    fprintf(stdout, "Finished sending %s!\n\n", fileName);
    fprintf(stdout, "> Waiting..\n\n");
    fclose(fp);
    closesocket(fd);

    return 0;
}
