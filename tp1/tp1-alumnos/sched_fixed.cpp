#include "sched_fixed.h"
#include <iostream>

using namespace std;

SchedFixed::SchedFixed(vector<int> argn) {
}

SchedFixed::~SchedFixed() {
}

void SchedFixed::initialize() {
}

void SchedFixed::load(int pid) {
	listos.push(make_pair<int, int>(period(pid), pid));
}

void SchedFixed::unblock(int pid) {
	listos.push(make_pair<int, int>(period(pid), pid));
}

int SchedFixed::tick(int cpu, const enum Motivo m) {
        //TICK, BLOQ, EXIT
	int sig_pid = -1;
	bool hay_listos = !listos.empty();
	if (m==EXIT) {
		if( hay_listos ){
			sig_pid = listos.top().second;
			listos.pop();
		}
	} else if (m==TICK) {
		int curr_pid = current_pid(cpu);
		sig_pid = curr_pid;
		if( hay_listos ){
			if ((curr_pid == IDLE_TASK) || (period(curr_pid) > listos.top().first)){
				sig_pid = listos.top().second;
				listos.pop();
				listos.push(make_pair<int, int>(period(curr_pid), curr_pid));
			}
		}
	} else { //BLOQ
		sig_pid = listos.top().second;
		listos.pop();
	}
	return (sig_pid<0)? IDLE_TASK : sig_pid;
}
