#ifndef RUNGROUPS_H
#define RUNGROUPS_H

// Every Task gets one of these structs
struct RunGroupInfo {
  uint8_t run_group_id;

  uint8_t run_group_active : 1;
  uint8_t task_active : 1;
  uint8_t cycle_id : 6;

  RunGroupInfo() {
    run_group_id = 0xFF;
    run_group_active = 0;
    task_active = 1;
    cycle_id = 0;
  }
};

class RunGroups {
private:
  static uint8_t current_cycle_id;
  static uint8_t count_active_group_tasks;
  static uint8_t current_group_id;
  static uint8_t lowest_active_run_group_id;
  static uint8_t next_run_group_id;

public:
  inline static uint8_t getCurrentRunGroupID();
  inline static uint8_t getCurrentCycleID();

  static void updateTask(RunGroupInfo *run_group_info);
  static void updateGroupStats(RunGroupInfo *run_group_info);
  static void updateCycleId();
};

/*
  class Task {
    // ...

    RunGroupInfo run_group_info;

    // ...

    void loopWrapper() {

      // ...

      while (1) {

        run_group_info->task_active = loop();

        yield();
      }
    }
  }

  class Scheduler {
    bool can_schedule(Task *t) {
      // ...

      // Check the run_group_active and task_active status
                if (!(t->run_group_info.run_group_active &&
t->run_group_info.task_active) return false;

                // ...
                return true;
    }

    void begin() {
      // ...

      RunGroups::updateTask(&current->run_group_info);

      // ...

      if (can_schedule(current))
        cont_run(&current->context, task_trampoline);

      // ...

      RunGroups::updateGroupStats(&current->run_group_info);

      // ...
    }
  }

  class SchedulerTask {
    bool loop() {
      // ...

      RunGroups::updateCycleId();

      // ...
    }
  }
*/
#endif
