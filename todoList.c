#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//struct to handle data
typedef struct {
    char *task;
    int completed;
} Task;

//initialize the struct tasks for our data storage
Task *tasks = NULL;
int length = 0;

//remove warnings, add definitions
void addTask(char *task);
void displayTasks();
void markCompleted(int index);
void markIncomplete(int index);
void deleteTask(int index);
void editTask(int index, const char *task_to_add);
void loadTasksFromFile();
void saveTasksToFile();

//add a new task
void addTask(char *task) {
    tasks = (Task *)realloc(tasks, (length + 1) * sizeof(Task));
    if (tasks == NULL) {
        printf("Failed to allocate memory for tasks.\n");
        return;
    }

    tasks[length].task = (char *)malloc(strlen(task) + 1);
    if (tasks[length].task == NULL) {
        printf("Failed to allocate memory for task string.\n");
        return;
    }

    strcpy(tasks[length].task, task);
    tasks[length].completed = 0;
    length++;
    printf("Task added successfully.\n");
}

void addTaskNoMessage(char *task) {
    tasks = (Task *)realloc(tasks, (length + 1) * sizeof(Task));
    if (tasks == NULL) {
        printf("Failed to allocate memory for tasks.\n");
        return;
    }

    tasks[length].task = (char *)malloc(strlen(task) + 1);
    if (tasks[length].task == NULL) {
        printf("Failed to allocate memory for task string.\n");
        return;
    }

    strcpy(tasks[length].task, task);
    tasks[length].completed = 0;
    length++;
}

//display tasks
void displayTasks() {
    char status;
    for (int i = 0; i < length; i++) {
        if (tasks[i].completed) {
            status = ' ';
        } else {
            status = 'X';
        }
        printf("%d   %c   %s\n", i + 1, status, tasks[i].task);
    }
}

//mark task as complete
void markCompleted(int index) {
    if (index <= length && index > 0) {
        tasks[index - 1].completed = 1;
        printf("Task marked as completed.\n");
    } else {
        printf("Invalid index!\n");
    }
}

//mark task as incomplete
void markIncomplete(int index) {
    if (index <= length && index > 0) {
        tasks[index - 1].completed = 0;
        printf("Task marked as incomplete.\n");
    } else {
        printf("Invalid index!\n");
    }
}

//delete task
void deleteTask(int index) {
    if (index <= length && index > 0) {
        index--;
        free(tasks[index].task);

        for (int i = index; i < length - 1; i++) {
            tasks[i] = tasks[i + 1];
        }

        tasks = (Task *)realloc(tasks, (length - 1) * sizeof(Task));
        length--;
        printf("Task deleted successfully.\n");
    } else {
        printf("Invalid index!\n");
    }
}

//edit a task
void editTask(int index, const char *task_to_add) {
    if (index <= length && index > 0) {
        index--;
        char *editedTask = (char *)realloc(tasks[index].task, strlen(task_to_add) + 1);
        if (editedTask != NULL) {
            tasks[index].task = editedTask;
            strcpy(tasks[index].task, task_to_add);
            printf("Task updated successfully!\n");
        } else {
            printf("Failed to allocate memory for the edited task.\n");
        }
    } else {
        printf("Invalid index!\n");
    }
}

//load tasks from file
void loadTasksFromFile() {
    FILE *file = fopen("listData.txt", "r");
    if (file == NULL) {
        printf("No existing tasks found.\n");
        return;
    }

    char buffer[256];
    char completed;
    int bufferLength;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        bufferLength = strcspn(buffer, "\n");
        buffer[bufferLength] = '\0'; //strcspn will calculate the number of characters before first occurence of \n and set it to \0
        completed = buffer[0];
        memmove(buffer, buffer + 1, bufferLength);
        addTaskNoMessage(buffer);
        if (completed=='1') {
            tasks[length-1].completed = 1;
        } else {
            tasks[length-1].completed = 0;
        }
    }

    fclose(file);
}

//save tasks to file
void saveTasksToFile() {
    FILE *file = fopen("listData.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < length; i++) {
        fprintf(file, "%d%s\n",tasks[i].completed, tasks[i].task);
    }

    fclose(file);
}

int main() {
    loadTasksFromFile();

    int programOn = 1;
    int choice;
    int indexChoice;
    char taskInput[256]; // Allocate a buffer for user input

    printf("\nOptions\n");
    printf("1. Add a task.\n");
    printf("2. List out the tasks.\n");
    printf("3. Mark task completed.\n");
    printf("4. Mark task as incomplete.\n");
    printf("5. Delete a task\n");
    printf("6. Edit a task\n");
    printf("7. Exit\n");

    while (programOn) {
        printf("Enter Choice (1,2,3,4,5,6,7): ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("What should be the task to be added? : ");
            getchar();
            fgets(taskInput, sizeof(taskInput), stdin);
            taskInput[strcspn(taskInput, "\n")] = '\0';
            addTask(taskInput);
            break;
        case 2:
            printf("Following are your tasks: \n");
            displayTasks();
            break;
        case 3:
            printf("What numbered task do you want to mark completed? : ");
            scanf("%d", &indexChoice);
            markCompleted(indexChoice);
            break;
        case 4:
            printf("What numbered task do you want to mark incomplete? : ");
            scanf("%d", &indexChoice);
            markIncomplete(indexChoice);
            break;
        case 5:
            printf("What numbered task do you want to remove? : ");
            scanf("%d", &indexChoice);
            deleteTask(indexChoice);
            break;
        case 6:
            printf("What numbered task do you want to edit? : ");
            scanf("%d", &indexChoice);
            printf("Enter the new edited task: ");
            getchar(); 
            fgets(taskInput, sizeof(taskInput), stdin);
            taskInput[strcspn(taskInput, "\n")] = '\0';
            editTask(indexChoice, taskInput);
            break;
        case 7:
            programOn = 0;
            saveTasksToFile(); // Save tasks to file before exiting
            break;
        default:
            printf("Choose from valid numbers!\n");
            break;
        }
    }

    // Free allocated memory
    for (int i = 0; i < length; i++) {
        free(tasks[i].task);
    }
    free(tasks);

    return 0;
}
