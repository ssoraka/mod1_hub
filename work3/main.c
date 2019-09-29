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

#include "ft_mod1.h"

#define CONST_WIDTH 2000
#define CONST_HEINTH 1500
#define CAM_X 10
#define CAM_Y 10
#define CONST_LEN 0.800
#define KOEFF (1.0 / (DELTA_XY))
#define SLEEP1


/*
** к каждому пикселю присваивать координату z в дополнительном массиве
** проверять позицию, куда ты ее рисуешь
** нужно сделать структуру полигонов, которая будет отрисосывать 2 линии и
** заполнять значения треугольника координатами z, чтоб другие точки там не
** отрисовывались!!!
** работать в векторе, тогда все просто и понятно
** научиться рисовать треугольники
*/



int ft_znak(int num)
{
	int znak;

	znak = 0;
	if (num > 0)
		znak = 1;
	else if (num < 0)
		znak = -1;
	return (znak);
}



void ft_put_pixel_to_img(int *addr, int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= CONST_WIDTH || y >= CONST_HEINTH)
		return ;
	addr[y * CONST_WIDTH + x] = color;
}

int ft_interpolation(int percent, int color1, int color2, int byte)
{
	int deltared;

	while (byte > 1)
	{
		color1 = color1 >> 8;
		color2 = color2 >> 8;
		byte--;
	}
	color1 = color1 & 0xFF;
	color2 = color2 & 0xFF;
	deltared = ((color2 - color1) * percent) / 100 + color1;
	return (deltared);
}

int ft_grad_color(int delta1, int delta2, int color1, int color2)
{
	int color;
	int grad;

	grad = 100 - (delta1 * 100) / delta2;
	color = ft_interpolation(grad, color1, color2, 1);
	color = color + ((ft_interpolation(grad, color1, color2, 2)) << 8);
	color = color + ((ft_interpolation(grad, color1, color2, 3)) << 16);
	return (color);
}


void ft_set_color_to_line(t_line *line, int lower_45)
{
	int delta1;
	int delta2;

	if (lower_45)
	{
		delta1 = ABS(line->x2 - line->x1);
		delta2 = line->deltax;
	}
	else
	{
		delta1 = ABS(line->y2 - line->y1);
		delta2 = line->deltay;
	}
	line->color = ft_grad_color(delta1, delta2, line->p1->color, line->p2->color);
}



void draw_line_img_lower_45(t_line *line, int *addr)
{
	int error;

	error = 0;
	while (line->x1 != line->x2)
	{
		//ft_set_color_to_line(line, 1);
		ft_put_pixel_to_img(addr, line->x1, line->y1, line->color);
		error = error + line->deltay;
		if (2 * error >= line->deltax)
		{
			line->y1 += line->diry;
			error = error - line->deltax;
		}
		line->x1 += line->dirx;
	}
	ft_put_pixel_to_img(addr, line->x2, line->y2, line->color);
}


void draw_line_img_over_45(t_line *line, int *addr)
{
	int error;

	error = 0;
	while (line->y1 != line->y2)
	{
		//ft_set_color_to_line(line, 0);
		ft_put_pixel_to_img(addr, line->x1, line->y1, line->color);
		error = error + line->deltax;
		if (2 * error >= line->deltay)
		{
			line->x1 += line->dirx;
			error = error - line->deltay;
		}
		line->y1 +=line->diry;
	}
	ft_put_pixel_to_img(addr, line->x2, line->y2, line->color);
}


int draw_line_img(t_line *line, int *addr)
{
	line->x1 = line->p1->abs_x;// - max_x * len / 2;
	line->y1 = line->p1->abs_y;// + max_y * len / 2;
	line->x2 = line->p2->abs_x;// - max_x * len / 2;
	line->y2 = line->p2->abs_y;// + max_y * len / 2;
	line->diry = ft_znak(line->y2 - line->y1);
	line->dirx = ft_znak(line->x2 - line->x1);
	if (!line->diry && !line->dirx)
		return (0);
	line->deltay = line->diry * (line->y2 - line->y1);
	line->deltax = line->dirx * (line->x2 - line->x1);
	if (line->deltax >= line->deltay)
	{
		draw_line_img_lower_45(line, addr);
	}
	else
	{
		draw_line_img_over_45(line, addr);
	}
	return (0);
}


