#ifndef __BASETASK_H__
#define __BASETASK_H__

#include <vector>
#include <map>
#include <utility>
#include <string>

typedef void (TaskBase)(int, std::vector<int> params);

/* Funciones que llaman las tareas para simular uso */

void uso_CPU(int pid, unsigned int ms);
void uso_IO(int pid, unsigned int ms);

/* Factory (ignorar) */
#define register_task(tipo, nprms) { task_defs[#tipo] = ptski(tipo, nprms); }
typedef std::pair<TaskBase*, int> ptski;
extern std::map<std::string, ptski> task_defs;

struct ptsk {
	TaskBase* tsk; std::vector<int> prms; unsigned int start; char type;
	// Agregados para poder implementar el paper de Liu 73
	unsigned int period; unsigned int cputime;
	ptsk(TaskBase* vtsk, const std::vector<int>& vprms, unsigned int vstart, char vtype);
	ptsk(TaskBase* vtsk, const std::vector<int>& vprms, unsigned int vstart, char vtype, unsigned int vperiod, unsigned int vcputime);
	ptsk(void);
};
std::vector<ptsk> tasks_load(const char* filename);
#endif
