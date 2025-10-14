# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/13 21:40:00 by aalegria          #+#    #+#              #
#    Updated: 2025/10/13 21:40:00 by aalegria         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME        = minishell
CC          = clang
CFLAGS      = -Wall -Wextra -Werror -I includes/ -I libft/includes/
LIBFT       = -L libft -lft -lreadline -lhistory -ltermcap
HEADER      = includes/minishell.h


BUILTINS    = cd echo env exit export pwd unset

ENV         = env_convert env_sort env_utils get_env shell_level

EXECUTOR    = exec_bin exec_builtin executor redirection executor_utils executor_utilities

MAIN        = main input_loop

PARSER      = expand_utils parse_line tokenizer parsing parsing_utils heredoc parse_utils

UTILS       = fd free token type expansions signals history terminal mem_free

SRC = $(addsuffix .c, $(addprefix srcs/builtins/, $(BUILTINS))) \
      $(addsuffix .c, $(addprefix srcs/env/, $(ENV))) \
      $(addsuffix .c, $(addprefix srcs/executor/, $(EXECUTOR))) \
      $(addsuffix .c, $(addprefix srcs/main/, $(MAIN))) \
      $(addsuffix .c, $(addprefix srcs/parser/, $(PARSER))) \
      $(addsuffix .c, $(addprefix srcs/utils/, $(UTILS))) \
      srcs/input.c

OBJ = $(SRC:c=o)

GREEN       = \033[0;32m
RED         = \033[0;31m
YELLOW      = \033[0;33m
RESET       = \033[0m

.PHONY: all clean fclean re test norm

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	@make -s -C libft/
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) && echo "$(GREEN)Minishell compiled successfully!$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make -s clean -C libft/
	@find . -name "*.o" -type f -delete
	@echo "$(RED)Object files removed$(RESET)"

fclean: clean
	@make -s fclean -C libft/
	@rm -f $(NAME)
	@echo "$(RED)Executable removed$(RESET)"

re: fclean all

test: all
	@echo "$(YELLOW)Running minishell...$(RESET)"
	@./$(NAME)

norm:
	@echo "$(YELLOW)Checking norminette...$(RESET)"
	@norminette $(SRC) includes/$(HEADER) | grep -v "OK!" || true

$(OBJ): $(HEADER)