t_vektr *ft_new_vektor(int x, int y, int z);




/*
**	избавление от погрешности
*/

void ft_norm_vektor(t_vektr *vek)
{
	double summ;

	summ = vek->x * vek->x + vek->y * vek->y + vek->z * vek->z;
	summ = sqrt(summ);
	vek->x = vek->x / summ;
	vek->y = vek->y / summ;
	vek->z = vek->z / summ;
}






t_vektr *ft_rot(t_vektr *ox, t_vektr *oy, double ang)
{
	double cosa;
	double sina;
	double temp_x;
	double temp_y;
	double temp_z;

	if (ang == 0)
		return (ox);
	cosa = cos(ang);
	sina = sin(ang);
	temp_x = ox->x * (cosa + (1 - cosa) * oy->x * oy->x);
	temp_x = temp_x + ox->y * ((1 - cosa) * oy->x * oy->y - sina * oy->z);
	temp_x = temp_x + ox->z * ((1 - cosa) * oy->x * oy->z + sina * oy->y);
	temp_y = ox->x * ((1 - cosa) * oy->y * oy->x + sina * oy->z);
	temp_y = temp_y + ox->y * (cosa + (1 - cosa) * oy->y * oy->y);
	temp_y = temp_y + ox->z * ((1 - cosa) * oy->y * oy->z - sina * oy->x);
	temp_z = ox->x * ((1 - cosa) * oy->z * oy->x - sina * oy->y);
	temp_z = temp_z + ox->y * ((1 - cosa) * oy->z * oy->y + sina * oy->x);
	temp_z = temp_z + ox->z * (cosa + (1 - cosa) * oy->z * oy->z);
	ox->x = temp_x;
	ox->y = temp_y;
	ox->z = temp_z;
	ft_norm_vektor(ox);
	return (ox);
}


t_vektr *ft_new_vektor(int x, int y, int z)
{
	t_vektr *tmp;

	tmp = (t_vektr *)malloc(sizeof(t_vektr));
	tmp->x = x;
	tmp->y = y;
	tmp->z = z;
	tmp->otn_x = x;
	tmp->otn_y = y;
	tmp->otn_z = z;
	tmp->color = 0xFFFFFF;
	tmp->next = NULL;
	tmp->down = NULL;
	return (tmp);
}

t_line *ft_new_line(t_vektr *p1, t_vektr *p2, int color)
{
	t_line *line;

	if (p1 == NULL || p2 == NULL)
		return (NULL);
	line = (t_line *)malloc(sizeof(t_line));
	if (line == NULL)
		return (NULL);
	line->p1 = p1;
	line->p2 = p2;
	if (color)
		line->color = color;
	else
		line->color = p1->color;
	return (line);
}

void ft_ret_abs_xyz(t_vektr *ox, t_vis *vis)
{
	ox->otn_x = (int)(ox->x * vis->len);
	ox->abs_x = ox->otn_x + vis->cam_x;
	ox->otn_y = (int)(ox->y * vis->len);
	ox->abs_y = ox->otn_y + vis->cam_y;
	ox->otn_z = (int)(ox->z * vis->len);
	ox->abs_z = ox->otn_z;
}



void ft_rotate_xyz(t_vis *vis)
{
	t_vektr *ox0 = vis->oxyz;
	t_vektr *oy0 = ox0->next;
	t_vektr *oz0 = oy0->next;

	if (vis->ang_z != 0)
	{
		oy0 = ft_rot(oy0, oz0, vis->ang_z);
		ox0 = ft_rot(ox0, oz0, vis->ang_z);
		vis->ang_z = 0;
	}
	if (vis->ang_x != 0)
	{
		oy0 = ft_rot(oy0, ox0, vis->ang_x);
		oz0 = ft_rot(oz0, ox0, vis->ang_x);
		vis->ang_x = 0;
	}
	if (vis->ang_y != 0)
	{
		ox0 = ft_rot(ox0, oy0, vis->ang_y);
		oz0 = ft_rot(oz0, oy0, vis->ang_y);
		vis->ang_y = 0;
	}
}


