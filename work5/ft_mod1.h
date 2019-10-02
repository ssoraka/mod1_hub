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
#define P_CONST 30.0
#define U_CONST 0.0
#define W_CONST 1.7
//#define CONST_RE 0.105
#define CONST_RE 1.1
#define CONST_GY 3.0
#define CONST_GX 0.0
#define T_DELTA 0.10
#define T_END 5.0
#define TAU 0.2
#define MAX_ITERATIONS 30
#define TOLERANCE 0.001
#define DELTA 32.0
#define DELTA_X DELTA
#define DELTA_Y DELTA
#define DELTA_X2 (DELTA_X * DELTA_X)
#define DELTA_Y2 (DELTA_Y * DELTA_Y)
#define DELTA_XY (DELTA_Y * DELTA_X)

#define SURF '~'
#define EMPTY ' '
#define WATER '1'
#define BLOB 'b'
#define OBSTACLES '#'
#define PARTS_COUNT 4


#define D_N		0b00000001
#define D_S		0b00000010
#define D_W		0b00000100
#define D_E		0b00001000


#define C_B		0b00000000     /* interior obstacle cells */
#define C_F		0b00010000     /* fluid cell */
#define C_R		0b00100000    /* surface */
#define C_X		0b01000000    /* blob */
#define C_A		0b10000000    /* empty celle */


#define C_N		(D_S << 8) /* adjacent to empty cells     */
#define C_S		(D_N << 8)	/* in the respective direction */
#define C_W		(D_W << 8)	 /* free surface cells          */
#define C_E		(D_E << 8)

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


#define B_N		(D_S)  /* obstacle cells adjacent to fluid cells */
#define B_S		(D_N)  /* in the respective direction            */
#define B_W		(D_W)
#define B_E		(D_E)
#define B_NE	(B_N + B_E)
#define B_SE	(B_S + B_E)
#define B_NW	(B_N + B_W)
#define B_SW	(B_S + B_W)


#define SURF_MASK		(C_N | C_S | C_W | C_E)
#define WATER_MASK		(B_N | B_S | B_W | B_E)

#define INTERIOR_WATER	(WATER_MASK | C_F)

char **ground;
char ***map;
int ***flags;
int ***flags_surface;
int ***parts;

REAL ***speed_v;
REAL ***speed_u;
REAL ***speed_w;

REAL ***lapl_u;
REAL ***lapl_v;
REAL ***lapl_w;

REAL ***flow_f;
REAL ***flow_g;
REAL ***flow_m;

REAL ***rhs;
REAL ***press_p;
REAL ***tmp;

REAL dx;
REAL dy;
REAL dz;

int imax;
int jmax;
int kmax;

/*
void	ft_print_fluid(t_fluid *fluid, REAL **arr);
void	ft_print_flags2(t_fluid *fluid, int **arr, int mask);
void	ft_print_flags(t_fluid *fluid, int **arr);
void	ft_arr_set(REAL **arr, int columns, REAL value);
void	ft_fill_iterations(t_iter *iter, int j, int jmax, int i, int imax);
void	ft_iteration_i(void *ptr, void (*f)(void *, int, int), t_iter *iter);
void	ft_iteration_j(void *ptr, void (*f)(void *, int, int), t_iter *iter);
void	ft_iteration(void *ptr, void (*f)(void *, int, int), t_iter *iter);
void	ft_fill_watercell_in_center(void *ptr, int j, int i);
void	ft_pressure_in_obstacle(REAL **press, int j, int i, int flag);
void	ft_speed_u_in_obstacle(REAL **speed, int j, int i, int flag);
void	ft_speed_v_in_obstacle(REAL **speed, int j, int i, int flag);
void	ft_fill_obstacle_in_center(void *ptr, int j, int i);
void	ft_top_boundary(void *ptr, int j, int i);
void	ft_down_boundary(void *ptr, int j, int i);
void	ft_left_boundary(void *ptr, int j, int i);
void	ft_right_boundary(void *ptr, int j, int i);
void	ft_inisialization(t_fluid *fluid);
void	ft_fill_boundary_value(t_fluid *fluid);
void	ft_flow_f_and_flow_g(void *ptr, int j, int i);
void	ft_flow_f_or_flow_g(void *ptr, int j, int i);
void	ft_right_hand_side(void *ptr, int j, int i);
void	ft_residual_pressure_center(void *ptr, int j, int i);
void	ft_check_params(t_fluid *fluid, int *ewns, int i, int j);
void	ft_residual_pressure_boundary(void *ptr, int j, int i);
void	ft_boundary_aproximation_press(void *ptr, int j, int i);
void	ft_aproximation_press(void *ptr, int j, int i);
void	ft_successive_overrelaxation(t_fluid *fluid);
void	ft_flows(t_fluid *fluid);
void	ft_max_speed(t_fluid *fluid, REAL speed_u, REAL speed_v);
void	ft_speed_u_and_speed_v(void *ptr, int j, int i);
void	ft_speed_u_or_speed_v(void *ptr, int j, int i);
void	ft_new_velocity(t_fluid *fluid);
char	**ft_create_map(int jmax, int imax);
int		**ft_create_flags(t_fluid *fluid);
t_fluid	*ft_initialization(char **map, int jmax, int imax);
void	ft_solver(t_fluid *fluid);
int		ft_is_water(int flag);
int		ft_is_interior_water(int flag);
int		ft_is_surface(int flag);
int		ft_is_empty(int flag);
*/
/*
void	ft_put_pixel_to_img(int *addr, int x, int y, int color);
int		ft_interpolation(int percent, int color1, int color2, int byte);
int		ft_grad_color(int delta1, int delta2, int color1, int color2);
void	ft_set_color_to_line(t_line *line, int lower_45);
void	draw_line_img_lower_45(t_line *line, int *addr);
void	draw_line_img_over_45(t_line *line, int *addr);
int		draw_line_img(t_line *line, int *addr);
t_vektr	*ft_new_vektor(int x, int y, int z);;
void	ft_norm_vektor(t_vektr *vek);
t_vektr	*ft_rot(t_vektr *ox, t_vektr *oy, double ang);
t_vektr	*ft_new_vektor(int x, int y, int z);
t_line	*ft_new_line(t_vektr *p1, t_vektr *p2, int color);
void	ft_ret_abs_xyz(t_vektr *ox, t_vis *vis);
void	ft_rotate_xyz(t_vis *vis);
void	ft_change_points(t_vis *vis);
void	ft_print_lines(t_vis *vis);
int		deal_key(int key, void *param);
void	ft_add_line(t_line **begin, t_vektr *p1, t_vektr *p2, int color);
int		ft_read_map_from_file2(char *name);
t_vektr	*ft_add_vektor(void *ptr, int x, int y, int z);
void	ft_create_xyz(t_vis *vis);
void	ft_create_four_lines_of_cell(void *ptr, int i, int j);
void	ft_create_water_in_cell(void *ptr, int i, int j);
void	ft_create_points_in_cells(t_vis *vis);
int		ft_create_img(t_vis *vis);
void	ft_print_points(t_vis *vis, t_vektr **points);
int		loop_hook(void *param);
void	ft_recalk_parts(void *param);
*/
#endif
