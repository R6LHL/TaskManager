#define CPU_CLOCK (12000000)   // Hz at UNO328p
#include "TaskManager.h"
#include "interrupts.h"
#include "Tasks.h"

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  noInterrupts();
  pinMode(LED_BUILTIN, OUTPUT);

  TCCR2B |= (1<<CS22);    // (clk/64)
  TIMSK2 |= (1<<TOIE2);   // ovf interrupt enabled
  interrupts();
  TaskManager::SetTask_(Task::LED_ON, 0);
}

// the loop function runs over and over again forever
void loop() {TaskManager::ProcessTaskQueue_();}
/*
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
*/
