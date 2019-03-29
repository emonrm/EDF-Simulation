#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>


using namespace std;
bool DEBUG;
class task_set
{
public:
	int p[10]; //periods in milli-seconds
	double c[10]; // worst-case execution times in micro-seconds
	//double e_c[10];
	long hyper_period;
};


bool preemptive_edf_schedule(task_set set, int n, int c_time = 0, int w = 1)
{
	//cout << endl;
	long hyper_period = set.hyper_period * 1000; //microseconds
	//cout << "hyper_period: " <<hyper_period << endl;

	bool active_jobs[10];
	double remaining_execution[10];
	for(int i = 0; i < n; i++)
	{
		active_jobs[i] = false;
		remaining_execution[i]  = 0;
	}
	int current_task = -1; // task index of highest priority job
	int previous_task = 0;
	long last_empty_slot = -1;
	int halt_execution = 0;
	
	long time_elapsed;

	for(time_elapsed = 1; time_elapsed <= hyper_period; time_elapsed++)
	{
		//find job released
		
		for(int i = 0; i < n; i++)
		{
			if(((time_elapsed - 1) % (set.p[i] * 1000)) == 0) // new job has been released of a task
			{
				//cout << "at t= " << time_elapsed << " : " ;
				//cout << "J(" << i + 1 << "," << (time_elapsed / (set.p[i] * 1000)) +1 <<") " << endl;

				//check if the previous job still needs time
				if(remaining_execution[i] != 0)
				{
					if(DEBUG) cout << "preemptive: J(" << i + 1 << "," <<  (time_elapsed / (set.p[i] * 1000)) +1 << ") has missed deadline at t= " << time_elapsed - 1 << " us, exe_remain: " << remaining_execution[i] << " us" << endl;
					return false;
				}

				// activate the job, set the execution time
				active_jobs[i] = true;
				remaining_execution[i] = set.c[i] * w;
				//cout << "Acitve job: " << set.p[i]*1000 << " Ex_remaining: " << remaining_execution[i] << endl;
			}
		}
		

		// check for highest priority
		current_task = -1;
		for(int i = 0; i < n; i++)
		{
			if(active_jobs[i])
			{
				current_task = i;
				/*if ((current_task != previous_task) && (remaining_execution[previous_task] != 0))
				{
					cout << "at t= " << time_elapsed-1 << ",  J(" << previous_task+1 << "," << (time_elapsed / (set.p[previous_task] * 1000)) +1 << ") with remaining_execution: " <<remaining_execution[previous_task]  <<  " of us, has been preempted by J(" << current_task+1 << "," << (time_elapsed / (set.p[current_task] * 1000)) +1 << ") !" << endl;
				}*/
				//cout << "at t = " << time_elapsed << " active job: " << "J(" << i+1 <<"," << (time_elapsed / (set.p[i] * 1000)) +1 << ")" << endl;
				break;
			}
		}

		if (current_task == -1) // no jobs to execute
		{
			if(last_empty_slot == -1)
			{
				last_empty_slot = time_elapsed - 1;
				//cout << "last_empty_slot: " << last_empty_slot << endl;
			}
			continue;
		}

		// check for the context switch time
		if(previous_task != current_task)
		{
			if(last_empty_slot == -1)
			{
				halt_execution = c_time;
			}
			else if(time_elapsed - 1 - last_empty_slot < c_time)
			{
				halt_execution = time_elapsed - 1 - last_empty_slot;
			}
			
		}

		if(halt_execution == 0)
		{
			remaining_execution[current_task]--; //execute one time unit
			last_empty_slot = -1;
			if(remaining_execution[current_task] == 0) //inactivate job if all execution done
			{
				active_jobs[current_task] = false;
				remaining_execution[current_task] = 0;
				//cout << "at t = " << time_elapsed  << ", J(" << current_task+1 <<"," << (time_elapsed / (set.p[current_task] * 1000)) +1 << ") finished!" <<endl;
				//time_elapsed += c_time; // add switch time since starting a new job
			}
		}
		
		

		//check at any particular time what jobs are active
		/*if (time_elapsed == 1)
		{
			cout << "at t = " << time_elapsed << endl;
			for(int i = 0; i < n; i++)
			{
				if(active_jobs[i]) cout << "J(" << i+1 <<"," << (time_elapsed / (set.p[i] * 1000)) +1 << ") , remaining_execution: " << remaining_execution[i] << endl;
			}
		}*/
		if(halt_execution > 0) halt_execution--;
		previous_task = current_task;


	}

	return true;
}

