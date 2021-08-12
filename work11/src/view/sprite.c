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

int	ft_get_color1(t_real len, int rad, int color)
{
	if (len <= rad)
		return (ft_grad_color(rad - len, rad, color, 0x000001));
	return (0);
}

int	ft_get_color2(t_real len, int rad, int color)
{
	if (len <= rad * 0.2 || (len >= rad - 1 && len <= rad))
		return (0x000001);
	else if (len < rad * 0.6)
		return (ft_grad_color(rad * 0.6 - len, rad * 0.4, 0x000001, color));
	else if (len < rad)
		return (0xF0F0D0);
	return (0);
}

void	ft_fill_picture(t_pict *pict, int color, int (*func)(t_real, int, int))
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
			pict->addr[y * pict->size_line + x] = func(len, rad, color);
			x++;
		}
		y++;
	}
}

t_bool	ft_init_picture(t_pict *pict, t_param *param, int color)
{
	int	diameter;

	diameter = param->len;
	ft_bzero((void *)pict, sizeof(t_pict));
	if (!(diameter % 2))
		diameter++;
	pict->size_line = diameter;
	pict->addr = ft_memalloc(sizeof(int) * diameter * (diameter + 1));
	if (!pict->addr)
		return (FALSE);
	if (param->print_sprite == BALL_WATER)
		ft_fill_picture(pict, color, ft_get_color1);
	else if (param->print_sprite == HOLLOW_BALL_WATER)
		ft_fill_picture(pict, color, ft_get_color2);
	return (TRUE);
}
