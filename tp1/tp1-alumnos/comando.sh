#!/bin/bash
if [ -z "$1" ];
then
	echo "Uso: comando.sh <archivo.tsk>"
	exit 1
fi
FILE=$1
if [ ! -f ${FILE} ];
then
	echo "El archivo ${FILE} no existe!!!"
	exit 2
fi
FILE_SALIDA=salida_$(basename ${FILE})_.csv
rm -f ${FILE_SALIDA}
for core in $(echo 1 2 4 6 8); do 
for quant in $(seq 1 2 10); do
echo "Corrida para ${FILE} con ${core} cores y ${quant} como quantum...";
for i in $(seq 1 10); do
./simusched ejercicio7.tsk ${core} 1 2 SchedRR ${quant} 2>/dev/null | awk -f filtro.awk; done | awk  -v ncore=${core} -v quantum=${quant} -v print_varianza="off"  -f filtro2.awk >> ${FILE_SALIDA}; done; done
