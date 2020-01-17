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

#include "libft.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "mlx.h"
#include "ft_mod1_struct.h"

#include <time.h>

#define ABS(nbr) ((nbr) >= 0 ? (nbr) : (-1) * (nbr))

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
#define IMAX 40
#define JMAX 40
#define KMAX 40
#define I0 1
#define J0 1
#define K0 1
#define CELL_COUNT (KMAX + 1) * (JMAX + 1) * (IMAX + 1)

#define DELTA 1.0
#define DELTA_X DELTA
#define DELTA_Y DELTA
#define DELTA_Z DELTA

#define SURF '~'
#define EMPTY ' '
#define WATER '1'
#define BLOB 'b'
#define OBSTACLES '#'
#define BOUNDARY '\0'
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
#define CONST_B 2.0
//коэффициент линнейной искусственной вязкости (0.02 - 0.03)
#define CONST_A 1.08
//соотношение максимальной скорости частицы со скоростью звука ~ 10 раз
#define CONST_E 0.1
//коэффициент влияния
#define CONST_EP 0.3

#define VISCOSITY 010.0
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
char	***map;
int		***flags_surface;

t_part	****parts;
t_part	*g_cell;
t_arr	*g_parts;

t_dpoint delta;

t_dpoint g;

REAL norm_speed;
REAL max_c;
REAL min_h;

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

/*
**	main.c
*/
int		ft_znak(int num);
void	ft_print_lines(t_vis *vis, t_line *line);
void	ft_print_points(t_vis *vis, t_part *points);
void	ft_print_points2(t_vis *vis, t_vektr *points);
void	ft_create_cube_poligons(t_plgn **plgn, t_vektr **p, int color);
void	ft_create_obstacles(void *ptr, int j, int i, int k);
void	ft_cycle_cube(void *param, void (*f)(void *, int, int, int), t_point *start, t_point *end);
void	ft_mark_obstacles_on_map(void *param, int j, int i, int k);
void	ft_fill_map_from_ground(int **ground);
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
void	ft_del_all_print_error(char *msg_error);
void	ft_create_stable_level_of_water(void *param, int j, int i, int k);
void	ft_create_first_water(void);

/*
** initialization.c
*/
void	***ft_cube_arr(int jmax, int imax, int kmax, int size);
void	ft_init_variable(void);
void	ft_init_delta_xyz(void);
void	ft_init_map_arrs(void);
void	ft_initialization_of_global_variable(void);
void	ft_del_cube_arr(void ****arr);
void	ft_del_variable(void);

/*
**	speeds.c
*/
REAL	ft_laplasian(REAL ***speed, int j, int i, int k);
REAL	ft_vortex(REAL ***speed, int j, int i, int k);
void	ft_recalc_flows(void *param, int j, int i, int k);
void	ft_comp_fg(void);

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
**	surface.c
*/
/*
void	ft_set_uvp_surface(void);
void	ft_surface_speed_and_pressure(void *param, int j, int i, int k);
void	ft_surface_speed_u(int j, int i, int k);
void	ft_surface_speed_v(int j, int i, int k);
void	ft_surface_speed_w(int j, int i, int k);
*/

/*
**	rotate.c
*/
void	ft_norm_vektor(t_dpoint *vek);
void	ft_rotate_vek_around_vek_by_ang(t_dpoint *ox, t_dpoint *oy, double ang);
void	ft_change_points4(t_vis *vis, t_vektr *p, int rotate);
void	ft_rotate_xyz(t_oxyz *oxyz, t_dpoint *ang);
void	ft_ret_zoom_xyz(t_vektr *ox, t_vis *vis);
void	ft_change_points5(t_vis *vis, t_vektr *p);

/*
**	images.c
*/
int		ft_create_img(t_vis *vis);
int		ft_not_need_print(t_line *line, t_pict *pic);
void	ft_put_pixel_to_img2(t_pict *pic, t_point *p, int color);

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
void	ft_swap_points(t_vektr **p1, t_vektr **p2);
void	ft_sort_points_by_y(t_plgn *plgn);
void	ft_vektr_interpolation_by_y(t_vektr *p, t_vektr *p1, t_vektr *p2, int grad);
int		ft_need_print_traing(t_vektr **p, t_pict *pic);
void	ft_draw_traing(t_pict *pic, t_vektr **p, int grad, int color);
void	ft_print_plgn(t_plgn *plgn, t_pict *pic, int grad);
void	ft_print_poligons(t_vis *vis, t_plgn *plgn);
/*
**	keys.c
*/
int		ft_rotate_and_csale(t_vis *vis, int key);
int		ft_shift(t_vis *vis, int key);
int		deal_key(int key, void *param);

