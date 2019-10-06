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

void	ft_comp_uvw(void *param, int j, int i, int k)
{
	t_point *p;

	p = (t_point *)param;
	speed_u[j][i][k] = flow_f[j][i][k] - (press_p[j][i + 1][k] - press_p[j][i][k]) * p->x;
	speed_v[j][i][k] = flow_g[j][i][k] - (press_p[j + 1][i][k] - press_p[j][i][k]) * p->y;
	speed_w[j][i][k] = flow_h[j][i][k] - (press_p[j][i][k + 1] - press_p[j][i][k]) * p->z;
}


void	ft_adap_uvw(void)
{
	t_point start;
	t_point end;
	t_point delta;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_fill_point(&delta, deltat / dy, deltat / dx, deltat / dz);
	//считаем все скорости во всех точках...
	ft_cycle_cube(&delta, &ft_comp_uvw, &start, &end);
}
