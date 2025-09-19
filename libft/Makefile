# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/18 12:20:11 by aalegria          #+#    #+#              #
#    Updated: 2025/04/03 19:38:31 by aalegria         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INCLUDE = libft.h
NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

SRCS =  ft_strlen.c     ft_strlcat.c    ft_bzero.c       ft_memset.c \
        ft_memcpy.c     ft_memmove.c    ft_atoi.c       ft_isdigit.c \
        ft_isalnum.c    ft_isalpha.c	ft_isascii.c    ft_isprint.c \
        ft_strncmp.c    ft_tolower.c    ft_toupper.c	ft_strchr.c \
	ft_strrchr.c	ft_memchr.c	ft_memcmp.c	ft_strnstr.c \
	ft_calloc.c	ft_strdup.c	ft_substr.c	ft_strjoin.c \
	ft_strtrim.c	ft_split.c	ft_itoa.c	ft_strmapi.c \
	ft_striteri.c	ft_putchar_fd.c	ft_putstr_fd.c	ft_putendl_fd.c \
	ft_putnbr_fd.c	ft_strlcpy.c ft_set_key.c ft_set_void.c ft_key_value.c \
	ft_atod.c ft_atof_s.c ft_atod_s.c ft_atof.c ft_is_valid_number.c \

BONUSSOURCE = 	ft_lstnew_bonus.c	ft_lstadd_front_bonus.c \
		ft_lstsize_bonus.c	ft_lstlast_bonus.c \
		ft_lstadd_back_bonus.c	ft_lstdelone_bonus.c \
		ft_lstclear_bonus.c	ft_lstiter_bonus.c \
		ft_lstmap_bonus.c

OBJS = $(SRCS:.c=.o)

BONUSOBJS = $(BONUSSOURCE:.c=.o)

all: $(NAME)


$(NAME): $(OBJS) $(INCLUDE)
	$(AR) $(NAME) $(OBJS)

bonus: $(OBJS) $(BONUSOBJS) $(INCLUDE)
	$(AR) $(NAME) $(OBJS) $(BONUSOBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(BONUSOBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
