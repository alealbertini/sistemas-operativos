#include "sched_dynamic.h"
#include <iostream>
#include <cstdlib>

using namespace std;

SchedDynamic::SchedDynamic(vector<int> argn) {
	listos = new priority_queue<pair<int,int>,
			    vector<pair<int, int> >,
			    greater<pair<int, int> > >();
}

SchedDynamic::~SchedDynamic() {
	delete listos;	
}

void SchedDynamic::initialize() {
	n_ticks = 0;
}

void SchedDynamic::load(int pid) {
	//Agregamos la tarea a la cola de prioridad de listos con su deadline.
	listos->push(make_pair<int, int>(n_ticks + period(pid), pid)); 
}

void SchedDynamic::unblock(int pid) {
	//Agregamos la tarea a la cola de prioridad de listos con su deadline.
	int deadline_pid = deadlines[pid];
	deadlines.erase(pid);
	listos->push(make_pair<int, int>(deadline_pid, pid)); 
}

//El tick solo lo calculamos para el cpu 0 porque asumimos que todos los cores son de la misma velocidad.
int SchedDynamic::tick(int cpu, const enum Motivo m) {
	if(cpu==0)
		n_ticks++;

	int sig_pid = -1;
	bool hay_listos = !listos->empty();
	int curr_pid = current_pid(cpu);
	if( m==EXIT ){
		if ( hay_listos ){
			sig_pid = listos->top().second;
			deadlines[sig_pid] = listos->top().first;
			listos->pop();
		} 
		if(deadlines.size()==0)
			exit(13);
		deadlines.erase(curr_pid);
	} else if ( m==TICK ){
		sig_pid = curr_pid;
		if ( hay_listos){
			int mejor_listo_pid = listos->top().second;
			int mejor_listo_deadline = listos->top().first;
			if( (curr_pid == IDLE_TASK) || deadlines[curr_pid] > mejor_listo_deadline){
				sig_pid = mejor_listo_pid;
				deadlines[sig_pid] = mejor_listo_deadline;
				listos->pop();
			}
		}
	} else {  //m==BLOCK
		if ( hay_listos){
			sig_pid = listos->top().second;
			deadlines[sig_pid] = listos->top().first;
			listos->pop();
		}	
	}

	return (sig_pid<0)? IDLE_TASK : sig_pid;
}
