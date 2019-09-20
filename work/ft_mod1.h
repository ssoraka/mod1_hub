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
#define P_CONST 1000.0
#define U_CONST 2
#define W_CONST 1.0
#define CONST_RE 100.0
#define CONST_GY 0
#define CONST_GX 0
#define T_DELTA 1
#define T_END 2.0
#define TAU 0.7
#define MAX_ITERATIONS 30
#define TOLERANCE 0.001
#define DELTA 10
#define DELTA_X DELTA
#define DELTA_Y DELTA
#define DELTA_X2 (DELTA_X * DELTA_X)
#define DELTA_Y2 (DELTA_Y * DELTA_Y)
#define DELTA_XY (DELTA_Y * DELTA_X)

#define C_B		0b000000
#define D_N		0b000001
#define D_S		0b000010
#define D_W		0b000100
#define D_E		0b001000
#define C_F		0b010000
#define C_A		0b100000


#define B_N		(D_S) //поменял для отображения
#define B_S		(D_N) //поменял для отображения
#define B_W		(D_W)
#define B_E		(D_E)
#define B_NE	(D_N + D_E)
#define B_SE	(D_S + D_E)
#define B_WE	(D_W + D_E)
#define B_NW	(D_N + D_W)
#define B_SW	(D_S + D_W)
#define B_EW	(D_E + D_W)
#define B_NS	(D_N + D_S)
#define B_WS	(D_W + D_S)
#define B_ES	(D_E + D_S)

void	ft_print_fluid(t_fluid *fluid, REAL **arr);
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
void	ft_fill_watercell(t_fluid *fluid);
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
int		**ft_create_flags(char **map, int jmax, int imax);
t_fluid	*ft_initialization(char **map, int jmax, int imax);
void	ft_solver(t_fluid *fluid);


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
void	ft_add_line(t_line **begin, t_vektr *p1, t_vektr *p2);
int		ft_read_map_from_file(char *name, t_inf **inf);
t_vektr	*ft_add_vektor(void *ptr, int x, int y, int z);
void	ft_create_xyz(t_vis *vis);
void	ft_create_four_lines_of_cell(void *ptr, int i, int j);
void	ft_create_water_in_cell(void *ptr, int i, int j);
void	ft_create_points_in_cells(t_vis *vis);
int		ft_create_img(t_vis *vis);
void	ft_print_points(t_vis *vis, t_vektr *points);
int		loop_hook(void *param);
void	ft_recalk_parts(void *param);

#endif
