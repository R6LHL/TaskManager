/*
  Blink with Task Manager
  for Arduino Nano ATMega328p
  by R6LHL
  Usually Task Manager used when tasks number > 5...10
*/

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned const char T_TASK_QUEUE_SIZE = 5;  //for Task Manager - size of Task Queue
#include <TaskManager.h>

typedef TaskManager <T_TASK_QUEUE_SIZE> TaskManager5;
TaskManager5 OS;
//////////////////////////////////////////////////////////////////////////////////////
//TIMER INTERRUPT HANDLER - shifts task queue when timer overflow interrupt is called
ISR (TIMER2_OVF_vect)
{
  OS.TimerTaskService_();
}
////////////////////////////////////////////////////////////////
// TASKS DECLARATIONS - need for tasks which calls each other. You can make this section with namespace Tasks::
extern void LED_On (void);
extern void LED_Off (void);
/////////////////////////////////////////////////////////////////
//TASKS DEFINITIONS////////////////////////////////////////////////
void LED_On(void)
{
  digitalWrite(LED_BUILTIN, HIGH);          // turn the LED on (HIGH is the voltage level)
   OS.SetTask_(LED_Off,1000);    // let's start LED_Off tasks from current task
}
void LED_Off(void)
{
  digitalWrite(LED_BUILTIN, LOW);       // turn the LED off by making the voltage LOW}
  OS.SetTask_(LED_Off,1000);  // let's start LED_On tasks from current task
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
 ///////CONFIGURE TIMER2 for 1 overflow in 1ms
  noInterrupts();
  TCCR2B |= (1<<CS22); // (clk/64)
  TIMSK2 |= (1<<TOIE2); // ovf interrupt enabled
  interrupts();
////////////////////////  
//NOW WE ADD TASK IN QUEUE
  OS.SetTask_(LED_On,1000); // (name of task, start delay time in ms)
}

// the loop function runs over and over again forever
void loop() {
 OS.ProcessTaskQueue_(); //calls the next task in queue each time after previous is ends
}
/*
 * Please - check TaskManager.h to see other API functions working with tasks
 * /
 */