bool non_preemptive_edf_schedule(task_set set, int n, int c_time = 0, int w = 1)
{
	//cout << endl;
	long hyper_period = set.hyper_period * 1000; //microseconds
	//cout << "hyper_period: " <<hyper_period << endl;

	bool active_jobs[10];
	double remaining_execution[10];
	for(int i = 0; i < n; i++)
	{
		active_jobs[i] = false;
		remaining_execution[i]  = 0;
	}
	int current_task = -1; // task index of highest priority job
	int previous_task = 0;
	long last_empty_slot = -1;
	int halt_execution = 0;
	
	long time_elapsed;

	for(time_elapsed = 1; time_elapsed <= hyper_period; time_elapsed++)
	{
		//find job released
		
		for(int i = 0; i < n; i++)
		{
			if(((time_elapsed - 1) % (set.p[i] * 1000)) == 0) // new job has been released of a task
			{
				//cout << "at t= " << time_elapsed << " : " ;
				//cout << "J(" << i + 1 << "," << (time_elapsed / (set.p[i] * 1000)) +1 <<") " << endl;

				//check if the previous job still needs time
				if(remaining_execution[i] != 0)
				{
					if(DEBUG) cout << "non-preemptive: J(" << i + 1 << "," <<  (time_elapsed / (set.p[i] * 1000)) +1 << ") has missed deadline at t= " << time_elapsed - 1 << " us, exe_remain: " << remaining_execution[i] << " us" << endl;
					return false;
				}

				// activate the job, set the execution time
				active_jobs[i] = true;
				remaining_execution[i] = set.c[i] * w;
				//cout << "Acitve job: " << set.p[i]*1000 << " Ex_remaining: " << remaining_execution[i] << endl;
			}
		}
		

		// check if the current task is still running
		if(remaining_execution[previous_task] != 0)
		{
			current_task = previous_task;
		}
		else
		{
			// previous task finished, check for highest priority task now
			current_task = -1;
			for(int i = 0; i < n; i++)
			{
				if(active_jobs[i])
				{
					current_task = i;
				
					//cout << "at t = " << time_elapsed << " active job: " << "J(" << i+1 <<"," << (time_elapsed / (set.p[i] * 1000)) +1 << ")" << endl;
					break;
				}
			}
		}
		

		if (current_task == -1) // no jobs to execute
		{
			if(last_empty_slot == -1)
			{
				last_empty_slot = time_elapsed - 1;
				//cout << "last_empty_slot: " << last_empty_slot << endl;
			}
			continue;
		}

		// check for the context switch time
		if(previous_task != current_task)
		{
			if(last_empty_slot == -1)
			{
				halt_execution = c_time;
			}
			else if(time_elapsed - 1 - last_empty_slot < c_time)
			{
				halt_execution = time_elapsed - 1 - last_empty_slot;
			}
			
		}

		if(halt_execution == 0)
		{
			remaining_execution[current_task]--; //execute one time unit
			last_empty_slot = -1;
			if(remaining_execution[current_task] == 0) //inactivate job if all execution done
			{
				active_jobs[current_task] = false;
				remaining_execution[current_task] = 0;
				//cout << "at t = " << time_elapsed  << ", J(" << current_task+1 <<"," << (time_elapsed / (set.p[current_task] * 1000)) +1 << ") finished!" <<endl;
			}
		}
		
		

		//check at any particular time what jobs are active
		/*if (time_elapsed == 1)
		{
			cout << "at t = " << time_elapsed << endl;
			for(int i = 0; i < n; i++)
			{
				if(active_jobs[i]) cout << "J(" << i+1 <<"," << (time_elapsed / (set.p[i] * 1000)) +1 << ") , remaining_execution: " << remaining_execution[i] << endl;
			}
		}*/
		if(halt_execution > 0) halt_execution--;
		previous_task = current_task;


	}

	return true;
}

int gcd(int a, int b) 
{
  if (b == 0) return a;
  return gcd(b, a%b);
}

int lcm(int a [], int n) 
{
  int res = 1;
  for (int i = 0; i < n; i++) 
  {
    res = (res*a[i])/gcd(res, a[i]);
  }
  return res;
}


double cal_utilization(task_set set, int n, double w)
{
	double utilization = 0;
	for (int i = 0; i < n; i++)
	{
		double temp_uti = (w * set.c[i]) / (set.p[i] * 1000);
		utilization += temp_uti;
	}

	if (utilization > 1) return -100;
	return utilization;
}

bool utilizaiton_condtion(task_set set, int n, int w = 1)
{

	double utilization  = 0;
	for (int i = 0; i < n; i++)
	{
		double temp_uti = (w * set.c[i]) / (set.p[i] * 1000);
		utilization += temp_uti;
	}
	return (utilization <=1)? true : false;
	//return utilization;

}

//jeffay condition
bool non_preemptive_condition(task_set set, int n)
{

	int p_1 = set.p[0] * 1000;
	//cout << "P_1 : " << p_1 << endl;
	for(int i = 1; i < n; i++)
	{
		int p_i = set.p[i] * 1000;
		double c_i = set.c[i];

		//cout << "p_i: " << p_i << " c_i: " << c_i << endl; 
		//int L = p_1 + (p_i - p_1);
		for(int x = p_1 +1; x < p_i; x++)
		{
			double temp_cost = c_i;
			//cout << "Temp cost: " <<temp_cost << endl;
			for(int j = 0; j < i-1; j++)
			{
				temp_cost += (floor(((x - 1) * 1.0)/ (set.p[j] * 1000)) * set.c[j]);
			}

			if(x < temp_cost)
			{
				cout << "Failed Jeffay test : L = " << x << " cost = " << temp_cost << endl; 
				return false;
			}
		}
	}
	return true;
}

