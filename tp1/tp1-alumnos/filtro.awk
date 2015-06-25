	#Esta es la salida para el script de scala
	#formato de salida:
	#last_tick;tiempo_espera_promedio;maximo_tiempo_espera;uso core1; ... ; uso coreN
		salida=last_tick;
		salida=salida";"tiempo_espera_promedio;
		
		maximo_tiempo_espera=0;
		for(iter9=0; iter9<ntareas; iter9++){
			if(maximo_tiempo_espera<tiempo_espera[iter9]){
				maximo_tiempo_espera=tiempo_espera[iter9];
			}
		}
		salida=salida";"maximo_tiempo_espera;	
		suma_uso=0;
		for(iter3=0; iter3<ncores;iter3++){
			uso = ((last_tick - idle_core[iter3])*100)/last_tick;
			#salida=salida";"uso;
			suma_uso = suma_uso + uso;
		}
		promedio_uso = suma_uso/ncores;
		salida=salida";"promedio_uso;
		#Si se desea "." en lugar de "," en los numeros, comentar esta linea.
		#gsub("\\.",",",salida);
		print salida;
	}
	
}

function print_extra_stats(){
	print "---------------- begin extra stats -------------------"
	print "lote =",lote;
	print "last_tick =",last_tick;
	print "resumen tareas =>";
	for(iter7=0; iter7<ntareas; iter7++){
		print "tarea:",get_name_using_id[iter7];
		print "\ttiempo_espera["get_name_using_id[iter7]"] =",tiempo_espera[iter7];
		print "\ttiempo_bloqueado["get_name_using_id[iter7]"] =",tiempo_bloqueado[iter7];
		print "\ttiempo_ejecutando["get_name_using_id[iter7]"] =",tiempo_ejecutando[iter7];
	}

	print "resumen cores =>";
	for(iter8=0; iter8<ncores; iter8++){
		print "core:",iter8;
		print "\tidle_core["iter8"] =", idle_core[iter8];
	}
	print "---------------- end extra stats ---------------------"
}
