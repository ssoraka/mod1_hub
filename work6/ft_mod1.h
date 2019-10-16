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

#define ABS(nbr) ((nbr) >= 0 ? (nbr) : (-1) * (nbr))

#define T_INFINITY 10000000000.0
#define P_CONST 0.0
#define U_CONST 0.0
#define W_CONST 1.7
//#define CONST_RE 0.105
#define CONST_RE 100.11
#define CONST_GY -2.00
#define CONST_GX 0.0
#define CONST_GZ 0.0
#define T_DELTA 0.1
#define T_END 5.0
#define TAU 0.2
#define MAX_ITERATIONS 50
#define TOLERANCE 0.001

#define MAX_POINT 50
#define IMAX 50
#define JMAX 50
#define KMAX 50

#define DELTA 32.0
#define DELTA_X DELTA
#define DELTA_Y DELTA


#define SURF '~'
#define EMPTY ' '
#define WATER '1'
#define BLOB 'b'
#define OBSTACLES '#'
#define BOUNDARY '\0'
#define PARTS_COUNT 4
#define TEST_WATER_LEVEL 8
#define TEST_WATER_WALL 25
#define MAP_HEIGTH 50
#define WATER_COLOR 0xFFFF
#define OBSTACLES_TOP_COLOR 0x704214
#define OBSTACLES_FRONT_COLOR 0x5b432d
//#define OBSTACLES_RIGHT_COLOR 0xFF
#define OBSTACLES_RIGHT_COLOR 0x654321

#define D_N		0b000000000001
#define D_S		0b000000000010
#define D_W		0b000000000100
#define D_E		0b000000001000
#define D_O		0b000000010000
#define D_I		0b000000100000


#define C_B		0b000010000000     /* interior obstacle cells */
#define C_F		0b000100000000     /* fluid cell */
#define C_R		0b001000000000   /* surface */
#define C_X		0b010000000000    /* blob */
#define C_A		0b100000000000    /* empty cell */


#define C_N		(D_N << 12)	/* adjacent to empty cells     */
#define C_S		(D_S << 12)	/* in the respective direction */
#define C_W		(D_W << 12)	 /* free surface cells          */
#define C_E		(D_E << 12)
#define C_O		(D_O << 12)
#define C_I		(D_I << 12)

//NSWE
#define C_NS	(C_N + C_S)
#define C_NW	(C_N + C_W)
#define C_NE	(C_N + C_E)
#define C_SW	(C_S + C_W)
#define C_SE	(C_S + C_E)
#define C_WE	(C_W + C_E)
#define C_NSW	(C_N + C_S + C_W)
#define C_NSE	(C_N + C_S + C_E)
#define C_NWE	(C_N + C_W + C_E)
#define C_SWE	(C_S + C_W + C_E)
#define C_NSWE	(C_N + C_S + C_W + C_E)


#define B_N		(D_N)  /* obstacle cells adjacent to fluid cells */
#define B_S		(D_S)  /* in the respective direction            */
#define B_W		(D_W)
#define B_E		(D_E)
#define B_O		(D_O)
#define B_I		(D_I)
#define B_NE	(B_N + B_E)
#define B_SE	(B_S + B_E)
#define B_NW	(B_N + B_W)
#define B_SW	(B_S + B_W)


#define SURF_MASK		(C_N | C_S | C_W | C_E | C_O | C_I)
#define WATER_MASK		(B_N | B_S | B_W | B_E | B_O | B_I)

#define INTERIOR_WATER	(WATER_MASK | C_F)


#define MSG_ERROR1 "Malloc error\n"



char **ground;
char ***map;
int ***flags;
int ***flags_surface;

t_vektr ****parts;

REAL ***speed_v;
REAL ***speed_u;
REAL ***speed_w;

REAL ***flow_f;
REAL ***flow_g;
REAL ***flow_h;

REAL ***press_p;

REAL dx;
REAL dy;
REAL dz;

REAL dx2;
REAL dy2;
REAL dz2;

REAL dxy;
REAL dxz;
REAL dyz;

