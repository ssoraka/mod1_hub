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

# include "../libft/libft.h"
# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <OpenCL/opencl.h>
# include <time.h>
# include "../libs/minilibx_macos/mlx.h"
# include "ft_cl.h"
# include "ft_cl_struct.h"
# include "ft_mod1_struct.h"
# include "ft_buttons.h"

/*
**	добавляем по ADD_POINT вершины между вершинами расчетной модели для сплайна
*/
# define ADD_POINT 4
# define DEVICE_TYPE CL_DEVICE_TYPE_GPU
//# define DEVICE_TYPE CL_DEVICE_TYPE_CPU

# define MAP_KOEF 0.6
# define HEIGTH_KOEF 0.8
//# define MAP_HEIGTH2 ((JMAX + J0) * 100)
# define COLOR_UP 0xFFFFFF
# define COLOR_DOWN 0x704214

# define WATER_COLOR 0xFFFF
# define MAGMA_COLOR 0xFF0000
# define OBSTACLES_FRONT_COLOR 0x5b432d

// 0.05 / 180.0
# define ROTATE_ANGLE 0.0003
# define CONST_WIDTH 2000
# define CONST_HEINTH 1360
# define CAM_X 1000
# define CAM_Y 680

# define RADIUS 3

# define DEFAULT_INDEX 0

# define PIXEL 2
# define INDEX 4
# define CHECK_Z_BUFFER 8
# define MARK_Z_BUFFER 16
# define GRADIENT 32

//# define PARTICLE		( PIXEL | CHECK_Z_BUFFER )
//# define DEFAULT_POINT	( PIXEL | INDEX | CHECK_Z_BUFFER | MARK_Z_BUFFER )
//# define DEFAULT_IMAGE	( PIXEL | CHECK_Z_BUFFER | MARK_Z_BUFFER )

# define PARTICLE		0b001010
# define DEFAULT_POINT	0b011110
# define DEFAULT_IMAGE	0b011010

# define PROGRAMM_SIZE 5000

typedef pthread_mutex_t	t_mut;
typedef t_dpoint		t_dpnt;
typedef t_open_cl		t_op_cl;

extern int						**g_ground;
extern int						**g_comlex_ground;
extern t_mut					g_mutex;
extern t_arr					*g_cell;
extern t_arr					*g_parts;
extern t_arr					*g_iparts;
extern t_arr					*g_iparts_copy;
extern t_arr					*g_cell_map;
extern t_arr					*g_neighs;
extern t_arr					*g_cl_prop;
extern t_open_cl				*g_cl;
extern t_earth					*g_earth;
extern t_vis					*g_vis;
extern t_solver				g_solver;
extern t_prog					g_compile[PROGRAMS_COUNT + 2];

typedef enum e_print
{
	COLOR = 1,
	RADIUS2,
	COLUMN_COUNT2
}				t_print;

typedef enum e_column
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

typedef enum e_form
{
	POINT,
	RECTANGLE,
	CIRCLE,
	IMAGE
}				t_form;

typedef enum e_earth_view
{
	SMOOTH_EARTH,
	CUB_EARTH,
	COLORED_EARTH,
	V_EARTH_COUNT
}				t_earth_view;

typedef enum e_water_view
{
	SIMPLE_WATER,
	BALL_WATER,
	HOLLOW_BALL_WATER,
	V_WATER_COUNT
}				t_water_view;

typedef enum e_stat_g
{
	OFF_UPDATING_PARAM,
	NEED_STOP_UPDATING,
	ON_UPDATING_PARAM,
	LAST_UPDATE_PARAM_ON_CL,
	NEED_UPDATE_PARAM_ON_CL,
	PARAM_UPDATED
}				t_stat_g;

/*
**	main.c
*/
void	ft_prepare_one_buffer(t_buff *buff);

/*
** initialization.c
*/
void	ft_initialization_of_global_variable(void);
int		ft_del_all(char *message);
void	ft_init(void);

/*
**	color_interpolation.c
*/
int		ft_interpolation(int percent, int color1, int color2, int byte);
int		ft_grad_color(int delta1, int delta2, int color1, int color2);
int		ft_set_color_to_point(t_line *line, t_point *p, int lower_45);
int		ft_int_interpolation(int y, int delta_y, int x1, int x2);

