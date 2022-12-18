// Christopher Cao
// ch282858
// COP4600, Fall 2022

// pre-processor directives
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// global variables & structs
typedef struct Process
{
	char name[32];
	int arrival;
	int burst;
	int wait;
	int turnaround;
} Process;

char algorithm[32];
int processCount;
int runFor;
int quantum;
Process* processes;

// start of first-come first-serve function
void fcfs()
{
	// variable declaration & initialization
	int i = 0, j = 0, k = 0;
	int temp = 0;
	int scheduledArray[processCount];
	int timer = 0;
	int finishedCount = 0;
	int arrival;
	int arrivalFinal;
	int burst;
	
	// initialize arrivalFinal
	for (i = 0; i < processCount; i++)
	{
		if (arrivalFinal < processes[i].arrival)
		{
			arrivalFinal = processes[i].arrival;
		}
	}
	
	// initialize arrivalArray
	int arrivalArray[arrivalFinal + 1];
	
	for (i = 0; i < (arrivalFinal + 1); i++)
	{
		arrivalArray[i] = -1;
	}
	
	for (i = 0; i < processCount; i++)
	{
		arrivalArray[processes[i].arrival] = i;
	}
	
	for (i = 0; i < arrivalFinal + 1; i++)
	{
		if (arrivalArray[i] > -1)
		{
			scheduledArray[temp] = arrivalArray[i];
			temp = temp + 1;
		}
	}
	
	// loop to run scheduling algorithm
	while (true)
	{
		for (i = 0; i < processCount; i++)
		{
			temp = scheduledArray[i];
			arrival = processes[temp].arrival;
			burst = processes[temp].burst;
			
			// detect if a process can arrive
			if (arrival <= timer)
			{
				// print arrival time if reached
				if (arrival == timer)
				{
					printf("Time %d: %s arrived\n", timer, processes[temp].name);
				}
				
				printf("Time %d: %s selected (burst %d)\n", timer, processes[temp].name, burst);
				
				// detect if the next processes to run have arrived
				for (j = 0; j < processes[temp].burst; j++)
				{
					for (k = i; k < processCount; k++)
					{
						if (processes[temp].arrival == j)
						{
							printf("Time %d: %s arrived\n", timer, processes[temp].name);
						}
					}
				}
				
				// set the wait and turnaround times of the current process
				processes[temp].wait = timer - processes[temp].arrival;
				processes[temp].turnaround = processes[temp].wait + processes[temp].burst;
				
				
				
				timer = timer + processes[temp].burst;
				
				// break if runtime has been exceeded
				if (timer > runFor)
				{
					break;
				}
				
				processes[temp].burst = 0;
				printf("Time %d: %s finished\n", timer, processes[temp].name);
				finishedCount = finishedCount + 1;
				
				// break if runtime has been reached
				if (timer >= runFor)
				{
					break;
				}
				
				else if (finishedCount == processCount)
				{
					printf("Time %d: IDLE\n", timer++);
				}
			}
			
			// default output if no process has arrived
			else
			{
				printf("Time %d: IDLE\n", timer);
				timer = arrival;
				i = i - 1;
			}
		}
		
		// print finish time if all processes are done
		if (finishedCount == processCount)
		{
			printf("Finished at time %d\n", timer);
			break;
		}
		
		// print finish time if runtime has been reached
		else if (timer >= runFor)
		{
			printf("Finished at time %d\n", runFor);
			break;
		}
		
		// continue the algorithm
		else
		{
			continue;
		}
	}
	
	printf("\n");
	
	// print each process's wait & turnaround time
	for (i = 0; i < processCount; i++)
	{
		printf("%s wait %d turnaround %d\n", processes[i].name, processes[i].wait, processes[i].turnaround);
	}
}

