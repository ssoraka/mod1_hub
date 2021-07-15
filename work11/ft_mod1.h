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

#include "libft/libft.h"
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
#define T_END 5.0
#define TAU 0.2
#define MAX_ITERATIONS 50
#define TOLERANCE 0.001

/*
**	добавляем по 2 вершины между вершинами расчетной модели для сплайна
*/
#define ADD_POINT 1

#define MAP_KOEF 0.7
#define HEIGTH_KOEF 0.9
//перевести на даблы и избавиться
#define MAP_HEIGTH2 ((JMAX + J0) * 100)
#define COLOR_UP 0xFFFFFF
#define COLOR_DOWN 0x704214
//#define MAX_POINT 50


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


//ускорение свободного падения
#define FG CONST_GY


//масса сферы одной частицы
//#define PART_MASS_0 (DENSITY_0 * DELTA * DELTA * DELTA *0.99 / PARTS_COUNT / PARTS_COUNT / PARTS_COUNT)

//Постоянна Куранта для расчета времени 0-1 для расчета промежутка времени
#define CONST_CUR 0.5
//для итерирования по окружению
#define COUNT_NEAR_CELL_IN_ONE_DIMENSION 3
#define COUNT_NEAR_CELL 9

//#define SIGMA 5000000.0000000
#define SIGMA 0000000.0000000
/*
**	images
*/
#define CONST_WIDTH 2000
#define CONST_HEINTH 1360
#define CAM_X 1200
#define CAM_Y 1100
//#define RADIUS (DELTA * CONST_LEN * 0.7)
#define RADIUS 3
#define CONST_LEN (CONST_HEINTH / JMAX)
#define KOEFF (1.0 / (DELTA_XY))
#define SLEEP1


int		**ground;

t_arr	*g_cell;
t_arr	*g_parts;
t_arr	*g_iparts;
t_arr	*g_cell_map;
t_arr	*g_neighs;

t_open_cl	*g_cl;
t_earth *g_earth;

t_dpoint g;


long g_clock;
long g_clock2;
long g_tmp;

t_vis *vis;

t_solver solver;



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

typedef enum	e_rain
{
	RAIN_FALSE,
	NEED_STOP_PRINT_FOR_RAIN,
	RAIN_ACTIVATE,
}				t_rain;


char *g_names[PROGRAMS_COUNT + 10];

char *g_kernel[PROGRAMS_COUNT + 10];

t_prog    g_compile[PROGRAMS_COUNT + 10];


#define PROGRAMM_SIZE 5000



int		g_color[FLUIDS][COLUMN_COUNT2];
REAL	g_param[FLUIDS][COLUMN_COUNT];


/*
**	main.c
*/
int		ft_znak(int num);
void	ft_print_lines(t_vis *vis, t_line *line);
void	ft_print_points2(t_vis *vis, t_vektr *points);
void	ft_create_cube_poligons(t_plgn **plgn, t_vektr **p, int color);
void	ft_create_obstacles(void *ptr, int j, int i, int k);
void	ft_cycle_cube(void *param, void (*f)(void *, int, int, int), t_point *start, t_point *end);
void	ft_create_points_in_cells(t_vis *vis);
void	ft_print_water_in_cell(void *param, int j, int i, int k);
void	ft_print_all_water(t_vis *vis);
void	ft_refresh_picture(t_vis *vis);
void	ft_print_int(void *param, int j, int i, int k);
void	ft_print_char(void *param, int j, int i, int k);
void	ft_print_real(void *param, int j, int i, int k);
void	ft_print_arr(void *arr, void (*f)(void *, int, int, int), int k);
int		loop_hook(void *param);
void	ft_del_each_parts(void *param, int j, int i, int k);
void	ft_create_stable_level_of_water(void *param, int j, int i, int k);
void	ft_create_first_water(void);
void	ft_prepare_one_buffer(t_buff *buff);

/*
** initialization.c
*/
void	***ft_cube_arr(int jmax, int imax, int kmax, int size);
void	ft_init_variable(void);
void	ft_init_delta_xyz(void);
void	ft_init_map_arrs(void);
void	ft_initialization_of_global_variable(void);
void	ft_del_all(char *message);


/*
**	color_interpolation.c
*/
int		ft_interpolation(int percent, int color1, int color2, int byte);
int		ft_grad_color(int delta1, int delta2, int color1, int color2);
int		ft_set_color_to_point(t_line *line, t_point *p, int lower_45);
int		ft_int_interpolation(int y, int delta_y, int x1, int x2);