void quick_sort(task_set & arr, int left, int right) 
{
      int i = left, j = right;
      int pivot = arr.p[left];
 
      while (i <= j) 
      {
            while (arr.p[i] < pivot)
                  i++;
            while (arr.p[j] > pivot)
                  j--;
            if (i <= j) {
            	int tmp, tmc;
                tmp = arr.p[i]; tmc = arr.c[i];
                arr.p[i] = arr.p[j]; arr.c[i] = arr.c[j];
                arr.p[j] = tmp; arr.c[j] = tmc;
                i++;
                j--;
            }
      };
 
      if (left < j)
            quick_sort(arr, left, j);
      if (i < right)
            quick_sort(arr, i, right);
}

int main(int argc, char* argv[])
{
	if(argc > 2)
	{
		cout << endl;
		cout << "Usage: g++ hw3_q1.cpp -o out && ./out arg1" << endl;
		cout << "arg1: true or false  (DEBUG activate or deactivate)" << endl;
		cout << endl;
		exit(1);
	}

	if(argc ==2)
	{
		string str(argv[1]);
		if(str == "true"){DEBUG = true;}
		else{DEBUG = false;}
	}
	else{
		DEBUG = false;
	}
	
	

	srand(time(NULL));
	//task tasks [25][10]; // 25 task sets each having 10 periodic tasks
	

	
	int context_switch_time[5] = {6, 206, 406, 806, 1006};
	task_set task_system[25]; 
	int no_of_task_set = 0;

	do{
		task_set candidate_set;
		for (int j = 0; j < 10; j++)
		{
			int period  = rand() % 8 + 3; // generate random periods between 3 - 10 milli-seconds
			int exe_time = rand() % 501 + 1; // execution time  between 1 - 500 micro seconds
			if(exe_time <= 0) exe_time = 1;
			candidate_set.p[j]= period;
			candidate_set.c[j] = exe_time;
		}
	
		quick_sort(candidate_set, 0, 9);

		

		if (utilizaiton_condtion(candidate_set, 10) && non_preemptive_condition(candidate_set, 10))
		{
			candidate_set.hyper_period = lcm(candidate_set.p, 10);
			task_system[no_of_task_set] = candidate_set;
			no_of_task_set++;

			/*for(int j=0; j < 10; j++)
			{
				cout << "T"<< j+1 << ": " <<candidate_set.p[j] << " " << candidate_set.c[j] / 1000 << endl;
			}
			cout << endl;*/
		}	

	}while(no_of_task_set < 25);

/*
 * ***************************************************Context switch time***********************************
 */

	for(int c=0; c < 5; c++)
	{
		cout << "================ context switch time: " << context_switch_time[c] <<" us =======================" << endl;
		if(!DEBUG) cout << "Scheduling tasks ..." << endl;
		int preemptive_w = 0;
		int non_preemptive_w = 0;

		double preemptive_uti = 0;
		double non_preemptive_uti = 0;

		for(int i = 0; i < no_of_task_set; i++)
		{

			//task system
			if(DEBUG){
				cout << "Tasks (p_i, c_i), (time in ms):  ";
				for(int l = 0; l < 10; l++)
				{
					cout << "(" << task_system[i].p[l] << "," << task_system[i].c[l] / 1000 << ") ";
				}
				cout << endl;
			} 

			//preemptive test
			int j;
			for(j = 2;; j++)
			{
				if(!preemptive_edf_schedule(task_system[i], 10, context_switch_time[c], j)) // context switch time 6
				{
					preemptive_w += (j -1);
					preemptive_uti += cal_utilization(task_system[i], 10, j-1);
					break;
				}
			}
			
			//non-preemptive test
			int k;
			for(k = 2;; k++)
			{
				if(!non_preemptive_edf_schedule(task_system[i], 10, context_switch_time[c], k)) // context switch time 6
				{
					non_preemptive_w += (k -1);
					non_preemptive_uti += cal_utilization(task_system[i], 10, k-1);
					break;
				}
			}

			if(DEBUG) cout << "Task System: " << i+1 << ", preemptive_breakdown_w: " << j-1 << ", non-preemptive_breakdown_w: " << k-1 << endl;
			if(DEBUG) cout << "--------------------------------------------------------------------"<< endl;
		}

		cout << "preemptive_w_average: " << preemptive_w / (no_of_task_set * 1.0) << endl;
		cout << "non-preemptive_w_average: " << non_preemptive_w / (no_of_task_set * 1.0) << endl;

		cout << "preemptive_avg_breakdown_uti: " << preemptive_uti / (no_of_task_set * 1.0) << endl;
		cout << "non-preemptive_avg_breakdown_uti:: " << non_preemptive_uti / (no_of_task_set * 1.0) << endl;

		cout << "For context switch time: " << context_switch_time[c] << " us" << endl;

		cout << "=====================================================================" << endl;
	}

		

	

	
	// test random task set


	return 0;
}