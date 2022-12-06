/*
 * scheduler.c
 *
 *  Created on: Nov 30, 2022
 *      Author: 84336
 */
#include "scheduler.h"

sTasks SCH_tasks_G[SCH_MAX_TASKS];

void SCH_Init (void) {
	unsigned char i;
	for (i = 0; i < SCH_MAX_TASKS; i++) {
		SCH_Delete_Task(i);
	}
}

void SCH_Add_Task (void (*pFunction)(),
						  uint32_t DELAY,
						  uint32_t PERIOD,
						  uint32_t OneShot)
		{
			unsigned char index = 0;
			while ((SCH_tasks_G[index].pTask != 0) && (index < SCH_MAX_TASKS)) {
				index++;
			}
			if (index == SCH_MAX_TASKS) {
				return SCH_MAX_TASKS;
			}
			SCH_tasks_G[index].pTask = pFunction;
			SCH_tasks_G[index].Delay = DELAY;
			SCH_tasks_G[index].Period = PERIOD;
			SCH_tasks_G[index].RunMe = 0;
			SCH_tasks_G[index].OneShot = 0;
		}

void SCH_Update (void) {
	unsigned char index;
	for (index = 0; index < SCH_MAX_TASKS; index++) {
		if (SCH_tasks_G[index].pTask) {
			if (SCH_tasks_G[index].Delay == 0) {
				SCH_tasks_G[index].RunMe += 1;
				if (SCH_tasks_G[index].Period) {
					SCH_tasks_G[index].Delay = SCH_tasks_G[index].Period;
				}
			}
			else {
				SCH_tasks_G[index].Delay -= 1;
			}
		}
	}
}

void SCH_Dispatch_Tasks (void) {
	unsigned char index;
	for (index = 0; index < SCH_MAX_TASKS; index++) {
		if (SCH_tasks_G[index].RunMe > 0) {
			(*SCH_tasks_G[index].pTask)();
			SCH_tasks_G[index].RunMe--;
			if (SCH_tasks_G[index].Period == 0) {
				SCH_Delete_Task(index);
			}
		}
	}
}

void SCH_Delete_Task (unsigned char index) {
	SCH_tasks_G[index].pTask = 0x0000;
	SCH_tasks_G[index].Delay = 0;
	SCH_tasks_G[index].Period = 0;
	SCH_tasks_G[index].RunMe = 0;
	SCH_tasks_G[index].OneShot = 0;
}

