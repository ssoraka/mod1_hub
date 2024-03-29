/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delta_speed.cl                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 17:13:40 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/25 17:13:40 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/ft_cl.h"

__kernel	void delta_speed(__global t_part *p, __global t_neighs *n)
{
	int i;
	int j;
	int num;
	REAL press_ij;
	REAL c_ij;
	REAL viscos;
	REAL mu_ij;
	REAL h_ij;
	REAL tmp;
	t_dpoint speed;
	t_dpoint r_ij;
	t_dpoint u_ij;
	t_dpoint a;


	i = get_global_id(0);

	//определяем ускорение

	a = (t_dpoint){0.0, 0.0, 0.0};
	num = 0;
	while (num < n[i].count)
	{
		j = n[i].j[num].j;

		r_ij = n[i].j[num].r_ij;
		u_ij = n[i].j[num].u_ij;
		h_ij = n[i].j[num].h_ij;

		mu_ij =
			(r_ij.y * u_ij.y + r_ij.x * u_ij.x + r_ij.z * u_ij.z) /
			(r_ij.y * r_ij.y + r_ij.x * r_ij.x + r_ij.z * r_ij.z +
			CONST_E * CONST_E * h_ij * h_ij);

		tmp = 4 * VISCOSITY * mu_ij / (p[i].density + p[j].density);

		press_ij = (p[i].press + p[j].press) * 0.5;
		if (press_ij > 0.0)
		{
			c_ij = SPEED_OF_SOUND_C;
			viscos = 0.0;
			if (mu_ij > 0.0)
				viscos = h_ij * mu_ij * (h_ij * mu_ij * CONST_B - CONST_A * c_ij) / press_ij;

			tmp -= p[i].press / (p[i].density * p[i].density)
			+ p[j].press / (p[j].density * p[j].density)
			+ viscos;

		}

		a.y += PART_MASS_0 * tmp * n[i].j[num].nabla_w_ij.y;
		a.x += PART_MASS_0 * tmp * n[i].j[num].nabla_w_ij.x;
		a.z += PART_MASS_0 * tmp * n[i].j[num].nabla_w_ij.z;

		num++;
	}

	//определяем новую скорость
	speed = p[i].speed;
	speed.y += (a.y + CONST_GY) * D_TIME;
	speed.x += (a.x + CONST_GX) * D_TIME;
	speed.z += (a.z + CONST_GZ) * D_TIME;

	tmp = 0.10 * SPEED_OF_SOUND_C / sqrt(speed.y * speed.y + speed.x * speed.x + speed.z * speed.z);
	if (tmp < 1.0)
		p[i].speed = (t_dpoint){speed.y * tmp, speed.x * tmp, speed.z * tmp};
	else
		p[i].speed = speed;

}