void ft_change_points(t_vis *vis)
{
	t_vektr *p;
	t_vektr *ox0 = vis->oxyz;
	t_vektr *oy0 = ox0->next;
	t_vektr *oz0 = oy0->next;

	p = vis->points;
	while (p)
	{
		p->abs_z = ox0->z * p->otn_x * vis->len + oy0->z * p->otn_y * vis->len + oz0->z * p->otn_z * vis->len;
		p->abs_x = ox0->x * p->otn_x * vis->len + oy0->x * p->otn_y * vis->len + oz0->x * p->otn_z * vis->len;
		p->abs_y = ox0->y * p->otn_x * vis->len + oy0->y * p->otn_y * vis->len + oz0->y * p->otn_z * vis->len;

		p->abs_x = p->abs_x + vis->cam_x;
		p->abs_y = p->abs_y + vis->cam_y;
		p = p->next;
	}
}

void ft_change_points2(t_vis *vis, t_vektr *p)
{
	t_vektr *ox0 = vis->oxyz;
	t_vektr *oy0 = ox0->next;
	t_vektr *oz0 = oy0->next;

	p->abs_z = ox0->z * p->otn_x * vis->len + oy0->z * p->otn_y * vis->len + oz0->z * p->otn_z * vis->len;
	p->abs_x = ox0->x * p->otn_x * vis->len + oy0->x * p->otn_y * vis->len + oz0->x * p->otn_z * vis->len;
	p->abs_y = ox0->y * p->otn_x * vis->len + oy0->y * p->otn_y * vis->len + oz0->y * p->otn_z * vis->len;

	p->abs_x = p->abs_x + vis->cam_x;
	p->abs_y = p->abs_y + vis->cam_y;
}

void ft_print_rect(int *addr, t_vektr *p, int color)
{
	int i;
	int j;
	int len;
	int shift;

	j = 0;
	len = 3;
	shift = - len / 2;
	while (j <= len)
	{
		i = 0;
		while (i <= len)
		{
			ft_put_pixel_to_img(addr, p->abs_x + i + shift, p->abs_y + j + shift, color);
			i++;
		}
		j++;
	}
}



void ft_print_lines(t_vis *vis)
{
	t_line *line;

	line = vis->lines;
	while (line)
	{
		draw_line_img(line, vis->addr);
		line = line->next;
	}
	/*line = vis->water;
	while (line)
	{
		draw_line_img(line, vis->addr);
		ft_print_rect(vis->addr, line->p1, 0xFFFF);
		line = line->next;
	}*/
}


int deal_key(int key, void *param)
{
	t_vis *vis;


	vis = (t_vis *)param;
	if (key == 12)
		vis->ang_z += M_PI / 180;
	if (key == 13)
		vis->ang_y += M_PI / 180;
	if (key == 14)
		vis->ang_x += M_PI / 180;
	if (key == 53)//ESC
		exit(0);
	if (key == 124)
		vis->cam_x+=10;
	if (key == 123)
		vis->cam_x-=10;
	if (key == 126)
		vis->cam_y-=10;
	if (key == 125)
		vis->cam_y+=10;
	if (key == 69)
		vis->len*=1.1;
	if (key == 78)
		vis->len/=1.1;
	ft_rotate_xyz(vis);
	return (0);
}


void ft_add_line(t_line **begin, t_vektr *p1, t_vektr *p2, int color)
{
	t_line *line;
	t_line *tmp;

	line = ft_new_line(p1, p2, color);

	if (!line)
		return ;
	tmp = *begin;
	*begin = line;
	line->next = tmp;
}


