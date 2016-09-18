/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>


#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF){
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void *server (void* pf){
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    int number[3];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 9090;
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
        
        printf("Process received.\n\t PID: %i, ",number[0]);
        printf("burst: %i, ",number[1]);
        printf("priority: %i\n",number[2]);

        if (n < 0) error("ERROR writing to socket");
        
        close(newsockfd);
        }
    close(sockfd);
}

int main(int argc, char *argv[]) { 
    //Input del usuario.
    char dump[100], p;
    int algoritmo, quantum;
    pthread_t server_thread;
   
    
    printf("\nTecnológico de Costa Rica\n");
    printf("\tOperative Systems' Principles\n\n");
    printf("Teacher: \t Erika Marín Schumann\n");
    printf("Students: \t Carlos Girón Alas \t2014113159\n");
    printf("\t\t Julián J. Méndez O. \t2014121700\n");
    printf("\t\t Jasson Moya Álvarez \t2014082335\n");

    printf("\n\n\nCPU Scheduler Simulator 2000 SERVER\n\n");

    while (1) {
        printf("Choose scheduling algorithm\n");
        printf("\t 1. First In First Out (FIFO)\n");
        printf("\t 2. Shortest Job First (SJF)\n");
        printf("\t 3. Highest Priority First (HPF)\n");
        printf("\t 4. Round Robin\n\n");
        
        printf("\t 5. Cancel & exit.\n\n");

        if (!fgets(dump, sizeof dump, stdin)){
            printf("\nERROR retrieving input.\n");
        }

        else {
            if ((int) strtol(dump, (char **)NULL, 10)) {
                algoritmo = (int) strtol(dump, (char **)NULL, 10);

                if (algoritmo > 0 && algoritmo < 6) {
                    //Cases
                    switch(algoritmo) {
                        case 1:
                            printf("\n FIFO selected.\n\n");
                            break;
                        case 2: 
                            printf("\n SJF selected.\n\n");
                            break;
                        case 3:
                            printf("\n HPF selected.\n\n");
                            break;
                        case 4: 
                            printf("\n Round Robin selected.\n\n");
                            while (1) { 
                                printf("Enter quantum value:\n");
                            
                                if (!fgets(dump, sizeof dump, stdin)){
                                    printf("\nERROR retrieving input.\n");
                                }

                                else {
                                    if ((int) strtol(dump, (char **)NULL, 10)) {
                                        quantum = (int) strtol(dump, (char **)NULL, 10);

                                        //Llamar al round robin
                                        break;
                                    }
                                
                                    else {
                                        printf ("\n\nEnter a valid quantum time.\n\n");
                                    }
                                }
                            }

                        case 5:
                            printf("\n\nProgram finished.\n\n");
                            exit(1);
                        default:
                            break;
                    }
                    printf("%i\n", algoritmo);  
                    break;
                }
            }
            printf("\n\nEnter a valid option.\n\n");
        }
    }
    
    pthread_create(&server_thread, NULL, server, NULL);    
    while(1) {
        if (kbhit()) {
            p = getchar();
            if (p == 'd' || p == 'D') { //for Display
                printf("\n\t This is the current queue:\n");
                printf("\t\t Insert functional queue instead of this print.\n");
            }
            else if (p == 27) { //ESC = 27
                pthread_cancel(server_thread);
                printf("\n\t These are the resulting stats:\n");
                printf("\t\t Insert stats instead of this print.\n");

                printf("Program finished.");
                break;
            }
            else {
                printf("You pressed '%c'! That does nothing\n", p);
            }
        } 
    } 
    return 0; 
}

