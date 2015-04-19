#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	cores = argn[0];
	for(int i=1; i < (int)argn.size(); i++){
		cpu_quantum[i] = argn[i];
	}
}

SchedRR::~SchedRR() {
}

void initialize() {
	for(int core=1; core<=cores; core++){
		remaining_q_per_core[core] = TaskQuantumPair(IDLE_TASK, 0);
	} 
}


void SchedRR::load(int pid) {
	process.push(pid);
}

void SchedRR::unblock(int pid) {
	process.push(pid);
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	int sig;
	if(m == EXIT){
		sig = next();
		loadOnCore(sig, cpu);
	}
	else if(m == TICK){
		TaskQuantumPair runningTask = remaining_q_per_core[cpu];
		// Si no hay mas quantum, pasar a la siguiente tarea
		if(runningTask.second <= 0){
			sig = next();
			loadOnCore(sig, cpu);
			// la tarea actual pasa a Ready: al final de la cola.
			process.push(runningTask.first);
		}
		else {
			// La tarea sigue corriendo
			runningTask.second -= 1;
			sig = runningTask.first;
		}
	}
	else {  // BLOQ
		sig = next();
		loadOnCore(sig, cpu);
	}
	
	return sig;
}

void loadOnCore(int pid, int core){
	remaining_q_per_core[core].first = pid;
	
	// resolver el quantum de la tarea (0 si es IDLE)
	if(pid == IDLE_TASK){
		remaining_q_per_core[core].second = 0;
	}
	else {
		remaining_q_per_core[core].second = cpu_quantum[core];
	}
}

int SchedRR::next() {
	if(process.empty()){
		return IDLE_TASK;
	}
	return process.pop();
}
