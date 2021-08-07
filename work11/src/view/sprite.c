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
	int		rad;
	int		x;
	int		y;
	t_real	len;

	rad = pict->size_line / 2;
	if (!rad)
		rad++;
	y = 0;
	while (y < pict->size_line)
	{
		x = 0;
		while (x < pict->size_line)
		{
			len = sqrt((rad - x) * (rad - x) + (rad - y) * (rad - y));
			if (len <= rad)
				pict->addr[y * pict->size_line + x] = ft_grad_color(
						rad - len, rad, color, 0);
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