/*
**	rotate.c
*/
void	ft_rotate_vek_around_vek_by_ang(t_dpoint *ox, t_dpoint *oy, double ang);
void	ft_rotate_point_around_point(t_param *param, t_vektr *p);
void	ft_rotate_oxyz_around_v(t_param *param, t_dpoint *v, t_real ang);
t_dpnt	ft_rot_dpoint(t_dpoint *v, t_oxyz *oxyz);

/*
**	images.c
*/
int		ft_create_img(t_pict *pic, void *mlx, int width, int heigth);
t_vis	*ft_create_mlx(int width, int heigth, char *name);
void	ft_destroy_img(t_pict *pic);
t_vis	*ft_destroy_mlx(t_vis **vis);

/*
**	picture.c
*/
void	ft_clear_image(t_pict *pic);
void	ft_save_image(t_pict *pic);
void	ft_return_image(t_pict *pic);

/*
**	sprite.c
*/
t_bool	ft_init_picture(t_pict *pict, t_param *param, int color);

/*
**	lines_vektrs.c
*/
void	draw_line_img(t_line *line, t_pict *pic, t_prop prop);

/*
**	poligons.c
*/
void	ft_sort_points_by_y(t_vektr **p);
void	ft_vektr_interpolation_by_y(t_vektr *p, t_vektr *p1, t_vektr *p2,
			int grad);
int		ft_need_print_traing(t_vektr **p);
void	ft_print_plgn(t_plgn *plgn, t_pict *pic, t_prop prop);

/*
**	poligons2.c
*/
void	ft_draw_traing(t_pict *pic, t_vektr **p, t_prop prop, int grad);
void	ft_prepare_plgn_for_printing(t_plgn *plgn, t_param *param);

/*
**	keys.c
*/
int		ft_rotate_and_csale(t_param *vis, int key);
int		ft_shift(t_param *vis, int key);
int		deal_key(int key, void *param);

/*
**	keys2.c
*/
int		ft_change_brush(t_param *param, int key);
void	change_gravity(t_param *param);
int		ft_csale_picture(t_param *param, int button, t_point *mouse);

/*
**	point.c
*/
void	ft_fill_point(t_point *p, int y, int x, int z);
void	ft_fill_dpoint(t_dpoint *p, t_real y, t_real x, t_real z);
t_real	ft_vektr_len(t_dpoint *a);
t_real	ft_vekt_cos(t_dpoint a, t_dpoint b);
t_real	ft_dot_dpoints(t_dpoint *a, t_dpoint *b);

/*
**	point2.c
*/
void	ft_normalize_vektor(t_dpoint *vek);
void	ft_create_xyz(t_oxyz *oxyz);
void	ft_rotate_xyz_around_v(t_oxyz *oxyz, t_dpoint *v, double ang);
t_dpnt	ft_ret_norm(t_dpoint *a, t_dpoint *b, t_dpoint *c);

/*
**	parts.c
*/
void	ft_create_new_water_in_cell(void *param, int j, int i, int k);
t_arr	*ft_init_all_clear_parts(void);
void	ft_create_new_area_of_water(t_arr *parts,
			t_point *start, t_point *end, t_fluids type);

/*
**	map.c
*/
t_real	ft_return_heigth(t_real value);
int		**ft_read_ground_from_file(char *name);

/*
** map_math.c
*/
int		ft_find_points(t_map *map, char *arr);
void	ft_create_first_and_last_points(t_map *map);
void	ft_superposition(t_map *map, int i, int k);

/*
**	ground.c
*/
int		**ft_create_complex_ground_from_simple(int **simple_ground);

/*
**	cells.c
*/
int		ft_get_index(int j, int i, int k);
t_point	ft_get_index_d3(int cell_number);
int		ft_is_cell_obstacle(int **ground, int cell_number);
void	ft_fill_cells_from_ground(t_arr *cells, int **ground);
void	ft_move_water_cell(t_arr *cells, t_param *param);

