/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mod1_struct.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 20:59:39 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/19 13:13:30 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MOD1_STRUCT_H
# define FT_MOD1_STRUCT_H

# define MAX_POINT 50
# define MAX_NEIGHBORS1 50

#include "ft_mod1.h"

typedef struct		s_map
{
	int				count;
	int				delta;
	t_point			p[MAX_POINT];
	t_point			p_max;
	t_point			p_min;
	t_point			first;
	int				**arr;
}					t_map;

typedef struct		s_vektr
{
	int				color;
	struct s_dpoint	abs;
	struct s_point	zoom;
}					t_vektr;

typedef struct		s_line
{
	struct s_point	dir;
	struct s_point	delta;
	struct s_vektr	*p1;
	struct s_vektr	*p2;
	struct s_line	*next;
	int				index;
}					t_line;


typedef struct		s_plgn
{
	struct s_vektr	*p[4];
	struct s_dpoint	n;
	struct s_dpoint	rot_n;
	REAL cos;
}					t_plgn;

typedef struct		s_pict
{
	void			*img;
	int				*addr;
	int				*index;
	int				*z_buffer;
	int				*addr_copy;
	int				*z_buffer_copy;
	int				bits_per_pixel;
	int				size_line;
	int				count_line;
	int				count_byte;
	int				endian;
}					t_pict;


typedef struct		s_oxyz
{
	struct s_dpoint	oy;
	struct s_dpoint	ox;
	struct s_dpoint	oz;
}					t_oxyz;


typedef struct		s_param
{
	int				cam_x;
	int				cam_y;
	int				target_x;
	int				target_y;

	int				pause;
	int				exit;
	int				rain;
	int				brush;
	int				is_obstacles_change;
	int				is_smooth_relief;
	int				is_need_print_obstacles;
	int				is_relief_changed;
	int				is_printing;
	int				is_reading;
	int				is_water_change;
	int				is_need_print_water;
	int				grad;
	double			len;

	struct s_point	mouse;

	int				right_button_press;
	struct s_point	pos;

	int				left_button_press;
	int				rounds;
	struct s_point	rot_pos;
	struct s_point	first_pos;

	int				active_id;
	struct s_vektr	centr;

	struct s_dpoint	light;
	struct s_dpoint	ang;
	struct s_dpoint	cos;
	struct s_point	water;
	struct s_oxyz	oxyz;
}					t_param;


typedef struct		s_vis
{
	struct s_param	param;
	struct s_pict	pic;
	struct s_pict	pict;
	void			*mlx;
	void			*win;
}					t_vis;

typedef struct		s_solver
{
	pthread_t		tid;
	pthread_attr_t	attr;
	t_open_cl		*cl;
	struct s_prog	*compile;
	t_param			*param;
}					t_solver;

typedef struct		s_earth
{
	t_arr			*points;
	t_arr			*poligons;
	int				rows;
	int				columns;
}					t_earth;

typedef struct		s_prog
{
	char			file[50];
	char			kernel[50];
	unsigned int	arg_count;
	unsigned int	arg[3];
}					t_prog;

typedef struct		s_prop
{
	int				color;
	int				index;
	int				params;
}					t_prop;

typedef struct		s_shape
{
	int				color;
	int				len;
	struct s_prop	params;
	struct s_pict	*pic;
	t_bool			(*print)(t_pict *, t_point *, struct s_shape *);
}					t_shape;
#endif
