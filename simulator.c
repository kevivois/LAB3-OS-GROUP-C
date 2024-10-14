#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RR_QUANTUM 2    // round-robin time quantum
#define CNTXT_SWITCH 1  // context switch time


typedef struct processStruct {
    int pid;                // process id
    int arrival_time;       // arrival time of the process
    int execution_time;     // time needed for execution
    int priority;           // priority level of the process
} process_in;


typedef struct processReturn {
    int pid;                 // process id
    int turnaround_time;     // time taken for completion
    int waiting_time;        // time spent waiting in the queue
    int prempted_n;          // number of times the process was preempted
} process_out;


process_in* readFile(char* filename, int* countLines) {
    FILE* file = fopen(filename, "r");
    // handle file opening error
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    *countLines = 0;
    while (fgets(line, sizeof(line), file)) {
        (*countLines)++;
    }

    // allocate memory for processes
    process_in* processes = (process_in*)malloc((*countLines) * sizeof(process_in));
    if (processes == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    // reset file pointer to the beginning of the file
    rewind(file);

    // read the processes information from the file
    for (int i = 0; i < *countLines; i++) {
        fscanf(file, "%d %d %d %d", &processes[i].pid, &processes[i].arrival_time,
               &processes[i].execution_time, &processes[i].priority);
    }

    fclose(file);
    return processes;
}

// function to write process execution results to a file
void writeFile(const char* filename, process_out* processes, int n) {
    FILE* file = fopen(filename, "w");
    // handle file opening error
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < n; i++) {
        fprintf(file, "%d,%d,%d,%d\n", processes[i].pid, processes[i].turnaround_time, processes[i].waiting_time, processes[i].prempted_n);
    }

    fclose(file);
}

// FCFS scheduling algorithm
process_out* FCFS(process_in* processes, int n){
    // allocate memory for return values
    process_out* returnValues = (process_out*)malloc(n * sizeof(process_out));
    if (returnValues == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    int timer = 0;

    // process each job in the order of arrival
    for (int i = 0; i < n; i++) {
        if (timer < processes[i].arrival_time) {
            timer = processes[i].arrival_time;  // wait until the process arrives
        }
        returnValues[i].pid = processes[i].pid;
        if (timer > processes[i].arrival_time) {
            returnValues[i].waiting_time = timer - processes[i].arrival_time;
        } else {
            returnValues[i].waiting_time = 0;
        }
        returnValues[i].turnaround_time = processes[i].execution_time + returnValues[i].waiting_time;
        returnValues[i].prempted_n = 0;

        timer += processes[i].execution_time;  // update current time after executing the process
    }
    return returnValues;
}

// RR scheduling algorithm
process_out* RR(process_in* processes, int n){
    process_out* returnValues = (process_out*) malloc(n * sizeof(process_out));
    if (returnValues == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    int time = 0;
    int completed = 0;
    int prempted_count[n];
    int waiting_time[n];
    int turnaround_time[n];
    int remaining_time[n];

    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].execution_time;
        prempted_count[i] = 0;
        waiting_time[i] = 0;
        turnaround_time[i] = 0;
    }

    int current_process = -1;
    int quantum = RR_QUANTUM;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && remaining_time[i] > 0) {
                if (current_process != i) {
                    prempted_count[i]++;
                }
                current_process = i;
                if (remaining_time[i] > quantum) {
                    time += quantum;
                    remaining_time[i] -= quantum;
                } else {
                    time += remaining_time[i];
                    remaining_time[i] = 0;
                    completed++;
                    printf("Process %d completed at time %d\n", processes[i].pid, time);
                    turnaround_time[i] = time - processes[i].arrival_time;
                    waiting_time[i] = turnaround_time[i] - processes[i].execution_time;
                }
            }
            else {
                time++;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        returnValues[i].pid = processes[i].pid;
        returnValues[i].turnaround_time = turnaround_time[i];
        returnValues[i].waiting_time = waiting_time[i];
        returnValues[i].prempted_n = prempted_count[i];
    }


    return returnValues;
}

