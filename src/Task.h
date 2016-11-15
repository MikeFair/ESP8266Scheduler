#ifndef TASK_H
#define TASK_H

#include <Arduino.h>
#include "RunGroups.h"
#include "Delay.h"

extern "C" {
#include "cont.h"
}

class Task {
public:
  Task() { cont_init(&context); }

protected:
  virtual void setup() {}

  virtual bool loop() { return false; }

  // Task
  void yield() { cont_yield(&context); }

  virtual bool shouldRun() { return true; }

  // Delay
  void delay(uint32_t ms) { Delay::startDelay(&delay_info, ms); }
  inline bool is_delayed() { return Delay::isDelayed(&delay_info); }

  // RunGroups
  void setRunGroupId(uint8_t group_id) {
    run_group_info.run_group_id = group_id;
  }

  struct RunGroupInfo run_group_info;

private:
  friend class SchedulerClass;
  friend void task_tramponline();

  Task *next;
  Task *prev;
  cont_t context;

  bool setup_done = false;
  struct DelayInfo delay_info;

  void loopWrapper() {
    if (!setup_done) {
      setup();
      setup_done = true;
    }

    while (1) {
      if (shouldRun())
        run_group_info.task_active = loop();
      yield();
    }
  }
};

#endif