/*
**	print_shapes.c
*/
void	ft_print_rect2(t_pict *pic, t_point *center, int len, int color);
void	plot_circle2(t_pict *pic, t_point *p, t_point *center, int color_code);
void	circle2(t_pict *pic, t_vektr *center, int radius, int color_code);

/*
**	rotate.c
*/
void	ft_norm_vektor(t_dpoint *vek);
void	ft_rotate_vek_around_vek_by_ang(t_dpoint *ox, t_dpoint *oy, double ang);
void	ft_change_points4(t_param *vis, t_vektr *p);
void	ft_rotate_xyz(t_oxyz *oxyz, t_dpoint *ang);
t_dpoint	ft_rot_dpoint(t_dpoint *v, t_oxyz *oxyz);
void	ft_ret_zoom_xyz(t_vektr *ox, t_param *vis);
void	ft_change_points5(t_param *param, t_vektr *p);

/*
**	images.c
*/
int		ft_create_img(t_pict *pic, void *mlx, int width, int heigth);
t_vis	*ft_create_mlx(int width, int heigth, char *name);
int		ft_not_need_print(t_line *line, t_pict *pic);
int		ft_put_pixel_to_img2(t_pict *pic, t_point *p, int color);
int		ft_put_pixel_to_img(t_pict *pic, t_point *p, int color);
void	ft_destroy_img(t_pict *pic);
void	ft_clear_image(t_pict *pic);
void	ft_save_image(t_pict *pic);
void	ft_return_image(t_pict *pic);
t_vis	*ft_destroy_mlx(t_vis **vis);


/*
**	lines_vektrs.c
*/
t_vektr	*ft_new_vektor2(REAL x, REAL y, REAL z, int color);
t_vektr	*ft_add_vektor2(void *ptr, t_point *p, int color);
t_line	*ft_new_line(t_vektr *p1, t_vektr *p2, int color);
void	ft_add_line(t_line **begin, t_vektr *p1, t_vektr *p2, int color);
void	ft_del_vektor(t_vektr **begin);
void	ft_del_lines(t_line **begin);
void	draw_line_img_lower_452(t_line *line, t_point *p, t_pict *pic, int grad);
void	draw_line_img_over_452(t_line *line, t_point *p, t_pict *pic, int grad);
void	draw_line_img2(t_line *line, t_pict *pic, int grad);
/*
**	poligons.c
*/
t_plgn	*ft_create_poligon(t_vektr *p1, t_vektr *p2, t_vektr *p3, int color);
void	ft_del_poligines(t_plgn **begin);
void	ft_swap_ptr(void **ptr1, void **ptr2);
void	ft_sort_points_by_y(t_vektr **p);
void	ft_vektr_interpolation_by_y(t_vektr *p, t_vektr *p1, t_vektr *p2, int grad);
int		ft_need_print_traing(t_vektr **p, t_pict *pic);
void	ft_draw_traing(t_pict *pic, t_vektr **p, int grad, int color);
void	ft_print_plgn(t_plgn *plgn, t_pict *pic, int grad);
void	ft_print_poligons(t_plgn *plgn, t_vektr *points, t_pict *pic, t_param *param);
void	ft_prepare_plgn_for_printing(t_plgn *plgn, t_param *param);
/*
**	keys.c
*/
int		ft_rotate_and_csale(t_param *vis, int key);
int		ft_shift(t_param *vis, int key);
int		deal_key(int key, void *param);

/*
**	point.c
*/
void	ft_fill_point(t_point *p, int y, int x, int z);
void	ft_fill_dpoint(t_dpoint *p, REAL y, REAL x, REAL z);
void	ft_create_xyz(t_oxyz *oxyz);
t_dpoint	ft_ret_norm(t_dpoint *a, t_dpoint *b, t_dpoint *c);
REAL	ft_vekt_cos(t_dpoint a, t_dpoint b);
REAL	ft_dot_dpoints(t_dpoint *a, t_dpoint *b);

/*
**	parts.c
*/
void	ft_create_new_water_in_cell(void *param, int j, int i, int k);
void	ft_del_part(t_part **begin);
t_part	*ft_new_part(t_dpoint *p, int type);
t_part	*ft_add_part(void *ptr, t_dpoint *p, int type);
t_arr	*ft_init_all_clear_parts(void);
void	ft_create_new_area_of_water(t_arr *parts, t_point *start, t_point *end, int type);
void	ft_fill_interface(t_arr *parts, t_arr *iparts);
int		ft_del_unused_part(void *elem, void *param);
/*
**	map.c
*/
REAL	ft_return_heigth(REAL value);
int		**ft_read_ground_from_file3(char *name);
char	*ft_read_string_from_file(char *name);
int		ft_find_points(t_map *map, char **arr);
int		ft_find_point(t_map *map, char **args);
void	ft_create_first_and_last_points(t_map *map);
int		**ft_create_map(t_map *map);
void	ft_fill_map_arr(t_map *map);
void	ft_superposition(void *param, int j, int i, int k);
REAL	ft_sigmoida(int i, int k);
/*
**	ground.c
*/
int		**ft_create_complex_ground_from_simple(int  **simple_ground);
void	ft_bicube_interpolate_y(void *param, int j, int i, int k);
void	ft_bicube_interpolate_x(void *param, int j, int i, int k);
int		ft_cube_interpolate (int *arr, REAL x);