// priority scheduling algorithm
process_out* priorityScheduler(process_in* processes, int n) {
    process_out* returnValues = (process_out*)malloc(n * sizeof(process_out));
    if (returnValues == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    int timer = 0;
    int completed = 0;      // number of completed processes
    int prempted_count[n];  // number of preemptions for each process
    int waiting_time[n];
    int turnaround_time[n];
    int remaining_time[n];  // remaining execution time for each process
    int priority[n];

    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].execution_time;
        priority[i] = processes[i].priority;
        prempted_count[i] = 0;
        waiting_time[i] = 0;
        turnaround_time[i] = 0;
    }

    int current_process = -1;  // current running process

    while (completed < n) {
        int highest_priority = 4;  // initialize with a higher priority value
        int highest_priority_index = -1;

        // find the highest priority process that can be executed
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= timer && priority[i] < highest_priority && remaining_time[i] > 0) {
                highest_priority = priority[i];
                highest_priority_index = i;
            }
        }

        if (highest_priority_index == -1) {  // if no process can be executed, increment time
            timer++;
            continue;
        }

        // preempt the current process if it has a lower priority
        if (current_process != -1 && current_process != highest_priority_index && remaining_time[current_process] > 0) {
            prempted_count[current_process]++;
            timer = timer + CNTXT_SWITCH;  // simulate context switching time
        }

        current_process = highest_priority_index;

        remaining_time[highest_priority_index]--;
        timer++;

        if (remaining_time[highest_priority_index] == 0) {  // process is completed
            printf("Process %d completed at time %d\n", processes[highest_priority_index].pid, timer);
            completed++;  // increment the number of completed processes
            turnaround_time[highest_priority_index] = timer - processes[highest_priority_index].arrival_time;
            waiting_time[highest_priority_index] = turnaround_time[highest_priority_index] - processes[highest_priority_index].execution_time;
        }
    }


    for (int i = 0; i < n; i++) {
        returnValues[i].pid = processes[i].pid;
        returnValues[i].turnaround_time = turnaround_time[i];
        returnValues[i].waiting_time = waiting_time[i];
        returnValues[i].prempted_n = prempted_count[i];
    }

    return returnValues;
}

// SRTF scheduling algorithm
process_out* SRTF(process_in* processes, int n) {
    process_out* returnValues = (process_out*)malloc(n * sizeof(process_out));
    if (returnValues == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    int timer = 0;
    int completed = 0;      // number of completed processes
    int prempted_count[n];  // number of preemptions for each process
    int waiting_time[n];
    int turnaround_time[n];
    int remaining_time[n];

    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].execution_time;
        prempted_count[i] = 0;
        waiting_time[i] = 0;
        turnaround_time[i] = 0;
    }

    int current_process = -1;  // current running process

    while (completed < n) {
        int shortest_remaining_time = 1000;  // initialize with a large value
        int shortest_remaining_time_index = -1;

        // find the process with the shortest remaining time that can be executed
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= timer && remaining_time[i] > 0 && remaining_time[i] < shortest_remaining_time) {
                shortest_remaining_time = remaining_time[i];
                shortest_remaining_time_index = i;
            }
        }

        if (shortest_remaining_time_index == -1) {  // if no process can be executed, increment time
            timer++;
            continue;
        }

        // preempt the current process if it has a longer remaining time
        if (current_process != -1 && remaining_time[current_process] > 0 && current_process != shortest_remaining_time_index) {
            prempted_count[current_process]++;
            timer++;
        }

        current_process = shortest_remaining_time_index;

        timer++;  // increment time
        remaining_time[shortest_remaining_time_index]--;

        if (remaining_time[shortest_remaining_time_index] == 0) {  // process is completed
            completed++;  // increment the number of completed processes
            turnaround_time[shortest_remaining_time_index] = timer - processes[shortest_remaining_time_index].arrival_time;
            waiting_time[shortest_remaining_time_index] = turnaround_time[shortest_remaining_time_index] - processes[shortest_remaining_time_index].execution_time;
        }
    }

    for (int i = 0; i < n; i++) {
        returnValues[i].pid = processes[i].pid;
        returnValues[i].turnaround_time = turnaround_time[i];
        returnValues[i].waiting_time = waiting_time[i];
        returnValues[i].prempted_n = prempted_count[i];
    }
    return returnValues;
}


int main(int argc, char* argv[]) {
    int index_of_function = 1;

    char filename[100] = "data.csv";
    process_in* processes;  // array to store process input data
    int n = 0;  // number of processes
    processes = readFile(filename, &n);  // read processes from file
    process_out* returnValues = (process_out*)malloc(n * sizeof(process_out));  // allocate memory for return values

    if (returnValues == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    switch (index_of_function)
    {
        case 0:
            returnValues = FCFS(processes, n);  // execute FCFS
            writeFile("outputFCFS.csv", returnValues, n);  // write results to file
            break;
        case 1:
            returnValues = RR(processes, n);  // execute round-robin
            writeFile("outputRR.csv", returnValues, n);  // write results to file
            break;
        case 2:
            returnValues = priorityScheduler(processes, n);  // execute priority scheduling
            writeFile("outputPR.csv", returnValues, n);  // write results to file
            break;
        case 3:
            returnValues = SRTF(processes, n);  // execute shortest remaining time first
            writeFile("outputSRTF.csv", returnValues, n);  // write results to file
            break;

        default:
            break;
    }

    free(processes);  // free the allocated memory for processes
    free(returnValues);  // free the allocated memory for return values
    return 0;
}
