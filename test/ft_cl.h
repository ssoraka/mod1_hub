/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cl.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 20:59:39 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/19 13:13:30 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CL_H
# define FT_CL_H

#define IMAX 40
#define JMAX 40
#define KMAX 40
#define I0 1
#define J0 1
#define K0 1
#define CELL_COUNT (KMAX + 2) * (JMAX + 2) * (IMAX + 2)

typedef double REAL;

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

typedef struct		s_neigh
{
	int				index;
	REAL			h_ij;
	REAL			w_ij;
	REAL			mu_ij;//коэффициент динамической вязкости
	struct s_dpoint	r_ij;
	struct s_dpoint	u_ij;
	struct s_dpoint	nabla_w_ij;
	//struct s_part	*part_j;
}					t_neigh;

typedef struct		s_neighs
{
	struct s_neigh	j[32];
	int				count;
}					t_neighs;

typedef struct		s_part
{
	int				type;
	REAL			density;
	REAL			delta_density;
	REAL			press;
	struct s_point	jik;
	struct s_dpoint	a;
	struct s_dpoint	speed;
	struct s_dpoint	delta_pos;
	struct s_dpoint	pos;
	struct s_neighs	n;
}					t_part;

typedef struct		s_cell
{
	int				obstacle;
	int				index[27];
}					t_cell;

typedef struct		s_ipart
{
	int				type;
	struct s_dpoint	pos;
}					t_ipart;

#endif
