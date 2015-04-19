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


void SchedRR::load(int pid) {
	process.push(make_pair<int, int>(0, pid));
}

void SchedRR::unblock(int pid) {
	if(process.front().first > 0){
		process.push(make_pair<int, int>(process.front().first, pid));
	}else{
		process.pop();	
	}
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	int sig;
	if (m == EXIT) {
		// Si el pid actual terminó, sigue el próximo.
		if (process.empty()){
			int sig = IDLE_TASK;
		} else {
			// saco el primero de la lista porque ya termino y sigue el siguiente de la lista
			process.pop();
			int sig = process.front().second; 
		}
	} else if (m==TICK) {
		// Le resto un tick al proceso actual
		process.front().first--;
		if ( !process.empty()) {
			if(process.front().first > 0){
				int sig = process.front().second;
			}else {
				// si se le acabo el quantum lo pongo al final de la cola
				// y cuando esta por ejecutarse de vuelta le asigno el quantum segun el cpu al que va a entrar
				process.push(make_pair<int, int>(0, process.front().second));  // no tendria que usar current_pid(cpu)?
				process.pop();
			}
		} else {
			int sig = IDLE_TASK;		// puede llegar aca?	
		}			
	} else {  // BLOQ
		if ( !process.empty() ){
			process.pop();
			int sig = process.front().second;
		}else {
			int sig = IDLE_TASK;
		}
	}
	
	return sig;
}

int SchedRR::next(int cpu) {
	// HAY UN SOLO VECTOR GLOBAL DE PROCESOS PARA TODAS LAS CPU.
	int pid_sig = process.front().second;
	process.front().first = cpu_quantum[cpu];
	return pid_sig;
}
