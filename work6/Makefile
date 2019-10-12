#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/11 12:49:25 by ssoraka           #+#    #+#              #
#    Updated: 2019/03/11 20:57:23 by ssoraka          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME1 = a.out
NAME2 = checker
NAME3 = rand_int_gen
SRC1 = /Users/ssoraka/Desktop/days/Libft/libft/libft.a
SRC2 = /Users/ssoraka/Desktop/days/Libft/libft/*.o

C_FLAGS = -g

SRC_DIR = src/
OBJ_DIR = obj/
COM_DIR = common/
LEM_DIR = lem-in/
VIS_DIR = visu-hex/

LIBFT = libft.a
LIBFT_HEADER = -I ./
OBJ = main.c initialization.c clear_map.c adap_uvw.c mark_flags.c \
poisson.c setbecond.c solver_3d.c speeds.c surface.c parts.c
MLX_LIB = -L /usr/local/lib/ -lmlx
MLX_HEAD = -I /usr/local/include
FRAMEWORK = -framework OpenGL -framework AppKit

all:
	##gcc $(C_FLAGS) -o a.out main.c libft.a $(MLX_LIB) $(MLX_HEAD) $(FRAMEWORK)
	gcc $(MLX_HEAD) $(OBJ) $(LIBFT) $(LIBFT_HEADER) $(MLX_LIB) $(FRAMEWORK)
	##gcc main.c libft.a -L /usr/local/lib/ -lmlx -I /usr/local/include -framework OpenGL  -framework AppKit
	@/Users/ssoraka/Desktop/days/Libft/help_mat/clean
	./$(NAME1)
	@##./$(NAME1) > command.txt

test:
	gcc -o $(NAME1) test.c solver.c libft.a
	@/Users/ssoraka/Desktop/days/Libft/help_mat/clean
	./$(NAME1)

rand:
	gcc -o $(NAME3) rand_int_gen.c libft.a
	@/Users/ssoraka/Desktop/days/Libft/help_mat/clean
	./$(NAME3) 10 > arr.txt

norm:
	norminette -R CheckForbiddenSourceHeader

clean:
	rm -rf *.o

fclean: clean
	rm -rf *.o

re: fclean all