/*
**	cell_change.c
*/
int		ft_is_need_print_cell(t_arr *cells, int j, int i, int k);
void	ft_print_all_cell(t_arr *cells, t_pict *pic, t_param *param);
void	ft_print_water_cell(t_arr *cells, t_pict *pic, t_param *param);

/*
**	cell_change.c
*/
void	ft_del_cell(void *ptr, int j, int i, int k);
void	ft_add_cell(void *ptr, int j, int i, int k);
void	ft_mark_extern_cell(void *ptr, int j, int i, int k);
void	ft_mark_cell_as_water(void *ptr, int j, int i, int k);
int		ft_change_obstacles(t_arr *cell, int cell_number, int button,
			t_param *param);

/*
**	cell_print.c
*/
void	print_one_cell(t_point cell, t_pict *pic, t_param *param, t_prop prop);

/*
**	open_cl_buffers.c
*/
int		ft_create_all_buffers(t_open_cl *cl);
int		ft_create_buffers(t_open_cl *cl, int num);
int		ft_write_buffers(t_open_cl *cl, int num, int need_wait);
int		ft_read_buffers(t_open_cl *cl, int num, int need_wait);
int		recreate_buffers(t_open_cl *cl, int num, size_t new_elem_count,
			int need_write);

/*
**	open_cl_read_build_run.c
*/
int		is_read_file(char *buffer, char *filename, unsigned int size);
int		ft_read_and_build_programs(t_open_cl *cl, t_prog *compile);
int		ft_run_kernels(t_open_cl *cl);
int		ft_set_kernel_arg(t_open_cl *cl, t_prog *compile);

/*
**	open_cl_init.c
*/
t_op_cl	*ft_init_open_cl(int device);

/*
**	open_cl_free.c
*/
void	ft_free_open_cl(t_open_cl **open_cl);
void	ft_stop_cl(t_open_cl *cl);

/*
**	thread_solver.c
*/
void	ft_create_thread_for_solver(t_solver *solver, t_open_cl *cl,
			t_param *param, t_prog *compile);
void	*ft_solver(void *param);

/*
**	earth.c
*/
t_earth	*ft_create_earth(void);
void	ft_del_earth(t_earth **earth);

/*
**	print_smooth_earth.c
*/
void	ft_print_relief(t_earth *earth, t_arr *cells,
			t_pict *pic, t_param *param);

/*
**	create_smooth_earth.c
*/
int		ft_create_relief(t_earth *earth, int **ground);

/*
**	print_shapes.c
*/
void	ft_init_shape(t_shape *shape, t_form form, t_prop print);
t_prop	set_param(size_t params, size_t index, int color);

/*
**	shapes.c
*/
t_bool	ft_put_pixel_to_img2(t_pict *pic, t_point *p, t_shape *shape);
t_bool	ft_print_rect(t_pict *pic, t_point *center, t_shape *shape);
t_bool	circle(t_pict *pic, t_point *center, t_shape *shape);
t_bool	print_img_to_img(t_pict *pic, t_point *center, t_shape *shape);

/*
** animation.c
*/
int		ft_move_camera(t_param *param);
int		ft_is_cam_moved(t_param *param);
int		ft_is_cam_rotated(t_param *param);
int		ft_auto_rotate(t_param *param);

/*
** hooks.c
*/
int		loop_hook(void *parameters);
void	ft_init_hooks(t_vis *vis);

/*
** print.c
*/
void	ft_refresh_picture(t_vis *vis, t_arr *cell, t_arr *ipart);

/*
** mouse.c
*/
int		ft_mouse_on_window(int x, int y);
int		ft_mouse_get_new_pos(int x, int y, t_param *param);

/*
** validator.c
*/
t_bool	ft_get_nums_from_string(char **string, t_point *p);

/*
** helper.c
*/
void	ft_prepare_one_buffer(t_buff *buff);
void	ft_init_buffers(t_buff *buff, t_arr *arr);
t_bool	ft_copy_arrs(t_arr *dst, t_arr *src);
t_bool	del_elem(void *elem, void *param);
void	ft_cycle_cube(void *param, void (*f)(void *, int, int, int),
			t_point *start, t_point *end);

#endif
