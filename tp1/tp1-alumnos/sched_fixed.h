#ifndef __SCHED_FIXED_
#define __SCHED_FIXED_

#include <vector>
#include <queue>
#include <algorithm>
#include "basesched.h"

using namespace std;

class SchedFixed : public SchedBase {
	public:
		SchedFixed(std::vector<int> argn);
        	~SchedFixed();
		virtual void initialize();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		//std::queue<int> q;
		std::priority_queue< pair<int, int>,
				     vector<pair<int, int> >,
				     greater<pair<int, int> > 
				   > *listos;
};

#endif
