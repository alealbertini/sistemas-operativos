#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <utility> 
#include "basesched.h"

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		virtual void initialize();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		virtual void loadOnCore(int pid, int core);
		int next();
		int cores;
		std::queue<int> process;
		// Quantum definido para cada core.
		std::vector<int> cpu_quantum;
		// El quantum que queda por core. Si no hay nada corriendo (IDLE)
		// el valor está en 0
		std::map<int, TaskQuantumPair> remaining_q_per_core;
};

#endif
