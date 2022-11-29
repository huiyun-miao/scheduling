#include "project.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// TODO:: implement your project here!

int main(void){
    struct meeting m = { "", 0, 0, 0, NULL };
    int go_on = 1;

    while (go_on) {
        char buffer[1000];
        fgets(buffer, 1000, stdin);

        switch (buffer[0])
        {
        case 'A':
            addMeeting(&m, buffer);
            break;
        
        case 'D':
            deleteMeeting(&m, buffer);
            break;
        
        case 'L':
            listMeeting(&m);
            break;

        case 'W':
            writeToFile(&m, buffer);
            break;
        
        case 'O':
            loadFile(&m, buffer);
            break;

        case 'Q':
            deleteAllMeeting(&m);
            printf("SUCCESS\n");
            return 0;
        
        default:
            printf("Invalid command %c\n", buffer[0]);
            break;
        }
    }
    return 1;
}


// find the pointer to the node before the node with the given time
// return NULL if there is no meeting found at given time
struct meeting* findTime(struct meeting* m, int month, int day, int hour) {
    struct meeting* previous;
    struct meeting* next;
    previous = m;
    next = m->next;

    while (next != NULL) {
        if (next->month == month && next->day == day && next->hour == hour) {
            return previous;
        }
        previous = next;
        next = next->next;
    }
    return NULL;
}


void add(struct meeting* m, char* description, int month, int day, int hour) {
    struct meeting* new_node;
    new_node = malloc(sizeof(struct meeting));
    
    strcpy(new_node->description, description);
    new_node->month = month;
    new_node->day = day;
    new_node->hour = hour;
    new_node->next = NULL;
    if (m->next == NULL) {
        m->next = new_node;
    } else {
        struct meeting* last;
        last = m;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_node;
    }
}


void addMeeting(struct meeting* m, char* str) {
    char description[500];
    int month;
    int day;
    int hour;

    if (sscanf(str, "%*c %s %d %d %d", description, &month, &day, &hour) != 4) {
        printf("A should be followed by exactly 4 arguments.\n");
        return;
    }

    if (month < 1 || month > 12) {
        printf("Month cannot be less than 1 or greater than 12.\n");
        return;
    }

    if (day < 1 || day > 31) {
        printf("Day cannot be less than 1 or greater than 31.\n");
        return;
    }

    if (hour < 0 || hour > 23) {
        printf("Hour cannot be negative or greater than 23.\n");
        return;
    }

    //check if the system has another meeting at this time
    if (findTime(m, month, day, hour) != NULL) {
        printf("The time slot %02d.%02d at %02d is already allocated.\n", day, month, hour);
        return;
    }

    // add meeting to the system
    add(m, description, month, day, hour);
    printf("SUCCESS\n");
}


void deleteMeeting(struct meeting* m, char* str) {
    int month;
    int day;
    int hour;

    if (sscanf(str, "%*c %d %d %d", &month, &day, &hour) != 3) {
        printf("D should be followed by exactly 3 arguments.\n");
        return;
    }

    if (month < 1 || month > 12) {
        printf("Month cannot be less than 1 or greater than 12.\n");
        return;
    }

    if (day < 1 || day > 31) {
        printf("Day cannot be less than 1 or greater than 31.\n");
        return;
    }

    if (hour < 0 || hour > 23) {
        printf("Hour cannot be negative or greater than 23.\n");
        return;
    }

    struct meeting* previous = findTime(m, month, day, hour);

    if (previous == NULL) {
        printf("The time slot %02d.%02d at %02d is not in the calendar.\n", day, month, hour);
        return;
    } 

    // delete meeting 
    struct meeting* delete;
    delete = previous->next;
    previous->next = previous->next->next;
    free(delete);
    printf("SUCCESS\n");
}


// compare the time for 2 meeting, swap the order if b is before a
int swap(struct meeting *a, struct meeting *b) {
    char tempStr[500];
    int tempM, tempD, tempH;

    int timeA = a->month * 10000 + a->day * 100 + a->hour;
    int timeB = b->month * 10000 + b->day * 100 + b->hour;

    if (timeA > timeB) {
        strcpy(tempStr, a->description);
        tempM = a->month;
        tempD = a->day;
        tempH = a->hour;

        strcpy(a->description, b->description);
        a->month = b->month;
        a->day = b->day;
        a->hour = b->hour;

        strcpy(b->description, tempStr);
        b->month = tempM;
        b->day = tempD;
        b->hour = tempH;
        return 1;
    } else {
        return 0;
    }
}


void bubbleSort(struct meeting* m) {
    int swapped; 
    struct meeting* node; 
    
    // check if there is zero or one element in the list
    if (m->next == NULL) {
        return;
    } 

    if (m->next->next == NULL) {
        return;
    }
  
    do { 
        swapped = 0; 
        node = m->next; // first element in the linked list
  
        while (node->next != NULL) { 
            if (swap(node, node->next)) {
                swapped = 1; 
            } 
            node = node->next; 
        }
    
    } while (swapped); 
}


void listMeeting(struct meeting* m) {
    bubbleSort(m);
    struct meeting* last;
    last = m->next;
    while (last != NULL)
    {
        printf("%s %02d.%02d at %02d\n", last->description, last->day, last->month, last->hour);
        last = last->next;
    }
    printf("SUCCESS\n");
}


void writeToFile(struct meeting* m, char* str) {
    char filename[50];

    if (sscanf(str, "%*c %s", filename) != 1) {
        printf("W should be followed by exactly 1 arguments.\n");
        return;
    }

    FILE* fp = fopen(filename, "w");
    struct meeting* last = m->next;

	if (!fp) {
        printf("Cannot write to file %s.\n", filename);
		return;
	}

	while (last != NULL)
    {
        fprintf(fp, "%s %02d.%02d at %02d\n", last->description, last->day, last->month, last->hour);
        last = last->next;
    }

	fclose(fp);
	printf("SUCCESS\n");
}


// delete all meetings in the linked list
void deleteAllMeeting(struct meeting* m) {
    struct meeting* node;
    
    while (m->next != NULL)
    {
        node = m;
        while (node->next->next != NULL) {
            node = node->next;
        }

        free(node->next);
        node->next = NULL;
    }
}


void loadFile(struct meeting* m, char* str) {
    char filename[50];

    if (sscanf(str, "%*c %s", filename) != 1) {
        printf("O should be followed by exactly 1 arguments.\n");
        return;
    }

    FILE* fp = fopen(filename, "r");

    char line[1000];
    char description[500];
    int month, day, hour;

    if (!fp) {
        printf("Cannot open file %s for reading.\n", filename);
		return;
	}

    deleteAllMeeting(m);

    while (fgets(line, 1000, fp) != NULL) {
		sscanf(line, "%s %02d.%02d at %02d\n", description, &day, &month, &hour);
        add(m, description, month, day, hour);
	}

    fclose(fp);
    printf("SUCCESS\n");
}