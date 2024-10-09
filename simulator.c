#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // For atoi
#define RR_QUANTUM 2
#define CNTXT_SWITCH 1

//Process IN class (id, arrival time, execution time, priority)
typedef struct ProcessIn {
    int id;
    int arrival_time;
    int execution_time;
    int priority;
} Process_in;

//Process OUT class (id,turnaround time =completion-arrival , waiting time = turnaround-execution, preemptions)
typedef struct ProcessOut {
    int id;
    int turnaround_time;
    int waiting_time;
    int nb_preemptions;
} Process_out;

//Node IN class
typedef struct NodeStructIn {
    Process_in process;
    struct NodeStructIn * next;
} Node_in;

//Node Out class
typedef struct NodeStructOut {
    Process_out process;
    struct NodeStructOut * next;
} Node_out;

//IN : When creating data is used to add a node to the end of a linked list
void add_processIn(Node_in **head, Process_in new_process) {
    Node_in *new_node = (Node_in *)malloc(sizeof(Node_in));
    new_node->process = new_process;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        Node_in * current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

//OUT : When processing data is used to add a node to the end of a linked list
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

//Print all the process of a linked list IN
void print_processes_in(Node_in *head) {
    Node_in *current = head;
    while (current != NULL) {
        printf("Process ID: %d, Arrival Time: %d, Execution Time: %d, Priority: %d\n",
               current->process.id,
               current->process.arrival_time,
               current->process.execution_time,
               current->process.priority);
        current = current->next;  // Move to the next node
    }
    printf("\n");
}
//Print all the process of a linked list OUT
void print_processes_out(Node_out *head) {
    Node_out *current = head;
    while (current != NULL) {
        printf("Process ID: %d, turnaround_time: %d, waiting_time: %d, nb_preemptions: %d, \n",
               current->process.id,
               current->process.turnaround_time,
               current->process.waiting_time,
               current->process.nb_preemptions);
        current = current->next;  // Move to the next node
    }
    printf("\n");
}

//Avoid memory links by removing the head in the memory
void free_process_list(Node_in *head) {
    Node_in *current = head;
    Node_in *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

//Create all the data (adding in list process) by reading tasks.csv
void process_file(Node_in **process_list) {
    char filepath[] = "./tasks.csv";
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char line[256];
    //Go through all the lines and parse them
    while (fgets(line, sizeof(line), file)) {
        char *col1, *col2, *col3, *col4;
        col1 = strtok(line, " ");
        col2 = strtok(NULL, " ");
        col3 = strtok(NULL, " ");
        col4 = strtok(NULL, " ");
        //Create process and add it to the linked list
        Process_in process = {atoi(col1), atoi(col2), atoi(col3), atoi(col4)};
        add_processIn(process_list, process);
    }
    print_processes_in(*process_list);
    fclose(file);
}

//Write the output file (process id, turnaround time, waiting time, num preempted)
void write_file(Node_out * e){
    FILE *myStream = fopen("execution.csv", "w");
    print_processes_out(e);
    fclose(myStream);
}

//FCFS : executed in order of arrival time
Node_out fcfs(Node_in * head){
    Node_out *head_of_fcfs_list = 0;
    Node_in *current = head;

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
    return *head_of_fcfs_list;
}





int main() {
    //INPUT DATA (creation of data)
    Node_in *Input_head = 0;
    process_file(&Input_head);

    //Methods
    Node_out Output_data = fcfs(Input_head);

    //OUTPUT DATA (writing data)
    write_file(&Output_data);
    free_process_list(Input_head);
    return 0;
}