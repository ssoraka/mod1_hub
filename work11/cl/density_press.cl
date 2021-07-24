/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   density_press.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 17:13:40 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/25 17:13:40 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/ft_cl.h"

__kernel	void density_press(__global t_part *p, __global t_neighs *n, __global t_cl_prop *params)
{
	int i;
	int j;
	int num;
	REAL tmp;
	REAL density;
	REAL d_density;
	t_dpoint d_speed;
	REAL m_j;

	i = get_global_id(0);

	//определяем плотность


	density = params->f[p[i].type].m;
	num = 0;
	while (num < n[i].count)
	{
		j = n[i].j[num].j;
		m_j = params->f[p[j].type].m;
		density += m_j * n[i].j[num].w_ij;
		num++;
	}

	//определяем изменение плотности

	d_density = 0.0;
	num = 0;
	while (num < n[i].count)
	{
		j = n[i].j[num].j;
		d_speed = (t_dpoint){
			p[i].speed.y - p[j].speed.y,
			p[i].speed.x - p[j].speed.x,
			p[i].speed.z - p[j].speed.z};

		tmp =
			d_speed.y * n[i].j[num].nabla_w_ij.y +
			d_speed.x * n[i].j[num].nabla_w_ij.x +
			d_speed.z * n[i].j[num].nabla_w_ij.z;

		n[i].j[num].u_ij = d_speed;
		m_j = params->f[p[j].type].m;
		d_density += m_j * tmp;
		num++;
	}

	//определяем новую плотность и давление

	density += d_density * D_TIME;
	p[i].density = density;

	REAL density_0 = params->f[p[i].type].dens;
	REAL press_g = params->f[p[i].type].p;
	p[i].press = press_g * (pow(density / density_0, GAMMA) - 1.0);
	if (p[i].press < 0.0)
		p[i].press = 0.0;
}