// start of preemptive shortest job first function
void sjf()
{
	// variable declaration & initialization
	int i = 0, j = 0, k = 0;
	int arrivalCount = 0;
	int waitingCount = 0;
	int finishedCount = 0;
	Process temp, running;
	running.burst = -1;
	Process *order = malloc(sizeof(Process) * processCount);
	Process *waiting = malloc(sizeof(Process) * processCount);
	Process *finished = malloc(sizeof(Process) * processCount);
	
	// store processes array
	for (i = 0; i < processCount; i++)
	{
		order[i] = processes[i];
	}
	
	// order processes as they arrive
	for (i = 0; i < (processCount - 1); i++)
	{
		for (j = 0; j < (processCount - 1); j++)
		{
			if (((order[j].arrival > order[j + 1].arrival)) || ((order[j].arrival == order[j + 1].arrival) && (order[j].burst > order[j + 1].burst)))
			{
				temp = order[j];
				order[j] = order[j + 1];
				order[j + 1] = temp;
			}
		}
	}
	
	// runtime check
	for (i = 0; i < runFor; i++)
	{
		// update waitlist
		while (order[arrivalCount].arrival == i)
		{
			printf("Time %d: %s arrived\n", i, order[arrivalCount].name);
			
			waiting[waitingCount] = order[arrivalCount];
			waiting[waitingCount].wait = 0;
			arrivalCount = arrivalCount + 1;
			waitingCount = waitingCount + 1;
			
			// detect if there are multiple processes waiting
			if (waitingCount > 1)
			{
				// compare the processes and their burst times
				for (j = 0; j < (waitingCount - 1); j++)
				{
					for (k = 0; k < (waitingCount - 1); k++)
					{
						// swap the order of the processes if the one in front has a longer burst time
						if (waiting[k].burst > waiting[k + 1].burst)
						{
							temp = waiting[k];
							waiting[k] = waiting[k + 1];
							waiting[k + 1] = temp;
						}
					}
				}
			}
		}
		
		// detect if something is running
		if (running.burst != -1)
		{
			running.burst = running.burst - 1;
			
			// detect if a process has finished and stop it from running if it has
			if (running.burst == 0)
			{
				printf("Time %d: %s finished\n", i, running.name);
				running.turnaround = i - running.arrival;
				finished[finishedCount] = running;
				finishedCount = finishedCount + 1;
				running.burst = -1;
			}
		}
		
		// detect if nothing is running
		if (running.burst == -1)
		{
			// detect if the waitlist has any processes and run the first one on the list
			if (waitingCount > 0)
			{
				running = waiting[0];
				printf("Time %d: %s selected (burst %d)\n", i, running.name, running.burst);
				
				// detect if the waitlist has multiple processes
				if (waitingCount > 1)
				{
					// update waitlist
					for (j = 0; j < (waitingCount - 1); j++)
					{
						waiting[j] = waiting[j + 1];
					}
					
					waiting[j].burst = -1;
				}
				
				// remove the process from the waitlist
				waitingCount = waitingCount - 1;
			}
			
			// print idle time otherwise
			else
			{
				printf("Time %d: IDLE\n",i);
			}
		}
		
		// detect if the running process has a longer burst time than a process on the waitlist
		else if (waiting[0].burst < running.burst)
		{
			// swap the current running process and the shortest process on the waitlist
			temp = running;
			running = waiting[0];
			waiting[0] = temp;
			printf("Time %d: %s selected (burst %d)\n", i, running.name, running.burst);
			
			// update the waitlist to account for the swap
			for (j = 0; j < waitingCount - 1; j++)
			{
				for (k = 0; k < (waitingCount - 1); k++)
				{
					if (waiting[k].burst > waiting[k + 1].burst)
					{
						temp = waiting[k];
						waiting[k] = waiting[k + 1];
						waiting[k + 1] = temp;
					}
				}
			}
		}
		
		// update wait times for the remaining processes
		for (j = 0; j < waitingCount; j++)
		{
			waiting[j].wait = waiting[j].wait + 1;
		}
	}
	
	// print finish time
	printf("Finished at time %d\n\n",runFor);
	
	// reorder the wait and turnaround time arrays
	for (i = 0; i < finishedCount; i++)
	{
		for (j = 0; j < processCount; j++)
		{
			if (strcmp(finished[i].name,processes[j].name) == 0)
			{
				processes[j].wait = finished[i].wait;
				processes[j].turnaround = finished[i].turnaround;	
			}
			
		}
	}
	
	// print final results
	for (i = 0; i < processCount; i++)
	{
		printf("%s wait %d turnaround %d\n", processes[i].name, processes[i].wait, processes[i].turnaround);
	}
	
	// free memory
	free(order);
	free(waiting);
	free(finished);
}

