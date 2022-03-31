/* 
* CPU Scheduling Method: Sorted Dynamic Round Robin with Shortest Job First
* Sorted based on earlier arrival time then based on shortest burst time
* Dynamic time quantum based on shortest remaining burst time of each processes that have not exited the CPU fully
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<ctype.h>

int main(int argc, char *argv[]) {
	FILE *file;
	char *filename, *token;
    char ch, buffer[10], s[10] = " ";

    int lines = 1, numProcesses = 1;
	int arrivalTime[50];
	int burstTime[50];

	/* Check if a filename has been specified in the command */
	if (argc < 2) {		
        printf("Missing Filename\n");
        return(1);
	}
	else {
        filename = argv[1];
	}
	/* Read the file and calculate number of input processes while storing their arrival and burst time */
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Missing file\n");
        return 0;
    }
	/* Count number of newline("\n") and deducting it by 1 to get number of processes */
	int current_lines = 1, current_numProcesses = 1;
    for (ch = fgetc(file); ch != EOF; ch = getc(file)){
        if (ch == '\n' || ch == '\r') {					
            current_lines += 1;
			current_numProcesses += 1;				
			lines = current_lines - 1;
			numProcesses = current_numProcesses - 1;
        }
    }

	int temp_lines;
    lines = temp_lines;
	temp_lines = 0;
    fclose(file);

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Missing file\n");
        return 0;
    }
	/* look for first occurence of new line and carriage return within the buffer */
    while(fgets(buffer, 10, file)) {
        buffer[strcspn(buffer, "\n")] = 0;			
		buffer[strcspn(buffer, "\r")] = 0;

		/* tokenize word base on a space splitter */
        token = strtok(buffer, s);					
        arrivalTime[temp_lines] = atoi(token);

        token = strtok(NULL, s);					
        burstTime[temp_lines] = atoi(token);

        temp_lines++;
    }

    fclose(file);
	
	int i, j, k, m, waitTime[20], turnAroundTime[20], dynamicSlicer[20], process[20], backupBurstTime[20];
	int  maxBurstTime = 0, arranged = 0, count = 0, responseTime[20], completionTime[20], timeSlice[20], leastBurstTime = 0;
	float totalWaitTime = 0, totalTurnAroundTime = 0, totalResponseTime = 0, totalCompletionTime = 0, totalTimeSlice = 0;
	float maxTurnAroundTime = 0;
	float maxWaitingTime = 0;

	/* store process number and backupBurstTime is used to store the original burst time of each processes */
	for (i = 0; i < numProcesses; i++) {
		process[i] = i + 1;                        		
		backupBurstTime[i] = burstTime[i];  			
	}  
	
	for (j = 0; j < numProcesses; j++) {

		for (k = j + 1; k < numProcesses; k++) {
			/* Sort process base on arrival time earliest to latest */
			if (arrivalTime[j] > arrivalTime[k]) {
				arranged = burstTime[j];
				backupBurstTime[j] = burstTime[j] = burstTime[k];
				backupBurstTime[k] = burstTime[k] = arranged;

				arranged = process[j];
				process[j] = process[k];
				process[k] = arranged;

				arranged = arrivalTime[j];
				arrivalTime[j] = arrivalTime[k];
				arrivalTime[k] = arranged;
			}
			/* If arrival time is the same, sort by SJF */
			else if (arrivalTime[j] == arrivalTime[k] && burstTime[j] > burstTime[k]) {			
					arranged = burstTime[j];
					backupBurstTime[j] = burstTime[j] = burstTime[k];
					backupBurstTime[k] = burstTime[k] = arranged;

					arranged = process[j];
					process[j] = process[k];
					process[k] = arranged;

					arranged = arrivalTime[j];
					arrivalTime[j] = arrivalTime[k];
					arrivalTime[k] = arranged;
			}
        }	
	}
	arranged = 0;

	/* Find the largest burst time out of all the processes */
	maxBurstTime = burstTime[0];  
	for (i = 1; i < numProcesses; i++) {        
		if (maxBurstTime < burstTime[i]) {
			 maxBurstTime = burstTime[i];
        } 
    }

	/* Find the smallest burst time out of all the processes to be used as the first time quantum  */
	leastBurstTime = burstTime[0]; 
    for (i = 1; i < numProcesses; i++) {        
		if (leastBurstTime > burstTime[i]) {
			leastBurstTime = burstTime[i];
        } 
    }
	
	for (j = 0; j < numProcesses + 1; j++)  {  
		/* Store the time quantum for each cycle */
		timeSlice[j] = leastBurstTime;
		for (i = 0; i < numProcesses; i++)  { 
			/* if remaining burst time is not 0 means that the process have not fully executed in CPU */
			if (burstTime[i] != 0) {                     
				/* if burst time is smaller than time slice */
				if (burstTime[i] <= timeSlice[j]) {        
					/* 
					* turnaround time = arranged value + burst time
					* arranged value = last arranged value + burst time
					* remaining burst time = 0 (process exit)
					*/
					turnAroundTime[i] = arranged + burstTime[i];         
					arranged = arranged + burstTime[i];                     
					burstTime[i] = 0;                                       
					
				}
				/* if burst time is larger than time slice */
				else if (burstTime[i] > timeSlice[j]) {     
					/* 
					* remaining burst time = burst time - time slice
					* set arranged value = last arranged value + time slice
					*/
					burstTime[i] = burstTime[i] - timeSlice[j];            
					arranged = arranged + timeSlice[j];                    
				}
            }  
	    } 
		/* if there is only 1 process left that have not fully executed and calculate total time quantum */
		if (timeSlice[j] == 999) {
			totalTimeSlice = totalTimeSlice + (maxBurstTime - totalTimeSlice);
		}
		/* calculate the shortest remaining burst time to be used as the time quantum for the next cycle */
		else {
			totalTimeSlice = totalTimeSlice + timeSlice[j];
			leastBurstTime = 999;
			for (k = 1; k < numProcesses; k++) {        
				if (leastBurstTime > burstTime[k] && burstTime[k] != 0) {
					leastBurstTime = burstTime[k];
				}
			}
		}
	}
	
	/* calculate the waiting time of each processes and calculate the total and largest waiting time of all processes*/
	for (int i = 0; i < numProcesses; i++) {
		waitTime[i] = turnAroundTime[i] - backupBurstTime[i] - arrivalTime[i];	
			if (waitTime[i] > maxWaitingTime) {
				maxWaitingTime = waitTime[i];						
			}
		totalWaitTime += waitTime[i]; 
	}

	/* calculate the turnaround time for each processes and calculate the total and largest turnaround time of all processes */
	for (int j = 0; j < numProcesses; j++) {
		turnAroundTime[j] = turnAroundTime[j] - arrivalTime[j];
		if (turnAroundTime[j] > maxTurnAroundTime) {
			maxTurnAroundTime = turnAroundTime[j];				
		}
		totalTurnAroundTime += turnAroundTime[j]; 				
	}

	/* Calculate the response time for each of the processes and calculate the total response time of all processes*/
	for (int k = 0; k < numProcesses; k++) {
		if (k == 0) {
			responseTime[k] = 0;
		}
		else {
			responseTime[k] = (k * timeSlice[0]) - burstTime[k - 1] - arrivalTime[k];
        	totalResponseTime += responseTime[k]; 
		}
	}

	/* calculate the completion time/fully exit time of each processes and calculate the total completion time of all processes */
	for (int l = 0; l < numProcesses; l++) {
		completionTime[l] = turnAroundTime[l] + arrivalTime[l];
		totalCompletionTime += completionTime[l];				
	}

	/* Display All data of each processes */
	printf("\n\nSorted Dynamic Round Robin Scheduling with Shortest Job First\n");

	printf("\nPROCESS\t BURST TIME \t WAITING TIME \t TURNAROUND TIME");
	printf("\tRESPONSE TIME \t\tCOMPLETION TIME \tARRIVAL TIME\n");
	for(i = 0; i < numProcesses; i++) {
		printf("%d\t %d\t\t %d\t\t %d\t\t",process[i], backupBurstTime[i], waitTime[i], turnAroundTime[i]); 
		printf("\t%d\t\t\t%d \t\t\t%d\n", responseTime[i], completionTime[i], arrivalTime[i]);
	}

    printf("\nAverage Turnaround Time = %.2f\n", totalTurnAroundTime / numProcesses);
    printf("\nMaximum Turnaround Time = %.2f\n", maxTurnAroundTime);  
	printf("\nAverage Waiting Time = %.2f\n", totalWaitTime / numProcesses);
    printf("\nMaximum Waiting Time = %.2f\n", maxWaitingTime);
	printf("\nAverage Time Slice %.2f\n", totalTimeSlice / numProcesses);
    printf("\nThe Average Response time: %.2f\n", totalResponseTime / numProcesses);

	return 0;
}