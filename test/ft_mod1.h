/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mod1.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 20:59:39 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/19 13:13:30 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MOD1_H
# define FT_MOD1_H

// в файле cells.c
#define EMPTY_MAP
#define CHANGE_FLUIDS1

#define PROGRAMS_COUNT 3
#define PART_COUNT 10

#include "libft.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <OpenCL/opencl.h>
#include "mlx.h"
#include "ft_cl.h"
#include "ft_cl_struct.h"
#include "ft_mod1_struct.h"



#include <time.h>

#define ABS(nbr) ((nbr) >= 0 ? (nbr) : (-1) * (nbr))

#define THREAD_COUNT 4

#define T_INFINITY 10000000000.0
#define P_CONST 0.0
#define U_CONST 0.0
#define W_CONST 1.7
//#define CONST_RE 0.105
#define CONST_RE 100.011
#define CONST_GY -10.0
#define CONST_GX 0.0
#define CONST_GZ 0.0
#define T_DELTA 0.01
#define T_END 5.0
#define TAU 0.2
#define MAX_ITERATIONS 50
#define TOLERANCE 0.001

/*
**	добавляем по 2 вершины между вершинами расчетной модели для сплайна
*/
#define ADD_POINT 1

#define MAP_KOEF 0.7
#define MAP_HEIGTH2 1000
#define COLOR_UP 0xFFFFFF
#define COLOR_DOWN 0x704214
//#define MAX_POINT 50



#define DELTA 1.0
#define DELTA_X DELTA
#define DELTA_Y DELTA
#define DELTA_Z DELTA

#define EMPTY ' '
#define OBSTACLES '#'
#define PARTS_COUNT 1
#define TEST_WATER_LEVEL (0)
#define TEST_WATER_WALL 0
//#define MAP_HEIGTH 50
#define WATER_COLOR 0xFFFF
#define OBSTACLES_TOP_COLOR 0x704214
#define OBSTACLES_FRONT_COLOR 0x5b432d
//#define OBSTACLES_RIGHT_COLOR 0xFF
#define OBSTACLES_RIGHT_COLOR 0x654321

#define MSG_ERROR1 "Malloc error\n"



#define DIR_X 1
#define DIR_Y 2
#define DIR_Z 4
#define SCALAR (DIR_X + DIR_Y + DIR_Z)

//начальная плотность жидкости
#define DENSITY_0 1000.0
//ускорение свободного падения
#define FG CONST_GY
//характеристика адиабаты
#define GAMMA 7
//радиус сферы у одной частицы
#define PART_H 0.5 * DELTA

#define PI 3.141592
//масса сферы одной частицы
//#define PART_MASS_0 (DENSITY_0 * DELTA * DELTA * DELTA *0.99 / PARTS_COUNT / PARTS_COUNT / PARTS_COUNT)
#define PART_MASS_0 (DENSITY_0 * DELTA * DELTA * DELTA * 0.990)
//высоту столба воды можно как-то иначе определить, тут ее на шару поставил...
#define PRESS_0 (200 * DENSITY_0 * FG * 3.0 * DELTA / GAMMA)
//#define PRESS_0 (2200000000)
//скорость звука в среде
#define SPEED_OF_SOUND_C 120.0
//Постоянна Куранта для расчета времени 0-1 для расчета промежутка времени
#define CONST_CUR 0.5
//для итерирования по окружению
#define COUNT_NEAR_CELL_IN_ONE_DIMENSION 3
#define COUNT_NEAR_CELL 9

//для расчета искусственной вязкости
//коэффициент квадратичной искусственной вязкости (0.02 - 0.03)
#define CONST_B 0.00
//коэффициент линнейной искусственной вязкости (0.02 - 0.03)
#define CONST_A 0.00
//соотношение максимальной скорости частицы со скоростью звука ~ 10 раз
#define CONST_E 0.1
//коэффициент влияния
#define CONST_EP 0.3

#define VISCOSITY 0000.0
//#define SIGMA 5000000.0000000
#define SIGMA 0000000.0000000
/*
**	images
*/
#define CONST_WIDTH 2000
#define CONST_HEINTH 1500
#define CAM_X 1700
#define CAM_Y 1300
//#define RADIUS (DELTA * CONST_LEN * 0.7)
#define RADIUS 3
#define CONST_LEN 100.0
#define KOEFF (1.0 / (DELTA_XY))
#define SLEEP1


int		**ground;

t_cell	*g_cell;

t_dpoint g;

REAL norm_speed;

REAL renolds;

REAL deltat;

int imax;
int jmax;
int kmax;
int iteration;

long g_clock;
long g_clock2;
long g_tmp;

t_vis *vis;


typedef enum	e_fluids
{
	//EMPTY = 0,
	WATER = 1,
	WATER2,
	MAGMA,
	MAGMA2,
	BLOB,
	OBSTCL,
	FLUIDS
}				t_fluids;

typedef enum	e_print
{
	COLOR = 1,
	RADIUS2,
	COLUMN_COUNT2
}				t_print;

typedef enum	e_column
{
	FLUID = 0,
	F_H,
	F_C,
	F_MASS,
	F_PRESS,
	F_DENS,
	F_VIS,
	F_Y_SPEED,
	COLUMN_COUNT
}				t_column;



char *g_names[PROGRAMS_COUNT + 10];

char *g_kernel[PROGRAMS_COUNT + 10];

t_prog    g_compile[PROGRAMS_COUNT + 10];

#define PROGRAMM_SIZE 5000



int		g_color[FLUIDS][COLUMN_COUNT2];
REAL	g_param[FLUIDS][COLUMN_COUNT];


/*
**	main.c
*/
void	ft_error_and_exit(t_open_cl *cl, char *message);

/*
**	read_program.c
*/
int		is_read_programm(char *buffer, char *filename);



/*
** initialization.c
*/
void	pfn_notify2(cl_program program, void *user_data);
void	pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data);
void	ft_context_error(cl_int errcode_ret);
void	ft_queue_error(cl_int errcode_ret);
t_open_cl	*ft_init_open_cl(void);
int		ft_read_and_build_programs(t_open_cl *cl);
void	ft_free_open_cl(t_open_cl **open_cl);
int		ft_create_buffers(t_open_cl *cl, int num, void *src, size_t size);
int		ft_read_buffers(t_open_cl *cl, int num, void *dest, size_t size);
int		ft_run_kernels(t_open_cl *cl);
int		ft_set_kernel_arg(t_open_cl *cl);
#endif
