#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // For atoi

#define RR_QUANTUM 2
#define CNTXT_SWITCH 1


typedef struct ProcessStruct {
    int id;
    int arrival_time;
    int execution_time;
    int priority;
} Process;

typedef struct ProcessOut {
    int id;
    int turnaround_time;
    int waiting_time;
    int nb_preemptions;
} Process_out;

typedef struct NodeStruct {
    Process process;
    struct NodeStruct * next;
} Node;

typedef struct NodeStructOut {
    Process_out process;
    struct NodeStructOut * next;
} Node_out;

void add_process(Node **head, Process new_process) {
    Node *new_node = (Node *)malloc(sizeof(Node)); // Allocate memory for a new node
    new_node->process = new_process;              // Set the process data
    new_node->next = NULL;                        // This will be the last node

    if (*head == NULL) {                          // If the list is empty
        *head = new_node;                         // New node becomes the head
    } else {
        Node * current = *head;
        while (current->next != NULL) {          // Traverse to the last node
            current = current->next;
        }
        current->next = new_node;                 // Link the last node to the new node
    }
}

void add_processOut(Node_out **head, Process_out new_processOut) {
    Node_out *new_node = (Node_out *)malloc(sizeof(Node_out)); // Allocate memory for a new node
    new_node->process = new_processOut;                        // Set the process_out data
    new_node->next = NULL;                                     // This will be the last node

    if (*head == NULL) {                                       // If the list is empty
        *head = new_node;                                      // New node becomes the head
    } else {
        Node_out *current = *head;
        while (current->next != NULL) {                        // Traverse to the last node
            current = current->next;
        }
        current->next = new_node;                              // Link the last node to the new node
    }
}



void print_processes(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("Process ID: %d, Arrival Time: %d, Execution Time: %d, Priority: %d\n",
               current->process.id,
               current->process.arrival_time,
               current->process.execution_time,
               current->process.priority);
        current = current->next;  // Move to the next node
    }
}

void print_processes_out(Node_out *head) {
    Node_out *current = head;
    while (current != NULL) {
        printf("Process ID: %d, nb_preemptions: %d, turnaround_time: %d, waiting_time: %d\n",
               current->process.id,
               current->process.nb_preemptions,
               current->process.turnaround_time,
               current->process.waiting_time);
        current = current->next;  // Move to the next node
    }
}

void free_process_list(Node *head) {
    Node *current = head;
    Node *next;
    while (current != NULL) {
        next = current->next;  // Save the next node
        free(current);         // Free the current node
        current = next;       // Move to the next node
    }
}

void process_file(Node **process_list) {
    char filepath[] = "./tasks.csv";
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        char *col1, *col2, *col3, *col4;
        col1 = strtok(line, ",");
        col2 = strtok(NULL, ",");
        col3 = strtok(NULL, ",");
        col4 = strtok(NULL, ",");

        Process process = {atoi(col1), atoi(col2), atoi(col3), atoi(col4)};
        //printf("%s Process : %d, %d, %d,  \n", line, process.id, process.arrival_time, process.execution_time,
               //process.priority);
            add_process(process_list, process);

        // create task
        // add task to queue
    }
    fclose(file);
}

void write_file(Node * e){
    FILE *myStream = fopen("execution.csv", "w");
    print_processes(e);
    fclose(myStream);
}


void fcfs(Node * head){
    Node_out *head_of_fcfs_list = 0;
    Node *current = head;

    int timer = 0;
    int completionTime = current -> process.arrival_time;
    int turnaroundTime;
    int waitingTime;

    while(current != NULL){
        current -> process.arrival_time;
        if (timer >= current -> process.arrival_time){
            //Execution
            if(timer == completionTime + current->process.execution_time){
                //End of Execution
                completionTime = timer;
                turnaroundTime = completionTime - current -> process.arrival_time;
                waitingTime = turnaroundTime - current -> process.execution_time;

                Process_out processOut = {current -> process.id, turnaroundTime, waitingTime, 0};
                add_processOut(&head_of_fcfs_list, processOut);
                current = current -> next;

                if(current != NULL) {
                    timer = current->process.arrival_time;
                }else {
                    break;
                }
            }
        }
        timer++;
    }
    print_processes_out(head_of_fcfs_list);

}

int main() {

    Node *head_of_process_list = 0;

    process_file(&head_of_process_list);
    fcfs(head_of_process_list);
    //printf("Process List:\n");
    //print_processes(head_of_process_list);
    //write_file(process_list);
    // Free the memory allocated for the linked list
    free_process_list(head_of_process_list);

    return 0;
}