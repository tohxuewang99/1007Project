// CPU Scheduling Method: Round Robin (RR) and Shortest Job First (SJF)

#include<stdio.h> 

int main() {
	
	int i, j, k, m, numProcesses = 10, waitTime[20], turnAroundTime[20], backupBurstTime[20], process[20];
	int sum = 0, timeSlice = 5, maxBurstTime = 0, arranged = 0, responseTime[20];
	int burstTime[20] = {3,2,2,1,5,3,2,1,1,3}; // burst time for each process, currently manual input
	
	float avgWaitTime = 0, avgTurnAroundTime = 0, avgResponseTime = 0;
	float avgTimeSlice = 0;

	for (i = 0; i < numProcesses; i++) {
		process[i] = i;                        // number for the process
		backupBurstTime[i] = burstTime[i];  
	}  
	
	for (j = 0; j < numProcesses; j++) {

		for (k = j + 1; k < numProcesses; k++) {

			if (burstTime[j] > burstTime[k]) {

					arranged = burstTime[j];
					backupBurstTime[j] = burstTime[j] = burstTime[k];
					backupBurstTime[k] = burstTime[k] = arranged;

					arranged = process[j];
					process[j] = process[k];
					process[k] = arranged;
			}
        }	
	}
	arranged  = 0;
	for (m = 0; m < numProcesses; m++) {

		sum += burstTime[m];
	}
	avgTimeSlice = sum/numProcesses; 


	 maxBurstTime = burstTime[0];  

	for  (i = 1; i < numProcesses; i++) {        // get largest burst time
		if (maxBurstTime < burstTime[i]) {
			 maxBurstTime = burstTime[i];
        } 
    } 
	
	for (j = 0; j < (maxBurstTime / avgTimeSlice) + 1; j++)  {   

		for (i = 0; i < numProcesses; i++)  { 

			if (burstTime[i] != 0) {                        // as long as burst time is not 0
				
				if (burstTime[i] <= avgTimeSlice) {         // if burst time is smaller than time slice

					turnAroundTime[i] = arranged + burstTime[i];            // turnaround time = arranged value + current burst time
					arranged = arranged + burstTime[i];                     // arranged value = last arranged value + current burst time
					burstTime[i] = 0;                                       // reset current burst time
				}
				else if (burstTime[i] > avgTimeSlice) {     // if burst time is larger than time slice

					burstTime[i] = burstTime[i] - avgTimeSlice;             // current burst time = burst time - time slice
					arranged = arranged + avgTimeSlice;                     // set arranged value = last arranged value + time slice
				}
            }  
		}

	} 

	for (i = 0; i < numProcesses; i++)
	{
		waitTime[i] = turnAroundTime[i] - backupBurstTime[i]; // current process waiting time
        avgWaitTime += waitTime[i];  // total waiting time

        turnAroundTime[i] = waitTime[i] + backupBurstTime[i];
		avgTurnAroundTime += turnAroundTime[i]; // total turnaround time

        responseTime[i] = (i*timeSlice) - 0;
        avgResponseTime += responseTime[i];
	}

	printf("\n\nSorted Round Robin Scheduling\n");

	printf("\nPROCESS\t BURST TIME \t WAITING TIME \t TURNAROUND TIME \t RESPONSE TIME\n");
	for(i = 0; i<numProcesses; i++){
		printf("%d \t %d \t\t %d \t\t %d \t\t\t %d \n",process[i]+1, backupBurstTime[i], waitTime[i], turnAroundTime[i], responseTime[i]);
	}

    printf("\nAverage Turnaround Time = %f\n",avgTurnAroundTime/numProcesses);
    printf("\nMaximum Turnaround Time = \n");  
	printf("\nAverage Waiting Time= %f\n",avgWaitTime/numProcesses);
    printf("\nMaximum Waiting Time= \n");

	printf("\nAverage Time Slice %f\n", avgTimeSlice);
    printf("\nThe Average Response time: %.2f\n", avgResponseTime / numProcesses);
	
	return 0;
} 