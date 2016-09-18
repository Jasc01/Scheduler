#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

void* sleep1SEC(void* data){
	int numbers = 0;
	while(numbers < 10){
		printf("abc: %i\n",numbers);
		numbers++;
		//sleep(1);
	}
}

void createRandomProcesses(){

}

void* sendInfo(void* data){
	int *values = (int *)data;
	int values3[3] = {values[0], values[1], values[2]};
	int sockfd = 0, n = 0;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	printf("test\n");
	int counter = 0;
	while(counter < 1){
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0) {
	    	printf("ERROR creating socket\n"); 
	    }
	    server = gethostbyname("localhost");
	    if (server == NULL) {
	        printf("ERROR, no such hostv\n");
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


		//Sends process' pid
		n = write(sockfd, &values3, sizeof(values3));
		if(n < 0){ printf("ERROR sending pId to socket\n"); }

		//Sends process' burst time
		//n = write(sockfd, &values[1], sizeof(values[1]));
		//if(n < 0){ printf("ERROR sending burst to socket.\n"); }

		//Sends process' priority value
		//n = write(sockfd, &values[2], sizeof(values[2]));
		//if(n < 0) { printf("ERROR sending priority to socket.\n"); }

		close(sockfd);	//Closes socket
		counter++;
	}
}

int main(){
	pthread_t threads[1000];
    int pid = 0;
    int burst, priority;

    FILE* file;
    if (file = fopen("processes.txt", "r")) {
        while(fscanf(file, "%i, %i", &burst, &priority) == 2){
        	int values[3] = {pid, burst, priority};
        	        	
        	pthread_create(&threads[pid], NULL, sendInfo, values);
        	pthread_join(threads[pid],NULL);		//Ver donde se deja para que funcione mejor
        	pid++; 			//Increases process id
        }
        fclose(file);
    }else{ 
    	createRandomProcesses();
    }
}