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

	//if (map[j][i][k] != WATER)
	if (!ft_is_interior_water(flags[j][i][k]))
	//if (map[j][i][k] != WATER || flags_surface[j][i][k])
		return ;
	rhs = 1.0 / deltat
	* ((flow_f[j][i][k] - flow_f[j][i - 1][k]) / dx
	+ (flow_g[j][i][k] - flow_g[j - 1][i][k]) / dy
	+ (flow_h[j][i][k] - flow_h[j][i][k - 1]) / dz);

	press_p[j][i][k] = *((REAL *)param)
	* ((press_p[j][i + 1][k] + press_p[j][i - 1][k]) / dx2
	+ (press_p[j + 1][i][k] + press_p[j - 1][i][k]) / dy2
	+ (press_p[j][i][k + 1] + press_p[j][i][k - 1]) / dz2
	- rhs);
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


void	ft_poisson(void)
{

	ft_successive_overrelaxation();
}