int		ft_read_map_from_file(char *name, t_inf **inf)
{
	int		fd;
	int		i;

	i = 0;
	*inf = (t_inf *)ft_memalloc(sizeof(t_inf));
	if ((fd = open(name, O_RDWR)) < 0 || read(fd, NULL, 0) < 0)
	{
		ft_putstr("error");
		return (FAIL);
	}
	(*inf)->map = (char **)ft_memalloc(sizeof(char *) * 200);
	while (get_next_line(fd, &((*inf)->map[i])))
		i++;
	close(fd);
	(*inf)->jmax = i - 1;
	(*inf)->imax = ft_strlen((*inf)->map[1]) - 1;
	(*inf)->map[(*inf)->jmax + 1] = ft_strnew((*inf)->imax + 2);//для защиты от сеги, когда флаги расставляю...
	(*inf)->dx = DELTA;
	(*inf)->dy = DELTA;
	(*inf)->dx2 = DELTA * DELTA;
	(*inf)->dy2 = DELTA * DELTA;
	(*inf)->dxy = DELTA * DELTA;
	return (SUCCESS);
}


t_vektr *ft_add_vektor(void *ptr, int x, int y, int z)
{
	t_vektr *tmp;
	t_vektr **begin;

	begin = (t_vektr **)ptr;
	tmp = ft_new_vektor(x, y, z);
	tmp->next = *begin;
	*begin = tmp;
	return (tmp);
}

void	ft_create_xyz(t_vis *vis)
{
	t_vektr *xyz;

	vis->oxyz = ft_new_vektor(0, 0, 1);
	xyz = ft_new_vektor(0, 1, 0);
	xyz->next = vis->oxyz;
	vis->oxyz = xyz;
	xyz = ft_new_vektor(1, 0, 0);
	xyz->next = vis->oxyz;
	vis->oxyz = xyz;
}



void	ft_create_four_lines_of_cell(void *ptr, int j, int i)
{
	t_vis *vis;
	t_vektr *p1;
	t_vektr *p2;
	t_vektr *p3;
	t_vektr *p4;

	vis = (t_vis *)ptr;
	p1 = ft_add_vektor((void *)(&vis->points), i * vis->inf->dx, j * vis->inf->dy, 0);
	p2 = ft_add_vektor((void *)(&vis->points), i * vis->inf->dx, (j - 1) * vis->inf->dy, 0);
	p3 = ft_add_vektor((void *)(&vis->points), (i - 1) * vis->inf->dx, j * vis->inf->dy, 0);
	p4 = ft_add_vektor((void *)(&vis->points), (i - 1) * vis->inf->dx, (j - 1) * vis->inf->dy, 0);
	ft_add_line(&(vis->lines), p1, p2, 0xFFFFFF);
	ft_add_line(&(vis->lines), p2, p4, 0xFFFFFF);
	ft_add_line(&(vis->lines), p4, p3, 0xFFFFFF);
	ft_add_line(&(vis->lines), p3, p1, 0xFFFFFF);
	if (vis->inf->map[j][i] == OBSTACLES)
	{
		ft_add_line(&(vis->lines), p1, p4, 0xFFFF00);
		ft_add_line(&(vis->lines), p2, p3, 0xFFFF00);
	}
}


/*
void	ft_create_water_in_cell(void *ptr, int j, int i)
{
	int x0;
	int y0;
	int x1;
	int y1;
	int delta;
	t_vis *vis;

	delta = DELTA / 4;
	vis = (t_vis *)ptr;
	x0 = i * vis->inf->dx + delta;
	y0 = j * vis->inf->dy + delta;
	x1 = (i + 1) * vis->inf->dx - delta;
	y1 = (j + 1) * vis->inf->dy - delta;


	if (vis->inf->map[j][i] == WATER)
	{
		ft_add_vektor((void *)(&vis->water2), x0, y0, 0);
		ft_add_vektor((void *)(&vis->water2), x1, y0, 0);
		ft_add_vektor((void *)(&vis->water2), x0, y1, 0);
		ft_add_vektor((void *)(&vis->water2), x1, y1, 0);
	}
}
*/







void	ft_create_water_in_cell(void *ptr, int j, int i)
{
	int x0;
	int y0;
	int num;
	int delta;
	t_vis *vis;

	vis = (t_vis *)ptr;
	if (vis->inf->map[j][i] != WATER)
		return ;

	int n;
	int k;

	j--;
	i--;
	delta = DELTA / PARTS_COUNT;
	n = 0;
	while (n < PARTS_COUNT)
	{
		k = 0;
		while (k < PARTS_COUNT)
		{
			x0 = delta * k + delta / 2 + DELTA * i;
			y0 = delta * n + delta / 2 + DELTA * j;
			num = (j * PARTS_COUNT + n) * (vis->inf->imax * PARTS_COUNT) + i * PARTS_COUNT + k;
			vis->water[num] = ft_new_vektor(x0, y0, 0);

			k++;
		}
		n++;
	}

}


