# ==========================================
# Makefile - Proyecto Deportistas (Automatizado)
# ==========================================

# 1. DETECCIÓN DEL SISTEMA OPERATIVO
ifeq ($(OS),Windows_NT)
    EXE = .exe
    RM = del /Q
    NULLDEV = NUL
    MKDIR = if not exist
    RUN = .\$(BUILD_DIR)\$(TARGET)$(EXE)
    RUN_BENCH = .\$(BUILD_DIR)\$(BENCHMARK)$(EXE)
else
    EXE =
    RM = rm -f
    NULLDEV = /dev/null
    MKDIR = mkdir -p
    RUN = ./$(BUILD_DIR)/$(TARGET)$(EXE)
    RUN_BENCH = ./$(BUILD_DIR)/$(BENCHMARK)$(EXE)
endif

# 2. NOMBRES Y DIRECTORIOS
TARGET = sistema_deportistas
BENCHMARK = medicion

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BUILD_DIR = build
DB_DIR = db
PLOTS_DIR = plots

# 3. COMPILADOR Y BANDERAS
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
INCLUDE = -I$(INC_DIR)

# ==========================================
# ARCHIVOS (Aquí ocurre la magia)
# ==========================================

# Detecta todos los .c en src/
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Convierte la lista de .c en .o dentro de la carpeta obj/
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Identificamos los objetos que tienen un 'main' específico
MAIN_OBJ = $(OBJ_DIR)/main.o
BENCH_OBJ = $(OBJ_DIR)/medicion.o

# Objetos comunes: Todos los .o MENOS los que tienen funciones main propias
# Así, cualquier .c nuevo que agregues se incluirá aquí automáticamente.
COMMON_OBJS = $(filter-out $(MAIN_OBJ) $(BENCH_OBJ), $(OBJ_FILES))

# ==========================================
# REGLAS PRINCIPALES
# ==========================================

all: folders $(BUILD_DIR)/$(TARGET)$(EXE) $(BUILD_DIR)/$(BENCHMARK)$(EXE)

# Programa principal: Usa su main + todos los archivos comunes
$(BUILD_DIR)/$(TARGET)$(EXE): $(MAIN_OBJ) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDE)

# Programa de medición: Usa su propio main + todos los archivos comunes
$(BUILD_DIR)/$(BENCHMARK)$(EXE): $(BENCH_OBJ) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDE)

# Regla genérica para compilar cualquier archivo .c a .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDE)

# ==========================================
# CARPETAS
# ==========================================

folders:
ifeq ($(OS),Windows_NT)
	-@$(MKDIR) $(SRC_DIR) mkdir $(SRC_DIR) 2> $(NULLDEV)
	-@$(MKDIR) $(INC_DIR) mkdir $(INC_DIR) 2> $(NULLDEV)
	-@$(MKDIR) $(OBJ_DIR) mkdir $(OBJ_DIR) 2> $(NULLDEV)
	-@$(MKDIR) $(BUILD_DIR) mkdir $(BUILD_DIR) 2> $(NULLDEV)
	-@$(MKDIR) $(DB_DIR) mkdir $(DB_DIR) 2> $(NULLDEV)
	-@$(MKDIR) $(PLOTS_DIR) mkdir $(PLOTS_DIR) 2> $(NULLDEV)
else
	mkdir -p $(SRC_DIR) $(INC_DIR) $(OBJ_DIR) $(BUILD_DIR) $(DB_DIR) $(PLOTS_DIR)
endif

# ==========================================
# EJECUCIÓN Y OTROS
# ==========================================

run: $(BUILD_DIR)/$(TARGET)$(EXE)
	$(RUN)

benchmark: $(BUILD_DIR)/$(BENCHMARK)$(EXE)
	$(RUN_BENCH)

graficos: $(BUILD_DIR)/$(BENCHMARK)$(EXE)
	@echo Ejecutando mediciones...
	$(RUN_BENCH)
	@echo Generando graficos...
	gnuplot plot_tiempos.gp

clean:
ifeq ($(OS),Windows_NT)
	-$(RM) $(OBJ_DIR)\*.o 2> $(NULLDEV)
	-$(RM) $(BUILD_DIR)\*.exe 2> $(NULLDEV)
	-$(RM) $(DB_DIR)\*.csv 2> $(NULLDEV)
	-$(RM) $(PLOTS_DIR)\*.png 2> $(NULLDEV)
else
	rm -f $(OBJ_DIR)/*.o $(BUILD_DIR)/* $(DB_DIR)/*.csv $(PLOTS_DIR)/*.png
endif

.PHONY: all clean folders run benchmark graficos