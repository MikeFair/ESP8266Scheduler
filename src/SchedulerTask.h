#include <Arduino.h>
#include "RunGroups.h"
#include "Delay.h"

// SchedulerTask
class SchedulerTask : public Task {

protected:
  virtual bool loop() {
    RunGroups::updateCycleId();
    return true;
  }
};
// End SchedulerTask
