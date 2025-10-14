#!/bin/bash

# Función para probar un comando
test_command() {
    echo "Ejecutando: $1"
    echo -e "$1" | ./minishell
    echo "----------------------------------------"
}

# Probar comando que no existe
test_command "comandonoexistente"

# Probar comando que existe
test_command "ls"

# Probar comando con error seguido de comando válido
echo "Probando comando no existente seguido de 'ls':"
echo -e "comandonoexistente\nls" | ./minishell

echo -e "\nTodas las pruebas completadas."
