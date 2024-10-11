#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RR_QUANTUM 2
#define CNTXT_SWITCH 1

typedef struct processStruct {
    int pid;
    int arrival_time;
    int execution_time;
    int priority;
} process_t;

typedef struct processReturn {
    int pid;
    int turnaround_time;
    int waiting_time;
    int prempted_n;
} processReturn_t;

process_t* readFile(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    *count = 0;
    while (fgets(line, sizeof(line), file)) {
        (*count)++;
    }

    process_t* processes = (process_t*)malloc((*count) * sizeof(process_t));
    if (processes == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    rewind(file);

    for (int i = 0; i < *count; i++) {
        fscanf(file, "%d %d %d %d", &processes[i].pid, &processes[i].arrival_time,
               &processes[i].execution_time, &processes[i].priority);
    }

    fclose(file);
    return processes;
}

void writeFileExecution(const char* filename, processReturn_t* processes, int n) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "pid,turnaround_time,waiting_time,prempted_n\n");

    for (int i = 0; i < n; i++) {
        fprintf(file, "%d,%d,%d,%d\n", processes[i].pid, processes[i].turnaround_time, processes[i].waiting_time, processes[i].prempted_n);
    }

    fclose(file);
}

processReturn_t* FCFS(process_t* processes, int n){
    processReturn_t* returnValues = (processReturn_t*)malloc(n * sizeof(processReturn_t));
    if (returnValues == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    int time = 0;

    for (int i = 0; i < n; i++) {
        if (time < processes[i].arrival_time) {
            time = processes[i].arrival_time;
        }
        returnValues[i].pid = processes[i].pid;
        if (time > processes[i].arrival_time) {
            returnValues[i].waiting_time = time - processes[i].arrival_time;
        } else {
            returnValues[i].waiting_time = 0;
        }
        returnValues[i].turnaround_time = processes[i].execution_time + returnValues[i].waiting_time;
        returnValues[i].prempted_n = 0;

        time += processes[i].execution_time;
    }
    return returnValues;
}

processReturn_t* RR(process_t* processes, int n){
    processReturn_t* returnValues = (processReturn_t*) malloc(n * sizeof(processReturn_t));
    if (returnValues == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    int remaining_time[n];
    int completed = 0;
    int current_time = 0;
    int prempted_count[n];
    int waiting_time[n];
    int turnaround_time[n];

    while (completed < n) {
        int done = 1;
        for (int i = 0; i < n; i++){
            if (remaining_time[i]>0) {
                done = 0;

                if (remaining_time[i] > RR_QUANTUM) {
                    current_time += RR_QUANTUM;
                    remaining_time[i] -= RR_QUANTUM;
                    prempted_count[i]++;
                } else {
                    current_time += remaining_time[i];
                    remaining_time[i] = 0;
                    completed++;

                    turnaround_time[i] = current_time - processes[i].arrival_time;
                    waiting_time[i] = turnaround_time[i] - processes[i].execution_time;
                }

            }
        }

        if (done == 1) {
            break;
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

processReturn_t* priorityScheduler(process_t* processes, int n) {
    processReturn_t* returnValues = (processReturn_t*)malloc(n * sizeof(processReturn_t));
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
    int priority[n];

    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].execution_time;
        priority[i] = processes[i].priority;
        prempted_count[i] = 0;
        waiting_time[i] = 0;
        turnaround_time[i] = 0;
    }

    int current_process = -1;

    while (completed < n) {
        int highest_priority = 4;
        int highest_priority_index = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && priority[i] < highest_priority && remaining_time[i] > 0) {
                highest_priority = priority[i];
                highest_priority_index = i;
            }
        }

        if (highest_priority_index == -1) {
            time++;
            continue;
        }

        if (current_process != -1 && current_process != highest_priority_index && remaining_time[current_process] > 0) {
            prempted_count[current_process]++;
            time = time + CNTXT_SWITCH;
        }

        current_process = highest_priority_index;

        remaining_time[highest_priority_index]--;
        time++;

        if (remaining_time[highest_priority_index] == 0) {
            printf("Process %d completed at time %d\n", processes[highest_priority_index].pid, time);
            completed++;
            turnaround_time[highest_priority_index] = time - processes[highest_priority_index].arrival_time;
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


processReturn_t* SRTF(process_t* processes, int n) {
    processReturn_t* returnValues = (processReturn_t*)malloc(n * sizeof(processReturn_t));
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

    while (completed < n) {
        int shortest_remaining_time = 1000;
        int shortest_remaining_time_index = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && remaining_time[i] > 0 && remaining_time[i] < shortest_remaining_time) {
                shortest_remaining_time = remaining_time[i];
                shortest_remaining_time_index = i;
            }
        }

        if (shortest_remaining_time_index == -1) {
            time++;
            continue;
        }

        if (current_process != -1 && remaining_time[current_process] > 0 && current_process != shortest_remaining_time_index) {
            prempted_count[current_process]++;
            time++;
        }

        current_process = shortest_remaining_time_index;

        time++;
        remaining_time[shortest_remaining_time_index]--;
        shortest_remaining_time--;

        if (remaining_time[shortest_remaining_time_index] == 0) {
            completed++;
            turnaround_time[shortest_remaining_time_index] = time - processes[shortest_remaining_time_index].arrival_time;
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
    int index_of_function = 2;

    char filename[100] = "tasks.csv";
    process_t* processes;
    int n = 0;
    processes = readFile(filename, &n);
    processReturn_t* returnValues = (processReturn_t*)malloc(n * sizeof(processReturn_t));

    if (returnValues == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    switch (index_of_function)
    {
        case 0:
            returnValues = FCFS(processes, n);
            writeFileExecution("output.csv", returnValues, n);
            break;
        case 1:
            returnValues = RR(processes, n);
            writeFileExecution("output.csv", returnValues, n);
            break;
        case 2:
            returnValues = priorityScheduler(processes, n);
            writeFileExecution("output.csv", returnValues, n);
            break;
        case 3:
            returnValues = SRTF(processes, n);
            writeFileExecution("output.csv", returnValues, n);
            break;

        default:
            break;
    }

    free(processes);
    free(returnValues);
    return 0;
}