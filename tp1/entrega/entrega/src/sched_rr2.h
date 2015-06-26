#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <utility> 
#include "basesched.h"

class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        ~SchedRR2();
		virtual void initialize();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		virtual void loadOnCore(int pid, int core);
		virtual void unload(int pid);
		int next(int core);
		int cores;
		
		// afinidad de procesos con cores (en este caso es definitiva con uno)
		std::map<int, int> affinity;
		
		// Cantidad de procesos activos por core
		std::map<int, int> active_processes;
		
		// Cola de procesos por core
		std::vector< std::queue<int> > process_queue_per_core;
		
		// Quantum definido para cada core.
		std::vector<int> cpu_quantum;
		
		// El quantum que queda por core. Si no hay nada corriendo (IDLE)
		// el valor está en 0
		std::map<int, TaskQuantumPair> remaining_q_per_core;
		
		int selectCore();
};

#endif
