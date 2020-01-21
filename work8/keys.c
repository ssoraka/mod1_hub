/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:12:08 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:12:08 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

int		ft_rotate_and_csale(t_vis *vis, int key)
{
	if (key == 69 && vis->len < 100.0)
		vis->len *= 1.1;
	else if (key == 78 && vis->len > 0.01)
		vis->len /= 1.1;
	else if (key == 12)
		vis->ang.z += M_PI / 180;
	else if (key == 0)
		vis->ang.z -= M_PI / 180;
	else if (key == 13)
		vis->ang.y += M_PI / 180;
	else if (key == 1)
		vis->ang.y -= M_PI / 180;
	else if (key == 14)
		vis->ang.x += M_PI / 180;
	else if (key == 2)
		vis->ang.x -= M_PI / 180;
	else
		return (FALSE);
	ft_rotate_xyz(&(vis->oxyz), &(vis->ang));
	return (TRUE);
}


int		ft_shift(t_vis *vis, int key)
{
	if (key == 124)
		vis->cam_x += 10;
	else if (key == 123)
		vis->cam_x -= 10;
	else if (key == 126)
		vis->cam_y -= 10;
	else if (key == 125)
		vis->cam_y += 10;
	else
		return (FALSE);
	return (TRUE);
}



int		deal_key(int key, void *param)
{
	t_vis *vis;

	vis = (t_vis *)param;
	vis->is_rotate_or_csale = ft_rotate_and_csale(vis, key);
	vis->is_shift = ft_shift(vis, key);
	if (key == 53)//ESC
		exit(0);
	if (key == 35)
		vis->pause = !vis->pause;
	if (key == 15)
		vis->rain = !vis->rain;
	if (key == 34)
	{
		vis->is_need_print_obstacles = !vis->is_need_print_obstacles;
		if (vis->is_need_print_obstacles) //возможно следует изменить эту херь
			vis->is_rotate_or_csale = TRUE;
	}
	if (key == 5)
		vis->grad = !vis->grad;
	//printf("\n%d\n", key);
	return (0);
}
