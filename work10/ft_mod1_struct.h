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
	int color;
	struct s_dpoint	abs;
	struct s_point	zoom;
	struct s_vektr	*next;
}					t_vektr;

typedef struct		s_line
{
	int color;
	struct s_point dir;
	struct s_point delta;
	struct s_vektr *p1;
	struct s_vektr *p2;
	struct s_line *next;

}					t_line;


typedef struct		s_plgn
{
	int color;
	struct s_vektr	*p[4];
	struct s_plgn	*next;

}					t_plgn;

typedef struct		s_pict
{
	void			*img;
	int				*addr;
	int				*index;
	int				*z_buffer;
	int				*addr_copy;
	int				*z_buffer_copy;
	int				cell;
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
	int				grad;
	double			len;
	struct s_dpoint	ang;
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

typedef struct		s_arr
{
	void			*elems;
	void			*current;
	size_t			next;
	size_t			elems_count;
	size_t			elems_used;
	int				elem_size;
	void			(*func_del)(void *);
	void			*(*value)(void *);
}					t_arr;

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

#endif
