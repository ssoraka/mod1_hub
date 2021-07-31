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

#include "../includes/ft_mod1.h"

/*
**	избавление от погрешности
*/

void	ft_rotate_vek_around_vek_by_ang(t_dpoint *ox, t_dpoint *oy, double ang)
{
	double	cosa;
	double	sina;
	double	temp_x;
	double	temp_y;
	double	temp_z;

	if (ang == 0.0 || !ox || !oy || ox == oy)
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
}

t_dpoint	ft_rot_dpoint(t_dpoint *v, t_oxyz *oxyz)
{
	t_dpoint	rot_v;
	t_dpoint	*ox;
	t_dpoint	*oy;
	t_dpoint	*oz;

	ox = &(oxyz->ox);
	oy = &(oxyz->oy);
	oz = &(oxyz->oz);
	rot_v.x = (ox->x * v->x + oy->x * v->y + oz->x * v->z);
	rot_v.y = (ox->y * v->x + oy->y * v->y + oz->y * v->z);
	rot_v.z = (ox->z * v->x + oy->z * v->y + oz->z * v->z);
	return (rot_v);
}

void	ft_rotate_point_around_point(t_param *param, t_vektr *p)
{
	t_dpoint	rot_p;
	t_point		center;
	t_dpoint	*zero;

	zero = &param->centr.abs;
	center = param->centr.zoom;
	ft_fill_dpoint(&rot_p, p->abs.y - zero->y, p->abs.x - zero->x,
		p->abs.z - zero->z);
	rot_p = ft_rot_dpoint(&rot_p, &param->oxyz);
	p->zoom.x = (int)(rot_p.x * param->len) + center.x;
	p->zoom.y = (int)(rot_p.y * param->len) + center.y;
	p->zoom.z = (int)(rot_p.z * param->len) + center.z;
}

void	calc_light(t_param *param)
{
	param->cos.y = ft_vekt_cos(param->oxyz.oy, param->light);
	param->cos.x = ft_vekt_cos(param->oxyz.ox, param->light);
	param->cos.z = ft_vekt_cos(param->oxyz.oz, param->light);
}

void	ft_rotate_oxyz_around_v(t_param *param, t_dpoint *v, REAL ang)
{
	t_dpoint	g;
	t_oxyz		*oxyz;

	oxyz = &param->oxyz;
	ft_rotate_xyz_around_v(oxyz, v, ang);
	ft_fill_dpoint(&g, CONST_G, 0, 0);
	ft_fill_dpoint(&param->g,
		ft_vekt_cos(oxyz->oy, g) * CONST_G,
		ft_vekt_cos(oxyz->ox, g) * CONST_G,
		ft_vekt_cos(oxyz->oz, g) * CONST_G);
	calc_light(param);
	param->is_rotated = TRUE;
}
