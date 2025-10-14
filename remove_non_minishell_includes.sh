#!/bin/bash

# Remove all #include "*.h" lines except "minishell.h" from .c files
find /home/aalegria/Minishell/srcs -type f -name "*.c" -exec sed -i '' -E '/^#include ".*"$/ { /minishell\.h/!d; }' {} \;

# Add #include "minishell.h" if not present
find /home/aalegria/Minishell/srcs -type f -name "*.c" | while read -r file; do
    if ! grep -q '^#include "minishell\.h"' "$file"; then
        sed -i '' '1i\
#include "minishell.h"
' "$file"
    fi
done
