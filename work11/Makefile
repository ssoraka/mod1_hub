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

GCC = gcc
#GCC = gcc -Wall -Wextra -Werror

NAME = mod1
NAME1 = a.out
NAME2 = checker
NAME3 = rand_int_gen
SRC1 = /Users/ssoraka/Desktop/days/Libft/libft/libft.a
SRC2 = /Users/ssoraka/Desktop/days/Libft/libft/*.o

C_FLAGS = -g -Ofast

SRC_DIR = src/
OBJ_DIR = obj/
COM_DIR = common/
LEM_DIR = lem-in/
VIS_DIR = visu-hex/

PATH_OBJ	= obj
PATH_SRC	= .
PATH_INC	= includes
PATH_LIB	= libft


LIBFT = ./$(PATH_LIB)/libft.a
HEADERS = ./  ./$(PATH_LIB)
COMPILE_HEADER = $(addprefix -I ,${HEADERS})

MLX_LIB = -L /usr/local/lib/ -lmlx
MLX_HEAD = -I /usr/local/include
FRAMEWORK = -framework OpenGL -framework AppKit -framework OpenCL

SRC = color_interpolation.c ground.c images.c initialization.c \
keys.c lines_vektrs.c main.c map.c parts.c points.c poligons.c print_shapes.c \
rotate.c cells.c read_program.c open_cl.c thread_solver.c earth.c

OBJECTS		= $(patsubst %.c, $(PATH_OBJ)/%.o, $(SRC))

all: libs_refresh $(NAME)
	./$(NAME)

libs_refresh:
	@make -C ./$(PATH_LIB)/

#all:
#	##gcc $(C_FLAGS) -o a.out main.c libft.a $(MLX_LIB) $(MLX_HEAD) $(FRAMEWORK)
#	gcc $(MLX_HEAD) $(OBJ) $(LIBFT) $(LIBFT_HEADER) $(MLX_LIB) $(FRAMEWORK)
#	##gcc main.c libft.a -L /usr/local/lib/ -lmlx -I /usr/local/include -framework OpenGL  -framework AppKit
#	@/Users/ssoraka/Desktop/days/Libft/help_mat/clean
#	./$(NAME1)
#	@##./$(NAME1) > command.txt

$(PATH_OBJ)/%.o: $(addprefix $(PATH_SRC)/,%.c)  ${LIBFT_HEADER}
	@mkdir -p $(PATH_OBJ)
	$(GCC) -c $< -o $@  $(COMPILE_HEADER) $(MLX_HEAD)

$(NAME): $(OBJECTS) $(LIBFT)
	$(GCC) -o $@ $(OBJECTS)  $(MLX_HEAD) $(LIBFT) $(COMPILE_HEADER) $(MLX_LIB) $(FRAMEWORK)


test:
	gcc -o $(NAME1) test.c solver.c $(LIBFT)
	@/Users/ssoraka/Desktop/days/Libft/help_mat/clean
	./$(NAME1)

rand:
	gcc -o $(NAME3) rand_int_gen.c $(LIBFT)
	@/Users/ssoraka/Desktop/days/Libft/help_mat/clean
	./$(NAME3) 10 > arr.txt

norm:
	norminette -R CheckForbiddenSourceHeader

clean:
	rm -f $(OBJECTS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all
