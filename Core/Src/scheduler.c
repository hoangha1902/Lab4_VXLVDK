#include <scheduler.h>
char str[50];
Scheduler_Task Array[MAX];
uint32_t cur_i = 0;
uint8_t ERROR_CODE_G = NORMAL;


void Scheduler_Init(){
	 cur_i = 0;
}

void Scheduler_Update(){
	for(int i = 0 ; i  < cur_i ; ++i){
		if(Array[i].Delay > 0){
			Array[i].Delay -= TICK;
		}
		else{
			Array[i].Runme += 1;
			if(Array[i].Period >0){
				Array[i].Delay = Array[i].Period;
			}
		}
	}
}

void Scheduler_Dispatch_Task(){
	for(int i = 0 ; i  < cur_i; ++i){
		if(Array[i].Runme >0){
			(*Array[i].pfunction)();
			/* CODE TO TEST SCHEDULER BEGIN*/
			sprintf(str, "Task %d has Dispatch at Tick %ld ms \r\n",order,counter);
			/*CODE TO TEST SCHEDULER END*/
			Array[i].Runme -= 1;
			if(Array[i].Period == 0){
				Scheduler_Delete_Task(Array[i].Task_ID);
			}
		}
	}
}

uint32_t Scheduler_Add_Task(void (*pfunction)(),uint32_t delay, uint32_t period){
	if(cur_i < MAX){
		Array[cur_i].pfunction = pfunction;
		Array[cur_i].Delay = delay;
		Array[cur_i].Period = period;
		Array[cur_i].Runme = 0;
		Array[cur_i].Task_ID = cur_i;
		cur_i++;
		return cur_i;
	}
	else{
		ERROR_CODE_G = ERROR_SCHEDULER_TOO_MANY_TASK;
		return 0;
	}
}

uint32_t Scheduler_Delete_Task(uint32_t task_ID){
	if(task_ID >= cur_i || cur_i == 0){
		ERROR_CODE_G = ERROR_NO_TASK_TO_DELETE ;
		return 0;
	}
	else{
			for(int i = task_ID ; i < cur_i - 1; ++i){
				Array[i].pfunction = Array[i + 1].pfunction;
				Array[i].Delay = Array[i + 1].Delay;
				Array[i].Period = Array[i + 1].Period;
				Array[i].Runme = Array[i + 1].Runme;
				Array[i].Task_ID = Array[i + 1].Task_ID;
			}
			Array[cur_i].pfunction =0x0000;
			Array[cur_i].Delay = 0;
			Array[cur_i].Period = 0;
			Array[cur_i].Runme = 0;
			Array[cur_i].Task_ID = 0;
			cur_i--;
		return cur_i;
	}
}
