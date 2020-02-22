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

#include "ft_mod1.h"

void	ft_print_rect2(t_pict *pic, t_point *center, int len, int color)
{
	int i;
	int j;
	int shift;
	t_point p;

	j = 0;
	shift = - len / 2;
	while (j <= len)
	{
		i = 0;
		while (i <= len)
		{
			ft_fill_point(&p, center->y + j + shift, center->x + i + shift, center->z);
			ft_put_pixel_to_img2(pic, &p, color);
			i++;
		}
		j++;
	}
}


/* Вспомогательная функция, печатает точки, определяющие окружность */
void	plot_circle2(t_pict *pic, t_point *p, t_point *center, int color_code)
{
	t_point point;

	ft_fill_point(&point, center->y + p->y, center->x + p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, color_code);
	ft_fill_point(&point, center->y + p->y, center->x - p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, color_code);
	ft_fill_point(&point, center->y - p->y, center->x + p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, color_code);
	ft_fill_point(&point, center->y - p->y, center->x - p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, color_code);
}

/* Вычерчивание окружности с использованием алгоритма Мичнера */
void	circle2(t_pict *pic, t_vektr *center, int radius, int color_code)
{
	int x;
	int y;
	int delta;
	t_point p;

	x = 0;
	y = radius;
	delta = 3 - 2 * radius;
	while (x < y)
	{
		ft_fill_point(&p, x, y, 0);
		plot_circle2(pic, &p, &(center->zoom), color_code);
		ft_fill_point(&p, y, x, 0);
		plot_circle2(pic, &p, &(center->zoom),color_code);
		if (delta < 0)
			delta += 4 * x + 6;
		else
		{
			delta += 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
	if (x == y)
		plot_circle2(pic, &p, &(center->zoom),color_code);
}
