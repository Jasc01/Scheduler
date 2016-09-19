#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    int number[3];
    struct sockaddr_in serv_addr, cli_addr;
    int n, flag;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 9080;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
     
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    while(1) {
        
        listen(sockfd,5);

        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
        
        if (newsockfd < 0) error("ERROR on accept");
        
        bzero(buffer,256);
        //n = read(newsockfd,buffer,255);

        n = read(newsockfd, &number, sizeof(number));

        if (n < 0) error("ERROR reading from socket");
        
        //printf("Process received.\n\t PID: %i, ",number[0]);
        //printf("burst: %i, ",number[1]);
        //printf("priority: %i\n",number[2]);

        printf("Running process %i with a burst of %i and priority of %i\n", number[0], number[1], number[2]);



        if (n < 0) error("ERROR writing to socket");
        
        close(newsockfd);
        }
    close(sockfd);
    return 0; 
}