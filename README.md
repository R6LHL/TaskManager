# TaskManager
## Библиотека представляет собой планировщик задач для контроллеров AVR8
И может использоваться как в среде Arduino, так и в чистом программировании для AVR8.
Планировщик использует один из таймеров контроллера (вы сами выбираете какой). Например, на плате Nano328, 
если в прошивке используется Serial, то использовать Timer0 не получится.
Сам TaskManager написан в виде набора переменных и функций в пространстве имён:

```C++
namespace TaskManager{};
```
таким образом TaskManager использует минимум памяти программ и оперативной памяти.
Задачи планировщика представляют собой функции вида:

```C++
void functionName(void);
```

### Подключение к проекту

Прежде чем подключить к проекту TaskManager.h, необходимо определить величину 
очереди задач (массив указателей на функции), например:

```C++
#define T_TASK_QUEUE_SIZE (5)
#include <TaskManager.h>
```
иначе компилятор выдаст ошибку "T_TASK_QUEUE_SIZE is not defined!"     
!!!В библиотеке не предусмоторен контроль за переполнением очереди!!!  
Следите сами сколько одновременно задач вы ставите в очередь.
Также ничто не подскажет вам, что очередь задач пуста (кроме того, что ваше устройство, по-видимому, зависло)
и постоянно выполняется функция loop() и ничего более.

Далее, вам необходимо определиться, по какому из прерываний какого 
из таймеров будет происходить движение очереди задач.
Как было сказано выше, если используете Serial, то использовать Timer0
не сможете. Но можно сделать так:

```C++
//configuration

#define UART_ENABLED (1)

#if UART_ENABLED == 1
  const unsigned int UART_SPEED  = (9600);
#endif
////////////////////////////////////////////////

//interrupt handlers

#if UART_ENABLED == 1 //UART use TIMER0 so we use TIMER2 if UART is ENABLED
ISR(TIMER2_OVF_vect)
{
  //Serial.println("Interrupt is working");
  TaskManager::TimerTaskService_();
}
#else
ISR(TIMER0_OVF_vect)
{
  //Serial.println("Interrupt is working");
  TaskManager::TimerTaskService_();
}
#endif
```
Далее необходимо настроить периодичность прокрутки очереди, настроив периодичность
переполнения таймера, прерывание которого вы используете. Я предпочитаю период в 1 мс.

```C++
#define CPU_CLOCK (16000000) 	// Hz at Nano328p

void setup()
{
  noInterrupts();
  
/////////////////////////////////////
#if UART_ENABLED == 1     //UART use TIMER0 so we use TIMER2 if UART is ENABLED
  TCCR2B |= (1<<CS22);    // (clk/64)
  TIMSK2 |= (1<<TOIE2);   // ovf interrupt enabled
#else
 TCCR0B |= (1<<CS02);     // (clk/64)
 TIMSK0 |= (1<<TOIE0);    // ovf interrupt enabled
#endif
...
```
Осталось только добавить какую-нибудь начальную задачку и запустить обработку очереди:

```C++
...// мы еще в функции setup();
   TaskManager::SetTask_(here_will_be_your_task_name_without_branches, 0); //если вторым аргументом стоит 0, задача запустится 
   // сразу, как только выполнение перейдет к функции loop(). Вообще, это задержка выполнения задачи, в моём случае - 
   //в миллисекундах... Сколько можно задать максимум? Столько, сколько позволяет unsigned int.

} // конец функции Setup(); НЕ ЗАБУДЬТЕ РАЗРЕШИТЬ ПРЕРЫВАНИЯ ПО ТАЙМЕРАМ И ГЛОБАЛЬНО

void loop()
{
  //Serial.println("Queue is processing");
  TaskManager::ProcessTaskQueue_();
  // Не надо больше сюда ничего писать.
  // Все что должна делать ваша прошивка, должно быть сделано в функциях-задачах
}
```


### Функции-задачи
Функции-задачи - это обычные функции вида

```C++
void functionName(void);
```
объявляемые в глобальной области напрямую, либо в виде пространства имен, нарпимер

```C++
void Tasks::functionName(void);
```
Функции-задачи могут вызывать друг друга или сами себя. Но для этого сначала надо их 
предварительно объявить, например

```C++
namespace Tasks
{
  extern void task1 (void);
  extern void task2 (void);
  
  void function1()
  {
    TaskManager::setTask_(function2, 10);
  }
  
  void function2()
  {
   TaskManager::setTask_(function1, 0);
  }
}
```
иначе может возникнуть ситуация, когда одна функция будет вне области видимости другой.
Конечно же вы можете создать файлы Tasks.h и Tasks.cpp и поместить это все там.
И конечно же, вы можете вызывать функции-задачи, как обычные функции, но это - 
плохой стиль, может возникнуть путаница, и лучше так не делать, если TaskManager 
уже выполняется, то есть ДО функции loop().
А функции, которые выполняют, например, только математические вычисления, и не работают с периферийными устройствами, как правило, не стоит
оформлять в виде функций-задач, их стоит использовать как обычно.

### Интерфейсные функции TaskManager
