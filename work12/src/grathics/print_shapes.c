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

t_prop	set_param(size_t params, size_t index, int color)
{
	t_prop	print;

	print.color = color;
	print.params = params;
	print.index = index;
	return (print);
}

void	ft_init_shape(t_shape *shape, t_form form, t_prop prop)
{
	shape->params = prop;
	shape->color = prop.color;
	if (form == CIRCLE)
		shape->print = circle;
	else if (form == RECTANGLE)
		shape->print = ft_print_rect;
	else if (form == IMAGE)
		shape->print = print_img_to_img;
	else
		shape->print = ft_put_pixel_to_img2;
}
