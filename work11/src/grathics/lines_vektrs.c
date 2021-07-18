/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lines_vektrs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 17:51:18 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 17:51:18 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

void	draw_line_img_lower(t_line *line, t_pict *pic, int grad, t_shape *shape)
{
	t_point	p;
	t_point	error;

	ft_fill_point(&p, line->p1->zoom.y, line->p1->zoom.x, line->p1->zoom.z);
	ft_fill_point(&error, 0, 0, 0);
	while (p.x != line->p2->zoom.x)
	{
		if (grad)
			shape->color = ft_set_color_to_point(line, &p, 1);
		shape->print(pic, &p, shape);
		error.y = error.y + line->delta.y;
		if (2 * error.y >= line->delta.x)
		{
			p.y += line->dir.y;
			error.y = error.y - line->delta.x;
		}
		p.z = ft_int_interpolation(p.x - line->p1->zoom.x,
								line->p2->zoom.x - line->p1->zoom.x,
								line->p1->zoom.z, line->p2->zoom.z);
		p.x += line->dir.x;
	}
	if (grad)
		shape->color = line->p2->color;
	shape->print(pic, &(line->p2->zoom), shape);
}

void	draw_line_img_over(t_line *line, t_pict *pic, int grad, t_shape *shape)
{
	t_point	p;
	t_point	error;

	ft_fill_point(&p, line->p1->zoom.y, line->p1->zoom.x, line->p1->zoom.z);
	ft_fill_point(&error, 0, 0, 0);
	while (p.y != line->p2->zoom.y)
	{
		if (grad)
			shape->color = ft_set_color_to_point(line, &p, 0);
		shape->print(pic, &p, shape);
		error.x = error.x + line->delta.x;
		if (2 * error.x >= line->delta.y)
		{
			p.x += line->dir.x;
			error.x = error.x - line->delta.y;
		}
		p.z = ft_int_interpolation(p.y - line->p1->zoom.y,
								line->p2->zoom.y - line->p1->zoom.y,
								line->p1->zoom.z, line->p2->zoom.z);
		p.y += line->dir.y;
	}
	if (grad)
		shape->color = line->p2->color;
	shape->print(pic, &(line->p2->zoom), shape);
}

int		ft_not_need_print(t_line *line)
{
	if (line->p1->zoom.y <= 0 && line->p2->zoom.y <= 0)
		return (TRUE);
	if (line->p1->zoom.x <= 0 && line->p2->zoom.x <= 0)
		return (TRUE);
	if (line->p1->zoom.y >= CONST_HEINTH && line->p2->zoom.y >= CONST_HEINTH)
		return (TRUE);
	if (line->p1->zoom.x >= CONST_WIDTH && line->p2->zoom.x >= CONST_WIDTH)
		return (TRUE);
	return (FALSE);
}

void	draw_line_img(t_line *line, t_pict *pic, int grad)
{
	t_point	p;
	t_shape shape;

	if (ft_not_need_print(line))
		return ;
	ft_fill_point(&p, line->p1->zoom.y, line->p1->zoom.x, line->p1->zoom.z);
	line->dir.y = ft_znak(line->p2->zoom.y - p.y);
	line->dir.x = ft_znak(line->p2->zoom.x - p.x);
	line->dir.z = ft_znak(line->p2->zoom.z - p.z);
	if (!line->dir.y && !line->dir.x)
		return ;
	line->delta.y = line->dir.y * (line->p2->zoom.y - p.y);
	line->delta.x = line->dir.x * (line->p2->zoom.x - p.x);
	line->delta.z = line->dir.z * (line->p2->zoom.z - p.z);
	ft_init_shape(&shape, POINT, FALSE);
	shape.color = line->color;
	shape.index = line->index;
	if (line->delta.x >= line->delta.y)
		draw_line_img_lower(line, pic, grad, &shape);
	else
		draw_line_img_over(line, pic, grad, &shape);
}
