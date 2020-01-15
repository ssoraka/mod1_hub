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
	return (tmp);
}

void	ft_del_poligines(t_plgn **begin)
{
	t_plgn *tmp;

	tmp = *begin;
	while (tmp)
	{
		*begin = tmp->next;
		free(tmp);
		tmp = *begin;
	}
	*begin = NULL;
}

void	ft_swap_points(t_vektr **p1, t_vektr **p2)
{
	t_vektr *tmp;

	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

void	ft_sort_points_by_y(t_plgn *plgn)
{
	if (plgn->p[1]->zoom.y < plgn->p[0]->zoom.y)
		ft_swap_points(&(plgn->p[1]), &(plgn->p[0]));
	if (plgn->p[2]->zoom.y < plgn->p[0]->zoom.y)
		ft_swap_points(&(plgn->p[2]), &(plgn->p[0]));
	if (plgn->p[2]->zoom.y < plgn->p[1]->zoom.y)
		ft_swap_points(&(plgn->p[2]), &(plgn->p[1]));
}

void	ft_vektr_interpolation_by_y(t_vektr *p, t_vektr *p1, t_vektr *p2, int grad)
{
	int delta_y;
	int y;

	delta_y = p1->zoom.y - p2->zoom.y;
	if (!delta_y)
	{
		ft_fill_point(&(p->zoom), p2->zoom.y, p2->zoom.x, p2->zoom.z);
		p->color = p2->color;
		return ;
	}
	y = p->zoom.y - p2->zoom.y;
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
	//line.p1 = p[1];
	//line.p2 = p[2];
	//draw_line_img2(&line, pic, grad);
}



void	ft_print_plgn(t_plgn *plgn, t_pict *pic, int grad)
{
	t_vektr tmp;

	ft_sort_points_by_y(plgn);
	tmp.zoom.y = plgn->p[1]->zoom.y;
	ft_vektr_interpolation_by_y(&tmp, plgn->p[0], plgn->p[2], grad);
	plgn->p[3] = plgn->p[2];
	plgn->p[2] = &tmp;
	ft_draw_traing(pic, plgn->p, grad, plgn->color);
	ft_swap_points(&(plgn->p[3]), &(plgn->p[0]));
	ft_draw_traing(pic, plgn->p, grad, plgn->color);
	ft_swap_points(&(plgn->p[2]), &(plgn->p[3]));

}

void	ft_print_poligons(t_vis *vis, t_plgn *plgn)
{
	if (vis->is_rotate_or_csale || vis->is_shift)
		ft_change_points5(vis, vis->points);
	vis->is_rotate_or_csale = FALSE;
	vis->is_shift = FALSE;
	while (plgn)
	{
		ft_print_plgn(plgn, &(vis->pic), vis->grad);
		plgn = plgn->next;
	}
}
