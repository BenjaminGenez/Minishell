NAME = minishell

CC = clang

CFLAGS = -Wall -Wextra -Werror -I includes/ -I libft/includes/

LIBFT = -L libft -lft -lreadline -lncurses

HEADER = minishell.h

BUILTINS = cd echo env exit export pwd unset

ENV = env_to_array env_to_list get_env_value set_env_value unset_env 


EXEC = executor redirections

MAIN = main prompt

PARSER = expander lexer parser


UTILS = signals utils ft_strdup_until

SRC = $(addsuffix .c, $(addprefix srcs/builtins/, $(BUILTINS))) \
      $(addsuffix .c, $(addprefix srcs/executor/, $(EXEC))) \
      $(addsuffix .c, $(addprefix srcs/parser/, $(PARSER))) \
      $(addsuffix .c, $(addprefix srcs/main/, $(MAIN))) \
      $(addsuffix .c, $(addprefix srcs/utils/, $(UTILS))) \
	  $(addsuffix .c, $(addprefix srcs/env/, $(ENV)))


OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft/
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make clean -C libft/
	@rm -f $(OBJ)

fclean: clean
	@make fclean -C libft/
	@rm -f $(NAME)

re: fclean all

test: all
	./minishell

norm:
	norminette $(SRC) $(HEADER)


.PHONY: all clean fclean re