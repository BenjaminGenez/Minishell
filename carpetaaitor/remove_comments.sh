#!/bin/bash

# Remove all C-style // comments from .c and .h files
find /home/aalegria/Minishell/srcs -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i '/^[[:space:]]*\/\//d' {} \;

# Also remove inline comments (comments after code)
find /home/aalegria/Minishell/srcs -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's/\/\/.*$//' {} \;

# Remove empty lines left by comment removal
find /home/aalegria/Minishell/srcs -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i '/^[[:space:]]*$/d' {} \;
