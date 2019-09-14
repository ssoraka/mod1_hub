/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 17:13:40 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/25 17:13:40 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

typedef double REAL;
typedef int mytype;

#define T_INFINITY 10000000000.0
#define P_CONST 1000
#define CONST_RE 1000
#define CONST_GY 10
#define CONST_GX 0
#define T_END 100.0
#define TAU 0.7


#define D_N		0b00000001
#define D_S		0b00000010
#define D_W		0b00000100
#define D_E		0b00001000
#define C_B		0b00010000
#define C_F		0b00100000
#define C_A		0b01000000

mytype	dx = 100;
mytype	dy = 100;
int		imax = 10;
int		jmax = 10;
mytype	xlength = 1000;
mytype	ylength = 1000;

#define CONST_GX 0

REAL		str[10][10] =
{
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


//REAL differential(REAL **value, int delta)

void	ft_arr_set(REAL **arr, int column, REAL value)
{
	int i;

	while (*arr)
	{
		i = 0;
		while (i < column)
		{
			(*arr)[i] = value;
			i++;
		}
		arr++;
	}
}

REAL	ft_time_control(mytype dx, mytype dy, REAL max_u, REAL max_v)
{
	REAL deltat;
	REAL tmp;

	deltat = T_INFINITY;
	if (max_u < 0)
		max_u = -max_u;
	if (max_v < 0)
		max_v = -max_v;
	if (max_u && dx / max_u < deltat)
		deltat = dx / max_u;
	if (max_v && dy / max_v < deltat)
		deltat = dy / max_v;
	tmp = CONST_RE / 2 / (1 / (dx * dx) + 1 / (dy * dy));
	if (tmp < deltat)
		deltat = tmp;
	deltat *= TAU;
	return (deltat);
}

int		**ft_create_map(void)
{
	int **map;
	int i;
	int j;

	map = (int **)ft_mem_arr_new(jmax + 1, imax + 1, sizeof(int));
	if (!map)
		return (NULL);
	//столбец i=0 j=0 фиктивные
	j = 1;
	while (j <= jmax)
	{
		i = 1;
		while (i <= imax)
		{
			map[j][i] = str[j - 1][i - 1];
			i++;
		}
		j++;
	}
	return (map);
}



int		**ft_create_flags(int **map)
{
	int **flags;
	int i;
	int j;

	flags = (int **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(int));
	if (!flags)
		return (NULL);
	j = 1;
	while (j <= jmax)
	{
		i = 1;
		while (i <= imax)
		{
			//нули стоят по умолчанию, это символ препятствия
			//if (i == 0 || i == imax + 1 || j == 0 || j == jmax + 1)
			//	flags[j][i] = C_B;
			if (map[j][i])
				flags[j][i] = C_F;
			if (j > 1 && map[j - 1][i])
				flags[j][i] += D_S;
			if (j <= jmax && map[j + 1][i])
				flags[j][i] += D_N;
			if (i > 1 && map[j][i - 1])
				flags[j][i] += D_E;
			if (i <= imax && map[j][i + 1])
				flags[j][i] += D_W;
			i++;
		}
		j++;
	}
	return (flags);
}



int main(int argc, char **argv)
{

	int **map;
	int **flags;

	map = ft_create_map();
	flags = ft_create_flags(map);



	REAL **speed_v;
	REAL **speed_u;
	REAL **press_p;

/*
**	инициальзация массивов для скоростей и давления
*/
	speed_v = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	speed_u = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	press_p = (REAL **)ft_mem_arr_new(jmax, imax, sizeof(REAL));
	ft_arr_set(press_p, imax, P_CONST);

/*
**	инициальзация массивов для потоков и правых частей уравнения
*/

	REAL **flow_f;
	REAL **flow_g;
	REAL **RHS;
	flow_f = (REAL **)ft_mem_arr_new(jmax + 1, imax + 1, sizeof(REAL));
	flow_g = (REAL **)ft_mem_arr_new(jmax + 1, imax + 1, sizeof(REAL));
	RHS = (REAL **)ft_mem_arr_new(jmax + 1, imax + 1, sizeof(REAL));


	REAL t;
	REAL deltat;
	REAL max_u;
	REAL max_v;

	max_u = 0;
	max_v = 0;
	t = 0.0;
	while (t < T_END)
	{
		deltat = ft_time_control(dx, dy, max_u, max_v);
		// set boundary values for u and v




		t = t + deltat;
	}

	return (0);
}
