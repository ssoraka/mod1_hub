/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 17:54:52 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 17:54:52 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

/*
**	избавление от погрешности
*/

void	ft_norm_vektor(t_dpoint *vek)
{
	double summ;

	summ = vek->x * vek->x + vek->y * vek->y + vek->z * vek->z;
	summ = sqrt(summ);
	vek->x = vek->x / summ;
	vek->y = vek->y / summ;
	vek->z = vek->z / summ;
}

void	ft_rotate_vek_around_vek_by_ang(t_dpoint *ox, t_dpoint *oy, double ang)
{
	double cosa;
	double sina;
	double temp_x;
	double temp_y;
	double temp_z;

	if (ang == 0.0 || !ox || !oy)
		return ;
	cosa = cos(ang);
	sina = sin(ang);
	temp_x = ox->x * (cosa + (1 - cosa) * oy->x * oy->x);
	temp_x = temp_x + ox->y * ((1 - cosa) * oy->x * oy->y - sina * oy->z);
	temp_x = temp_x + ox->z * ((1 - cosa) * oy->x * oy->z + sina * oy->y);
	temp_y = ox->x * ((1 - cosa) * oy->y * oy->x + sina * oy->z);
	temp_y = temp_y + ox->y * (cosa + (1 - cosa) * oy->y * oy->y);
	temp_y = temp_y + ox->z * ((1 - cosa) * oy->y * oy->z - sina * oy->x);
	temp_z = ox->x * ((1 - cosa) * oy->z * oy->x - sina * oy->y);
	temp_z = temp_z + ox->y * ((1 - cosa) * oy->z * oy->y + sina * oy->x);
	temp_z = temp_z + ox->z * (cosa + (1 - cosa) * oy->z * oy->z);
	ox->x = temp_x;
	ox->y = temp_y;
	ox->z = temp_z;
	ft_norm_vektor(ox);
}


void	ft_rotate_xyz(t_oxyz *oxyz, t_dpoint *ang)
{
	if (ang->z != 0)
	{
		ft_rotate_vek_around_vek_by_ang(&(oxyz->oy), &(oxyz->oz), ang->z);
		ft_rotate_vek_around_vek_by_ang(&(oxyz->ox), &(oxyz->oz), ang->z);
	}
	if (ang->x != 0)
	{
		ft_rotate_vek_around_vek_by_ang(&(oxyz->oy), &(oxyz->ox), ang->x);
		ft_rotate_vek_around_vek_by_ang(&(oxyz->oz), &(oxyz->ox), ang->x);
	}
	if (ang->y != 0)
	{
		ft_rotate_vek_around_vek_by_ang(&(oxyz->ox), &(oxyz->oy), ang->y);
		ft_rotate_vek_around_vek_by_ang(&(oxyz->oz), &(oxyz->oy), ang->y);
	}
	ft_fill_dpoint(ang, 0.0, 0.0, 0.0);
}


t_dpoint	ft_rot_dpoint(t_dpoint *v, t_oxyz *oxyz)
{
	t_dpoint rot_v;
	t_dpoint *ox;
	t_dpoint *oy;
	t_dpoint *oz;

	ox = &(oxyz->ox);
	oy = &(oxyz->oy);
	oz = &(oxyz->oz);
	rot_v.x = (ox->x * v->x + oy->x * v->y + oz->x * v->z);
	rot_v.y = (ox->y * v->x + oy->y * v->y + oz->y * v->z);
	rot_v.z = (ox->z * v->x + oy->z * v->y + oz->z * v->z);
	return (rot_v);
}

void	ft_change_points4(t_param *param, t_vektr *p)
{
	t_dpoint rot_p;

	rot_p = ft_rot_dpoint(&p->abs, &param->oxyz);
	p->zoom.x = (int)(rot_p.x * param->len) + param->cam_x;
	p->zoom.y = (int)(rot_p.y * param->len) + param->cam_y;
	p->zoom.z = (int)(rot_p.z * param->len);
}
