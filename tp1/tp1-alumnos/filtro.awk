BEGIN {
	ntareas = 0;
	last_tick=-1;
	if(debug!="")
		debug="on";
	if(verbose!="")
		verbose="on";
		
	
}

/SETTINGS/ {
	lote = $4;
	ncores = $5;
	for(iter1=0;iter1<ncores;iter1++){
		ejecuto_ultimo_tick[iter1] = 0;
		idle_core[iter1] = 0;
	}
	get_name_using_id[-1]=-1;
	get_id_using_name[-1]=-1;
}

/^LOAD/{
	get_name_using_id[ntareas]=$3;
	get_id_using_name[$3]=ntareas;

	test_tick($2);
	running[get_id_using_name[$3]]=1;
	ntareas++;
	tiempo_ejecutando[get_id_using_name[$3]]=0;
	tiempo_bloqueado[get_id_using_name[$3]]=0;
	tiempo_espera[get_id_using_name[$3]]=0;
}

/^BLOCK/{
	test_tick($2);
	bloqueado[get_id_using_name[$3]]=1;
}

/^UNBLOCK/{
	test_tick($2);
	bloqueado[get_id_using_name[$3]]=0;
}

/^CPU/ {
	test_tick($2);
	if(get_id_using_name[$3]>=0){
		tiempo_ejecutando[get_id_using_name[$3]]++;
		ticks[get_id_using_name[$3]] = 1;
		ejecuto_ultimo_tick[$4]=1;
	}
}

/^EXIT/ {
	test_tick($2);
	running[get_id_using_name[$3]]=0;
}

END {
	test_tick(last_tick+1);

	tiempo_espera_total = 0;
	for(iter2=0;iter2<ntareas;iter2++){
		tiempo_espera_total += tiempo_espera[iter2];
	}
	print_stats();
	if(debug=="on"){
		print_extra_stats();
	}
}

function test_tick(tick_actual){
	if((last_tick>=0) && (last_tick!=tick_actual)){
		diff_ticks = tick_actual - last_tick;
		for(iter4=0; iter4<diff_ticks; iter4++){
			proc_cambio_tick();
		}
	}
	last_tick=tick_actual;
}

function proc_cambio_tick(){
	for(iter5=0; iter5<ntareas; iter5++){
		if(bloqueado[iter5]==0 && running[iter5]==1 && ticks[iter5]==0){
			tiempo_espera[iter5]++;
		}
		if(bloqueado[iter5]==1 && running[iter5]==1){
			tiempo_bloqueado[iter5]++;
		}
		ticks[iter5]=0;
	}	
	for(iter6=0; iter6<ncores;iter6++){
		if(ejecuto_ultimo_tick[iter6]==0){
			idle_core[iter6]++;
		}
		ejecuto_ultimo_tick[iter6] = 0;
	}
}

function print_stats(){
    tiempo_espera_promedio = tiempo_espera_total/ntareas;
    if(verbose=="on"){
		print "---------------- begin stats -------------------------"
		print "Tiempo espera promedio", tiempo_espera_promedio;
		print "El uso de los cores fue:"
		for(iter3=0; iter3<ncores;iter3++){
			uso = ((last_tick - idle_core[iter3])*100)/last_tick;
			print "core nro:",iter3,"uso",uso,"% cpu";
		}
		print "---------------- end stats ---------------------------"
	} else {
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
