#include "Tasks.h"
#include "TaskManager.h"
#include <Arduino.h>

void Task::LED_ON(void){
	digitalWrite(LED_BUILTIN, HIGH);   		// turn the LED on (HIGH is the voltage level)
	TaskManager::SetTask_(LED_OFF,1000);	// wait for a second
}
//////////////////////////////////////////////////////////////////////////////

void Task::LED_OFF(void){
	digitalWrite(LED_BUILTIN, LOW);    		// turn the LED off by making the voltage LOW
	TaskManager::SetTask_(LED_ON,1000);     // wait for a second
}
//////////////////////////////////////////////////////////////////////////////
