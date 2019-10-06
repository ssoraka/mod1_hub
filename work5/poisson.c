/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poisson.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 14:15:18 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/06 14:15:18 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	ft_aproximation_press(void *param, int j, int i, int k)
{
	REAL rhs;

	if (map[j][i][k] != WATER)
		return ;
	rhs = *((REAL *)param) / deltat
	* ((flow_f[j][i][k] - flow_f[j][i - 1][k]) / dx
	+ (flow_g[j][i][k] - flow_g[j - 1][i][k]) / dy
	+ (flow_h[j][i][k] - flow_h[j][i][k - 1]) / dz);

	press_p[j][i][k] = *((REAL *)param)
	* ((press_p[j][i + 1][k] + press_p[j][i - 1][k] / dx2)
	+ (press_p[j + 1][i][k] + press_p[j - 1][i][k] / dy2)
	+ (press_p[j][i][k + 1] + press_p[j][i][k - 1] / dz2))
	+ rhs;
}



void	ft_successive_overrelaxation(void)
{
	int it;
	t_point start;
	t_point end;
	REAL coeff;

	coeff = 0.5 / (1.0 / dx2 + 1.0 / dy2 + 1.0 / dz2);
	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	it = 0;
	//eps = T_INFINITY;
	while (it < MAX_ITERATIONS)// && eps >= TOLERANCE)
	{
		//вычисляем давление приблизительно
		ft_cycle_cube((void *)(&coeff), &ft_aproximation_press, &start, &end);
		//смотрим разброс и повторяем расчет, в случае необходимости
		//eps = ft_residual_pressure();
		it++;
	}
}


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

	if (map[j][i][k] != OBSTACLES)
		return ;
	num = 0;
	press = 0.0;
	if (flags[j][i][k] & C_E)
		ft_add_press(&press, press_p[j][i + 1][k], &num);
	if (map[j][i - 1][k] & C_W)
		ft_add_press(&press, press_p[j][i - 1][k], &num);
	if (map[j + 1][i][k] & C_N)
		ft_add_press(&press, press_p[j + 1][i][k], &num);
	if (map[j - 1][i][k] & C_S)
		ft_add_press(&press, press_p[j - 1][i][k], &num);
	if (map[j][i][k + 1] & C_O)
		ft_add_press(&press, press_p[j][i][k - 1], &num);
	if (map[j][i][k - 1] & C_I)
		ft_add_press(&press, press_p[j][i][k - 1], &num);
	if (num > 0)
		press_p[j][i][k] = press / num;
	else
		press_p[j][i][k] = 0.0;
}

void	ft_poisson(void)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 0, 0, 0);
	ft_fill_point(&end, jmax + 1, imax + 1, kmax + 1);
	//ft_fill_point(&start, 1, 1, 1);
	//ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube(NULL, &ft_fill_obstacles_pressure, &start, &end);
	// вроде предыдущее должно работать для обоих условий
	//ft_fill_boundary_pressure();
	ft_successive_overrelaxation();
}
