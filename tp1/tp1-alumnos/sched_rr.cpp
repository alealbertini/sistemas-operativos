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
	process.push_back(pid);
}

void SchedRR::unblock(int pid) {
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
		// Siempre sigue el pid actual mientras no termine.
		/*if (current_pid(cpu) == IDLE_TASK && !process.empty()) {
			int sig = process.front(); 
			return sig;
		} else {
			return current_pid(cpu);
		}*/
		// HABRIA QUE MIRAR SI LE QUEDA TIEMPO DE EJECUCION AL PROCESO O SI SE TERMINO EL QUANTUM?
	}
}

int SchedRR::next(int cpu) {
	// HAY UN SOLO VECTOR PARA TODAS LAS CPU.
	process.front();
}
