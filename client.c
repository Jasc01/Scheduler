#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <time.h>

static int pid = 0;
static int burst, priority, wait;
static pthread_t client, simulator;
static pthread_t threads[100000];
static time_t t;

void *sendInfo(void* data){
    int *values = (int *)data;
    int process[3] = {values[0], values[1], values[2]};
    int sockfd = 0, n = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    printf("Sending process: %i, with burst: %i, and priority: %i\n", values[0],values[1],values[2]);
    int counter = 0;

    while(counter < 1){
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
        serv_addr.sin_port = htons(9090);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
            printf("ERROR connecting.\n");
        }

        //Sends process
        n = write(sockfd, &process, sizeof(process));
        if(n < 0){ printf("ERROR sending PID to socket.\n"); }

        close(sockfd);  //Closes socket
        counter++;
    }
}

void *createRandomProcesses(void* data){
    srand((unsigned) time(&t));

    while (1) {
        burst = rand() % 20;
        priority = (rand() % 5) + 1;
        
        int values[3] = {pid, burst, priority};
                        
        pthread_create(&threads[pid], NULL, sendInfo, values);
        pthread_join(threads[pid],NULL);        //Ver donde se deja para que funcione mejor
        
        pid++;
        wait = rand() % 5; 
        sleep(wait);
    } 
}

void *readProcesses(void* data){
    srand((unsigned) time(&t));
    FILE* file;
    if (file = fopen("processes.txt", "r")) {
        while(fscanf(file, "%i, %i", &burst, &priority) == 2){
            int values[3] = {pid, burst, priority};
                        
            pthread_create(&threads[pid], NULL, sendInfo, values);
            pthread_join(threads[pid],NULL);        //Ver donde se deja para que funcione mejor
            pid++;          //Increases process id
            
            wait = rand() % 5; 
            sleep(wait);
        }
        fclose(file);
    }else{ 
        printf("Error, no se pudo leer el archivo con procesos.\n");
    }
}

int main() {

	//Input del usuario.
	char dump[100];
	int modo;

	printf("\nTecnológico de Costa Rica\n");
	printf("\tOperative Systems' Principles\n\n");
	printf("Teacher: \t Erika Marín Schumann\n");
	printf("Students: \t Carlos M. Girón Alas \t2014113159\n");
	printf("\t\t Julián J. Méndez O. \t2014121700\n");
	printf("\t\t Jasson J. Moya Álvarez \t2014082335\n");

	printf("\n\n\nCPU Scheduler Simulator 2000 CLIENT\n\n");
    
	while (1) {
		printf("Choose mode:\n");
		printf("\t 1. Manual\n");
		printf("\t 2. Automatic\n\n");

		printf("\t 3. Cancel & exit.\n\n");

		if (!fgets(dump, sizeof dump, stdin)){
			printf("\nERROR retrieving input.\n");
		}

		else{
  			char *chk;
    		
    		if ((int) strtol(dump, (char **)NULL, 10)) {
    			modo = (int) strtol(dump, (char **)NULL, 10);

    			if (modo > 0 && modo < 4) {
                 
    				//Cases
    				switch(modo) {
    					case 1:
    						printf("\n Manual mode selected.\n\n");
                            pthread_create(&client, NULL, readProcesses, NULL);
    						pthread_join(client, NULL);
                            
                            break;
    					case 2: 
    						printf("\n Automatic mode selected.\n\n");
                            pthread_create(&client, NULL, createRandomProcesses, NULL);
    						pthread_join(client, NULL);
                            
                            break;
    					case 3:
    						printf("\n\nProgram finished.\n\n");
    						exit(1);
    					default:
    						break;
    				}
    				printf("%i\n", modo);	
    				break;
    			}
    		}

			printf("\n\nEnter a valid option.\n\n"); 
		}
	}

	return 0;
}