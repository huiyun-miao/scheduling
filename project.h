#ifndef _PROJECT__H_
#define _PROJECT__H_

struct meeting {
	char description[500]; 
	int month;
	int day;
    int hour;
    struct meeting* next;
};

void addMeeting(struct meeting* m, char* str);
void deleteMeeting(struct meeting* m, char* str);
void listMeeting(struct meeting* m);
void deleteAllMeeting(struct meeting* m);
void writeToFile(struct meeting* m, char* str);
void loadFile(struct meeting* m, char* str);

#endif //! _PROJECT__H_