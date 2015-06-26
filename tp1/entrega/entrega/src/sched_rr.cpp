#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	cores = argn[0];
	//cerr << "Cores: " << cores << endl;
	cpu_quantum.resize(cores);
	for(int i=1; i < (int)argn.size(); i++){
		//cerr << "Quantum: " << argn[i] << endl; 
		cpu_quantum[i-1] = argn[i];
	}
}

SchedRR::~SchedRR() {
	
}

void SchedRR::initialize() {
	for(int core=0; core<cores; core++){
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
	int core = cpu;
	if(m == EXIT){
		sig = next();
		loadOnCore(sig, core);
		//cerr << "EXIT. El proximo es: " << sig << endl;
	}
	else if(m == TICK){
		TaskQuantumPair runningTask = remaining_q_per_core[core];
		//cerr << "TICK. Esta corriendo: " << runningTask.first << ". Le quedan: " << runningTask.second << endl;
		// Si no hay mas quantum, pasar a la siguiente tarea
		if(runningTask.second <= 1){
			// la tarea actual pasa a Ready: al final de la cola.
			if(runningTask.first != IDLE_TASK){
				process.push(runningTask.first);
			}
			
			// paso a la siguiente tarea (puede ser la misma si no queda otra)
			sig = next();
			loadOnCore(sig, core);
			//cerr << "El proximo es: " << sig << endl;
		}
		else {
			// La tarea sigue corriendo
			runningTask.second -= 1;
			remaining_q_per_core[core] = runningTask;
			sig = runningTask.first;
			//cerr << "Sigue corriendo: " << sig << endl;
		}
	}
	else {  // BLOQ
		sig = next();
		loadOnCore(sig, core);
		
		//cerr << "BLOQ. El proximo es: " << sig << endl;
	}
	
	return sig;
}

void SchedRR::loadOnCore(int pid, int core){
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
	int next = process.front();
	process.pop();
	return next;
}
