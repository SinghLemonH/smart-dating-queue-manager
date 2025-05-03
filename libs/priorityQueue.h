#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

typedef struct Partner {
    char name[50];          
    int relationshipScore;  
    char place[50];         
    char time[20];     
    struct Partner* next;   // point to next Partner in Priority Queue :< :A :O
} Partner;

//send to dijkstar file to fixed schedule table, send to dataManage file to manage data in Priority Queue
extern Partner* head; 
extern Partner* tail; 

void addPartner(const char* partnerName, int relationshipScore, const char* uid);
void deletePartner(const char* partnerName, const char* uid);
void viewPartnerStatus(const char* uid);
void freePriorityQueue();

const char* getRelationshipLevel(int score);
void displayRelation();

#endif // PRIORITYQUEUE_H