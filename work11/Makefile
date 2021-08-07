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

C_FLAGS = -g -Ofast

PATH_OBJ	= obj
PATH_SRC	= src
PATH_INC	= includes
PATH_LIB	= libft

LIBFT = ./$(PATH_LIB)/libft.a

HEADERS = ft_cl.h ft_buttons.h ft_cl_struct.h ft_mod1.h ft_mod1_struct.h
PATH_HEADERS = $(addprefix ./$(PATH_INC)/,${HEADERS})
COMPILE_HEADER = -I ./$(PATH_INC)

MLX_LIB = -L /usr/local/lib/ -lmlx

FILES = grathics/color_interpolation.c	grathics/picture.c \
		grathics/points.c	grathics/points2.c	grathics/print_shapes.c \
		grathics/images.c	grathics/lines_vektrs.c \
		grathics/polygons.c	grathics/polygons2.c \
		grathics/rotate.c	grathics/shapes.c \
		helper.c \
		hooks/animation.c	hooks/mouse.c	hooks/keys.c	hooks/hooks.c \
		hooks/keys2.c \
		open_cl/open_cl_buffers.c open_cl/open_cl_free.c \
		open_cl/open_cl_init.c open_cl/open_cl_read_build_run.c \
		model/ground.c model/parts.c \
		model/map.c model/map_math.c \
		model/earth.c model/print_smooth_earth.c model/create_smooth_earth.c \
		model/cells.c model/cell_change.c model/cell_print.c model/cells_print.c \
		view/print.c view/sprite.c \
		main.c  initialization.c \
		thread_solver.c

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
	./$(NAME) demo.txt

libs_refresh:
	@make -C ./$(PATH_LIB)/
	@make -sC ./$(LIBMAKE)

$(PATH_OBJ)/%.o: $(addprefix $(PATH_SRC)/,%.c)  ${PATH_HEADERS}
	@mkdir -p $(PATH_OBJ)
	@mkdir -p $(PATH_OBJ)/grathics
	@mkdir -p $(PATH_OBJ)/hooks
	@mkdir -p $(PATH_OBJ)/open_cl
	@mkdir -p $(PATH_OBJ)/model
	@mkdir -p $(PATH_OBJ)/view
	$(GCC) -c $< -o $@  $(COMPILE_HEADER)

$(NAME): $(OBJECTS) $(MAKES) $(LIBFT)
	$(GCC) -o $@ $(OBJECTS) $(LIBFT) $(COMPILE_HEADER) $(LIB)

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
