#ifndef VARIABLES_H
#define VARIABLES_H

#include "config.h"
#include "TaskManager.h"
typedef TaskManager<task_queue_size> TaskManager5;
extern TaskManager5 OS;

#endif
