/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   speeds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 19:54:12 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/06 19:54:12 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

REAL	ft_laplasian(REAL ***speed, int j, int i, int k)
{
	REAL lapl;

	//сумма двойных дифференциалов
	lapl = (
	(speed[j][i + 1][k] - 2 * speed[j][i][k] + speed[j][i - 1][k]) / dx2 +
	(speed[j + 1][i][k] - 2 * speed[j][i][k] + speed[j - 1][i][k]) / dy2 +
	(speed[j][i][k + 1] - 2 * speed[j][i][k] + speed[j][i][k - 1]) / dz2);
	return (lapl);
}

REAL	ft_vortex(REAL ***speed, int j, int i, int k)
{
	REAL vortex;

	vortex =
	((speed_u[j][i][k] * speed[j][i][k]
	- speed_u[j][i - 1][k] * speed[j][i - 1][k]) / dx
	+ (speed_v[j][i][k] * speed[j][i][k]
	- speed_v[j - 1][i][k] * speed[j - 1][i][k]) / dy
	+ (speed_w[j][i][k] * speed[j][i][k]
	- speed_w[j][i][k - 1] * speed[j][i][k - 1]) / dz) * 0.25;
	return (vortex);
}


void	ft_recalc_flows(void *param, int j, int i, int k)
{
	if (!ft_is_water(flags[j][i][k]))
	//if (map[j][i][k] != WATER || flags_surface[j][i][k])
	//if (!ft_is_interior_water(flags[j][i][k]))
		return ;
	flow_f[j][i][k] = speed_u[j][i][k] + deltat
	* (ft_laplasian(speed_u, j, i, k) / renolds + gx);// - ft_vortex(speed_u, j, i, k));
	flow_g[j][i][k] = speed_v[j][i][k] + deltat
	* (ft_laplasian(speed_v, j, i, k) / renolds + gy);// - ft_vortex(speed_v, j, i, k));
	flow_h[j][i][k] = speed_w[j][i][k] + deltat
	* (ft_laplasian(speed_w, j, i, k) / renolds + gz);// - ft_vortex(speed_w, j, i, k));
}


void	ft_comp_fg(void)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube(NULL, &ft_recalc_flows, &start, &end);
}
