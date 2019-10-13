/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adap_uvw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 14:12:50 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/06 14:12:51 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"


void	ft_max_speed(REAL speed_u, REAL speed_v, REAL speed_w)
{
	if (speed_u < 0)
		speed_u = -speed_u;
	if (max_u < speed_u)
		max_u = speed_u;
	if (speed_v < 0)
		speed_v = -speed_v;
	if (max_v < speed_v)
		max_v = speed_v;
	if (speed_w < 0)
		speed_w = -speed_v;
	if (max_w < speed_w)
		max_w = speed_w;
}

void	ft_comp_uvw(void *param, int j, int i, int k)
{
	//t_point *p;

	//p = (t_point *)param;
	if (!ft_is_water(flags[j][i][k]))
		return ;
	if (ft_is_water(flags[j][i + 1][k]))
		speed_u[j][i][k] = flow_f[j][i][k]
		- (press_p[j][i + 1][k] - press_p[j][i][k]) * deltat / dx;
	if (ft_is_water(flags[j + 1][i][k]))
		speed_v[j][i][k] = flow_g[j][i][k]
		- (press_p[j + 1][i][k] - press_p[j][i][k]) * deltat / dy;
	if (ft_is_water(flags[j][i][k + 1]))
		speed_w[j][i][k] = flow_h[j][i][k]
		- (press_p[j][i][k + 1] - press_p[j][i][k]) * deltat / dz;
	ft_max_speed(speed_u[j][i][k], speed_v[j][i][k], speed_w[j][i][k]);
}


void	ft_adap_uvw(void)
{
	t_point start;
	t_point end;
	t_point delta;

	ft_fill_point(&start, jmax, 1, 1);
	ft_fill_point(&end, 1, imax, kmax);
	//ft_fill_point(&delta, deltat / dy, deltat / dx, deltat / dz);
	//считаем все скорости во всех точках...
	ft_cycle_cube(&delta, &ft_comp_uvw, &start, &end);
}
