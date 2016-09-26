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
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <netdb.h>

struct Process{
    int pid;
    int burst;
    int priority;

    int burstleft;
    int isEmpty;
    
    clock_t start;
    double tat;
    double wt;
};

static struct Process* processes;
static int lowestProcessIdForRR = 0;
static int algoritmo, quantum;
static int memorysize = 5;
static double *results;
static FILE *f;
static int globpid, globburst, globpiority;
static double globtat, globwt;
static clock_t lazyClock;
static double lazySUM;


void error(const char *msg) {
    perror(msg);
    exit(1);
}

void printQueue() {
    printf("-------------------------------------------------------------------------------\n");
    for(int i = 0; i < memorysize; ++i)
    {
        if(processes[i].isEmpty == 0)
            printf("Index: %i || ID: %i || Burst: %i || Priority: %i || Current TAT: %f \n", 
                i, processes[i].pid, processes[i].burst, processes[i].priority, ((double)clock() - processes[i].start)/CLOCKS_PER_SEC );
    }
    printf("-------------------------------------------------------------------------------\n\n");
}

void bootstrap(){
    f = fopen("results.txt", "w");
    if (f == NULL) {
        error("ERROR opening results file!\n");
    }

    processes = malloc(sizeof(struct Process)*memorysize);
    for (int i = 0; i < memorysize; ++i) {
        processes[i].pid = -1;
        processes[i].burst = -1;
        processes[i].priority = -1;

        processes[i].burstleft = -1;
        processes[i].isEmpty = 1;

        processes[i].tat = -1;
        processes[i].wt = -1;
    }
    lazyClock = clock();
}

int checkAllProcessesDead(){
    int allDead = 1;
    for (int i = 0; i < memorysize; ++i)
    {
        if(processes[i].isEmpty == 0) {
            allDead = 0;
        }
    }

    return allDead;
}

int changeRound(){
    int maxValue = processes[0].pid;
    int maxValueLocation = 0;

    for (int i = 0; i < memorysize; ++i)
    {
        if (processes[i].pid > maxValue)
        {
            maxValue = processes[i].pid;
            maxValueLocation = i;
        }
    }

    if(lowestProcessIdForRR == maxValue)
    {
        return 1;
    }

    return 0;
}

int getMinimunFiFo() {
    int minimum = processes[0].pid;
    int minumumLocation = 0;
    for(int i = 0; i < memorysize; ++i) {
        if(processes[i].isEmpty == 0) {
            minimum = processes[i].pid;
            minumumLocation = i;
            break;
        }
    }

    for (int i = 0; i < memorysize; ++i) 
    {
        if (processes[i].pid < minimum && processes[i].isEmpty == 0) 
        {
           minimum = processes[i].pid;
           minumumLocation = i;
        }
    }
    if(checkAllProcessesDead() == 0) {
        return minumumLocation;
    }
    return -1;
}

int getMinimunSJF() {
    int minimum = processes[0].burst;
    int minumumLocation = 0;
    for(int i = 0; i < memorysize; ++i) {
        if(processes[i].isEmpty == 0) {
            minimum = processes[i].burst;
            minumumLocation = i;
            break;
        }
    }

    for (int i = 0; i < memorysize; ++i) {
        if (processes[i].burst < minimum && processes[i].isEmpty == 0) 
        {
           minimum = processes[i].burst;
           minumumLocation = i;
        }
        else if(processes[i].burst == minimum && processes[i].isEmpty == 0) 
        {
            minimum = processes[i].pid < processes[minumumLocation].pid ? minimum = processes[i].burst : processes[minumumLocation].burst;
            minumumLocation = processes[i].pid < processes[minumumLocation].pid ? minimum = i : minumumLocation;
        }
    }
    if(checkAllProcessesDead() == 0) {
        return minumumLocation;
    }
    return -1;
}

