/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delta_coord.cl                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 17:13:40 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/25 17:13:40 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/ft_cl.h"

__kernel	void delta_coord(__global t_part *p, __global t_neighs *n, __global t_cl_prop *params)
{
	int i;
	int j;
	int num;
	t_real tmp;
	t_dpoint d_speed;
	t_dpoint d_pos;
	t_real m_j;


	i = get_global_id(0);

	//определяем перемещение

	d_pos = (t_dpoint){0.0, 0.0, 0.0};
	num = 0;
	while (num < n[i].count)
	{
		j = n[i].j[num].j;
		d_speed = (t_dpoint){
			p[i].speed.y - p[j].speed.y,
			p[i].speed.x - p[j].speed.x,
			p[i].speed.z - p[j].speed.z};

		m_j = params->f[p[j].type].m;
		tmp = m_j * 2.0 / (p[i].density + p[j].density) * n[i].j[num].w_ij;

		d_pos.y += tmp * d_speed.y;
		d_pos.x += tmp * d_speed.x;
		d_pos.z += tmp * d_speed.z;

		num++;
	}

	//определяем новые координаты
	p[i].pos.y += (p[i].speed.y + CONST_EP * d_pos.y) * D_TIME;
	p[i].pos.x += (p[i].speed.x + CONST_EP * d_pos.x) * D_TIME;
	p[i].pos.z += (p[i].speed.z + CONST_EP * d_pos.z) * D_TIME;
}
