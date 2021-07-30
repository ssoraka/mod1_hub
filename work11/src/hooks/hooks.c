/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:28:19 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:28:20 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

int	ft_mouse_press(int button, int x, int y, void *parameters)
{
	t_vis	*vis;
	int		cell_number;
	t_param	*param;

	vis = (t_vis *)parameters;
	param = &vis->param;
	if (param->exit || !ft_mouse_on_window(x, y))
		return (0);
	cell_number = vis->pic.index[y * CONST_WIDTH + x];
	if (!cell_number)
		ft_csale_picture(param, button, &param->mouse);
	else if (ft_change_obstacles(g_cell, cell_number, button, &vis->param))
		vis->param.is_relief_changed = TRUE;
	else
		return (FALSE);
	vis->param.need_refresh = TRUE;
	return (TRUE);
}

int	ft_mouse_release(int button, int x, int y, void *parameters)
{
	t_vis	*vis;
	t_param	*param;

	vis = (t_vis *)parameters;
	param = &vis->param;
	if (param->exit || !ft_mouse_on_window(x, y))
		return (0);
	if (button == MIDDLE_BUTTON)
		param->right_button_press = FALSE;
	else if (button == LEFT_BUTTON)
		param->left_button_press = FALSE;
	return (0);
}

int	ft_mouse_move(int x, int y, void *parameters)
{
	t_vis	*vis;
	t_param	*param;

	vis = (t_vis *)parameters;
	param = &vis->param;
	if (param->exit || !ft_mouse_get_new_pos(x, y, param))
		return (0);
	if (ft_is_cam_moved(param))
		param->need_refresh = TRUE;
	if (ft_is_cam_rotated(param))
		param->need_refresh = TRUE;
	return (0);
}

void	ft_init_hooks(t_vis *vis)
{
	void	*param;
	void	*win;

	param = (void *)&vis->param;
	win = vis->win;
	mlx_hook(win, DESTROY, 0, ft_del_all, NULL);
	mlx_hook(win, KEY_PRESS, KEY_PRESS_MASK, deal_key, param);
	mlx_hook(win, MOTION_NOTIFY, POINTER_MOTION_MASK, ft_mouse_move,
		 (void *)vis);
	mlx_hook(win, BUTTON_PRESS, BUTTON_PRESS_MASK, ft_mouse_press,
		 (void *)vis);
	mlx_hook(win, BUTTON_RELEASE, BUTTON_RELEASE_MASK, ft_mouse_release,
		 (void *)vis);
	mlx_loop_hook(vis->mlx, loop_hook, (void *)vis);
}
