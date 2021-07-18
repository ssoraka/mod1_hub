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

__kernel	void density_press(__global t_part *p, __global t_neighs *n)
{
	int i;
	int j;
	int num;
	REAL tmp;
	REAL density;
	REAL d_density;
	t_dpoint d_speed;


	i = get_global_id(0);

	//определяем плотность

	density = PART_MASS_0;
	num = 0;
	while (num < n[i].count)
	{
		density += PART_MASS_0 * n[i].j[num].w_ij;
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
		d_density += PART_MASS_0 * tmp;
		num++;
	}

	//определяем новую плотность и давление

	density += d_density * D_TIME;
	p[i].density = density;

	p[i].press = PRESS_G * (pow(density / DENSITY_0, GAMMA) - 1.0);
	if (p[i].press < 0.0)
		p[i].press = 0.0;
}
