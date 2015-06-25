rm -f salida.txt
for core in $(echo 1 2 4 6 8); do 
for quant in $(seq 1 2 10); do
echo "Corrida para ${core} cores y ${quant} como quantum...";
for i in $(seq 1 10); do
./simusched ejercicio7.tsk ${core} 1 2 SchedRR ${quant} 2>/dev/null | awk -f filtro.awk; done | awk  -v ncore=${core} -v quantum=${quant} -v print_varianza="off"  -f filtro2.awk >> salida.txt; done; done
