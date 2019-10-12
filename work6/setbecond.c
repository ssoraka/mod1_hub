/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setbecond.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 13:23:57 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/07 13:23:57 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"


static void	ft_add_press(REAL *press, REAL add_press, int *counter)
{
	if (add_press != 0.0)
	{
		*press += add_press;
		(*counter)++;
	}
}


void	ft_fill_obstacles_pressure(void *param, int j, int i, int k)
{
	int num;
	REAL press;

	num = 0;
	press = 0.0;
	if (flags[j][i][k] & B_E)
		ft_add_press(&press, press_p[j][i + 1][k], &num);
	if (flags[j][i][k] & B_W)
		ft_add_press(&press, press_p[j][i - 1][k], &num);
	if (flags[j][i][k] & B_N)
		ft_add_press(&press, press_p[j + 1][i][k], &num);
	if (flags[j][i][k] & B_S)
		ft_add_press(&press, press_p[j - 1][i][k], &num);
	if (flags[j][i][k] & B_O)
		ft_add_press(&press, press_p[j][i][k + 1], &num);
	if (flags[j][i][k] & B_I)
		ft_add_press(&press, press_p[j][i][k - 1], &num);
	if (num > 0)
		press_p[j][i][k] = press / num;
	else
		press_p[j][i][k] = 0.0;
}


void	ft_fill_obstacles_speed_u(void *param, int j, int i, int k)
{
	if (flags[j][i][k] & B_N)
	{
		speed_u[j][i][k] = -speed_u[j + 1][i][k];
		//speed_u[j][i - 1][k] = speed_u[j][i][k];
	}
	else if (flags[j][i][k] & B_S)
	{
		speed_u[j][i][k] = -speed_u[j - 1][i][k];
		//speed_u[j][i - 1][k] = speed_u[j][i][k];
	}
	else if (flags[j][i][k] & B_O)
	{
		speed_u[j][i][k] = -speed_u[j][i][k + 1];
		//speed_u[j][i - 1][k] = speed_u[j][i][k];
	}
	else if (flags[j][i][k] & B_I)
	{
		speed_u[j][i][k] = -speed_u[j][i][k - 1];
		//speed_u[j][i - 1][k] = speed_u[j][i][k];
	}
	if (flags[j][i][k] & B_E)
		speed_u[j][i][k] = 0.0;
	if (flags[j][i][k] & B_W)
		speed_u[j][i - 1][k] = 0.0;
}

void	ft_fill_obstacles_speed_v(void *param, int j, int i, int k)
{
	if (flags[j][i][k] & B_E)
	{
		speed_v[j][i][k] = -speed_v[j][i + 1][k];
		//speed_v[j - 1][i][k] = speed_v[j][i][k];
	}
	else if (flags[j][i][k] & B_W)
	{
		speed_v[j][i][k] = -speed_v[j][i - 1][k];
		//speed_v[j - 1][i][k] = speed_v[j][i][k];
	}
	else if (flags[j][i][k] & B_O)
	{
		speed_v[j][i][k] = -speed_v[j][i][k + 1];
		//speed_v[j - 1][i][k] = speed_v[j][i][k];
	}
	else if (flags[j][i][k] & B_I)
	{
		speed_v[j][i][k] = -speed_v[j][i][k - 1];
		//speed_v[j - 1][i][k] = speed_v[j][i][k];
	}
	if (flags[j][i][k] & B_N)
		speed_v[j][i][k] = 0.0;
	if (flags[j][i][k] & B_S)
		speed_v[j - 1][i][k] = 0.0;
}

void	ft_fill_obstacles_speed_w(void *param, int j, int i, int k)
{
	if (flags[j][i][k] & B_E)
	{
		speed_w[j][i][k] = -speed_w[j][i + 1][k];
		//speed_w[j][i][k - 1] = speed_w[j][i][k];
	}
	else if (flags[j][i][k] & B_W)
	{
		speed_w[j][i][k] = -speed_w[j][i - 1][k];
		//speed_w[j][i][k - 1] = speed_w[j][i][k];
	}
	else if (flags[j][i][k] & B_O)
	{
		speed_w[j][i][k] = -speed_w[j][i][k + 1];
		//speed_w[j][i][k - 1] = speed_w[j][i][k];
	}
	else if (flags[j][i][k] & B_I)
	{
		speed_w[j][i][k] = -speed_w[j][i][k - 1];
		//speed_w[j][i][k - 1] = speed_w[j][i][k];
	}
	if (flags[j][i][k] & B_O)
		speed_w[j][i][k] = 0.0;
	if (flags[j][i][k] & B_I)
		speed_w[j][i][k - 1] = 0.0;
}

void	ft_fill_obstacles(void *param, int j, int i, int k)
{
	if (map[j][i][k] != OBSTACLES && map[j][i][k] != BOUNDARY)
		return ;
	ft_fill_obstacles_pressure(param, j, i, k);
	ft_fill_obstacles_speed_u(param, j, i, k);
	ft_fill_obstacles_speed_v(param, j, i, k);
	ft_fill_obstacles_speed_w(param, j, i, k);
}



void	ft_setbcond(void)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, jmax + 1, 0, 0);
	ft_fill_point(&end, 0, imax + 1, kmax + 1);
	//ft_fill_point(&start, 1, 1, 1);
	//ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube(NULL, &ft_fill_obstacles, &start, &end);
	// вроде предыдущее должно работать для обоих условий
	//ft_fill_boundary_pressure();
}
