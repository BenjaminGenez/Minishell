#!/bin/bash

# Files to clean
FILES=(
    "srcs/parser/parse_line.c"
    "srcs/parser/parsing.c"
    "srcs/parser/tokenizer.c"
    "srcs/parser/parsing_utils.c"
    "srcs/parser/parse_utils.c"
    "srcs/parser/tokenizer_utils.c"
    "srcs/utils/expansions.c"
    "srcs/utils/tokenizer_utils.c"
)

# Clean each file
for file in "${FILES[@]}"; do
    # Keep only minishell.h include
    sed -i '' -E '/^#include ".*"/ { /minishell\.h/!d; }' "$file"
    
    # Add minishell.h if not present
    if ! grep -q '^#include "minishell\.h"' "$file"; then
        sed -i '' '1i\
#include "minishell.h"
' "$file"
    fi
done
