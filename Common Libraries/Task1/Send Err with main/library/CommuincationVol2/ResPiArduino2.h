#ifndef RESPIARDUINO2_H
#define RESPIARDUINO2_H
#include "Useable.h"

struct Device device={};
Message ErrorQueue[ERROR_QUEUE_SIZE];
char readdarr[MAX_MESSAGE_LENGTH];
struct InformationQueue Queue1;
struct InformationQueue Queue2;
struct InformationQueue Queue3;
struct InformationQueue Queue5;
#endif