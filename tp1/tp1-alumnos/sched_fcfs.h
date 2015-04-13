#ifndef __SCHED_FCFS__
#define __SCHED_FCFS__

#include <vector>
#include <queue>
#include "basesched.h"

using namespace std;

class SchedFCFS : public SchedBase {
	public:
		SchedFCFS(std::vector<int> argn);
        ~SchedFCFS();
		virtual void initialize() {};
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		int cores;
		std::queue<int> q;
		vector<int> execution_time;
};

#endif
