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

void	ft_ret_zoom_xyz(t_vektr *ox, t_vis *vis)
{
	ox->otn.x = (int)(ox->abs.x * vis->len);
	ox->zoom.x = ox->otn.x + vis->cam_x;
	ox->otn.y = (int)(ox->abs.y * vis->len);
	ox->zoom.y = ox->otn.y + vis->cam_y;
	ox->otn.z = (int)(ox->abs.z * vis->len);
	ox->zoom.z = ox->otn.z;
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

void	ft_change_points4(t_vis *vis, t_vektr *p, int rotate)
{
	t_dpoint *ox;
	t_dpoint *oy;
	t_dpoint *oz;

	ox = &(vis->oxyz.ox);
	oy = &(vis->oxyz.oy);
	oz = &(vis->oxyz.oz);
	if (rotate)
	{
		p->otn.z = (int)((ox->z * p->abs.x + oy->z * p->abs.y + oz->z * p->abs.z) * vis->len);
		p->otn.x = (int)((ox->x * p->abs.x + oy->x * p->abs.y + oz->x * p->abs.z) * vis->len);
		p->otn.y = (int)((ox->y * p->abs.x + oy->y * p->abs.y + oz->y * p->abs.z) * vis->len);
	}
	p->zoom.x = p->otn.x + vis->cam_x;
	p->zoom.y = p->otn.y + vis->cam_y;
	p->zoom.z = p->otn.z;
}

void	ft_change_points5(t_vis *vis, t_vektr *p)
{
	while (p)
	{
		ft_change_points4(vis, p, vis->is_rotate_or_csale);
		p = p->next;
	}
}
