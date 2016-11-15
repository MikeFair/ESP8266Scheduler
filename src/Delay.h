
#ifndef DELAY_H
#define DELAY_H

// Every Task gets one of these structs
struct DelayInfo {
  uint32_t delay_start = 0;
  uint32_t delay_ms = 0;
};

class Delay {
public:
  static void startDelay(DelayInfo *delay_info, uint32_t ms) {
    if (ms) {
      delay_info->delay_start = millis();
      delay_info->delay_ms = ms;
    }

    yield(); // Do not return to the calling Task; move to the next Task
  }

  static bool isDelayed(DelayInfo *delay_info) {
    return (delay_info->delay_ms != 0);
  }

  static void updateDelay(DelayInfo *delay_info) {
    if (delay_info->delay_ms == 0)
      return; // Optimize for the non-delayed case

    // This comparison is "rollover safe"
    uint32_t now = millis();
    if ((now - delay_info->delay_start) >= delay_info->delay_ms)
      delay_info->delay_ms = 0;
  }
};
/*
class Task {
        // ...
        DelayInfo delay_info;
        // ...

        void delay(uint32_t ms) {
                Delay::delay(&delay_info, ms);
        }

        void loop() {
                // ...
                delay(1000);
                // ...
        }
}
class Scheduler {
        bool can_schedule(Task* t) {
                // ...
                if (Delay::is_delayed(&t->delay_info) return false;
                // ...
                return true;
        }

        void begin() {
                // ...

                // Pump the DelayInfo
                Delay::updateDelayInfo(&current->delay_info);

                if (can_schedule(current))
                        cont_run(&current->context, task_trampoline);

                // ...
        }
}
*/

#endif
