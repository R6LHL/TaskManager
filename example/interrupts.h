#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "variables.h"
//interrupt handlers

ISR(TIMER2_OVF_vect)
{
  //Serial.println("Interrupt is working");
  OS.TimerTaskService_();
}
/*
ISR(PCINT2_vect )
{
  if (rangeSensorFront.read() < MINIMUM_RANGE_CM) CHASSIS_stop();
}
*/
#endif //_INTERRUPTS_H
