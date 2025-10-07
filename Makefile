# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: user <user@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/04 00:29:57 by user              #+#    #+#              #
#    Updated: 2025/10/04 00:29:59 by user             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = clang

CFLAGS = -Wall -Wextra -Werror -I includes/ -I libft/includes/

LIBFT = -L libft -lft

HEADER = minishell.h

BUILTINS = cd cd_aux echo env exit export export_aux pwd unset unset_aux

ENV = env_convert env_sort env_utils get_env shell_level

EXECUTOR = exec_bin exec_builtin executor redirection exec_utils executor_utils

MAIN = main main_aux

PARSER = expand_utils parse_line parse_line_aux tokenizer tokenizer_aux parsing parsing_aux parsing_utils parsing_checks

UTILS = fd free token type expansions mem_free signals tokenizer_utils

SRC = $(addsuffix .c, $(addprefix srcs/builtins/, $(BUILTINS))) \
	  $(addsuffix .c, $(addprefix srcs/env/, $(ENV))) \
	  $(addsuffix .c, $(addprefix srcs/executor/, $(EXECUTOR))) \
	  $(addsuffix .c, $(addprefix srcs/main/, $(MAIN))) \
	  $(addsuffix .c, $(addprefix srcs/parser/, $(PARSER))) \
	  $(addsuffix .c, $(addprefix srcs/utils/, $(UTILS))) \

OBJ = $(SRC:c=o)

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft/
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)

%.o: %.c
	@${CC} ${CFLAGS} -c $< -o $@

clean:
	@make clean -C libft/
	@rm -f $(OBJ)

fclean:
	@make fclean -C libft/
	@rm -f $(OBJ)
	@rm -f $(NAME)

re: fclean all

test: all
	./minishell

norm:
	norminette $(SRC) includes/$(HEADER)

.PHONY: clean fclean re