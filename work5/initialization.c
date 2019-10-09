/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 19:05:18 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/04 19:05:19 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	ft_init_variable(void)
{
	map = NULL;
	flags = NULL;
	flags_surface = NULL;
	parts = NULL;
	speed_v = NULL;
	speed_u = NULL;
	speed_w = NULL;
	flow_f = NULL;
	flow_g = NULL;
	flow_h = NULL;
	press_p = NULL;
}

void	ft_init_delta_xyz(void)
{
	dx = DELTA;
	dy = DELTA;
	dz = DELTA;
	dx2 = dx * dx;
	dy2 = dy * dy;
	dz2 = dz * dz;
	dxy = dx * dy;
	dxz = dx * dz;
	dyz = dy * dz;

	gx = CONST_GX;
	gy = CONST_GY;
	gz = CONST_GZ;

	max_u = U_CONST;
	max_v = U_CONST;
	max_w = U_CONST;

	renolds = CONST_RE;
	iteration = 0;
}

void ***ft_cube_arr(int jmax, int imax, int kmax, int size)
{
	void ***arr;
	int j;

	if (!(arr = (void ***)ft_memalloc(sizeof(void **) * (jmax + 1))))
		return (NULL);
	j = 0;
	while (j < jmax)
	{
		if (!(arr[j] = (void **)ft_mem_arr_new(imax, kmax, size)))
		{
			ft_del_cube_arr(&arr);
			break ;
		}
		j++;
	}
	return (arr);
}

void	ft_init_map_arrs(void)
{
	if (!(map = (char ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(char))))
		ft_del_variable();
	if (!(flags = (int ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(int))))
		ft_del_variable();
	if (!(flags_surface = (int ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(int))))
		ft_del_variable();
	if (!(speed_v = (REAL ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(REAL))))
		ft_del_variable();
	if (!(speed_u = (REAL ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(REAL))))
		ft_del_variable();
	if (!(speed_w = (REAL ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(REAL))))
		ft_del_variable();
	if (!(flow_f = (REAL ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(REAL))))
		ft_del_variable();
	if (!(flow_g = (REAL ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(REAL))))
		ft_del_variable();
	if (!(flow_h = (REAL ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(REAL))))
		ft_del_variable();
	if (!(press_p = (REAL ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(REAL))))
		ft_del_variable();
	if (!(parts = (t_vektr ****)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(t_vektr *))))
		ft_del_variable();
}


void	ft_fill_first_value(void *param, int j, int i, int k)
{
	if (map[j][i][k] == WATER)
	{
		press_p[j][i][k] = P_CONST;
		speed_u[j][i][k] = U_CONST;
	}
}



void	ft_init_first_value(void)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube(NULL, &ft_fill_first_value, &start, &end);
}


void	ft_initialization(void)
{
	ft_init_variable();
	ft_init_delta_xyz();
	ft_init_map_arrs();
	ft_init_first_value();
}

void	ft_del_cube_arr(void ****arr)
{
	void	***tmp;
	int		i;

	i = 0;
	tmp = *arr;
	while (tmp[i])
	{
		ft_str_arr_free((char ***)(tmp + i));
		i++;
	}
	ft_memdel((void *)arr);
}

void	ft_del_variable(void)
{
	ft_str_arr_free((char ***)(&ground));
	ft_del_cube_arr((void ****)(&map));
	ft_del_cube_arr((void ****)(&flags));
	ft_del_cube_arr((void ****)(&flags_surface));
	ft_del_cube_arr((void ****)(&speed_v));
	ft_del_cube_arr((void ****)(&speed_u));
	ft_del_cube_arr((void ****)(&speed_w));
	ft_del_cube_arr((void ****)(&flow_f));
	ft_del_cube_arr((void ****)(&flow_g));
	ft_del_cube_arr((void ****)(&flow_h));
	ft_del_cube_arr((void ****)(&press_p));
	ft_del_cube_arr((void ****)(&parts));
	exit(0);
}