int getMinimunHPF() {
    int minimum = processes[0].priority;
    int minumumLocation = 0;
    for(int i = 0; i < memorysize; ++i) {
        if(processes[i].isEmpty == 0) {
            minimum = processes[i].priority;
            minumumLocation = i;
            break;
        }
    }

    for (int i = 0; i < memorysize; ++i) 
    {
        if (processes[i].priority < minimum && processes[i].isEmpty == 0) 
        {
           minimum = processes[i].priority;
           minumumLocation = i;
        }
        else if(processes[i].priority == minimum && processes[i].isEmpty == 0) 
        {
            minimum = processes[i].pid < processes[minumumLocation].pid ? minimum = processes[i].priority : processes[minumumLocation].priority;
            minumumLocation = processes[i].pid < processes[minumumLocation].pid ? minimum = i : minumumLocation;
        }
    }
    if(checkAllProcessesDead() == 0) {
        return minumumLocation;
    }
    return -1;
}

int getMinimunRR() {
    int minimum = processes[0].pid;
    int minumumLocation = 0;
    for(int i = 0; i < memorysize; ++i) {
        if(processes[i].isEmpty == 0 && processes[i].pid > lowestProcessIdForRR) {
            minimum = processes[i].pid;
            minumumLocation = i;
            break;
        }
    }

    for (int i = 0; i < memorysize; ++i) 
    {
        if (processes[i].pid < minimum && processes[i].isEmpty == 0 && processes[i].pid > lowestProcessIdForRR) 
        {
           minimum = processes[i].pid;
           minumumLocation = i;
        }
    }
    lowestProcessIdForRR = minimum;
    if(checkAllProcessesDead() == 0) {
        return minumumLocation;
    }
    return -1;
}

