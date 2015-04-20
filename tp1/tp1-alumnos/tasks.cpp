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


// params: total_cpu, cant_bloqueos
void TaskBatch(int pid, vector<int> params){
        int total_cpu = params[0];
        int cant_bloqueos = params[1];

    //Inicializamos la semilla de la secuencia de numeros aleatorios.
        srand(time(NULL));

        //En este vector vamos a guardar los ticks en los que hay llamadas bloqueantes
        //ticks_bloq[i] == 1 => en el tick i se usa CPU y hay una llamada bloqueante.
        //ticks_bloq[i] == 0 => en el tick i solo se usa CPU
    vector<int> ticks_bloq;
        ticks_bloq.resize(total_cpu, 0);

        int ticks_disponibles = total_cpu;
        while(cant_bloqueos>0){
            //Sorteamos el tick bloqueante.
                int tick_elegido = (rand() % ticks_disponibles);

                //Nos posicionamos en el tick_elegido
                //buscamos el iesimo 0 salteando los 1
                int contador=0;
                for(int i=0; contador!=tick_elegido; i++)
                        if(ticks_bloq[i]==0)
                                        contador++;

                //Seteamos la llamada bloqueante.
                ticks_bloq[contador] = 1;

                //actualizamos, queda un tick menos y una llamada bloqueante menos.
                cant_bloqueos--;
                ticks_disponibles--;
        }


        //Juntamos todos el uso de CPU consecutivo, si encontramos una llamada bloqueante "flusheamos" el uso de CPU.
        int cpu_acumulado = 0;
        for(int i=0; i < total_cpu; i++){
                cpu_acumulado++;

                if(ticks_bloq[i]==1){
                        uso_CPU(pid, cpu_acumulado);
                        uso_IO(pid, 1);
                        cpu_acumulado = 0;
                }
        }
        //Si no hubo llamada bloqueante en el ultimo tick, hay que informar el uso de CPU restante.
        if(cpu_acumulado>0){
                uso_CPU(pid, cpu_acumulado);
        }

}
/*
void TaskBatch(int pid, vector<int> params){	// params: total_cpu, cant_bloqueos
	int total_cpu = params[0];
	int cant_bloqueos = params[1];
	int cant = cant_bloqueos;
	int cant_ticks = 0;

	for(int i=0; i < cant_bloqueos; i++){
		// inicioBloq es el numero de tick donde comienza a ejecutarse la llamada bloqueante
		int inicioBloq = (rand() % total_cpu) - cant;	
		// lo acomodo dentro del rango posible porque rand me puede tirar cualquier numero aleatorio

		uso_CPU(pid, inicioBloq - cant_ticks - 1);  
		// uso la cpu desde que finalizo la ultima llamada bloqueante hasta que aparezca la nueva llamada bloqueante   
		uso_IO(pid, 1);

		cant--;
		cant_ticks = inicioBloq + 1;
		// cant_ticks es la cantidad de ticks que usamos en total hasta el momento
		total_cpu -= cant_ticks;
		// total_cpu es la cantidad de ticks que me quedan disponibles para usar.
	}
}
*/

void TaskConsola(int pid, vector<int> params) {  // params: n, bmin, bmax
	int n = params[0];
	int bmin = params[1];
	int bmax = params[2];
	for(int i = 0; i < n; i++){
		int numAleatorio = (rand() % (bmax-bmin+1)) + bmin; // genera un numero aleatorio entre bmin y bmax
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
	register_task(TaskConsola, 3);
	register_task(TaskBatch, 2);
}

