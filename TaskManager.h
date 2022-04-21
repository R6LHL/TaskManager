/* 
* TaskManager.h
*
* Created: 19.02.2018 20:04:55
* Author: R6LHL
*/

#ifndef __TASKMANAGER_H__
#define __TASKMANAGER_H__

#include "TaskManager_config.h"

#ifndef T_TASK_QUEUE_SIZE
#error T_TASK_QUEUE_SIZE is not defined! 
#endif

namespace TaskManager
{
	//variables
	 extern void (*(taskQueue_[T_TASK_QUEUE_SIZE]))(void);        	// Array of function pointers
	 extern void (*(delayedTasksQueue_[T_TASK_QUEUE_SIZE]))(void);  	// Array of function pointers
	 extern unsigned int taskDelaysQueue_[T_TASK_QUEUE_SIZE];   	// Array of delays
	 extern void (*(currentTask_))(void);				// Function pointer
	 
	 //functions
	
	void clearTasks_(void);
	void ProcessTaskQueue_(void);
	void TimerTaskService_(void);							// This function must be called from timer interrupt handler
	void SetTask_(void(*f)(void), unsigned int t);
	void DeleteTask_(void(*f)(void));
	void ChangeTaskDelay_(void(*f)(void), unsigned int t);

}

#endif
