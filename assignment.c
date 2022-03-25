// CPU Scheduling Method: Round Robin (RR) and Shortest Job First (SJF) 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<ctype.h>

int main(int argc, char *argv[])
{
	FILE *file;
	char *filename, *token;
    char ch, buffer[10], s[10] = " ";

    int lines = 1, numProcesses = 1;
	int arrivalTime[50];
	int burstTime[50];
	// int *arrivalTime = (int* ) malloc(lines * sizeof(int));
    // int *burstTime = (int* ) malloc(lines * sizeof(int));

	if (argc < 2) {					// Check if a filename has been specified in the command
        printf("Missing Filename\n");
        return(1);
	}
	else {
        filename = argv[1];
        // printf("Filename : %s\n", filename);
	}

    file = fopen(filename, "r");
	
    if (file == NULL) {
        printf("Missing file\n");
        return 0;
    }

	int current_lines = 1, current_numProcesses = 1;

    for (ch = fgetc(file); ch != EOF; ch = getc(file)){
        if (ch == '\n' || ch == '\r') {					// count number of lines
            current_lines += 1;
			current_numProcesses += 1;					// scale number of processes
			lines = current_lines - 1;
			numProcesses = current_numProcesses - 1;

			// lines += 1;
			// numProcesses += 1;
         }
    }

	int temp_lines;
    lines = temp_lines;
	temp_lines = 0;
    fclose(file);

    file = fopen(filename, "r");
    if(file == NULL) {
        printf("Missing file\n");
        return 0;
    }

    while(fgets(buffer, 10, file)) {
        buffer[strcspn(buffer, "\n")] = 0;			// look for  first occurence of new line within the buffer
		buffer[strcspn(buffer, "\r")] = 0;			// look for  first occurence of carriage return within the buffer

        token = strtok(buffer, s);					// break into token based on space
        arrivalTime[temp_lines] = atoi(token);

        token = strtok(NULL, s);					// break into token based on space
        burstTime[temp_lines] = atoi(token);

        temp_lines++;
    }

    fclose(file);
	
	int i, j, k, m, waitTime[20], turnAroundTime[20], dynamicSlicer[20], process[20], backupBurstTime[20];
	int sum = 0, maxBurstTime = 0, arranged = 0, count = 0, responseTime[20], completionTime[20];
	float totalWaitTime = 0, totalTurnAroundTime = 0, totalResponseTime = 0, totalCompletionTime = 0, mediumBT = 0;

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

	for (i = 1; i < numProcesses; i++) {        // get largest burst time
		if (maxBurstTime < burstTime[i]) {
			 maxBurstTime = burstTime[i];
        } 
    }

    if (sum % numProcesses == 0) {
        mediumBT = sum / numProcesses;
    }
    else {
        mediumBT = (sum / numProcesses) + 1;
    }
    

    
	for (j = 0; j < (maxBurstTime / mediumBT) + 1; j++)  {   

		for (i = 0; i < numProcesses; i++)  { 

			if (burstTime[i] != 0) {                     // as long as burst time is not 0
				
				if (burstTime[i] <= mediumBT) {         // if burst time is smaller than time slice

					turnAroundTime[i] = arranged + burstTime[i];            // turnaround time = arranged value + current burst time
					arranged = arranged + burstTime[i];                     // arranged value = last arranged value + current burst time
					burstTime[i] = 0;                                       // reset current burst time
					
				}
				else if (burstTime[i] > mediumBT) {     // if burst time is larger than time slice

					burstTime[i] = burstTime[i] - mediumBT;             // current burst time = burst time - time slice
					arranged = arranged + mediumBT;                     // set arranged value = last arranged value + time slice
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
			responseTime[k] = 0;
		}
		else {
			responseTime[k] = (k * mediumBT) - burstTime[k - 1];
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

	printf("\nAverage Time Slice %.2f\n", mediumBT);
    printf("\nThe Average Response time: %.2f\n", totalResponseTime / numProcesses);


    // free(arrivalTime);
    // free(burstTime);

	return 0;
}