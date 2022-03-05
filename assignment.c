// CPU Scheduling Method: Round Robin (RR) and Shortest Job First (SJF) 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<ctype.h>

int main(int argc, char *argv[]) {
	
	FILE *file;
	char *filename;
	char ch;
	
	if (argc < 2) {					// Check if a filename has been specified in the command
        printf("Missing Filename\n");
        return(1);
	}
	else {
        filename = argv[1];
        printf("Filename : %s\n", filename);
	}
	
	file = fopen(filename,"r");		// Open file in read-only mode
	
	if (file) {						// If file opened successfully, then print the contents
        printf("File contents: \n");
        while ((ch = fgetc(file)) != EOF) {
			printf("%c",ch);
        }
		fclose(file);
    }
	else {
		printf("Failed to open the file\n");
    }
	
	int i, j, k, m, numProcesses = 5, waitTime[20], turnAroundTime[20], backupBurstTime[20], process[20];
	int sum = 0, maxBurstTime = 0, arranged = 0, count = 0, responseTime[20], completionTime[20];
	int burstTime[20] = {9, 7, 11, 8, 3};			// burst time for each process, currently manual input
	int arrivalTime[20] = {0, 2, 4, 1, 1};			// arrival time for each process, currently manual input

	float totalWaitTime = 0, totalTurnAroundTime = 0, totalResponseTime = 0, totalCompletionTime = 0;
	float timeSlice = 5;

	for (i = 0; i < numProcesses; i++) {
		process[i] = i + 1;                        		// number for the process
		backupBurstTime[i] = burstTime[i];  			// backupBurstTime is also burst time but acts as temporary storage for the value of burst time
		
	}  
	
	for (j = 0; j < numProcesses; j++) {

		for (k = j + 1; k < numProcesses; k++) {

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
			
			else if (arrivalTime[j] == arrivalTime[k] && burstTime[j] > burstTime[k]) {			// sorting of process number by shortest job first

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
	for (m = 0; m < numProcesses; m++) {

		sum += burstTime[m];
	}
	maxBurstTime = burstTime[0];  

	for  (i = 1; i < numProcesses; i++) {        // get largest burst time
		if (maxBurstTime < burstTime[i]) {
			 maxBurstTime = burstTime[i];
        } 
    } 
	
	for (j = 0; j < (maxBurstTime / timeSlice) + 1; j++)  {   

		for (i = 0; i < numProcesses; i++)  { 

			if (burstTime[i] != 0) {                     // as long as burst time is not 0
				
				if (burstTime[i] <= timeSlice) {         // if burst time is smaller than time slice

					turnAroundTime[i] = arranged + burstTime[i];            // turnaround time = arranged value + current burst time
					arranged = arranged + burstTime[i];                     // arranged value = last arranged value + current burst time
					burstTime[i] = 0;                                       // reset current burst time
					
				}
				else if (burstTime[i] > timeSlice) {     // if burst time is larger than time slice

					burstTime[i] = burstTime[i] - timeSlice;             // current burst time = burst time - time slice
					arranged = arranged + timeSlice;                     // set arranged value = last arranged value + time slice
				}
				
            }  
		}

	} 

	float maxTurnAroundTime = turnAroundTime[0];
	float maxWaitingTime = waitTime[0];

	for (int i = 0; i < numProcesses; i++) {
		waitTime[i] = turnAroundTime[i] - backupBurstTime[i] - arrivalTime[i];	// current process waiting time
		if (waitTime[i] > maxWaitingTime) {
			maxWaitingTime = waitTime[i];						// find maximum turnaround time
		}
        totalWaitTime += waitTime[i];  							// total waiting time
	}

	for (int j = 0; j < numProcesses; j++) {
		turnAroundTime[j] = turnAroundTime[j] - arrivalTime[j];
		if (turnAroundTime[j] > maxTurnAroundTime) {
			maxTurnAroundTime = turnAroundTime[j];				// find maximum turnaround time
		}
		totalTurnAroundTime += turnAroundTime[j]; 				// total turnaround time calculation
	}

	for (int k = 0; k < numProcesses; k++) {
		if (k == 0) {
			responseTime[k] == 0;
		}
		else {
			responseTime[k] = (k * timeSlice) - burstTime[k - 1];
        	totalResponseTime += responseTime[k]; 				// total response time calculation
		}
	}

	for (int l = 0; l < numProcesses; l++) {
		completionTime[l] = turnAroundTime[l] + arrivalTime[l];
		totalCompletionTime += completionTime[l];				// total completionTime
	}
        
		

	printf("\n\nSorted Round Robin Scheduling\n");

	printf("\nPROCESS\t BURST TIME \t WAITING TIME \t TURNAROUND TIME");
	printf("\tRESPONSE TIME \t\tCOMPLETION TIME \tARRIVAL TIME\n");
	for(i = 0; i<numProcesses; i++) {
		printf("%d\t %d\t\t %d\t\t %d\t\t",process[i], backupBurstTime[i], waitTime[i], turnAroundTime[i]); 
		printf("\t%d\t\t\t%d \t\t\t%d\n", responseTime[i], completionTime[i], arrivalTime[i]);
	}

    printf("\nAverage Turnaround Time = %.2f\n",totalTurnAroundTime / numProcesses);
    printf("\nMaximum Turnaround Time = %.2f\n", maxTurnAroundTime);  
	printf("\nAverage Waiting Time = %.2f\n",totalWaitTime / numProcesses);
    printf("\nMaximum Waiting Time = %.2f\n", maxWaitingTime);

	printf("\nAverage Time Slice %.2f\n", timeSlice);
    printf("\nThe Average Response time: %.2f\n", totalResponseTime / numProcesses);

	return 0;
}

