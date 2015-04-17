#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	cores = argn[0];
	for(int i=0; i < cores; i++){
		queue<int> q;
		vector<int> v;
		cola_de_cpu[i] = q;
		execution_time[i] = v;
	}
}

SchedRR2::~SchedRR2() {
	/*for(unsigned int i=0; i < cola_de_cpu.size(); i++){
		delete cola_de_cpu[i];
		delete execution_time[i];
	}*/
}


void SchedRR2::load(int pid) {
	
}

void SchedRR2::unblock(int pid) {
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
}

int SchedRR2::next(int cpu) {
	int result = cola_de_cpu[cpu].front();
	cola_de_cpu[cpu].pop();
	return result;
}
