/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_shapes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:39:07 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:39:08 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

t_bool	ft_put_pixel_to_img(t_pict *pic, t_point *p, int color)
{
	if (p->x < 0 || p->y < 0 || p->x >= CONST_WIDTH || p->y >= CONST_HEINTH)
		return (FALSE);
	pic->addr[p->y * CONST_WIDTH + p->x] = color;
	return (TRUE);
}

t_bool	ft_put_pixel_to_img2(t_pict *pic, t_point *p, t_shape *shape)
{
	if (p->x < 0 || p->y < 0 || p->x >= CONST_WIDTH || p->y >= CONST_HEINTH)
		return (FALSE);
	if (!shape->is_particles)
	{
		if (pic->z_buffer[p->y * CONST_WIDTH + p->x] > p->z)
			return (FALSE);
		pic->index[p->y * CONST_WIDTH + p->x] = shape->index;
		if (shape->only_index)
			return (TRUE);
		pic->z_buffer[p->y * CONST_WIDTH + p->x] = p->z;
	}
	pic->addr[p->y * CONST_WIDTH + p->x] = shape->color;
	return (TRUE);
}

t_bool	ft_print_rect2(t_pict *pic, t_point *center, t_shape *shape)
{
	int i;
	int j;
	int shift;
	t_point p;

	j = 0;
	shift = -shape->len / 2;
	while (j <= shape->len)
	{
		i = 0;
		while (i <= shape->len)
		{
			ft_fill_point(&p, center->y + j + shift,
						  center->x + i + shift, center->z);
			ft_put_pixel_to_img2(pic, &p, shape);
			i++;
		}
		j++;
	}
	return (TRUE);
}


/* Вспомогательная функция, печатает точки, определяющие окружность */
void	plot_circle(t_pict *pic, t_point *p, t_point *center, t_shape *shape)
{
	t_point point;

	ft_fill_point(&point, center->y + p->y, center->x + p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, shape);
	ft_fill_point(&point, center->y + p->y, center->x - p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, shape);
	ft_fill_point(&point, center->y - p->y, center->x + p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, shape);
	ft_fill_point(&point, center->y - p->y, center->x - p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, shape);
}

/* Вычерчивание окружности с использованием алгоритма Мичнера */
t_bool	circle(t_pict *pic, t_point *center, t_shape *shape)
{
	int		x;
	int		y;
	int		delta;
	t_point	p;

	x = -1;
	y = shape->len;
	delta = 3 - 2 * shape->len;
	while (++x < y)
	{
		ft_fill_point(&p, x, y, 0);
		plot_circle(pic, &p, center, shape);
		ft_fill_point(&p, y, x, 0);
		plot_circle(pic, &p, center, shape);
		if (delta < 0)
			delta += 4 * x + 6;
		else
		{
			delta += 4 * (x - y) + 10;
			y--;
		}
	}
	if (x == y)
		plot_circle(pic, &p, center, shape);
	return (TRUE);
}

t_bool	print_img_to_img(t_pict *pic, t_point *center, t_shape *shape)
{
	t_point p;
	t_pict *from;
	int shift;
	int x;
	int y;

	shift = shape->pic->size_line / 2;
	from = shape->pic;
	ft_init_shape(shape, POINT, FALSE);
	y = -1;
	while (++y < from->size_line)
	{
		x = -1;
		while (++x < from->size_line)
		{
			shape->color = from->addr[y * from->size_line + x];
			if (shape->color)
			{
				ft_fill_point(&p, center->y - shift + y, center->x - shift + x, center->z);
				shape->print(pic, &p, shape);
			}
		}
	}
	ft_init_shape(shape, IMAGE, FALSE);
	return (TRUE);
}

/*
 * todo сделать структуру параметров для текущего принта
 */
void	ft_init_shape(t_shape *shape, t_form form, t_bool is_particle)
{
	shape->only_index = FALSE;
	shape->is_particles = is_particle;
	shape->form = form;
	if (form == POINT_INDEX)
		shape->only_index = TRUE;
	else if (form == CIRCLE)
		shape->print = circle;
	else if (form == RECTANGLE)
		shape->print = ft_print_rect2;
	else if (form == IMAGE)
		shape->print = print_img_to_img;
	else
		shape->print = ft_put_pixel_to_img2;
}
