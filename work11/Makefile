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
PATH_SRC	= src
PATH_INC	= includes
PATH_LIB	= libft


LIBFT = ./$(PATH_LIB)/libft.a

HEADERS = ft_cl.h ft_cl_struct.h ft_mod1.h ft_mod1_struct.h
PATH_HEADERS = $(addprefix ./$(PATH_INC)/,${HEADERS})
COMPILE_HEADER = -I ./$(PATH_INC)

MLX_LIB = -L /usr/local/lib/ -lmlx
#FRAMEWORK = -framework OpenGL -framework AppKit -framework OpenCL

FILES = grathics/color_interpolation.c	grathics/picture.c \
		grathics/points.c	grathics/points2.c	grathics/print_shapes.c \
		grathics/images.c	grathics/lines_vektrs.c \
		grathics/polygons.c	grathics/polygons2.c \
		ground.c initialization.c \
		keys.c main.c map.c parts.c \
		rotate.c cells.c read_program.c open_cl.c thread_solver.c earth.c

SRC		= $(addprefix $(PATH_SRC)/,$(FILES))
OBJECTS		= $(patsubst %.c, $(PATH_OBJ)/%.o, $(FILES))

ifeq ($(OS),Windows_NT)
	detected_OS := Windows
else
	detected_OS := $(shell uname)
endif
ifeq ($(detected_OS),Linux)
	MAKES := ./libs/minilibx/libmlx.a
	LIBMAKE := libs/minilibx
	LIB := -L libs/minilibx -lmlx_Linux -lXext -lX11 -lm -framework OpenCL
	COMPILE_HEADER += -I ./libs/minilibx/
endif
ifeq ($(detected_OS),Darwin)
	MAKES = ./libs/minilibx_macos/libmlx.a
	LIBMAKE := libs/minilibx_macos
	LIB := -L libs/minilibx_macos -lmlx -framework OpenGL -framework Appkit -framework OpenCL
	COMPILE_HEADER += -I ./libs/minilibx_macos/
endif

all: libs_refresh $(NAME)
	./$(NAME)

libs_refresh:
	@make -C ./$(PATH_LIB)/
	@make -sC ./$(LIBMAKE)

#all:
#	##gcc $(C_FLAGS) -o a.out main.c libft.a $(MLX_LIB) $(MLX_HEAD) $(FRAMEWORK)
#	gcc $(MLX_HEAD) $(OBJ) $(LIBFT) $(LIBFT_HEADER) $(MLX_LIB) $(FRAMEWORK)
#	##gcc main.c libft.a -L /usr/local/lib/ -lmlx -I /usr/local/include -framework OpenGL  -framework AppKit
#	@/Users/ssoraka/Desktop/days/Libft/help_mat/clean
#	./$(NAME1)
#	@##./$(NAME1) > command.txt

$(PATH_OBJ)/%.o: $(addprefix $(PATH_SRC)/,%.c)  ${LIBFT_HEADER}
	@mkdir -p $(PATH_OBJ)
	@mkdir -p $(PATH_OBJ)/grathics
	$(GCC) -c $< -o $@  $(COMPILE_HEADER)

$(NAME): $(OBJECTS) $(MAKES) $(LIBFT)
	$(GCC) -o $@ $(OBJECTS) $(LIBFT) $(COMPILE_HEADER) $(LIB)

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
	#make -C libft fclean
	#make -C $(LIBMAKE) clean

re: fclean all