void sendResult(int pIndex) {
    int values[3] = {processes[pIndex].pid, processes[pIndex].burst, processes[pIndex].priority};
    int sockfd = 0, n = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;

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

void run(int pIndex) {
    lazySUM += ((double)clock() - lazyClock)/CLOCKS_PER_SEC;
    printf("Running process %i with a burst of %i and priority of %i\n", processes[pIndex].pid, processes[pIndex].burst, processes[pIndex].priority);
    sleep(processes[pIndex].burst);
    sendResult(pIndex);
    
    processes[pIndex].tat = ((double)clock() - processes[pIndex].start)/CLOCKS_PER_SEC;
    processes[pIndex].wt = processes[pIndex].tat - processes[pIndex].burst;

    //Escribir en .txt el proceso terminado
    fprintf(f, "%i,%i,%i,%f,%f\n", processes[pIndex].pid, processes[pIndex].burst, processes[pIndex].priority,processes[pIndex].tat,processes[pIndex].wt);
    processes[pIndex].isEmpty = 1;
    lazyClock = clock();
}

void runRR(int pIndex, int pQuantum) {
    lazySUM += ((double)clock() - lazyClock)/CLOCKS_PER_SEC;
    printf("Running process %i with a remaining burst of %i (originally %i) and priority of %i for %i\n", processes[pIndex].pid, processes[pIndex].burstleft, processes[pIndex].burst, processes[pIndex].priority, pQuantum);
    if(pQuantum < processes[pIndex].burstleft) 
    {
        sleep(pQuantum);
        processes[pIndex].burstleft -= pQuantum;
    }
    else
    {
        sleep(processes[pIndex].burstleft);
        sendResult(pIndex);
        processes[pIndex].burstleft = 0;

        processes[pIndex].tat = ((double)clock() - processes[pIndex].start)/CLOCKS_PER_SEC;
        processes[pIndex].wt = processes[pIndex].tat - processes[pIndex].burst;

        //Escribir en .txt el proceso terminado
        fprintf(f, "%i,%i,%i,%f,%f\n", processes[pIndex].pid, processes[pIndex].burst, processes[pIndex].priority,processes[pIndex].tat,processes[pIndex].wt);
        processes[pIndex].isEmpty = 1;
    }
    lazyClock = clock();
}

void fifo(){
    int n = 0;
    for (int i = 0; i < memorysize; ++i)
    {
        int first = getMinimunFiFo();
        if(first != -1) {
            n++;
            run(first);
        }
    }
    if(n == 0) {
        printf("Nothing to run... Yet\n");
    }
}

void sjf(){
    int n = 0;
    for (int i = 0; i < memorysize; ++i)
    {
        int first = getMinimunSJF();
        if(first != -1) {
            n++;
            run(first);
        }
    }
    if(n == 0) {
        printf("Nothing to run... Yet\n");
    }
}

void hpf(){
    int n = 0;
    for (int i = 0; i < memorysize; ++i)
    {
        int first = getMinimunHPF();
        if(first != -1) {
            n++;
            run(first);
        }
    }
    if(n == 0) {
        printf("Nothing to run... Yet\n");
    }
}

void rr(){
    while(checkAllProcessesDead() == 0) 
    {
        int first = getMinimunRR();
        runRR(first, quantum);
        if(changeRound() == 1)
        {
            lowestProcessIdForRR = 0;
        }
    }
    printf("Nothing to run... Yet\n");
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

void *jobsch (void* pf){
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
        
        //printf("Process received.\n\t PID: %i, ",number[0]);
        //printf("burst: %i, ",number[1]);
        //printf("priority: %i\n",number[2]);

        flag = 0;
        for (int i = 0; i < memorysize; ++i){
            if (processes[i].isEmpty == 1) {
                processes[i].pid = number[0];
                processes[i].burst = number[1];
                processes[i].priority = number[2];

                processes[i].burstleft = number[1];
                processes[i].isEmpty = 0;
                processes[i].start = clock();
                flag = 1;
                break;
            }
        }

        if (!flag) {
            printf("Ready queue full. Discarded process PID: %i, burst: %i, priority: %i. \n",number[0],number[1],number[2]);
        }



        if (n < 0) error("ERROR writing to socket");
        
        close(newsockfd);
        }
    close(sockfd);
}

void *cpusch (void* pf){

    switch (algoritmo) {
        case 1: 
            while(1) {
                fifo();
                sleep(1);
            }
            break;
        case 2: 
            while(1) {
                sjf();
                sleep(1);
            }
            break;
        case 3: 
            while(1) {
                hpf();
                sleep(1);
            }
            break;
        case 4:
            while(1) {
                rr();
                sleep(1);
            }
            break;
        default: 
            error("KERNEL PANIC LOLJK");
            break;
    }
}

void readResults(){
    FILE* file;
    int processesFinished = 0;
    double avgTAT = 0;
    double avgWT = 0;
    double lazyBastard = 0;
    if (file = fopen("results.txt", "r")) {
        printf("Results:\n");
        printf("PID\tBurst\tPriority\tTAT\tWT\n");
        while(fscanf(file, "%i,%i,%i,%lf,%lf",&globpid,&globburst,&globpiority,&globtat,&globwt) == 5){
            processesFinished++;
            avgTAT += globtat;
            avgWT += globwt;
            printf("%i\t%i\t%i\t\t%lf\t%lf\n", globpid, globburst, globpiority, globtat, globwt);
        }
        fclose(file);
        printf("Processes ran: %i\n", processesFinished);
        printf("Avg TAT: %lf\nAvg WT: %lf\n", (avgTAT/processesFinished), (avgWT/processesFinished));
        printf("Idle CPU: %lf\n", lazySUM);
    }else{ 
        printf("ERROR opening results file.\n");
    }
}
int main(int argc, char *argv[]) { 
    //Input del usuario.
    char dump[100], p;
    pthread_t server_thread;
    pthread_t cpu_sch_thread;
    
    bootstrap();
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
                                        break;
                                    }
                                
                                    else {
                                        printf ("\n\nEnter a valid quantum time.\n\n");
                                    }
                                }
                            }
                            break;

                        case 5:
                            printf("\n\nProgram finished.\n\n");
                            exit(1);
                        default:
                            break;
                    }
                    //printf("%i\n", algoritmo);  
                    break;
                }
            }
            printf("\n\nEnter a valid option.\n\n");
        }
    }
    
    pthread_create(&server_thread, NULL, jobsch, NULL);    
    pthread_create(&cpu_sch_thread, NULL, cpusch, NULL);    
        

    while(1) {
        if (kbhit()) {
            p = getchar();
            if (p == 'd' || p == 'D') { //for Display
                printf("\n\t This is the current queue:\n");
                printQueue();
            }
            else if (p == 27) { //ESC = 27
                pthread_cancel(server_thread);
                pthread_cancel(cpu_sch_thread);
                
                printf("\n\t These are the resulting stats:\n");
                printf("\t\t Insert stats instead of this print.\n");

                printf("Program finished.");
                fclose(f);
                break;
            }
            else {
                printf("You pressed '%c'! That does nothing\n", p);
            }
        } 
    } 
    readResults();

    return 0; 
}

