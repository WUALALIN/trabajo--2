# ==========================================
# Makefile - Proyecto Deportistas (Universal corregido)
# ==========================================

# 1. DETECCIÓN DEL SISTEMA OPERATIVO
ifeq ($(OS),Windows_NT)
   # Windows
    EXE = .exe

    RM = del /Q
    NULLDEV = NUL

    RUN = .\$(BUILD_DIR)\$(TARGET)$(EXE)
    RUN_BENCH = .\$(BUILD_DIR)\$(BENCHMARK)$(EXE)

    MKDIR_BUILD = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    MKDIR_OBJ = if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
    MKDIR_SRC = if not exist $(SRC_DIR) mkdir $(SRC_DIR)
    MKDIR_INC = if not exist $(INC_DIR) mkdir $(INC_DIR)
    MKDIR_DB = if not exist $(DB_DIR) mkdir $(DB_DIR)
    MKDIR_PLOTS = if not exist $(PLOTS_DIR) mkdir $(PLOTS_DIR)

else

    # Linux/macOS
    EXE =

    RM = rm -f
    NULLDEV = /dev/null

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
# ARCHIVOS
# ==========================================

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# ==========================================
# REGLAS PRINCIPALES
# ==========================================

all: folders $(BUILD_DIR)/$(TARGET)$(EXE) $(BUILD_DIR)/$(BENCHMARK)$(EXE)

# Programa principal
$(BUILD_DIR)/$(TARGET)$(EXE): $(OBJ_DIR)/main.o $(OBJ_DIR)/funciones.o
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDE)

# Programa de medición
$(BUILD_DIR)/$(BENCHMARK)$(EXE): $(OBJ_DIR)/medicion.o $(OBJ_DIR)/funciones.o
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDE)

# Compilar objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDE)

# ==========================================
# CARPETAS
# ==========================================

folders:

ifeq ($(OS),Windows_NT)

	-@$(MKDIR_SRC) 2> $(NULLDEV)
	-@$(MKDIR_INC) 2> $(NULLDEV)
	-@$(MKDIR_OBJ) 2> $(NULLDEV)
	-@$(MKDIR_BUILD) 2> $(NULLDEV)
	-@$(MKDIR_DB) 2> $(NULLDEV)
	-@$(MKDIR_PLOTS) 2> $(NULLDEV)

else

	mkdir -p $(SRC_DIR)
	mkdir -p $(INC_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(DB_DIR)
	mkdir -p $(PLOTS_DIR)

endif

# ==========================================
# EJECUCIÓN
# ==========================================

run: $(BUILD_DIR)/$(TARGET)$(EXE)
	$(RUN)

benchmark: $(BUILD_DIR)/$(BENCHMARK)$(EXE)
	$(RUN_BENCH)

# ==========================================
# GENERAR GRÁFICOS
# ==========================================

graficos: $(BUILD_DIR)/$(BENCHMARK)$(EXE)

	@echo Ejecutando mediciones...
	$(RUN_BENCH)

	@echo Generando graficos...
	gnuplot plot_tiempos.gp

# ==========================================
# LIMPIEZA
# ==========================================

clean:

ifeq ($(OS),Windows_NT)

	-$(RM) $(OBJ_DIR)\*.o 2> $(NULLDEV)
	-$(RM) $(BUILD_DIR)\*.exe 2> $(NULLDEV)
	-$(RM) $(DB_DIR)\*.csv 2> $(NULLDEV)
	-$(RM) $(PLOTS_DIR)\*.png 2> $(NULLDEV)

else

	rm -f $(OBJ_DIR)/*.o
	rm -f $(BUILD_DIR)/*
	rm -f $(DB_DIR)/*.csv
	rm -f $(PLOTS_DIR)/*.png

endif

# ==========================================
# PHONY
# ==========================================

.PHONY: all clean folders run benchmark graficos