REAL gy;
REAL gx;
REAL gz;

REAL max_u;
REAL max_v;
REAL max_w;

REAL renolds;

REAL deltat;

int imax;
int jmax;
int kmax;
int iteration;

t_vis *vis;

/*
**	main.c
*/
int		ft_znak(int num);
void	ft_fill_point(t_point *p, int y, int x, int z);
void	ft_put_pixel_to_img2(t_pict *pic, t_point *p, int color);
int		ft_interpolation(int percent, int color1, int color2, int byte);
int		ft_grad_color(int delta1, int delta2, int color1, int color2);
int		ft_set_color_to_point(t_line *line, t_point *p, int lower_45);
int		ft_int_interpolation(int y, int delta_y, int x1, int x2);
void	draw_line_img_lower_452(t_line *line, t_point *p, t_pict *pic, int grad);
void	draw_line_img_over_452(t_line *line, t_point *p, t_pict *pic, int grad);
int		ft_not_need_print(t_line *line, t_pict *pic);
void	draw_line_img2(t_line *line, t_pict *pic, int grad);
void	ft_norm_vektor(t_vektr *vek);
void	ft_rot2(t_vektr *ox, t_vektr *oy, double ang);
t_line	*ft_new_line(t_vektr *p1, t_vektr *p2, int color);
void	ft_ret_abs_xyz(t_vektr *ox, t_vis *vis);
void	ft_rotate_xyz(t_vis *vis);
void	ft_change_points4(t_vis *vis, t_vektr *p, int rotate);
void	ft_change_points5(t_vis *vis, t_vektr *p);
void	ft_print_rect2(t_pict *pic, t_point *center, int len, int color);
void	plot_circle2(t_pict *pic, t_point *p, t_point *center, int color_code);
void	circle2(t_pict *pic, t_vektr *center, int radius, int color_code);
void	ft_print_lines(t_vis *vis, t_line *line);
int		ft_rotate_and_csale(t_vis *vis, int key);
int		ft_shift(t_vis *vis, int key);
int		deal_key(int key, void *param);
void	ft_add_line(t_line **begin, t_vektr *p1, t_vektr *p2, int color);
int		ft_read_ground_from_file2(char *name);
void	ft_create_xyz(t_vis *vis);
t_vektr	*ft_new_vektor2(int x, int y, int z, int color);
t_vektr	*ft_add_vektor2(void *ptr, t_point *p, int color);
void	ft_create_lines(t_line **lines, t_vektr **p, int color);
t_plgn	*ft_create_poligon(t_vektr *p1, t_vektr *p2, t_vektr *p3, int color);;
void	ft_create_rectang_poligon(t_plgn **plgn, t_vektr **p, int color);
void	ft_create_cube_poligons(t_plgn **plgn, t_vektr **p, int color);
void	ft_create_obstacles(void *ptr, int j, int i, int k);
void	ft_create_new_water_in_cell(void *param, int j, int i, int k);
int		ft_create_img(t_vis *vis);
void	ft_cycle_cube(void *param, void (*f)(void *, int, int, int), t_point *start, t_point *end);
void	ft_mark_obstacles_on_map(void *param, int j, int i, int k);
void	ft_fill_map_from_ground(char **ground);
void	ft_create_points_in_cells(t_vis *vis);
void	ft_swap_points(t_vektr **p1, t_vektr **p2);
void	ft_sort_points_by_y(t_plgn *plgn);
void	ft_vektr_interpolation_by_y(t_vektr *p, t_vektr *p1, t_vektr *p2, int grad);
int		ft_need_print_traing(t_vektr **p, t_pict *pic);
void	ft_draw_traing(t_pict *pic, t_vektr **p, int grad, int color);
void	ft_print_plgn(t_plgn *plgn, t_pict *pic, int grad);
void	ft_print_poligons(t_vis *vis, t_plgn *plgn);
t_plgn	*ft_create_poligon(t_vektr *p1, t_vektr *p2, t_vektr *p3, int color);
void	ft_refresh_picture(t_vis *vis);
int		loop_hook(void *param);
void	ft_del_all_print_error(char *msg_error);
void	ft_del_lines(t_line **begin);
void	ft_del_vektor(t_vektr **begin);

