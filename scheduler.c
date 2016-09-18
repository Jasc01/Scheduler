#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

	//Input del usuario.
	char dump[100];
	int modo, algoritmo, quantum;


	printf("\nTecnológico de Costa Rica\n");
	printf("\tPrincipios de Sistemas Operativos\n\n");
	printf("Profesora: \t Erika Marín Schumann\n");
	printf("Estudiantes: \t Carlos Girón Alas \t2014113159\n");
	printf("\t\t Julián J. Méndez O. \t2014121700\n");
	printf("\t\t Jasson Moya Álvarez \t2014082335\n");


	printf("\n\n\nSimulador de planificador de CPU:\n\n");
	while (1) {
		printf("Seleccione el modo del programa\n");
		printf("\t 1. Manual\n");
		printf("\t 2. Automático\n\n");

		printf("\t 3. Cancelar y salir.\n\n");

		if (!fgets(dump, sizeof dump, stdin)){
			printf("\nOcurrió un error.\n");
		}

		else{
  			char *chk;
    		
    		if ((int) strtol(dump, (char **)NULL, 10)) {
    			modo = (int) strtol(dump, (char **)NULL, 10);

    			if (modo > 0 && modo < 4) {
    				//Cases
    				switch(modo) {
    					case 1:
    						printf("\n Modo Manual seleccionado.\n\n");
    						break;
    					case 2: 
    						printf("\n Modo Automático seleccionado.\n\n");
    						break;
    					case 3:
    						printf("\n\nPrograma finalizado.\n\n");
    						exit(1);
    					default:
    						break;
    				}
    				printf("%i\n", modo);	
    				break;
    			}
    		}

			printf("\n\nIngrese una opción válida.\n\n"); 
		}
	}

	while (1) {
		printf("Seleccione el algoritmo de planificación\n");
		printf("\t 1. First In First Out (FIFO)\n");
		printf("\t 2. Shortest Job First (SJF)\n");
		printf("\t 3. Highest Priority First (HPF)\n");
		printf("\t 4. Round Robin\n\n");

		printf("\t 5. Cancelar y salir.\n\n");

		if (!fgets(dump, sizeof dump, stdin)){
			printf("\nOcurrió un error.\n");
		}

		else {
  			
			if ((int) strtol(dump, (char **)NULL, 10)) {
    			algoritmo = (int) strtol(dump, (char **)NULL, 10);

    			if (algoritmo > 0 && algoritmo < 6) {
    				//Cases
    				switch(algoritmo) {
    					case 1:
							printf("\n FIFO seleccionado.\n\n");
    						break;
    					case 2: 
    						printf("\n SJF seleccionado.\n\n");
    						break;
    					case 3:
    						printf("\n HPF seleccionado.\n\n");
    						break;
    					case 4: 
    						printf("\n Round Robin seleccionado.\n\n");
    						while (1) { 
    							printf("Seleccione el valor del quantum:\n");
    						
								if (!fgets(dump, sizeof dump, stdin)){
									printf("\nOcurrió un error.\n");
								}

								else {
									if ((int) strtol(dump, (char **)NULL, 10)) {
    									quantum = (int) strtol(dump, (char **)NULL, 10);

    									//Llamar al round robin
    									break;
    								}
	    						
    								else {
    									printf ("\n\nIngrese un valor válido para el quantum\n\n");
    								}
    							}
							}

    					case 5:
    						printf("\n\nPrograma finalizado.\n\n");
    						exit(1);
    					default:
    						break;
    				}
    				printf("%i\n", algoritmo);	
    				break;
    			}
    		}

    		printf("\n\nIngrese una opción válida.\n\n");
		}
	}

	return 0;
}