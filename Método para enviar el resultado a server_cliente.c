void sendResult(int pIndex) {
//  printf("PRUEBA: %s\n", values);
    int values[3] = {processes[pIndex].pid, processes[pIndex].burst, processes[pIndex].priority};
    int sockfd = 0, n = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
//  printf("Sending process...\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR creating socket.\n"); 
    }

    server = gethostbyname("localhost");
    if (server == NULL) {
        printf("ERROR, no such host.\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(9080);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        printf("ERROR connecting.\n");
    }

    //Sends result
    n = write(sockfd, &values, sizeof(values));
    if(n < 0){ printf("ERROR sending PID to socket.\n"); }

    close(sockfd);  //Closes socket

}

// Esto se pone en el Run, donde se printea actualmente el resultado.
sendResult(pIndex);

//Hay que incluir esto
#include <netdb.h>