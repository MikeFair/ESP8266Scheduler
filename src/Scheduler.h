#ifndef SCHEDULER_H
#define SCHEDULER_H

// #include <Arduino.h>
#include "Task.h"
#include "SchedulerTask.h"

extern "C" void loop();
extern void task_tramponline();

class SchedulerClass {
public:
  SchedulerClass();

  static void start(Task *task);

  static void begin();

private:
  friend void task_tramponline();
  static bool can_schedule(Task *t);

  static SchedulerTask main;
  static Task *current;
};

extern SchedulerClass Scheduler;

#endif
