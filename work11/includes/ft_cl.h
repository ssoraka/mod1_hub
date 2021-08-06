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

# define PROGRAMS_COUNT 8
# define MAX_NEIGH_COUNT 32
# define CELL_MAP_SIZE 27

# define PI 3.141592

# define IMAX 100
# define JMAX 100
# define KMAX 10
# define I0 1
# define J0 1
# define K0 1
//# define CELL_COUNT ((KMAX + 2) * (JMAX + 2) * (IMAX + 2))

# define DELTA 1.0
//радиус сферы у одной частицы DELTA / 2
# define PART_H 0.5
//начальная плотность жидкости
# define DENSITY_0 1000.0
//масса одной частицы (DENSITY_0 * DELTA * DELTA * DELTA)
# define PART_MASS_0 DENSITY_0
//характеристика адиабаты
# define GAMMA 7

# define PRESS_G 220000000.0

# define D_TIME 0.0018

//соотношение максимальной скорости частицы со скоростью звука ~ 10 раз
# define CONST_E 0.1

# define VISCOSITY 0000.0
//скорость звука в среде
# define SPEED_OF_SOUND_C 120.0
//для расчета искусственной вязкости
//коэффициент квадратичной искусственной вязкости (0.02 - 0.03)
# define CONST_B 0.00
//коэффициент линнейной искусственной вязкости (0.02 - 0.03)
# define CONST_A 0.00

# define CONST_G 10.0
# define CONST_GY -10.0
# define CONST_GX 0.0
# define CONST_GZ 0.0

//коэффициент влияния
# define CONST_EP 0.3

typedef double	t_real;

typedef enum e_fluids
{
	NOTHING,
	WATER,
	MAGMA,
	FLUIDS
}				t_fluids;

typedef struct s_point
{
	int				y;
	int				x;
	int				z;
}					t_point;

typedef struct s_dpoint
{
	t_real			y;
	t_real			x;
	t_real			z;
}					t_dpoint;

typedef struct s_neigh
{
	int				j;
	t_real			h_ij;
	t_real			w_ij;
	t_real			mu_ij;
	struct s_dpoint	r_ij;
	struct s_dpoint	u_ij;
	struct s_dpoint	nabla_w_ij;
}					t_neigh;

typedef struct s_neighs
{
	struct s_neigh	j[MAX_NEIGH_COUNT];
	int				count;
}					t_neighs;

typedef struct s_part
{
	int				not_empty_near_cell;
	int				type;
	t_real			density;
	t_real			press;
	struct s_point	jik;
	struct s_dpoint	speed;
	struct s_dpoint	delta_pos;
	struct s_dpoint	pos;
}					t_part;

typedef struct s_cell_map
{
	short			obstacle;
	short			full;
	int				index[CELL_MAP_SIZE];
}					t_cell_map;

typedef struct s_cell
{
	char			obstacle;
	char			surface;
	char			water;
}					t_cell;

typedef struct s_ipart
{
	short			type;
	short			is_surface;
	struct s_dpoint	pos;
}					t_ipart;

typedef struct s_fluid
{
	int				color;
	int				size;
	t_real			h;
	t_real			c;
	t_real			m;
	t_real			p;
	t_real			dens;
	t_real			vis;
}					t_fluid;

typedef struct s_cl_prop
{
	struct s_fluid	f[FLUIDS];
	struct s_dpoint	g;
}					t_cl_prop;

#endif
