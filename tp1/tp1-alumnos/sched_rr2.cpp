#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>
#include <climits>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	cores = argn[0];
	cerr << "Cores: " << cores << endl;
	cpu_quantum.resize(cores);
	process_queue_per_core.resize(cores);
	for(int i=1; i < (int)argn.size(); i++){
		cerr << "Quantum: " << argn[i] << endl; 
		cpu_quantum[i-1] = argn[i];
	}
}

void SchedRR2::initialize() {
	for(int core=0; core<cores; core++){
		active_processes[core] = 0;
		remaining_q_per_core[core] = TaskQuantumPair(IDLE_TASK, 0);
	} 
}

SchedRR2::~SchedRR2() {
	
}

// Selecciona el core con menos carga
int SchedRR2::selectCore() {
	int minProcesses = INT_MAX;
	int selectedCore = 0;
	
	for(int core=0; core<cores; core++){
		int coreProcesses = active_processes[core];
		if(coreProcesses < minProcesses){
			minProcesses = coreProcesses;
			selectedCore = core;
		}
	}
	
	return selectedCore;
}

void SchedRR2::load(int pid) {
	int core = selectCore();
	process_queue_per_core[core].push(pid);
	affinity[pid] = core;
	active_processes[core] = active_processes[core] + 1;
}

void SchedRR2::unload(int pid) {
	// IDLE_TASK puede correr en más de un core a la vez y no se incluye en las estructuras
	if(pid != IDLE_TASK){
		int core = affinity[pid];
		// remuevo afinidad
		map<int,int>::iterator iter = affinity.find(1) ;
		affinity.erase(iter);
		
		// decremento la cantidad de procesos activos del core
		active_processes[core] = active_processes[core] - 1;
	}
}

void SchedRR2::unblock(int pid) {
	int core = affinity[pid];
	
	process_queue_per_core[core].push(pid);
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
	int sig;
	int core = cpu;
	TaskQuantumPair runningTask = remaining_q_per_core[core];
	if(m == EXIT){
		// el proceso actual terminó. Lo remuevo de las estructuras
		unload(runningTask.first);
		
		// cargo el siguiente proceso
		sig = next(core);
		loadOnCore(sig, core);
		cerr << "EXIT en core " << core << ". El proximo es: " << sig << endl;
	}
	else if(m == TICK){
		cerr << "TICK en core: " << core << ". Esta corriendo: " << runningTask.first << ". Le quedan: " << runningTask.second << endl;
		// Si no hay mas quantum, pasar a la siguiente tarea
		if(runningTask.second <= 1){
			// la tarea actual pasa a Ready: al final de la cola.
			if(runningTask.first != IDLE_TASK){
				process_queue_per_core[core].push(runningTask.first);
			}
			
			// paso a la siguiente tarea (puede ser la misma si no queda otra)
			sig = next(core);
			loadOnCore(sig, core);
			
			cerr << "El proximo en core: " << core << " es: " << sig << endl;
		}
		else {
			// La tarea sigue corriendo
			runningTask.second -= 1;
			remaining_q_per_core[core] = runningTask;
			sig = runningTask.first;
			cerr << "Core: " << core << ". Sigue corriendo: " << sig << endl;
		}
	}
	else {  // BLOQ
		sig = next(core);
		loadOnCore(sig, core);
		
		cerr << "Core: " << core << ". BLOQ. El proximo es: " << sig << endl;
	}
	
	return sig;
}

void SchedRR2::loadOnCore(int pid, int core){
	remaining_q_per_core[core].first = pid;
	
	// resolver el quantum de la tarea (0 si es IDLE)
	if(pid == IDLE_TASK){
		remaining_q_per_core[core].second = 0;
	}
	else {
		remaining_q_per_core[core].second = cpu_quantum[core];
	}
}

int SchedRR2::next(int core) {
	if(process_queue_per_core[core].empty()){
		return IDLE_TASK;
	}
	int next = process_queue_per_core[core].front();
	process_queue_per_core[core].pop();
	return next;
}
