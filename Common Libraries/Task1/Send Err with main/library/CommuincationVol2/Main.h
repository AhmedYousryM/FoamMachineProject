#ifndef MAIN_H
#define MAIN_H

#include "mainArduino.h"

struct Device device;
Message ErrorQueue[ERROR_QUEUE_SIZE];
struct InformationQueue Queue1;
struct InformationQueue Queue2;
struct InformationQueue Queue3;
struct InformationQueue Queue5;
struct InformationQueue Queue;
InformationQueue* queuepointer;
char Myarr1[MAX_MESSAGE_LENGTH]={};
char Myarr2[MAX_MESSAGE_LENGTH]={};
char Myarr3[MAX_MESSAGE_LENGTH]={};
#endif