/*
**	point.c
*/
void	ft_fill_point(t_point *p, int y, int x, int z);
void	ft_fill_dpoint(t_dpoint *p, REAL y, REAL x, REAL z);
void	ft_create_xyz(t_vis *vis);

/*
**	time.c
*/
REAL	ft_time_control(REAL max_c, REAL norm_speed, REAL min_h);

/*
**	parts.c
*/
void	ft_create_new_water_in_cell(void *param, int j, int i, int k);
void	ft_replace_part(t_part *prev, t_part **part, t_part ****parts);
int		ft_part_change_cell(t_part *part);
void	ft_del_part(t_part **begin);
t_part	*ft_new_part(t_dpoint *p, int type);
t_part	*ft_add_part(void *ptr, t_dpoint *p, int type);
t_arr	*ft_init_all_clear_parts(void);
void	ft_create_new_area_of_water(t_arr **parts);
void	ft_insert_part(t_part *part);
void	ft_cut_part(t_part *part);
void	ft_replace_part2(t_part *part);
/*
**	map.c
*/
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


/*
** arr.c
*/
void	ft_del_arr(t_arr **arr);
t_arr	*ft_create_arr(int elem_size, int elems_count, void (*func_del)(void *));
t_arr	*ft_realloc_arr(t_arr **arr);
void	*ft_arr_add(t_arr **arr, void *elem);
void	*ft_arr_get(t_arr *arr, int num);
void	ft_del_elem(t_arr *arr, int num);
void	ft_del_elems(t_arr *arr, int (*need_del)(void *));
void	ft_for_each_ptr(t_arr *arr, void (*func)(void *, void *), void *param);
void	ft_for_each_elem(t_arr *arr, void (*func)(void *, void *), void *param);
void	*ft_arr_get_next(t_arr *arr);

/*
**	sph.c
*/
REAL	ft_derivative_kernel_function2(REAL h, REAL r);
REAL	ft_kernel_function5(REAL h, REAL r);
REAL	kernel_function_xyz(REAL ri, REAL rj, REAL h, int projection);
REAL	ft_norma_vektora(REAL x, REAL y, REAL z);
REAL	ft_distanse(t_dpoint *pi, t_dpoint *pj, REAL radius, int projection);
REAL	ft_kernel_function(t_dpoint *pi, t_dpoint *pj, REAL h, int projection);
REAL	ft_calc_fake_viscosity(t_part *p_i, t_part *p_j, t_dpoint *d_speed, t_dpoint *d_pos);
REAL	ft_return_fake_viscosity(t_part *p_i, t_part *p_j);
void	ft_use_function(t_part *begin, void *param, void (*f)(t_part *, void *));
void	ft_use_function2(t_part **begin, void *param, void (*f)(t_part *, t_part **, void *));
void	ft_comparison_part_with_list(t_part *part1, t_part *begin2, void *param, void (*f)(void *, t_part *, t_part *));
void	ft_comparison_part_with_lists(t_part *part, t_part ***surround, void *param, void (*f)(void *, t_part *, t_part *));
void	ft_comparison_list_with_lists(t_cpart *cell, void *param, void (*f)(void *, t_part *, t_part *));
void	ft_fill_surrounding_of_cell_by_j_i(t_cpart *cell, int j, int i, int k);
//void	ft_first_density(void *param, t_part *p_i, t_part *part_j);
void	ft_fill_param_of_part(t_part *part, void *param);
void	ft_init_density2(void *p_i, void *param);
void	ft_init_parts(void *param, int j, int i, int k);
void	ft_recalk_delta_density(void *param, t_part *part_i, t_part *part_j);
void	ft_change_density(t_part *part, void *param);
void	ft_new_density(void *param, int j, int i, int k);
void	ft_recalk_pressure(t_part *part, void *param);
void	ft_new_pressure(void *param, int j, int i, int k);
void	ft_recalk_delta_speed(void *param, t_part *part_i, t_part *part_j);
void	ft_change_speeds(void *part, void *param);
void	ft_new_speeds(void *p_i, void *param);
void	ft_change_coordinates(t_part *part, void *param);
void	ft_new_coordinates(void *p_i, void *param);
void	ft_recalk_delta_coord(void *param, t_part *part_i, t_part *part_j);
void	ft_solve_and_move_parts(void);
void	ft_init_first_value_of_part_parameters(void);
#endif
