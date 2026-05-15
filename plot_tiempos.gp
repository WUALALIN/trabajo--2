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

# ==============================
# Gráficos de búsqueda
# ==============================

set datafile separator ","
set format y "%.1e"

# ==============================
# Gráfico 1: Búsqueda Caso PEOR
# ==============================

set output "plots/busqueda_peor.png"
set title "Comparación de algoritmos de búsqueda - Caso PEOR"

plot \
    "< awk -F, 'NR>1 && $2~/PEOR/ {print $1\",\"$3}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Secuencial", \
    "< awk -F, 'NR>1 && $2~/PEOR/ {print $1\",\"$4}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Binaria Recursiva", \
    "< awk -F, 'NR>1 && $2~/PEOR/ {print $1\",\"$5}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Exponencial", \
    "< awk -F, 'NR>1 && $2~/PEOR/ {print $1\",\"$6}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Interpolación"


# ==============================
# Gráfico 2: Búsqueda Caso PROMEDIO
# ==============================

set output "plots/busqueda_promedio.png"
set title "Comparación de algoritmos de búsqueda - Caso PROMEDIO"

plot \
    "< awk -F, 'NR>1 && $2~/PROMEDIO/ {print $1\",\"$3}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Secuencial", \
    "< awk -F, 'NR>1 && $2~/PROMEDIO/ {print $1\",\"$4}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Binaria Recursiva", \
    "< awk -F, 'NR>1 && $2~/PROMEDIO/ {print $1\",\"$5}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Exponencial", \
    "< awk -F, 'NR>1 && $2~/PROMEDIO/ {print $1\",\"$6}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Interpolación"

unset output

# ==========================================
# 3. Gráficos individuales de búsqueda
#    Peor caso vs Caso promedio
# ==========================================

set datafile separator ","
set format y "%.1e"
set logscale y
set xlabel "Tamaño del Arreglo (N)"
set ylabel "Tiempo (segundos)"
set grid
set key left

# ------------------------------------------
# Búsqueda Secuencial
# ------------------------------------------

set output "plots/busqueda_secuencial.png"
set title "Búsqueda Secuencial - Peor caso vs Caso promedio"

plot \
    "< awk -F, 'NR>1 && $2~/PEOR/ {print $1\",\"$3}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Peor caso", \
    "< awk -F, 'NR>1 && $2~/PROMEDIO/ {print $1\",\"$3}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Caso promedio"


# ------------------------------------------
# Búsqueda Binaria Recursiva
# ------------------------------------------

set output "plots/busqueda_binaria_recursiva.png"
set title "Búsqueda Binaria Recursiva - Peor caso vs Caso promedio"

plot \
    "< awk -F, 'NR>1 && $2~/PEOR/ {print $1\",\"$4}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Peor caso", \
    "< awk -F, 'NR>1 && $2~/PROMEDIO/ {print $1\",\"$4}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Caso promedio"


# ------------------------------------------
# Búsqueda Exponencial
# ------------------------------------------

set output "plots/busqueda_exponencial.png"
set title "Búsqueda Exponencial - Peor caso vs Caso promedio"

plot \
    "< awk -F, 'NR>1 && $2~/PEOR/ {print $1\",\"$5}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Peor caso", \
    "< awk -F, 'NR>1 && $2~/PROMEDIO/ {print $1\",\"$5}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Caso promedio"


# ------------------------------------------
# Búsqueda por Interpolación
# ------------------------------------------

set output "plots/busqueda_interpolacion.png"
set title "Búsqueda por Interpolación - Peor caso vs Caso promedio"

plot \
    "< awk -F, 'NR>1 && $2~/PEOR/ {print $1\",\"$6}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Peor caso", \
    "< awk -F, 'NR>1 && $2~/PROMEDIO/ {print $1\",\"$6}' db/tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Caso promedio"

unset output