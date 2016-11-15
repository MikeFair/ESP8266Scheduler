#include "Scheduler.h"

extern "C" {
    #include "cont.h"

    void yield();
}

SchedulerClass Scheduler;

SchedulerTask SchedulerClass::main;
Task *SchedulerClass::current = &SchedulerClass::main;

SchedulerClass::SchedulerClass() {
	main.next = &main;
	main.prev = &main;
}

void SchedulerClass::start(Task *task) {
    task->next = &main;
    task->prev = main.prev;

    main.prev->next = task;
    main.prev = task;
}

bool SchedulerClass::can_schedule(Task* t) {
	// Check Delay: if waiting/delayed
	if(Delay::isDelayed(&t->delay_info)) return false;

	// Check RunGroups: if run_group_active and task_active status  
	if(!(t->run_group_info.run_group_active && t->run_group_info.task_active)) return false;
	
	//Serial.println("can_schedule == true");
	return true;
}

void SchedulerClass::begin() {
    while (1) {
		//Serial.println("Updating Delay");
		Delay::updateDelay(&current->delay_info);

		//Serial.println("Updating RunGroups Task");
		RunGroups::updateTask(&current->run_group_info);
		
		//Serial.println("Testing can_schedule");
		if(can_schedule(current)) 
            cont_run(&current->context, task_tramponline);
		
		//Serial.println("Updating RunGroup Stats");
		RunGroups::updateGroupStats(&current->run_group_info);
		
		//Serial.println("Yielding...");
        yield();
		
		//Serial.println("Updating current to the next Task in the list");
        current = current->next;		
    }
}

void task_tramponline() {
    SchedulerClass::current->loopWrapper();
}
