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

typedef double REAL;
typedef double mytype;

typedef struct		s_iter
{
	int				i;
	int				j;
	int				imax;
	int				jmax;
}					t_iter;

typedef struct		s_fluid
{
	int				imax;
	int				jmax;
	int				count;
	mytype			dx;
	mytype			dy;
	REAL			eps;
	REAL			deltat;
	REAL			max_u;
	REAL			max_v;
	char			**map;
	int				**flags;
	int				**flags_surface;
	int				**parts;
	REAL			**tmp;
	REAL			**speed_v;
	REAL			**speed_u;
	REAL			**press_p;
	REAL			**lapl_u;
	REAL			**lapl_v;
	REAL			**flow_f;
	REAL			**flow_g;
	REAL			**rhs;
	struct s_vis	*vis;
}					t_fluid;

typedef struct		s_inf
{
	mytype dx;
	mytype dy;
	REAL dx2;
	REAL dy2;
	REAL dxy;
	int jmax;
	int imax;
	char **map;
}					t_inf;


typedef struct		s_vektr
{
	int abs_x;
	int abs_y;
	int abs_z;
	int color;
	double x;
	double y;
	double z;
	int otn_x;
	int otn_y;
	int otn_z;
	struct s_vektr *next;
	struct s_vektr *down;
}					t_vektr;

typedef struct		s_line
{
	int y1;
	int x1;
	int y2;
	int x2;
	int diry;
	int dirx;
	int deltay;
	int deltax;
	int error;
	int color;
	struct s_vektr *p1;
	struct s_vektr *p2;
	struct s_line *next;

}					t_line;

typedef struct		s_vis
{
	int				cam_x;
	int				cam_y;
	double			len;
	REAL			ang_x;
	REAL			ang_y;
	REAL			ang_z;
	int				*addr;
	void			*img;
	void			*mlx;
	void			*win;
	struct s_fluid	*fluid;
	struct s_vektr	**water;
	struct s_vektr	*oxyz;
	struct s_vektr	*points;
	struct s_line	*lines;
	struct s_inf	*inf;
}					t_vis;

#endif
