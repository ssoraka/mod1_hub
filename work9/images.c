/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:28:19 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:28:20 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"


/*
** надо пачку защит маллока наставить ...
*/

int		ft_create_img(t_vis *vis)
{
	vis->mlx = mlx_init();
	vis->win = mlx_new_window(vis->mlx, CONST_WIDTH, CONST_HEINTH, "mlx 42");
	vis->img = mlx_new_image(vis->mlx, CONST_WIDTH, CONST_HEINTH);
	vis->cam_x = CAM_X;
	vis->cam_y = CAM_Y;
	vis->len = CONST_LEN;
	vis->pic.addr = (int *)mlx_get_data_addr(vis->img, &(vis->pic.bits_per_pixel), &(vis->pic.size_line), &(vis->pic.endian));
	vis->pic.z_buffer = (int *)ft_memalloc(CONST_WIDTH * CONST_HEINTH * 4);
	vis->pic.addr_copy = (int *)ft_memalloc(CONST_WIDTH * CONST_HEINTH * 4);
	vis->pic.z_buffer_copy = (int *)ft_memalloc(CONST_WIDTH * CONST_HEINTH * 4);
	return (0);
}

int		ft_not_need_print(t_line *line, t_pict *pic)
{
	if (line->p1->zoom.y <= 0 && line->p2->zoom.y <= 0)
		return (TRUE);
	if (line->p1->zoom.x <= 0 && line->p2->zoom.x <= 0)
		return (TRUE);
	if (line->p1->zoom.y >= CONST_HEINTH && line->p2->zoom.y >= CONST_HEINTH)
		return (TRUE);
	if (line->p1->zoom.x >= CONST_WIDTH && line->p2->zoom.x >= CONST_WIDTH)
		return (TRUE);
	//хз, спорное условие...
	if (line->p1->zoom.x >= 0 && line->p1->zoom.x < CONST_WIDTH
	&& line->p1->zoom.y >= 0 && line->p1->zoom.y < CONST_HEINTH
	&& pic->z_buffer[line->p1->zoom.y * CONST_WIDTH + line->p1->zoom.x] > line->p1->zoom.z + 10
	&& line->p2->zoom.x >= 0 && line->p2->zoom.x < CONST_WIDTH
	&& line->p2->zoom.y >= 0 && line->p2->zoom.y < CONST_HEINTH
	&& pic->z_buffer[line->p2->zoom.y * CONST_WIDTH + line->p2->zoom.x] > line->p2->zoom.z + 10)
		return (TRUE);
	return (FALSE);
}

void	ft_put_pixel_to_img2(t_pict *pic, t_point *p, int color)
{
	if (p->x < 0 || p->y < 0 || p->x >= CONST_WIDTH || p->y >= CONST_HEINTH)
		return ;
	if (pic->z_buffer[p->y * CONST_WIDTH + p->x] > p->z)
		return ;
	pic->addr[p->y * CONST_WIDTH + p->x] = color;
	pic->z_buffer[p->y * CONST_WIDTH + p->x] = p->z;
}
