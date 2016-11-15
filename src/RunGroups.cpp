#include <Arduino.h>
#include "RunGroups.h"

uint8_t RunGroups::current_cycle_id;
uint8_t RunGroups::count_active_group_tasks;
uint8_t RunGroups::current_group_id;
uint8_t RunGroups::lowest_active_run_group_id;
uint8_t RunGroups::next_run_group_id;

uint8_t RunGroups::getCurrentRunGroupID() { return current_group_id; }
uint8_t RunGroups::getCurrentCycleID() { return current_cycle_id; }

void RunGroups::updateTask(RunGroupInfo *run_group_info) {
  // Executes before loop()
  uint8_t task_run_group_id = run_group_info->run_group_id;
  uint8_t task_cycle_id = run_group_info->cycle_id;
  /*
    Serial.print("Begin : ");
    Serial.print("\t SchedulerCycleID: "); Serial.print(current_cycle_id);
    Serial.print("\t TaskCycleID: "); Serial.print(task_cycle_id);
    Serial.print("\t SchedulerRunGroup: ");
    Serial.print(current_group_id);
    Serial.print("\t TaskRunGroup: "); Serial.print(task_run_group_id);
    Serial.print("\t Complete: ");
    Serial.print(run_group_info->task_active);
    Serial.println();
  */

  // First, check the cycle_id
  if (task_cycle_id != current_cycle_id) {
    // If the Task's cycle_id doesn't match; reset task_active
    // (run_group 0xFF manages it's own task_active flag; it's left alone here)
    if (task_run_group_id != 0xFF)
      run_group_info->task_active = true;

    // every run_group gets their current cycle_id updated
    run_group_info->cycle_id = current_cycle_id;
  }

  // Next, update the Task's run_group_active flag
  // run_group_active is always true when the run_group_id == 0xFF
  run_group_info->run_group_active =
      (task_run_group_id == 0xFF) || (task_run_group_id == current_group_id);

  // RunGroupId tracking: Calculate the next run_group_id, and the lowest
  // run_group_id to use when the cycle wraps.
  if (task_run_group_id < lowest_active_run_group_id)
    lowest_active_run_group_id = task_run_group_id;

  if (task_run_group_id > current_group_id &&
      task_run_group_id < next_run_group_id)
    next_run_group_id = task_run_group_id;
  /*
    Serial.print("Finish: ");
    Serial.print("\t SchedulerCycleID: "); Serial.print(current_cycle_id);
    Serial.print("\t TaskCycleID: "); Serial.print(task_cycle_id);
    Serial.print("\t SchedulerRunGroup: ");
    Serial.print(current_group_id);
    Serial.print("\t TaskRunGroup: "); Serial.print(task_run_group_id);
    Serial.print("\t Complete: ");
    Serial.print(run_group_info->task_active);
    Serial.println();
  */
}

void RunGroups::updateGroupStats(RunGroupInfo *run_group_info) {
  // This is called after the Task's loop() has run;
  // updateTask(&run_group_info) reset task_active and cycle_id if required
  // before loop() ran;
  // and if task_run_group_id was part of a currently active run_group, loop()
  // could have set run_group_info->task_active

  // If this Task is task_active and also part of the current run_group,
  // increment count_active_group_tasks; this group_id isn't done yet
  if (run_group_info->task_active &&
      run_group_info->run_group_id == current_group_id)
    count_active_group_tasks++;

  // if (current->next == main) updateRunGroupsCycleId();
}

void RunGroups::updateCycleId() {
  /*
    Serial.print("Begin : ");
    //Serial.print("\t Complete: ");
    Serial.print(run_group_info->task_active);
    Serial.print("\t count_active_group_tasks: ");
    Serial.print(count_active_group_tasks);
    Serial.print("\t next_run_group_id: 0x");
    Serial.print(next_run_group_id, HEX);
    Serial.print("\t lowest_active_run_group_id: 0x");
    Serial.print(lowest_active_run_group_id, HEX);
    Serial.print("\t current_group_id: "); Serial.print(current_group_id);
    Serial.print("\t current_cycle_id: "); Serial.print(current_cycle_id);
    Serial.println();
  */
  // If no Tasks in this group were active (all task_active == false), move to
  // the next group_id
  if (count_active_group_tasks == 0) {
    if (next_run_group_id == 0xFF) {   // If no next highest run_group_id exists;
                                       // then all run_groups have completed
      current_group_id = lowest_active_run_group_id;
      current_cycle_id =
          (current_cycle_id + 1) % 64; // As all run_groups have completed; incrment the current_cycle_id
    } else {
      current_group_id = next_run_group_id;
    }
  }
  count_active_group_tasks = 0;        // reset count of still active tasks in this run_group
  lowest_active_run_group_id = 0xFF;   // reset lowest_active_run_group_id
  next_run_group_id = 0xFF;            // reset next_run_group_id
  /*
    Serial.print("Finish: ");
    //Serial.print("\t Complete: ");
    Serial.print(run_group_info->task_active);
    Serial.print("\t count_active_group_tasks: ");
    Serial.print(count_active_group_tasks);
    Serial.print("\t next_run_group_id: 0x");
    Serial.print(next_run_group_id, HEX);
    Serial.print("\t lowest_active_run_group_id: 0x");
    Serial.print(lowest_active_run_group_id, HEX);
    Serial.print("\t current_group_id: "); Serial.print(current_group_id);
    Serial.print("\t current_cycle_id: "); Serial.print(current_cycle_id);
    Serial.println();
  */
}
