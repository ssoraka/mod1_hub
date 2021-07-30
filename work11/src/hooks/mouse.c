/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:28:19 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:28:20 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

int	ft_mouse_on_window(int x, int y)
{
	if (x < 0 || y < 0 || x >= CONST_WIDTH || y >= CONST_HEINTH)
		return (FALSE);
	return (TRUE);
}

int	ft_mouse_get_new_pos(int x, int y, t_param *param)
{
	if (!ft_mouse_on_window(x, y))
	{
		param->mouse.x = -1;
		param->mouse.y = -1;
		param->right_button_press = FALSE;
		param->left_button_press = FALSE;
		return (FALSE);
	}
	param->mouse.x = x;
	param->mouse.y = y;
	return (TRUE);
}