void	ft_print_all_water(t_vis *vis);
void	ft_print_water_in_cell(void *param, int j, int i, int k);
void	ft_print_points(t_vis *vis, t_vektr *points);


void	ft_print_arr(void *arr, void (*f)(void *, int, int, int), int k);
void	ft_print_real(void *param, int j, int i, int k);
void	ft_print_char(void *param, int j, int i, int k);
void	ft_print_int(void *param, int j, int i, int k);

/*
** initialization.c
*/
void	***ft_cube_arr(int jmax, int imax, int kmax, int size);
void	ft_init_variable(void);
void	ft_init_delta_xyz(void);
void	ft_init_map_arrs(void);
void	ft_fill_first_value(void *param, int j, int i, int k);
void	ft_init_first_value(void);
void	ft_initialization(void);
void	ft_del_cube_arr(void ****arr);
void	ft_del_variable(void);
/*
**	mark_flags.c
*/
void	ft_fill_boundary_flags(void);
void	ft_mark_in_out(void *param, int j, int i, int k);
void	ft_mark_left_right(void *param, int j, int i, int k);
void	ft_mark_top_down(void *param, int j, int i, int k);
int		ft_set_flag_for(char map);

void	ft_is_water_surround(int j, int i, int k);
void	ft_is_empty_surround(int j, int i, int k);
void	ft_mark_cell(void *param, int j, int i, int k);
void	ft_remark_flags();
/*
**	adap_uvw.c
*/
void	ft_comp_uvw(void *, int j, int i, int k);
void	ft_adap_uvw(void);
/*
**	clear_map.c
*/
void	ft_clear_cell_from_water(void *param, int j, int i, int k);
void	ft_clear_map_from_water(void);
/*
**	poisson.c
*/
void	ft_aproximation_press(void *param, int j, int i, int k);
void	ft_successive_overrelaxation(void);
void	ft_poisson(void);
/*
**	speeds.c
*/
REAL	ft_laplasian(REAL ***speed, int j, int i, int k);
REAL	ft_vortex(REAL ***speed, int j, int i, int k);
void	ft_recalc_flows(void *param, int j, int i, int k);
void	ft_comp_fg(void);
/*
**	setbecond.c
*/
void	ft_setbcond(void);
void	ft_fill_obstacles(void *param, int j, int i, int k);
void	ft_fill_obstacles_pressure(void *param, int j, int i, int k);
void	ft_fill_obstacles_speed_u(void *param, int j, int i, int k);
void	ft_fill_obstacles_speed_v(void *param, int j, int i, int k);
void	ft_fill_obstacles_speed_w(void *param, int j, int i, int k);
/*
**	surface.c
*/
void	ft_set_uvp_surface(void);
void	ft_surface_speed_and_pressure(void *param, int j, int i, int k);
void	ft_surface_speed_u(int j, int i, int k);
void	ft_surface_speed_v(int j, int i, int k);
void	ft_surface_speed_w(int j, int i, int k);
/*
**	solver_3d.c
*/
void	ft_solver(void);
int		ft_is_empty(int flag);
int		ft_is_obstacle(int flag);
int		ft_is_water(int flag);
int		ft_is_surface(int flag);
int		ft_is_interior_water(int flag);
REAL	ft_time_control(void);
void	ft_clear_old_cell(void);
void	ft_clear_all_params(void *param, int j, int i, int k);
/*
**	parts.c
*/
void	ft_create_new_water_in_cell(void *param, int j, int i, int k);
void	ft_recalk_parts(void);
void	ft_mark_water_on_map(void *param, int j, int i, int k);
void	ft_move_parts(void *param, int j, int i, int k);
void	ft_replace_part(t_vektr **begin, t_vektr *prev, t_vektr **p, t_point *jik);
void	ft_move_part(t_vektr *part, t_point *jik, REAL deltat);
#endif
