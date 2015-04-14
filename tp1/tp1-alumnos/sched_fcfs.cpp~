#include "sched_fcfs.h"

using namespace std;

SchedFCFS::SchedFCFS(vector<int> argn) {
	// FCFS recibe la cantidad de cores.
	cores = argn[0];
	for(int i = 1; i < (int)argn.size(); i++){
		execution_time.push_back(argn[i]);
	}
	// HABRIA QUE AGREGAR LOS PID A LA COLA Q, DE DONDE SACO LOS PID?
}

SchedFCFS::~SchedFCFS() {
}

void SchedFCFS::load(int pid) {
	q.push(pid); // llegó una tarea nueva
	// HABRIA QUE AGREGAR EN EXECUTION_TIME EL TIEMPO DE EJECUCION DE ESA TAREA?
}

void SchedFCFS::unblock(int pid) {
	// Uy! unblock!... bueno, ya seguir'a en el próximo tick
	// SI EL PID Q SE DESBLOQUEA LE QUEDA TIEMPO, ENTONCES AGREGARLO A LA COLA 
	// Y MODIFICAR EL TIEMPO RESTANTE Y SINO PONER Q LE QUEDA TIEMPO 0 EN EXECUTION_TIME

int SchedFCFS::tick(int cpu, const enum Motivo m) {
	if (m == EXIT) {
		// Si el pid actual terminó, sigue el próximo.
		if (q.empty()) return IDLE_TASK;
		else {
			int sig = q.front(); q.pop();
			return sig;
		}
	} else {
		// Siempre sigue el pid actual mientras no termine.
		if (current_pid(cpu) == IDLE_TASK && !q.empty()) {
			int sig = q.front(); q.pop();
			return sig;
		} else {
			return current_pid(cpu);
		}
	}
}
