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

#define PROGRAMS_COUNT 8
#define PART_COUNT 10
#define CELL_MAP_SIZE 27

#define PI 3.141592

#define IMAX 100
#define JMAX 100
#define KMAX 10
#define I0 1
#define J0 1
#define K0 1
#define CELL_COUNT (KMAX + 2) * (JMAX + 2) * (IMAX + 2)

#define DELTA 1.0
//радиус сферы у одной частицы
#define PART_H 0.5 * DELTA
//начальная плотность жидкости
#define DENSITY_0 1000.0
//масса одной частицы
#define PART_MASS_0 (DENSITY_0 * DELTA * DELTA * DELTA)
//характеристика адиабаты
#define GAMMA 7

#define PRESS_G 220000000.0

#define D_TIME 0.0018

//соотношение максимальной скорости частицы со скоростью звука ~ 10 раз
#define CONST_E 0.1

#define VISCOSITY 0000.0
//скорость звука в среде
#define SPEED_OF_SOUND_C 120.0
//для расчета искусственной вязкости
//коэффициент квадратичной искусственной вязкости (0.02 - 0.03)
#define CONST_B 0.00
//коэффициент линнейной искусственной вязкости (0.02 - 0.03)
#define CONST_A 0.00

#define CONST_GY -10.0
#define CONST_GX 0.0
#define CONST_GZ 0.0

//коэффициент влияния
#define CONST_EP 0.3

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
	int				j;
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
	REAL			press;
	struct s_point	jik;
	struct s_dpoint	speed;
	struct s_dpoint	delta_pos;
	struct s_dpoint	pos;
}					t_part;

typedef struct		s_cell_map
{
	int				obstacle;
	int				full;
	int				index[CELL_MAP_SIZE];
}					t_cell_map;

typedef struct		s_cell
{
	int				obstacle;
	int				water;
}					t_cell;

typedef struct		s_ipart
{
	int				type;
	struct s_dpoint	pos;
}					t_ipart;

#endif
