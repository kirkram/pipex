# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/05 17:33:34 by klukiano          #+#    #+#              #
#    Updated: 2024/02/15 17:09:19 by klukiano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
LIBFT_PATH = ./lib/libft
GNL_PATH = ./lib/get_next_line
LIBFT = ./lib/libft/libft.a
CFLAGS = -Wall -Wextra -Werror
HEADERS	= -I ./include
DEBUGFLAGS = -g -fsanitize=address,undefined,integer
SRCS = ./src/pipex_main.c ./src/pipex_split.c ./src/pipex_prep_func.c ./src/pipex_children.c ./src/pipex_paths.c
GNL_SRCS = ./lib/get_next_line/get_next_line_utils.c ./lib/get_next_line/get_next_line.c
PRINTF_SRCS = ./lib/ft_printf/ft_printf.c ./lib/ft_printf/ft_putchar.c ./lib/ft_printf/printf_putstr.c
OBJCTS = $(SRCS:.c=.o)
BONUS_OBJCTS = $(BONUS_SRCS:.c=.o)
GNL_OBJCTS = $(GNL_SRCS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(SRCS) $(PRINTF_SRCS) $(GNL_SRCS)
	cc $(CFLAGS) $(HEADERS) $(SRCS) $(PRINTF_SRCS) $(GNL_SRCS) $(LIBFT) -o $(NAME)


$(LIBFT):
	make -C $(LIBFT_PATH)

debug: .debug

.debug: $(LIBFT) $(SRCS) $(GNL_SRCS) $(PRINTF_SRCS) 
	cc $(DEBUGFLAGS) $(HEADERS) $(SRCS) $(GNL_SRCS) $(PRINTF_SRCS) $(LIBFT)   -o debug.out
	touch .debug

%.o: %.c
	cc $(CFLAGS) -c $< -o $@ $(HEADERS)

inout:
	$(RM) ./testfilesshell/infile 
	$(RM) ./testfilesshell/outfile
	touch ./testfilesshell/infile
	touch ./testfilesshell/outfile

#bonus: .bonus

#.bonus: $(LIBFT) $(SRCS) $(GNL_SRCS) $(PRINTF_SRCS)
#	cc $(CFLAGS) $(HEADERS) $(BONUS_SRCS) $(PRINTF_SRCS) $(GNL_SRCS) $(LIBFT)  -o $(NAME)
#	touch .bonus

#both: $(NAME) bonus

clean: 
	make clean -C $(LIBFT_PATH)
	$(RM) $(OBJCTS) $(BONUS_OBJCTS) $(PRINTF_OBJCTS)
	
fclean: clean
	make -C $(LIBFT_PATH) fclean
	$(RM) $(NAME)
	$(RM) debug.out
	$(RM) .debug
	$(RM) .bonus


re: fclean all

.PHONY: all clean fclean re bonus both debug