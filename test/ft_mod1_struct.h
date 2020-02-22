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
	struct s_point	otn;
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
	int 			bits_per_pixel;
	int				size_line;
	int				endian;
	int				*addr;
	int				*z_buffer;
	int				*addr_copy;
	int				*z_buffer_copy;
}					t_pict;


typedef struct		s_oxyz
{
	struct s_dpoint	oy;
	struct s_dpoint	ox;
	struct s_dpoint	oz;
}					t_oxyz;


typedef struct		s_vis
{
	int				cam_x;
	int				cam_y;
	int				pause;
	int				rain;
	int				is_shift;
	int				is_rotate_or_csale;
	int				is_need_print_obstacles;
	int				grad;
	double			len;
	struct s_dpoint	ang;
	struct s_pict	pic;
	struct s_oxyz	oxyz;
	void			*img;
	void			*mlx;
	void			*win;
	struct s_plgn	*plgn;
	struct s_vektr	*points;
	struct s_line	*lines;
}					t_vis;

typedef struct		s_prog
{
	char				file[50];
	char				kernel[50];
	unsigned int		arg_count;
	unsigned int		arg_1;
	unsigned int		arg_2;
	unsigned int		arg_3;
	unsigned int		global_work_size;
}					t_prog;


#endif
