/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 16:23:42 by ssoraka           #+#    #+#             */
/*   Updated: 2019/03/07 16:22:42 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "mlx.h"
#include "libft.h"

#define ABS(nbr) ((nbr) >= 0 ? (nbr) : (-1) * (nbr))


typedef double REAL;

#define DELTA 10
#define CONST_WIDTH 1000
#define CONST_HEINTH 1000
#define CONST_LEN 9


typedef struct		s_vis
{
	int				cam_x;
	int				cam_y;
	int				len;
	REAL			ang_x;
	REAL			ang_y;
	REAL			ang_z;
	int				*addr;
	void			*img;
	void			*mlx;
	void			*win;
	struct s_vektr	*water;
	struct s_vektr	*oxyz;
	struct s_vektr	*points;
	struct s_line	*lines;
	struct s_inf	*inf;
}					t_vis;

int speed_x = 2;



void ft_put_pixel_to_img(int *addr, int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= CONST_WIDTH || y >= CONST_HEINTH)
		return ;
	addr[y * CONST_WIDTH + x] = color;
}


int ft_create_img(t_vis *vis)
{
	int bits_per_pixel = 0;
	int size_line = 0;
	int endian = 0;

	vis->mlx = mlx_init();
	vis->win = mlx_new_window(vis->mlx, CONST_WIDTH, CONST_HEINTH, "mlx 42");
	vis->img = mlx_new_image(vis->mlx, CONST_WIDTH, CONST_HEINTH);
	vis->cam_x = 0;//CONST_WIDTH / 2;
	vis->cam_y = 0;//CONST_HEINTH / 2;
	vis->len = CONST_LEN;
	vis->addr = (int *)mlx_get_data_addr(vis->img, &bits_per_pixel, &size_line, &endian);
	//ft_bzero((void *)vis->addr, CONST_WIDTH * CONST_HEINTH * 4);
	return (0);
}


int deal_key(int key, void *param)
{
	t_vis *vis;

	vis = (t_vis *)param;
	if (key == 53)//ESC
		exit(0);
	return (0);
}

int loop_hook(void *param)
{
	t_vis *vis;

	vis = (t_vis *)param;
	mlx_clear_window(vis->mlx, vis->win);
//	ft_bzero((void *)vis->addr, CONST_WIDTH * CONST_HEINTH * 4);

	vis->cam_x += speed_x;
	if (vis->cam_x >= CONST_WIDTH || vis->cam_x <= 0)
	{
		speed_x = -speed_x;
		vis->cam_y = vis->cam_y + 2;
	}
	//vis->cam_x++;
	//vis->cam_y++;
	ft_put_pixel_to_img(vis->addr, vis->cam_x, vis->cam_y, 0xffffff);

	//mlx_hook(vis->win, 2, 0, deal_key, (void *)vis);

	mlx_put_image_to_window(vis->mlx, vis->win, vis->img, 0, 0);
	//sleep(1);
	return (0);
}


int main()
{
	t_vis *vis;

	vis = ft_memalloc(sizeof(t_vis));
/*	if (ft_read_map_from_file("text.txt", &(vis->inf)) == FAIL)
		return (0);

	ft_create_xyz(vis);
	ft_create_points_in_cells(vis);*/

	//ft_shift_points_to_center(vis);
	ft_create_img(vis);




	//

	//
	//while (1)
	//{
		//mlx_clear_window(vis->mlx, vis->win);


	//mlx_hook(vis->win, 12, 0, expose_hook, (void *)vis);
	//
	//mlx_loop(vis->mlx);
	//
	//}
	//mlx_hook(vis->win, deal_key, (void *)vis);
	//mlx_expose_hook(vis->win, expose_hook, (void *)vis);

	mlx_hook(vis->win, 2, 0, deal_key, (void *)vis);
	mlx_loop_hook(vis->mlx, loop_hook, (void *)vis);
	//mlx_expose_hook(vis->win, loop_hook, (void *)vis);
	mlx_loop(vis->mlx);


	return (0);
}