void	ft_new_pos_of_points(t_vis *vis)
{
	t_iter iter;

	ft_fill_iterations(&iter, 1, vis->inf->jmax, 1, vis->inf->imax);
	ft_iteration((void *)vis, &ft_create_water_in_cell, &iter);
}


void	ft_create_points_in_cells(t_vis *vis)
{
	t_iter iter;
	int parts_i;
	int parts_j;

	//создаем по 16 точек воды в каждой водяной клетке
	parts_j = vis->inf->jmax * PARTS_COUNT;
	parts_i = vis->inf->imax * PARTS_COUNT;
	vis->water = (t_vektr **)ft_memalloc(sizeof(t_vektr) * (parts_i * parts_j + 1));
	//ft_fill_iterations(&iter, 0, parts_j, 0, parts_i);
	ft_new_pos_of_points(vis);

	//рисуем по 4 линии на клетку

	ft_fill_iterations(&iter, 1, vis->inf->jmax, 1, vis->inf->imax);
	ft_iteration((void *)vis, &ft_create_four_lines_of_cell, &iter);
}


int ft_create_img(t_vis *vis)
{
	int bits_per_pixel = 0;
	int size_line = 0;
	int endian = 0;

	vis->mlx = mlx_init();
	vis->win = mlx_new_window(vis->mlx, CONST_WIDTH, CONST_HEINTH, "mlx 42");
	vis->img = mlx_new_image(vis->mlx, CONST_WIDTH, CONST_HEINTH);
	vis->cam_x = CAM_X;//CONST_WIDTH / 2;
	vis->cam_y = CAM_Y;//CONST_HEINTH / 2;
	vis->len = CONST_LEN;
	vis->addr = (int *)mlx_get_data_addr(vis->img, &bits_per_pixel, &size_line, &endian);
	return (0);
}

/*
void	ft_print_points(t_vis *vis, t_vektr **points)
{
	int i;

	i = 0;
	while (points[i])
	{
		//ft_put_pixel_to_img(vis->addr, points[i]->abs_x, points[i]->abs_y, 0xFFFF);
		if (points[i])
			ft_print_rect(vis->addr, points[i], 0xFFFF);
		i++;
	}
}*/


REAL	ft_move_parts_x(t_fluid *fluid, t_vektr *p, int j, int i)
{
	REAL x1;
	REAL u;

	x1 = i * DELTA;

	//printf("x1 = %d_%lf_%lf_%lf\n", j, y2, p1->y ,y1);
	u = (fluid->speed_u[j][i + 1] - fluid->speed_u[j][i]) / (DELTA_X)
	* (p->x - x1) + fluid->speed_u[j][i];
	return (u);
	//printf("%lf + %d\n",p1->otn_x, i, j);
}


REAL	ft_move_parts_y(t_fluid *fluid, t_vektr *p, int j, int i)
{
	REAL y1;
	REAL v;

	y1 = j * DELTA;

	v = (fluid->speed_v[j + 1][i] - fluid->speed_v[j][i]) / (DELTA_Y)
	* (p->y - y1) + fluid->speed_v[j][i];
	return (v);
}




void	ft_move_parts(t_fluid *fluid, t_vektr *parts)
{
	REAL u;
	REAL v;
	int i;
	int j;

	i = (int)(parts->x / DELTA);
	j = (int)(parts->y / DELTA);
	// на границе интерполировать не нужно, но рядом с препятствиями нужно
	//u = fluid->speed_u[j][i];
	//v = fluid->speed_v[j][i];
	u = ft_move_parts_x(fluid, parts, j + 1, i);
	v = ft_move_parts_y(fluid, parts, j, i + 1);
	/*if (fluid->flags[j + 1][i + 1] & C_X && fluid->map[j + 2][i + 1] != OBSTACLES)
	{
		u += fluid->deltat * CONST_GX * 2;
		v += fluid->deltat * CONST_GY * 2;
	}*/
	//printf("%d_%lf_%lf_%lf\n", i, i * DELTA, parts->x, u);
	//printf("%d_%lf_%lf_%lf\n", j, j * DELTA, parts->y, v);
	parts->x = parts->x + u * fluid->deltat;
	parts->y = parts->y + v * fluid->deltat;
	parts->otn_x = (int)parts->x;
	parts->otn_y = (int)parts->y;
}

