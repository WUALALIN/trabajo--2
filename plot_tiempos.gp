set terminal pngcairo size 800,600
set datafile separator ","
set xlabel "Tamaño del Arreglo (N)"
set ylabel "Tiempo (segundos)"
set grid

# ==========================================
# 1. COMPARATIVAS (Caso Aleatorio)
# ==========================================
set output "ordenamiento_comparativa_aleatorio.png"
set title "Comparativa: Algoritmos de Ordenamiento (Caso Aleatorio)"
plot "tiempos_ordenamiento_aleatorio.csv" using 1:2 with linespoints title "Bubble", \
     "tiempos_ordenamiento_aleatorio.csv" using 1:3 with linespoints title "Insertion", \
     "tiempos_ordenamiento_aleatorio.csv" using 1:4 with linespoints title "Selection", \
     "tiempos_ordenamiento_aleatorio.csv" using 1:5 with linespoints title "Cocktail"

set output "busqueda_comparativa.png"
set title "Comparativa: Búsqueda Secuencial vs Binaria (Peor Caso)"
plot "tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints title "Secuencial", \
     "tiempos_busqueda_peor_caso.csv" using 1:3 with linespoints title "Binaria"

# ==========================================
# 2. GRÁFICOS INDIVIDUALES DE ORDENAMIENTO
# ==========================================
# Bubble Sort (Comparando sus 3 casos)
set output "individual_bubble.png"
set title "Rendimiento Individual: Bubble Sort (Mejor vs Peor vs Aleatorio)"
plot "tiempos_ordenamiento_aleatorio.csv" using 1:2 with linespoints title "Aleatorio", \
     "tiempos_ordenamiento_mejor.csv" using 1:2 with linespoints title "Mejor Caso (O(n))", \
     "tiempos_ordenamiento_peor.csv" using 1:2 with linespoints title "Peor Caso (O(n^2))"

# Insertion Sort
set output "individual_insertion.png"
set title "Rendimiento Individual: Insertion Sort"
plot "tiempos_ordenamiento_aleatorio.csv" using 1:3 with linespoints lc rgb "green" title "Insertion"

# Selection Sort
set output "individual_selection.png"
set title "Rendimiento Individual: Selection Sort"
plot "tiempos_ordenamiento_aleatorio.csv" using 1:4 with linespoints lc rgb "blue" title "Selection"

# Cocktail Sort
set output "individual_cocktail.png"
set title "Rendimiento Individual: Cocktail Sort"
plot "tiempos_ordenamiento_aleatorio.csv" using 1:5 with linespoints lc rgb "purple" title "Cocktail"

# ==========================================
# 3. GRÁFICOS INDIVIDUALES DE BÚSQUEDA
# ==========================================
set output "individual_busqueda_secuencial.png"
set title "Rendimiento Individual: Búsqueda Secuencial"
plot "tiempos_busqueda_peor_caso.csv" using 1:2 with linespoints lc rgb "orange" title "Secuencial"

set output "individual_busqueda_binaria.png"
set title "Rendimiento Individual: Búsqueda Binaria"
plot "tiempos_busqueda_peor_caso.csv" using 1:3 with linespoints lc rgb "cyan" title "Binaria"