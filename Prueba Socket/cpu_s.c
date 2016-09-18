#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Process
{
	int pid;
	int burst;
	int priority;
	int isEmpty;
};

static struct Process* processes;
static int lowestProcessIdForRR = 0;

void printQueue() {
	printf("-------------------------------------------------------------------------------\n");
	for(int i = 0; i < 3; ++i)
	{
		printf("Index: %i || ID: %i || Burst: %i || Priority: %i\n", i, processes[i].pid, processes[i].burst, processes[i].priority);
	}
	printf("-------------------------------------------------------------------------------\n\n");
}

void makeSomeProcesses()
{
	processes = malloc(sizeof(struct Process)*3);
	for (int i = 0; i < 3; ++i)
	{
		//struct Process process;
		processes[i].pid = (int) (rand() % 100);
		processes[i].burst = (int) (rand() % 5) + 1;  //Tiene que ser de 20
		processes[i].priority = (int) (rand() % 5) + 1;
		processes[i].isEmpty = 0;
		//processes[i] = process;
	}
}

int getMinimunFiFo() 
{
	int minimum = processes[0].pid;
	int minumumLocation = 0;
	for(int i = 0; i < 3; ++i) {
		if(processes[i].isEmpty == 0) {
			minimum = processes[i].pid;
			minumumLocation = i;
			break;
		}
	}

    for (int i = 0; i < 3; ++i) 
    {
        if (processes[i].pid < minimum && processes[i].isEmpty == 0) 
        {
           minimum = processes[i].pid;
           minumumLocation = i;
        }
    }
    return minumumLocation;
}

int getMinimunSJF() 
{
	int minimum = processes[0].burst;
	int minumumLocation = 0;
	for(int i = 0; i < 3; ++i) {
		if(processes[i].isEmpty == 0) {
			minimum = processes[i].burst;
			minumumLocation = i;
			break;
		}
	}

    for (int i = 0; i < 3; ++i) 
    {
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
    return minumumLocation;
}

int getMinimunHPF() 
{
	int minimum = processes[0].priority;
	int minumumLocation = 0;
	for(int i = 0; i < 3; ++i) {
		if(processes[i].isEmpty == 0) {
			minimum = processes[i].priority;
			minumumLocation = i;
			break;
		}
	}

    for (int i = 0; i < 3; ++i) 
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
    return minumumLocation;
}

int getMinimunRR() 
{
	int minimum = processes[0].pid;
	int minumumLocation = 0;
	for(int i = 0; i < 3; ++i) {
		if(processes[i].isEmpty == 0 && processes[i].pid > lowestProcessIdForRR) {
			minimum = processes[i].pid;
			minumumLocation = i;
			break;
		}
	}

    for (int i = 0; i < 3; ++i) 
    {
        if (processes[i].pid < minimum && processes[i].isEmpty == 0 && processes[i].pid > lowestProcessIdForRR) 
        {
           minimum = processes[i].pid;
           minumumLocation = i;
        }
    }
    lowestProcessIdForRR = minimum;
    return minumumLocation;
}

int checkAllProcessesDead()
{
	int allDead = 1;
	for (int i = 0; i < 3; ++i)
	{
		if(processes[i].isEmpty == 0) {
			allDead = 0;
		}
	}

	return allDead;
}

int chageRound()
{
	int maxValue = processes[0].pid;
	int maxValueLocation = 0;

	for (int i = 0; i < 3; ++i)
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

void run(int pIndex) {
	printf("Running process %i with a burst of %i and priority of %i\n", processes[pIndex].pid, processes[pIndex].burst, processes[pIndex].priority);
	sleep(processes[pIndex].burst);
	processes[pIndex].isEmpty = 1;
}

void runRR(int pIndex, int pQuantum) {
	printf("Running process %i with a burst of %i and priority of %i while %i\n", processes[pIndex].pid, processes[pIndex].burst, processes[pIndex].priority, pQuantum);
	if(pQuantum < processes[pIndex].burst) 
	{
		sleep(pQuantum);
		processes[pIndex].burst -= pQuantum;
	}
	else
	{
		sleep(processes[pIndex].burst);
		processes[pIndex].burst = 0;
		processes[pIndex].isEmpty = 1;
	}
}

void fifo()
{
	for (int i = 0; i < 3; ++i)
	{
		int first = getMinimunFiFo();
		run(first);
	}

}

void sjf()
{
	for (int i = 0; i < 3; ++i)
	{
		int first = getMinimunSJF();
		run(first);
	}

}

void hpf()
{
	for (int i = 0; i < 3; ++i)
	{
		int first = getMinimunHPF();
		run(first);
	}
}

void rr(int quantum) 
{
	while(checkAllProcessesDead() == 0) 
	{
		int first = getMinimunRR();
		runRR(first, quantum);
		if(chageRound() == 1)
		{
			lowestProcessIdForRR = 0;
		}
	}
}

int main( int argc, char **argv)
{
	srand(time(NULL));

	makeSomeProcesses();
	printQueue();
	//printf("-------------------------FIFO------------------------------\n");
	//fifo();
	//printf("-------------------------SJF-------------------------------\n");
	//sjf();
	//printf("-------------------------HPF-------------------------------\n");
	//hpf();
	printf("-------------------------RR-------------------------------\n");
	rr(3);

    return 0;
}