void	ft_mark_water_on_map(t_fluid *fluid, t_vektr *parts)
{
	int i;
	int j;

	i = (int)(parts->x / DELTA) + 1;
	j = (int)(parts->y / DELTA) + 1;
	if (i >= 1 && j >= 1 && i <= fluid->imax && j <= fluid->jmax)
		if (fluid->map[j][i] == EMPTY)
			fluid->map[j][i] = WATER;
}


void	ft_recalk_parts(void *param)
{
	t_fluid *fluid;
	t_vektr **parts;
	int i;
	int n;

	fluid = (t_fluid *)param;
	parts = fluid->vis->water;
	n = fluid->imax * fluid->jmax * PARTS_COUNT * PARTS_COUNT + 1;
	i = 0;
	//printf("%d\n", n);
	while (i < n)
	{
		if (parts[i])
		{
			//printf("%d\n", i);
			ft_move_parts(fluid, parts[i]);
			ft_mark_water_on_map(fluid, parts[i]);
		}
		i++;
	}
}


void	ft_change_points3(t_vis *vis)
{
	t_vektr **parts;
	int i;
	int n;

	parts = vis->water;
	n = (vis->inf->imax) * (vis->inf->jmax) * PARTS_COUNT * PARTS_COUNT + 1;
	i = 0;

	while (i < n)
	{
		if (parts[i])
		{
			ft_change_points2(vis, parts[i]);
			ft_print_rect(vis->addr, parts[i], 0xFFFF);
		}
		i++;
	}
}



void	ft_refresh_picture(t_vis *vis)
{
	mlx_clear_window(vis->mlx, vis->win);
	ft_bzero((void *)vis->addr, CONST_WIDTH * CONST_HEINTH * 4);
	ft_change_points(vis);
	ft_change_points3(vis);
	ft_print_lines(vis);
	mlx_put_image_to_window(vis->mlx, vis->win, vis->img, 0, 0);
}


int loop_hook(void *param)
{
	t_vis *vis;
	t_fluid *fluid;


	fluid = (t_fluid *)param;
	vis = fluid->vis;

	ft_solver(fluid);
	ft_recalk_parts(param);


	//ft_print_flags(fluid, fluid->parts);
	//ft_print_flags(fluid, fluid->flags);
	//
	//ft_print_fluid(fluid, fluid->speed_v);
	//printf("%d_%d\n", fluid->jmax, fluid->imax);
	/*int i = 1;
	while (fluid->map[i])
	{
		printf("%s\n", fluid->map[i] + 1);
		i++;
	}*/
	//ft_print_fluid(fluid, fluid->press_p);
	ft_print_flags(fluid, fluid->flags_surface);
	ft_refresh_picture(vis);
	//ft_new_pos_of_points(vis);
	#ifdef SLEEP
	sleep(1);
	#endif
	return (0);
}


int main()
{
	t_vis *vis;


	//printf("%3$d_%2$d_%1$d\n", 5, 2.0, 16, 4.2, 3);


	vis = ft_memalloc(sizeof(t_vis));
	if (ft_read_map_from_file("text.txt", &(vis->inf)) == FAIL)
		return (0);

	//printf("%d_%d\n", vis->inf->jmax, vis->inf->imax);

	t_fluid *fluid;
	fluid = ft_initialization(vis->inf->map, vis->inf->jmax, vis->inf->imax);
	fluid->vis = vis;
	vis->fluid = fluid;

	ft_create_xyz(vis);
	ft_create_points_in_cells(vis);
	ft_create_img(vis);



	mlx_hook(vis->win, 2, 0, deal_key, (void *)vis);
	mlx_loop_hook(vis->mlx, loop_hook, (void *)fluid);
	mlx_loop(vis->mlx);


	return (0);
}
