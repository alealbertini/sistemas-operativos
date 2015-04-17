#include "tasks.h"
#include <iostream>
#include <cstdlib>
using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

void TaskConBloqueo(int pid, vector<int> params) {
	int tiempo = params[0];
	int inicioBloq = params[1];
	int finBloq = params[2];

	if(inicioBloq > tiempo){
		uso_CPU(pid, tiempo);
	}else{
		uso_CPU(pid,inicioBloq-2);
		uso_IO(pid,finBloq-inicioBloq+1);
		uso_CPU(pid,tiempo-finBloq);
	}
}

void TaskBatch(int pid, vector<int> params){	// params: total_cpu, cant_bloqueos
	int total_cpu = params[0];
	int cant_bloqueos = params[1];

	for(int i=0; i < cant_bloqueos; i++){
		
		int inicioBloq = rand();		// QUE PASA SI EL NUM ALEATORIO ES MENOR Q EL TIEMPO ACTUAL? O SEA, ES MENOR QUE LO QUE LLEVAMOS ACUMULADO

		if(inicioBloq > total_cpu){
			uso_CPU(pid, total_cpu);		// SI EL NUM ALEATORIO ES > Q EL TIEMPO TOTAL, EJECUTO SIEMPRE LA CPU
		}else{
			uso_CPU(pid, inicioBloq-1);    // SINO EJECUTO HASTA EL INICIO DEL BLOQUEO-1 Y LUEGO EJECUTO EL BLOQUEO DURANTE 1 MILISEGUNDO 
			uso_IO(pid, 1);
		}
	}
}

void TaskConsola(int pid, vector<int> params) {  // params: n, bmin, bmax
	for(int i = 0; i < params[0]; i++){
		int numAleatorio = (rand() % params[2]) + params[1]; // genera un numero aleatorio entre bmin y bmax
		uso_IO(pid, numAleatorio); // Uso IO "numAleatorio" milisegundos.
	}
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConBloqueo,3);
	register_task(TaskConsola,3);
}

