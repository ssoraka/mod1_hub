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

#ifndef FT_PARTICLE_STRUCT_H
# define FT_PARTICLE_STRUCT_H

# define MAX_NEIGHBORS 32

#include "ft_mod1.h"

typedef double REAL;

typedef struct		s_neigh
{
	REAL			h_ij;
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
	struct s_neigh	neigh[MAX_NEIGHBORS];
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



#endif
