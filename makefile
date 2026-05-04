# ==========================================
# Makefile - Proyecto Deportistas (Universal corregido)
# ==========================================

# 1. DETECCIÓN DEL SISTEMA OPERATIVO
ifeq ($(OS),Windows_NT)
    # Configuración para Windows (CMD)
    EXE = .exe
    RM = del /Q /F
    RMDIR = rmdir /S /Q
    MKDIR = if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
    # Comando de limpieza específico para CMD de Windows
    CLEAN_CMD = if exist $(OBJ_DIR) $(RMDIR) $(OBJ_DIR) & if exist *.exe $(RM) *.exe & if exist *.csv $(RM) *.csv & if exist *.png $(RM) *.png
else
    # Configuración para Linux/macOS (Bash)
    EXE =
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p $(OBJ_DIR)
    CLEAN_CMD = $(RMDIR) $(OBJ_DIR) $(TARGET) $(BENCHMARK) *.csv *.png
endif

# 2. NOMBRES Y DIRECTORIOS
TARGET = sistema_deportistas
BENCHMARK = medicion

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# 3. COMPILADOR Y BANDERAS
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I$(INC_DIR)

# 4. OBJETOS
COMMON_OBJS = $(OBJ_DIR)/funciones.o

# --- REGLAS PRINCIPALES ---

all: folders $(TARGET)$(EXE) $(BENCHMARK)$(EXE)

folders:
	@$(MKDIR)

# Programa Principal
$(TARGET)$(EXE): $(OBJ_DIR)/main.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Programa de Medición
$(BENCHMARK)$(EXE): $(OBJ_DIR)/medicion.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilación de objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# --- AUTOMATIZACIÓN Y LIMPIEZA ---

graficos: $(BENCHMARK)$(EXE)
	@echo Ejecutando mediciones...
	./$(BENCHMARK)$(EXE)
	@echo Generando graficos...
	gnuplot plot_tiempos.gp

clean:
	@echo Limpiando archivos...
	@$(CLEAN_CMD)
	@echo Proyecto limpio.

run: $(TARGET)$(EXE)
	./$(TARGET)$(EXE)

.PHONY: all clean folders run graficos