#!/bin/bash

# Colores para la salida
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Iniciando pruebas de Minishell ===${NC}"

# Función para ejecutar un comando y mostrar el resultado
test_command() {
    local cmd="$1"
    local expected_exit=$2
    local test_name="$3"
    
    echo -e "\n${GREEN}Probando: ${test_name}${NC}"
    echo -e "Comando: ${cmd}"
    
    # Ejecutar el comando y capturar la salida
    OUTPUT=$(echo "$cmd" | ./minishell 2>&1)
    EXIT_CODE=$?
    
    # Mostrar la salida
    echo "Salida:"
    echo "$OUTPUT"
    
    # Verificar el código de salida
    if [ $EXIT_CODE -eq $expected_exit ]; then
        echo -e "${GREEN}✅ Éxito: El comando terminó con el código de salida esperado (${expected_exit})${NC}"
    else
        echo -e "${RED}❌ Error: Se esperaba el código ${expected_exit} pero se obtuvo ${EXIT_CODE}${NC}"
    fi
}

# Prueba 1: Comando 'pwd'
test_command "pwd" 0 "Comando pwd"

# Prueba 2: Comando 'ls'
test_command "ls" 0 "Comando ls"

# Prueba 3: Comando 'echo'
test_command "echo Hola Mundo" 0 "Comando echo"

# Prueba 4: Comando 'cd' (cambiar a directorio home y volver)
CURRENT_DIR=$(pwd)
test_command "cd ~ && pwd && cd $CURRENT_DIR" 0 "Comando cd"

# Prueba 5: Comando 'exit' con código de salida
test_command "exit 42" 42 "Comando exit con código"

# Prueba 6: Señal SIGINT (Ctrl+C)
echo -e "\n${GREEN}Prueba de Ctrl+C (SIGINT)${NC}"
echo "Ejecutando 'sleep 5' - Presiona Ctrl+C para interrumpir"
(sleep 1; pkill -SIGINT minishell) & 
OUTPUT=$(echo "sleep 5" | ./minishell 2>&1)
EXIT_CODE=$?
echo "Salida:"
echo "$OUTPUT"
if [ $EXIT_CODE -eq 130 ]; then
    echo -e "${GREEN}✅ Éxito: SIGINT manejado correctamente (código 130)${NC}"
else
    echo -e "${RED}❌ Error: El manejo de SIGINT falló (código ${EXIT_CODE})${NC}"
fi

echo -e "\n${GREEN}=== Pruebas completadas ===${NC}"