int		ft_create_relief(t_vis *vis, int  **ground);
t_vektr	*ft_create_points_of_relief(int  **ground, t_vektr **p);
int		ft_create_points_from_string(t_vektr **begin, int *ground, int k);
void	ft_points_push_back(t_vektr *begin, t_vektr *last);
int		ft_color_from_z(int value);
t_plgn	*ft_create_poligons_of_relief(t_vektr **p_arr);
int		ft_create_line_of_poligons_of_relief(t_plgn **plgn, t_vektr *p1, t_vektr *p2);
int		ft_create_rectang_poligon2(t_plgn **plgn, t_vektr **p, int color);


///*
//** arr.c
//*/
//
//
//void	ft_del_arr(t_arr **arr);
//t_arr	*ft_create_arr(int elem_size, int elems_count, void (*func_del)(void *));
//t_arr	*ft_create_arr_of_ptr(int elems_count, void (*func_del)(void *));
//int		ft_realloc_arr(t_arr *arr, int new_count);
//void	*ft_arr_add(t_arr *arr, void *elem);
//void	*ft_arr_get(t_arr *arr, int num);
//void	ft_del_elem(t_arr *arr, int num);
//void	ft_del_elems_if(t_arr *arr, int (*need_del)(void *, void *), void *param);
//void	ft_for_each_ptr(t_arr *arr, void (*func)(void *, void *), void *param);
//void	ft_for_each_elem(t_arr *arr, void (*func)(void *, void *), void *param);
//void	*ft_arr_get_next(t_arr *arr);
//void	ft_for_each_ptr2(t_arr *arr, void (*func)(void *, void *), void *param);


/*
**	cells.c
*/
int		ft_change_obstacles(t_arr *cell, int cell_number, int button, t_param *param);
int		ft_get_index(int j, int i, int k);
int		ft_is_cell_obstacle(int **ground, int cell_number);
void	ft_fill_cells_from_ground(t_arr *cells, int **ground);
void	ft_print_all_cell(t_arr *cells, t_pict *pic, t_param *param);
void	ft_print_water_cell(t_arr *cells, t_pict *pic, t_param *param);
void	ft_move_water_cell(t_arr *cells, t_param *param);

/*
**	read_program.c
*/
int		is_read_programm(char *buffer, char *filename);



/*
** open_cl.c
*/
void	*ft_error(t_open_cl *cl, char *message);
void	pfn_notify2(cl_program program, void *user_data);
void	pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data);
void	ft_context_error(cl_int errcode_ret);
void	ft_queue_error(cl_int errcode_ret);
t_open_cl	*ft_init_open_cl(void);
int		ft_read_and_build_programs(t_open_cl *cl, t_prog *compile);
void	ft_prepare_to_compile(t_open_cl *cl, t_prog *compile, t_buff *buff);
void	ft_free_open_cl(t_open_cl **open_cl);
int		ft_create_buffers(t_open_cl *cl, int num, int need_wait);
int		ft_recreate_buffers(t_open_cl *cl, int num, int need_wait, size_t new_elem_count, int need_write);
int		ft_read_buffers(t_open_cl *cl, int num, int need_wait);
int		ft_write_buffers(t_open_cl *cl, int num, int need_wait);
void	ft_create_all_buffers(t_open_cl *cl);
int		ft_run_kernels(t_open_cl *cl);
int		ft_set_kernel_arg(t_open_cl *cl, t_prog *compile);
void	ft_stop_cl(t_open_cl *cl);


/*
**	thread_solver.c
*/
void	ft_create_thread_for_solver(t_solver *solver, t_open_cl *cl, t_param *param, t_prog *compile);
void	*ft_solver(void *param);

/*
**	earth.c
*/
t_earth	*ft_create_earth(void);
void	ft_del_earth(t_earth **earth);
int		ft_create_relief2(t_earth *earth, int  **ground);
void	ft_print_relief(t_earth *earth, t_arr *cells, t_pict *pic, t_param *param);

#endif
