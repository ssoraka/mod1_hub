/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_interpolation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:36:53 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:36:54 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

int		ft_interpolation(int percent, int color1, int color2, int byte)
{
	int deltared;

	color1 = color1 >> (8 * byte);
	color2 = color2 >> (8 * byte);
	color1 = color1 & 0xFF;
	color2 = color2 & 0xFF;
	deltared = ((color2 - color1) * percent) / 100 + color1;
	deltared = (deltared & 0xFF) << (8 * byte);
	return (deltared);
}

int		ft_grad_color(int delta1, int delta2, int color1, int color2)
{
	int color;
	int grad;

	grad = 100 - (delta1 * 100) / delta2;
	color = ft_interpolation(grad, color1, color2, 0);
	color |= ft_interpolation(grad, color1, color2, 1);
	color |= ft_interpolation(grad, color1, color2, 2);
	return (color);
}


int		ft_set_color_to_point(t_line *line, t_point *p, int lower_45)
{
	int delta1;
	int delta2;
	int color;

	if (lower_45)
	{
		delta1 = ABS(line->p2->zoom.x - p->x);
		delta2 = line->delta.x;
	}
	else
	{
		delta1 = ABS(line->p2->zoom.y - p->y);
		delta2 = line->delta.y;
	}
	color = ft_grad_color(delta1, delta2, line->p1->color, line->p2->color);
	return (color);
}


int		ft_int_interpolation(int y, int delta_y, int x1, int x2)
{
	int x;
	int delta_x;

	delta_x = x2 - x1;
	x = (delta_x * 2 *y) / delta_y;
	x = (x >> 1) + (x & 1);
	x += x1;
	return (x);
}
