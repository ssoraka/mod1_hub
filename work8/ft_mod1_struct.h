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

typedef double REAL;
typedef double mytype;


typedef struct		s_point
{
	int y;
	int x;
	int z;
}					t_point;

typedef struct		s_dpoint
{
	REAL y;
	REAL x;
	REAL z;
}					t_dpoint;

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
	int iteration;
	struct s_dpoint	abs;
	struct s_point	zoom;
	struct s_point	otn;
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

typedef struct		s_neigh
{
	REAL			h_ij;
	//REAL			r_ij;
	REAL			w_ij;
	REAL			mu_ij;//коэффициент динамической вязкости
	struct s_dpoint	r_ij;
	struct s_dpoint	u_ij;
	struct s_dpoint	nabla_w_ij;
	struct s_part	*part_j;
}					t_neigh;

typedef struct		s_neighs
{
	int				neigh_count;
	struct s_neigh	neigh[MAX_NEIGHBORS1];
}					t_neighs;


typedef struct		s_tens
{
	int				is_surface;
	REAL			color;
	REAL			smoothed_color;
	struct s_dpoint	normal;
	struct s_dpoint	unit_n;
	struct s_dpoint	k;
}					t_tens;


typedef struct		s_arr
{
	void			*elems;
	int				elems_count;
	int				elems_used;
	int				elem_size;
	int				next;
	void			(*func_del)(void *);
}					t_arr;

typedef struct		s_part
{
	int				type;
	REAL			density;
	REAL			delta_density;
	REAL			press_0;
	REAL			press;
	REAL			mass;
	REAL			h;
	REAL			c;
	struct s_point	jik;
	struct s_dpoint	a;
	struct s_dpoint	speed;
	struct s_dpoint	delta_pos;
	struct s_vektr	pos;
	//struct s_neighs	neighs;
	struct s_tens	tension;
	struct s_arr	*neigh;
	struct s_part	*next;
	struct s_part	*prev;
}					t_part;



typedef struct		s_cpart
{
	struct s_part	****begin;
	struct s_part	*origin;
	struct s_part	**surround[9];
}					t_cpart;

typedef struct		s_vis
{
	int				cam_x;
	int				cam_y;
	int				pause;
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


#endif
