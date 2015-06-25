BEGIN {
	count = 0;
	ncore_var="1";
	if(ncore!=""){
		ncore_var==ncore;
	}
}

{
	linea=$0;
	gsub(",","\\.",linea);
	split(linea, res, ";");
	total_campos = length(res);
	for(iter=0; iter<total_campos; iter++){
		suma[iter] += res[iter+1];
		suma_cuadrados[iter] += (res[iter+1]*res[iter+1]);
	}
	count++;
}
END {
	salida = ncore_var";";
	for(iter2=0; iter2<total_campos; iter2++){
		promedio = suma[iter2]/count;
		if(iter2!=0){
			salida=salida";";
		}
		salida=salida""promedio;
	}
	gsub("\\.",",",salida);
	print salida;

	salida="0;";
	for(iter3=0; iter3<total_campos; iter3++){
		desviacion_estandar = suma[iter3]*suma[iter3]/count;
		desviacion_estandar = (suma_cuadrados[iter3] - desviacion_estandar)/(count-1);
		desviacion_estandar = sqrt(desviacion_estandar);
		if(iter3!=0){
			salida=salida";";
		}
		salida=salida""desviacion_estandar;
	}
	gsub("\\.",",",salida);
	print salida;
}

