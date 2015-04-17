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
		execution_time[i] = argn[i];
		// HAY QUE AGREGAR LOS PROCESOS ACA?
	}
}

SchedRR::~SchedRR() {
}


void SchedRR::load(int pid) {
	process.push(pid);
}

void SchedRR::unblock(int pid) {
	if(execution_time[pid] > 0){
		process.push(pid);
	}else{
		execution_time[pid] = 0;	
	}
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	if (m == EXIT) {
		// Si el pid actual terminó, sigue el próximo.
		if (process.empty()) return IDLE_TASK;
		else {
			int sig = process.front(); 
			return sig;
		}
	} else {
		if (current_pid(cpu) == IDLE_TASK && !process.empty()) {
			int sig = process.front(); 
			return sig;
		} else if(!process.empty()){
					execution_time[current_pid(cpu)]--;
					if(execution_time[current_pid(cpu)] > 0){

					}		
				}
		// SI SE TERMINO EL QUANTUM?
	}
}

int SchedRR::next(int cpu) {
	// HAY UN SOLO VECTOR GLOBAL DE PROCESOS PARA TODAS LAS CPU.
	int result = process.front();
	process.pop();
	return result;

}
