/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poligons.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 17:51:29 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 17:51:29 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

t_plgn	*ft_create_poligon(t_vektr *p1, t_vektr *p2, t_vektr *p3, int color)
{
	t_plgn *tmp;

	tmp = (t_plgn *)ft_memalloc(sizeof(t_plgn));
	if (!tmp)
		return (NULL);
	tmp->p[0] = p1;
	tmp->p[1] = p2;
	tmp->p[2] = p3;
	tmp->color = color;
	tmp->n = ft_ret_norm(&p1->abs, &p2->abs, &p3->abs);
	return (tmp);
}

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

void	ft_vektr_interpolation_by_y(t_vektr *p, t_vektr *p1, t_vektr *p2, int grad)
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
	p->color = ft_grad_color(y, delta_y, p1->color, p2->color);
}


int		ft_need_print_traing(t_vektr **p, t_pict *pic)
{
	if (p[0]->zoom.y < 0 && p[1]->zoom.y < 0 && p[2]->zoom.y < 0)
		return (FALSE);
	if (p[0]->zoom.x < 0 && p[1]->zoom.x < 0 && p[2]->zoom.x < 0)
		return (FALSE);
	if (p[0]->zoom.y >= CONST_HEINTH && p[1]->zoom.y >= CONST_HEINTH && p[2]->zoom.y >= CONST_HEINTH)
		return (FALSE);
	if (p[0]->zoom.x >= CONST_WIDTH && p[1]->zoom.x >= CONST_WIDTH && p[2]->zoom.x >= CONST_WIDTH)
		return (FALSE);
	/*if (pic->z_buffer[p[0]->zoom.y * CONST_WIDTH + p[0]->zoom.x] > p[0]->zoom.z
	&& pic->z_buffer[p[1]->zoom.y * CONST_WIDTH + p[1]->zoom.x] > p[1]->zoom.z
	&& pic->z_buffer[p[2]->zoom.y * CONST_WIDTH + p[2]->zoom.x] > p[2]->zoom.z)
		return (FALSE);*/
	return (TRUE);
}

void	ft_draw_traing(t_pict *pic, t_vektr **p, int grad, int color)
{
	t_vektr tmp1;
	t_vektr tmp2;
	t_line line;
	int delta;
	int y;

	if (!ft_need_print_traing(p, pic))
		return ;
	line.p1 = &tmp1;
	line.p2 = &tmp2;
	line.color = color;
	delta = ft_znak(p[0]->zoom.y - p[1]->zoom.y);
	y = p[1]->zoom.y;
	while (y != p[0]->zoom.y)
	{
		tmp1.zoom.y = y;
		ft_vektr_interpolation_by_y(&tmp1, p[0], p[1], grad);
		tmp2.zoom.y = y;
		ft_vektr_interpolation_by_y(&tmp2, p[0], p[2], grad);
		draw_line_img2(&line, pic, grad);
		y += delta;
	}
	 line.p1 = p[1];
	 line.p2 = p[2];
	 draw_line_img2(&line, pic, grad);
}


void	ft_save_points_colors(t_plgn *plgn, int *colors)
{
	colors[0] = plgn->p[0]->color;
	colors[1] = plgn->p[1]->color;
	colors[2] = plgn->p[2]->color;
	colors[3] = plgn->color;
}

void	ft_change_points_colors(t_plgn *plgn)
{
	plgn->p[0]->color = ft_grad_color((int)(1024 * plgn->cos), 1024, plgn->p[0]->color, 0);
	plgn->p[1]->color = ft_grad_color((int)(1024 * plgn->cos), 1024, plgn->p[1]->color, 0);
	plgn->p[2]->color = ft_grad_color((int)(1024 * plgn->cos), 1024, plgn->p[2]->color, 0);
	plgn->color = ft_grad_color((int)(1024 * plgn->cos), 1024, plgn->color, 0);
}

void	ft_recovery_points_colors(t_plgn *plgn, int *colors)
{
	plgn->p[0]->color = colors[0];
	plgn->p[1]->color = colors[1];
	plgn->p[2]->color = colors[2];
	plgn->color = colors[3];
}


void	ft_prepare_plgn_for_printing(t_plgn *plgn, t_param *param)
{
	plgn->rot_n = ft_rot_dpoint(&plgn->n, &param->oxyz);
	plgn->cos = ft_vekt_cos(plgn->rot_n, param->light);
	if (plgn->cos < 0.0)
		plgn->cos = 0.0;
}


void	ft_print_plgn(t_plgn *plgn, t_pict *pic, int grad)
{
	t_vektr tmp;
	t_vektr *p[4];
	int colors[4];

	ft_save_points_colors(plgn, colors);
	ft_change_points_colors(plgn);
	p[0] = plgn->p[0];
	p[1] = plgn->p[1];
	p[2] = plgn->p[2];
	ft_sort_points_by_y(p);
	tmp.zoom.y = p[1]->zoom.y;
	ft_vektr_interpolation_by_y(&tmp, p[0], p[2], grad);
	p[3] = p[2];
	p[2] = &tmp;
	ft_draw_traing(pic, p, grad, plgn->color);
	p[0] = p[3];
	ft_draw_traing(pic, p, grad, plgn->color);
	ft_recovery_points_colors(plgn, colors);
}