// start of round-robin function
void rr()
{
	// variable declaration & initialization
	int i = 0, j = 0, k = 0;
	int temp = 0;
	int arrival;
	int burst;
	int finished = 0;
	int timer = 0;
	int start = 0;
	
	
	// loop to run round-robin algorithm
	while (true)
	{
		for (i = 0; i < processCount; i++)
		{
			arrival = processes[i].arrival;
			burst = processes[i].burst;
		
			if ((arrival <= timer) && (burst > 0))
			{
				start = 1;
				
				// detect if a process has arrived
				if (arrival == timer)
				{
					printf("Time %d: %s arrived\n", timer, processes[i].name);
					processes[i].wait = burst;
				}
				
				// compare burst & quantum times to use the shorter one
				if (burst < quantum)
				{
					temp = burst;
				}
				
				else if (burst >= quantum)
				{
					temp = quantum;
				}

				printf("Time %d: %s selected (burst %d)\n", timer, processes[i].name, burst);
			
				// update burst time
				processes[i].burst = processes[i].burst - temp;
				
				// detect if another process has arrived
				for (j = 0; j < temp; j++)
				{
					for (k = i; k < processCount; k++)
					{
						if ((processes[k].arrival == j) && (processes[k].name != processes[i].name))
						{
							printf("Time %d: %s arrived\n", timer, processes[i].name);
						}
					}
				}
				
				// update timer
				timer = timer + temp;
				
				// Check if the current process has finished.
				if (processes[i].burst == 0)
				{
					printf("Time %d: %s finished\n", timer, processes[i].name);
					finished = finished + 1;
					
					processes[i].turnaround = timer - processes[i].arrival;	
					processes[i].wait = processes[i].turnaround - processes[i].wait;
				}
				
				// break from algorithm if the runtime has been exceeded or all processes are done
				if ((timer >= runFor) || (finished == processCount))
				{
					break;
				}
			}
		}
		
		// detect if the algorithm is running
		if (!start)
		{
			printf("Time %d: IDLE\n", timer);
			timer = timer + 1;
		}
	
		// print finish time if all processes are done
		if (finished == processCount)
		{		
			if (timer < runFor)
			{
				for (i = timer; i < runFor; i++)
				{
					printf("Time %d: IDLE\n", i);
					timer = timer + 1;
				}
			}
			
			printf("Finished at time %d\n\n", timer);
			break;
		}
		
		// print finish time if runtime has been reached
		else if (timer >= runFor)
		{
			printf("Finished at time %d\n\n", timer);
			break;
		}
	}
	
	// print final results
	for (i = 0; i < processCount; i++)
	{
		printf("%s wait %d turnaround %d\n", processes[i].name, processes[i].wait, processes[i].turnaround);
	}
}

// start of main function
int main()
{
	// variable declaration & initialization
	FILE* inputFile;
	char string[100];
	int i;
	inputFile = fopen("processes.in", "r");
	
	// return 0 if the inputFile is missing
	if (inputFile == NULL)
	{
		printf("Invalid input file.\n");
		return 0;
	}
	
	// read the inputFile
	while (fscanf(inputFile, "%s", string) != EOF)
	{
		if (strcmp(string, "processcount") == 0)
		{
			fscanf(inputFile, "%d", &processCount);
			fscanf(inputFile, "%*[^\n]%*c");
		}
		
		else if (strcmp(string, "runfor") == 0)
		{
			fscanf(inputFile, "%d", &runFor);
			fscanf(inputFile, "%*[^\n]%*c");
		}
		
		else if (strcmp(string, "use") == 0)
		{
			fscanf(inputFile, "%s", algorithm);
			fscanf(inputFile, "%*[^\n]%*c");
		}
		
		else if (strcmp(string, "quantum") == 0)
		{
			fscanf(inputFile, "%d", &quantum);
			fscanf(inputFile, "%*[^\n]%*c");
		}
		
		else if (strcmp(string, "process") == 0)
		{
			i = 0;
			processes = malloc(sizeof(Process) * processCount);
			fscanf(inputFile, "%*s %s %*s %d %*s %d%*[^\n]%*c", processes[i].name, &processes[i].arrival, &processes[i].burst);
			processes[i].wait = processes[i].burst;
			
			for (i = 1; i < processCount; i++)
			{
				fscanf(inputFile, "%*s %*s %s %*s %d %*s %d%*[^\n]%*c", processes[i].name, &processes[i].arrival, &processes[i].burst);
				fscanf(inputFile, "%*[^\n]%*c");
				processes[i].wait = processes[i].burst;
			}
		}
		
		else if (strcmp(string, "end") == 0)
		{
			break;
		}
		
		else
		{
			fscanf(inputFile, "%*[^\n]%*c");
		}
	}
	
	// call algorithms
	if (strcmp(algorithm, "fcfs") == 0)
	{
		printf("%d processes\n", processCount);
		printf("Using First Come First Served\n\n");
		fcfs();
	}
	
	else if (strcmp(algorithm, "sjf") == 0)
	{
		printf("%d processes\n", processCount);
		printf("Using Shortest Job First (Pre)\n\n");
		sjf();
	}
	
	else if (strcmp(algorithm, "rr") == 0)
	{
		printf("%d processes\n", processCount);
		printf("Using Round-Robin\n");
		printf("Quantum %d\n\n", quantum);
		rr();
	}
	
	return 0;
}
