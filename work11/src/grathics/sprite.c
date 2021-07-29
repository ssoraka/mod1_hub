/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 16:23:42 by ssoraka           #+#    #+#             */
/*   Updated: 2019/03/07 16:22:42 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

void	ft_fill_picture(t_pict *pict, int color)
{
	int		radius;
	int		x;
	int		y;
	REAL	len;

	radius = pict->size_line / 2;
	if (!radius)
		radius++;
	y = 0;
	while (y < pict->size_line)
	{
		x = 0;
		while (x < pict->size_line)
		{
			len = sqrt((radius - x) * (radius - x) + (radius - y) * (radius - y));
			if (len <= radius)
				pict->addr[y * pict->size_line + x] = ft_grad_color(radius - len, radius, color, 0);
			x++;
		}
		y++;
	}
}

t_bool	ft_init_picture(t_pict *pict, int diameter, int color)
{
	ft_bzero((void *)pict, sizeof(t_pict));

	if (!(diameter % 2))
		diameter++;
	pict->size_line = diameter;
	pict->addr = ft_memalloc(4 * diameter * (diameter + 1));
	if (!pict->addr)
		return (FALSE);
	ft_fill_picture(pict, color);
	return (TRUE);
}