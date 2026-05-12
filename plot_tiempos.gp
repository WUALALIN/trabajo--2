set terminal pngcairo size 800,600
set datafile separator ","
set xlabel "Tamaño del Arreglo (N)"
set ylabel "Tiempo (segundos)"
set grid

# ==========================================
# 1. Quick Select (Mejor caso v/s Peor caso)
# ==========================================

set output "plots/quickselect.png"
set title "Quick Select - Peor vs Mejor Caso"

plot \
"db/tiempos_quickSelect.csv" using 1:2 with linespoints title "Peor Caso (ultimo elemento)", \
"db/tiempos_quickSelect.csv" using 1:3 with linespoints title "Mejor Caso (mediana de 3)"

