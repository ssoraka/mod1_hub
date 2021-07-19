/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygons2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 17:51:29 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 17:51:29 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

void	ft_swap_ptr(void **ptr1, void **ptr2)
{
	void *tmp;

	tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

void	ft_sort_points_by_y(t_vektr **p)
{
	if (p[1]->zoom.y < p[0]->zoom.y)
		ft_swap_ptr((void **)&p[1], (void **)&p[0]);
	if (p[2]->zoom.y < p[0]->zoom.y)
		ft_swap_ptr((void **)&p[2], (void **)&p[0]);
	if (p[2]->zoom.y < p[1]->zoom.y)
		ft_swap_ptr((void **)&p[2], (void **)&p[1]);
}

void	ft_vektr_interpolation_by_y(t_vektr *p, t_vektr *p1, t_vektr *p2,
									int grad)
{
	int delta_y;
	int y;

	delta_y = p1->zoom.y - p2->zoom.y;
	y = p->zoom.y - p2->zoom.y;
	if (!delta_y || !y)
	{
		ft_fill_point(&(p->zoom), p2->zoom.y, p2->zoom.x, p2->zoom.z);
		p->color = p2->color;
		return ;
	}
	p->zoom.x = ft_int_interpolation(y, delta_y, p2->zoom.x, p1->zoom.x);
	p->zoom.z = ft_int_interpolation(y, delta_y, p2->zoom.z, p1->zoom.z);
	if (grad)
		p->color = ft_grad_color(y, delta_y, p1->color, p2->color);
	else
		p->color = p1->color;
}

void	ft_draw_traing(t_pict *pic, t_vektr **p, t_prop prop, int grad)
{
	t_vektr	tmp1;
	t_vektr	tmp2;
	t_line	line;
	int		delta;
	int		y;

	line.p1 = &tmp1;
	line.p2 = &tmp2;
	delta = ft_znak(p[0]->zoom.y - p[1]->zoom.y);
	y = p[1]->zoom.y;
	while (y != p[0]->zoom.y)
	{
		tmp1.zoom.y = y;
		ft_vektr_interpolation_by_y(&tmp1, p[0], p[1], grad);
		tmp2.zoom.y = y;
		ft_vektr_interpolation_by_y(&tmp2, p[0], p[2], grad);
		draw_line_img(&line, pic, prop);
		y += delta;
	}
	line.p1 = p[1];
	line.p2 = p[2];
	draw_line_img(&line, pic, prop);
}

void	ft_prepare_plgn_for_printing(t_plgn *plgn, t_param *param)
{
	plgn->rot_n = ft_rot_dpoint(&plgn->n, &param->oxyz);
	plgn->cos = ft_vekt_cos(plgn->rot_n, param->light);
	if (plgn->cos < 0.0)
		plgn->cos *= -1.0;
}
