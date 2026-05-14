set terminal pngcairo size 800,600
set datafile separator ","
set xlabel "Tamaño del Arreglo (N)"
set ylabel "Tiempo (segundos)"
set key left
set grid

# ==========================================
# 1. Quick Select (Mejor caso v/s Peor caso)
# ==========================================

set output "plots/quickselect.png"
set title "Quick Select - Peor vs Mejor Caso"

plot \
"db/tiempos_quickSelect.csv" using 1:2 with linespoints title "Peor Caso (ultimo elemento)", \
"db/tiempos_quickSelect.csv" using 1:3 with linespoints title "Mejor Caso (mediana de 3)"

# ==========================================
# 2. Algoritmos de ordenamiento (Mejor, Peor y Promedio)
# ==========================================

set output "plots/ordenamiento_mejor_caso.png"
set title "Algoritmos de ordenamiento (Mejor caso)"

plot \
"db/tiempos_t2_mejor.csv" using 1:2 with linespoints title "Merge sort clásico", \
"db/tiempos_t2_mejor.csv" using 1:3 with linespoints title "Quick sort (ultimo elemento)", \
"db/tiempos_t2_mejor.csv" using 1:4 with linespoints title "Quick sort (primer elemento)", \
"db/tiempos_t2_mejor.csv" using 1:5 with linespoints title "Quick sort (elemento aleatorio)", \
"db/tiempos_t2_mejor.csv" using 1:6 with linespoints title "Quick sort (mediana)"

#--------------------------------------------

set output "plots/ordenamiento_peor_caso.png"
set title "Algoritmos de ordenamiento (Peor caso)"

plot \
"db/tiempos_t2_peor.csv" using 1:2 with linespoints title "Quick sort (ultimo elemento)", \
"db/tiempos_t2_peor.csv" using 1:3 with linespoints title "Quick sort (primer elemento)", \
"db/tiempos_t2_peor.csv" using 1:4 with linespoints title "Quick sort (elemento aleatorio)", \
"db/tiempos_t2_peor.csv" using 1:5 with linespoints title "Quick sort (mediana)"


#--------------------------------------------

set output "plots/ordenamiento_promedio_caso.png"
set title "Algoritmos de ordenamiento (Caso Promedio)"

plot \
"db/tiempos_t2_promedio.csv" using 1:2 with linespoints title "Merge sort clásico", \
"db/tiempos_t2_promedio.csv" using 1:3 with linespoints title "Merge sort (Umbral 8)", \
"db/tiempos_t2_promedio.csv" using 1:4 with linespoints title "Merge sort (Umbral 16)", \
"db/tiempos_t2_promedio.csv" using 1:5 with linespoints title "Merge sort (Umbral 32)", \
"db/tiempos_t2_promedio.csv" using 1:6 with linespoints title "Quick sort (ultimo elemento)", \
"db/tiempos_t2_promedio.csv" using 1:7 with linespoints title "Quick sort (primer elemento)", \
"db/tiempos_t2_promedio.csv" using 1:8 with linespoints title "Quick sort (elemento aleatorio)", \
"db/tiempos_t2_promedio.csv" using 1:9 with linespoints title "Quick sort (mediana)"

