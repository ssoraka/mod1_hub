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
#define CAM_X 1000
#define CAM_Y 1000
//#define RADIUS (DELTA * CONST_LEN * 0.7)
#define RADIUS 3
#define CONST_LEN 5.0
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

void	ft_fill_point(t_point *p, int y, int x, int z)
{
	if (!p)
		return ;
	p->x = x;
	p->y = y;
	p->z = z;
}


void ft_put_pixel_to_img2(t_pict *pic, t_point *p, int color)
{
	if (p->x < 0 || p->y < 0 || p->x >= CONST_WIDTH || p->y >= CONST_HEINTH)
		return ;
	if (pic->near[p->y * CONST_WIDTH + p->x] > p->z)
		return ;
	pic->addr[p->y * CONST_WIDTH + p->x] = color;
	pic->near[p->y * CONST_WIDTH + p->x] = p->z;
}

int ft_interpolation(int percent, int color1, int color2, int byte)
{
	int deltared;

	color1 = color1 >> (8 * byte);
	color2 = color2 >> (8 * byte);
	color1 = color1 & 0xFF;
	color2 = color2 & 0xFF;
	deltared = ((color2 - color1) * percent) / 100 + color1;
	deltared = (deltared & 0xFF) << (8 * byte);
	return (deltared);
}

int ft_grad_color(int delta1, int delta2, int color1, int color2)
{
	int color;
	int grad;

	grad = 100 - (delta1 * 100) / delta2;
	color = ft_interpolation(grad, color1, color2, 0);
	color |= ft_interpolation(grad, color1, color2, 1);
	color |= ft_interpolation(grad, color1, color2, 2);
	return (color);
}


int		ft_set_color_to_point(t_line *line, t_point *p, int lower_45)
{
	int delta1;
	int delta2;
	int color;

	if (lower_45)
	{
		delta1 = ABS(line->p2->abs.x - p->x);
		delta2 = line->delta.x;
	}
	else
	{
		delta1 = ABS(line->p2->abs.y - p->y);
		delta2 = line->delta.y;
	}
	color = ft_grad_color(delta1, delta2, line->p1->color, line->p2->color);
	return (color);
}


int		ft_int_interpolation(int y, int delta_y, int x1, int x2)
{
	int x;
	int delta_x;

	delta_x = x2 - x1;
	x = (delta_x * 2 *y) / delta_y;
	x = (x >> 1) + (x & 1);
	x += x1;
	return (x);
}




void draw_line_img_lower_452(t_line *line, t_point *p, t_pict *pic, int grad)
{
	t_point error;
	int color;

	ft_fill_point(&error, 0, 0, 0);
	color = line->color;
	while (p->x != line->p2->abs.x)
	{
		if (grad)
			color = ft_set_color_to_point(line, p, 1);
		ft_put_pixel_to_img2(pic, p, color);
		//printf("%d\n", p->z);
		error.y = error.y + line->delta.y;
		if (2 * error.y >= line->delta.x)
		{
			p->y += line->dir.y;
			error.y = error.y - line->delta.x;
		}
		p->z = ft_int_interpolation(p->x - line->p1->abs.x, line->p2->abs.x - line->p1->abs.x, line->p1->abs.z, line->p2->abs.z);
		/*error.z = error.z + line->delta.z;
		if (2 * error.z >= line->delta.x)
		{
			p->z += line->dir.z;
			error.z = error.z - line->delta.x;
		}*/

		p->x += line->dir.x;
	}
	if (grad)
		color = line->p2->color;
	ft_put_pixel_to_img2(pic, &(line->p2->abs), color);
}


void draw_line_img_over_452(t_line *line, t_point *p, t_pict *pic, int grad)
{
	t_point error;
	int color;

	ft_fill_point(&error, 0, 0, 0);
	color = line->color;
	while (p->y != line->p2->abs.y)
	{
		if (grad)
			color = ft_set_color_to_point(line, p, 0);
		ft_put_pixel_to_img2(pic, p, color);
		error.x = error.x + line->delta.x;
		if (2 * error.x >= line->delta.y)
		{
			p->x += line->dir.x;
			error.x = error.x - line->delta.y;
		}
		p->z = ft_int_interpolation(p->y - line->p1->abs.y, line->p2->abs.y - line->p1->abs.y, line->p1->abs.z, line->p2->abs.z);
		/*error.z = error.z + line->delta.z;
		if (2 * error.z >= line->delta.y)
		{
			p->z += line->dir.z;
			error.z = error.z - line->delta.y;
		}*/
		p->y +=line->dir.y;
	}
	if (grad)
		color = line->p2->color;
	ft_put_pixel_to_img2(pic, &(line->p2->abs), color);
}


int		ft_not_need_print(t_line *line, t_pict *pic)
{
	if (line->p1->abs.y <= 0 && line->p2->abs.y <= 0)
		return (TRUE);
	if (line->p1->abs.x <= 0 && line->p2->abs.x <= 0)
		return (TRUE);
	if (line->p1->abs.y >= CONST_HEINTH && line->p2->abs.y >= CONST_HEINTH)
		return (TRUE);
	if (line->p1->abs.x >= CONST_WIDTH && line->p2->abs.x >= CONST_WIDTH)
		return (TRUE);
	//хз, спорное условие...
	if (pic->near[line->p1->abs.y * CONST_WIDTH + line->p1->abs.x] > line->p1->abs.z + 10
	&& pic->near[line->p2->abs.y * CONST_WIDTH + line->p2->abs.x] > line->p2->abs.z + 10)
		return (TRUE);
	return (FALSE);
}


void	draw_line_img2(t_line *line, t_pict *pic, int grad)
{
	t_point p;

	if (ft_not_need_print(line, pic))
		return ;
	ft_fill_point(&p, line->p1->abs.y, line->p1->abs.x, line->p1->abs.z);
	line->dir.y = ft_znak(line->p2->abs.y - p.y);
	line->dir.x = ft_znak(line->p2->abs.x - p.x);
	line->dir.z = ft_znak(line->p2->abs.z - p.z);
	if (!line->dir.y && !line->dir.x)
		return ;
	line->delta.y = line->dir.y * (line->p2->abs.y - p.y);
	line->delta.x = line->dir.x * (line->p2->abs.x - p.x);
	line->delta.z = line->dir.z * (line->p2->abs.z - p.z);
	//printf("%d_%d\n", p.z, p.z);
	if (line->delta.x >= line->delta.y)
		draw_line_img_lower_452(line, &p, pic, grad);
	else
		draw_line_img_over_452(line, &p, pic, grad);
}






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






void ft_rot2(t_vektr *ox, t_vektr *oy, double ang)
{
	double cosa;
	double sina;
	double temp_x;
	double temp_y;
	double temp_z;

	if (ang == 0.0 || !ox || !oy)
		return ;
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
}




t_line *ft_new_line(t_vektr *p1, t_vektr *p2, int color)
{
	t_line *line;

	if (p1 == NULL || p2 == NULL)
		return (NULL);
	line = (t_line *)ft_memalloc(sizeof(t_line));
	if (line == NULL)
		return (NULL);
	line->p1 = p1;
	line->p2 = p2;
	line->color = color;
	return (line);
}

void ft_ret_abs_xyz(t_vektr *ox, t_vis *vis)
{
	ox->otn.x = (int)(ox->x * vis->len);
	ox->abs.x = ox->otn.x + vis->cam_x;
	ox->otn.y = (int)(ox->y * vis->len);
	ox->abs.y = ox->otn.y + vis->cam_y;
	ox->otn.z = (int)(ox->z * vis->len);
	ox->abs.z = ox->otn.z;
}



void ft_rotate_xyz(t_vis *vis)
{
	if (vis->ang_z != 0)
	{
		ft_rot2(&(vis->oxyz.oy), &(vis->oxyz.oz), vis->ang_z);
		ft_rot2(&(vis->oxyz.ox), &(vis->oxyz.oz), vis->ang_z);
		vis->ang_z = 0;
	}
	if (vis->ang_x != 0)
	{
		ft_rot2(&(vis->oxyz.oy), &(vis->oxyz.ox), vis->ang_x);
		ft_rot2(&(vis->oxyz.oz), &(vis->oxyz.ox), vis->ang_x);
		vis->ang_x = 0;
	}
	if (vis->ang_y != 0)
	{
		ft_rot2(&(vis->oxyz.ox), &(vis->oxyz.oy), vis->ang_y);
		ft_rot2(&(vis->oxyz.oz), &(vis->oxyz.oy), vis->ang_y);
		vis->ang_y = 0;
	}
}



void ft_change_points4(t_vis *vis, t_vektr *p, int rotate)
{
	t_vektr *ox;
	t_vektr *oy;
	t_vektr *oz;

	ox = &(vis->oxyz.ox);
	oy = &(vis->oxyz.oy);
	oz = &(vis->oxyz.oz);
	if (rotate)
	{
		p->otn.z = (int)((ox->z * p->x + oy->z * p->y + oz->z * p->z) * vis->len);
		p->otn.x = (int)((ox->x * p->x + oy->x * p->y + oz->x * p->z) * vis->len);
		p->otn.y = (int)((ox->y * p->x + oy->y * p->y + oz->y * p->z) * vis->len);
	}
	p->abs.x = p->otn.x + vis->cam_x;
	p->abs.y = p->otn.y + vis->cam_y;
	p->abs.z = p->otn.z;
}


void ft_change_points5(t_vis *vis, t_vektr *p)
{
	while (p)
	{
		ft_change_points4(vis, p, vis->is_rotate_or_csale);
		p = p->next;
	}
}



void ft_print_rect2(t_pict *pic, t_point *center, int len, int color)
{
	int i;
	int j;
	int shift;
	t_point p;

	j = 0;
	shift = - len / 2;
	while (j <= len)
	{
		i = 0;
		while (i <= len)
		{
			ft_fill_point(&p, center->y + j + shift, center->x + i + shift, center->z);
			ft_put_pixel_to_img2(pic, &p, color);
			i++;
		}
		j++;
	}
}




/* Вспомогательная функция, печатает точки, определяющие окружность */
void plot_circle2(t_pict *pic, t_point *p, t_point *center, int color_code)
{
	t_point point;

	ft_fill_point(&point, center->y + p->y, center->x + p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, color_code);
	ft_fill_point(&point, center->y + p->y, center->x - p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, color_code);
	ft_fill_point(&point, center->y - p->y, center->x + p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, color_code);
	ft_fill_point(&point, center->y - p->y, center->x - p->x, center->z);
	ft_put_pixel_to_img2(pic, &point, color_code);
}

/* Вычерчивание окружности с использованием алгоритма Мичнера */
void circle2(t_pict *pic, t_vektr *center, int radius, int color_code)
{
	int x;
	int y;
	int delta;
	t_point p;

	x = 0;
	y = radius;
	delta = 3 - 2 * radius;
	while (x < y)
	{
		ft_fill_point(&p, x, y, 0);
		plot_circle2(pic, &p, &(center->abs), color_code);
		ft_fill_point(&p, y, x, 0);
		plot_circle2(pic, &p, &(center->abs),color_code);
		if (delta < 0)
			delta += 4 * x + 6;
		else
		{
			delta += 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
	if (x == y)
		plot_circle2(pic, &p, &(center->abs),color_code);
}

void ft_print_lines(t_vis *vis, t_line *line)
{
	while (line)
	{
		draw_line_img2(line, &(vis->pic), vis->grad);
		line = line->next;
	}
}



int		ft_rotate_and_csale(t_vis *vis, int key)
{
	if (key == 69 && vis->len < 100.0)
		vis->len *= 1.1;
	else if (key == 78 && vis->len > 0.01)
		vis->len /= 1.1;
	else if (key == 12)
		vis->ang_z += M_PI / 180;
	else if (key == 0)
		vis->ang_z -= M_PI / 180;
	else if (key == 13)
		vis->ang_y += M_PI / 180;
	else if (key == 1)
		vis->ang_y -= M_PI / 180;
	else if (key == 14)
		vis->ang_x += M_PI / 180;
	else if (key == 2)
		vis->ang_x -= M_PI / 180;
	else
		return (FALSE);
	ft_rotate_xyz(vis);
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
	if (key == 34)
		vis->is_need_print_obstacles = !vis->is_need_print_obstacles;
	if (key == 5)
		vis->grad = !vis->grad;
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





void	ft_print_points(t_vis *vis, t_vektr *points)
{
	while (points)
	{
		ft_print_rect2(&(vis->pic), &(points->abs), RADIUS, WATER_COLOR);
		points = points->next;
	}
}

void	ft_print_points2(t_vis *vis, t_vektr *points)
{
	while (points)
	{
		circle2(&(vis->pic), points, RADIUS + 4, WATER_COLOR);
		points = points->next;
	}
}

	/*
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
				if (parts[i]->status == SURF)
					circle(vis->addr, parts[i], RADIUS, 0xFFFF);
					//ft_print_rect(vis->addr, parts[i], RADIUS * 2, 0xFFFF);
				else if (parts[i]->status == BLOB)
					circle(vis->addr, parts[i], RADIUS, 0xFF);
					//ft_print_rect(vis->addr, parts[i], RADIUS, 0xFFFF);
				else
					ft_print_rect(vis->addr, parts[i], 3, 0xFFFF);
			}
			i++;
		}
	}
	*/







int		ft_read_ground_from_file2(char *name)
{
	int		fd;
	int		k;

	if ((fd = open(name, O_RDWR)) < 0 || read(fd, NULL, 0) < 0)
	{
		ft_putstr("error");
		return (FAIL);
	}
	ground = (char **)ft_memalloc((200) * sizeof(char *));
	k = 0;
	while (get_next_line(fd, &(ground[k])))
		k++;
	close(fd);

	imax = ft_strlen(ground[1]);
	jmax = MAP_HEIGTH;
	kmax = k;

	return (SUCCESS);
}



void	ft_create_xyz(t_vis *vis)
{
	vis->oxyz.ox.x = 1.0;
	vis->oxyz.oy.y = 1.0;
	vis->oxyz.oz.z = 1.0;
}





t_vektr *ft_new_vektor2(int x, int y, int z, int color)
{
	t_vektr *tmp;

	tmp = (t_vektr *)malloc(sizeof(t_vektr));
	ft_fill_point(&(tmp->otn), y, x, z);
	tmp->x = x;
	tmp->y = y;
	tmp->z = z;
	tmp->color = color;
	tmp->next = NULL;
	return (tmp);
}


t_vektr *ft_add_vektor2(void *ptr, t_point *p, int color)
{
	t_vektr *tmp;
	t_vektr **begin;

	begin = (t_vektr **)ptr;
	tmp = ft_new_vektor2(p->x, p->y, p->z, color);
	tmp->next = *begin;
	*begin = tmp;
	return (tmp);
}


void	ft_del_vektor(t_vektr **begin)
{
	t_vektr *tmp;

	tmp = *begin;
	while (tmp)
	{
		*begin = tmp->next;
		free(tmp);
		tmp = *begin;
	}
	*begin = NULL;
}

void	ft_del_lines(t_line **begin)
{
	t_line *tmp;

	tmp = *begin;
	while (tmp)
	{
		*begin = tmp->next;
		free(tmp);
		tmp = *begin;
	}
	*begin = NULL;
}


void	ft_create_lines(t_line **lines, t_vektr **p, int color)
{
	int i;

	i = 0;
	while (i < 4)
	{
		if (i != 3)
		{
			ft_add_line(lines, p[i], p[i + 1], color);
			ft_add_line(lines, p[i + 4], p[i + 5], color);
		}
		else
		{
			ft_add_line(lines, p[i], p[i - 3], color);
			ft_add_line(lines, p[i + 4], p[i + 1], color);
		}
		ft_add_line(lines, p[i], p[i + 4], color);
		i++;
	}

}

t_plgn *ft_create_poligon(t_vektr *p1, t_vektr *p2, t_vektr *p3, int color);

void	ft_create_rectang_poligon(t_plgn **plgn, t_vektr **p, int color)
{
	t_plgn *tmp;

	tmp = ft_create_poligon(p[0], p[1], p[2], color);
	tmp->next = *plgn;
	*plgn = tmp;
	tmp = ft_create_poligon(p[0], p[3], p[2], color);
	tmp->next = *plgn;
	*plgn = tmp;
}

void	ft_create_cube_poligons(t_plgn **plgn, t_vektr **p, int color)
{
	t_vektr *tmp[4];

	ft_create_rectang_poligon(plgn, p, OBSTACLES_FRONT_COLOR);
	ft_create_rectang_poligon(plgn, p + 4, OBSTACLES_FRONT_COLOR);
	tmp[0] = p[1];
	tmp[1] = p[5];
	tmp[2] = p[4];
	tmp[3] = p[0];
	ft_create_rectang_poligon(plgn, tmp, OBSTACLES_TOP_COLOR);
	tmp[0] = p[3];
	tmp[1] = p[7];
	ft_create_rectang_poligon(plgn, tmp, OBSTACLES_TOP_COLOR);
	tmp[2] = p[6];
	tmp[3] = p[2];
	ft_create_rectang_poligon(plgn, tmp, OBSTACLES_RIGHT_COLOR);
	tmp[0] = p[1];
	tmp[1] = p[5];
	ft_create_rectang_poligon(plgn, tmp, OBSTACLES_RIGHT_COLOR);
}



void	ft_create_obstacles(void *ptr, int j, int i, int k)
{
	t_vis *vis;
	t_vektr *p[8];
	void *points;
	t_point xyz;

	if (map[j][i][k] != OBSTACLES)
		return ;
	vis = (t_vis *)ptr;
	points = (void *)(&(vis->points));
	ft_fill_point(&xyz, j * dy, i * dx, k * dz);
	p[0] = ft_add_vektor2(points, &xyz, 0xFF0000);
	ft_fill_point(&xyz, j * dy, (i - 1) * dx, k * dz);
	p[1] = ft_add_vektor2(points, &xyz, 0x00FF00);
	ft_fill_point(&xyz, (j - 1) * dy, (i - 1) * dx, k * dz);
	p[2] = ft_add_vektor2(points, &xyz, 0xFFFFFF);
	ft_fill_point(&xyz, (j - 1) * dy, i * dx, k * dz);
	p[3] = ft_add_vektor2(points, &xyz, 0xFFFFFF);

	ft_fill_point(&xyz, j * dy, i * dx, (k - 1) * dz);
	p[4] = ft_add_vektor2(points, &xyz, 0xFFFFFF);
	ft_fill_point(&xyz, j * dy, (i - 1) * dx, (k - 1) * dz);
	p[5] = ft_add_vektor2(points, &xyz, 0x0000FF);
	ft_fill_point(&xyz, (j - 1) * dy, (i - 1) * dx, (k - 1) * dz);
	p[6] = ft_add_vektor2(points, &xyz, 0xFFFFFF);
	ft_fill_point(&xyz, (j - 1) * dy, i * dx, (k - 1) * dz);
	p[7] = ft_add_vektor2(points, &xyz, 0xFF0000);

	//ft_create_lines(&(vis->lines), p, 0x00);
	ft_create_cube_poligons(&(vis->plgn), p, 0xFFFF00);
}






int ft_create_img(t_vis *vis)
{
	vis->mlx = mlx_init();
	vis->win = mlx_new_window(vis->mlx, CONST_WIDTH, CONST_HEINTH, "mlx 42");
	vis->img = mlx_new_image(vis->mlx, CONST_WIDTH, CONST_HEINTH);
	vis->cam_x = CAM_X;
	vis->cam_y = CAM_Y;
	vis->len = CONST_LEN;
	vis->pic.addr = (int *)mlx_get_data_addr(vis->img, &(vis->pic.bits_per_pixel), &(vis->pic.size_line), &(vis->pic.endian));
	vis->pic.near = (int *)ft_memalloc(CONST_WIDTH * CONST_HEINTH * 4);
	return (0);
}







void	ft_cycle_cube(void *param, void (*f)(void *, int, int, int), t_point *start, t_point *end)
{
	int k;
	int j;
	int i;
	t_point delta;

	//ft_fill_point(&delta, ft_znak(end->y - start->y), ft_znak(end->x - start->x), ft_znak(end->z - start->z));
	ft_fill_point(&delta, 1, 1, 1);
	if (end->y < start->y)
		delta.y = -1;
	if (end->x < start->x)
		delta.x = -1;
	if (end->z < start->z)
		delta.z = -1;
	j = start->y;
	while (j != end->y + delta.y)
	{
		i = start->x;
		while (i != end->x + delta.x)
		{
			k = start->z;
			while (k != end->z + delta.z)
			{
				f(param, j, i, k);
				k += delta.z;
			}
			i += delta.x;
		}
		j += delta.y;
	}
}


void	ft_mark_obstacles_on_map(void *param, int j, int i, int k)
{
	if (j <= ground[k - 1][i - 1] - '0')
		map[j][i][k] = OBSTACLES;
	else
		map[j][i][k] = EMPTY;
		//map[j][i][k] = WATER;
}



void	ft_fill_map_from_ground(char **ground)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube(NULL, &ft_mark_obstacles_on_map, &start, &end);
}


void	ft_create_points_in_cells(t_vis *vis)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube((void *)vis, &ft_create_obstacles, &start, &end);
}



void	ft_swap_points(t_vektr **p1, t_vektr **p2)
{
	t_vektr *tmp;

	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

void	ft_sort_points_by_y(t_plgn *plgn)
{
	if (plgn->p[1]->abs.y < plgn->p[0]->abs.y)
		ft_swap_points(&(plgn->p[1]), &(plgn->p[0]));
	if (plgn->p[2]->abs.y < plgn->p[0]->abs.y)
		ft_swap_points(&(plgn->p[2]), &(plgn->p[0]));
	if (plgn->p[2]->abs.y < plgn->p[1]->abs.y)
		ft_swap_points(&(plgn->p[2]), &(plgn->p[1]));
}





void	ft_vektr_interpolation_by_y(t_vektr *p, t_vektr *p1, t_vektr *p2, int grad)
{
	int delta_y;
	int y;

	delta_y = p1->abs.y - p2->abs.y;
	if (!delta_y)
	{
		ft_fill_point(&(p->abs), p2->abs.y, p2->abs.x, p2->abs.z);
		p->color = p2->color;
		return ;
	}
	y = p->abs.y - p2->abs.y;
	p->abs.x = ft_int_interpolation(y, delta_y, p2->abs.x, p1->abs.x);
	p->abs.z = ft_int_interpolation(y, delta_y, p2->abs.z, p1->abs.z);
	p->color = ft_grad_color(y, delta_y, p1->color, p2->color);
}


int		ft_need_print_traing(t_vektr **p, t_pict *pic)
{
	if (p[0]->abs.y < 0 && p[1]->abs.y < 0 && p[2]->abs.y < 0)
		return (FALSE);
	if (p[0]->abs.x < 0 && p[1]->abs.x < 0 && p[2]->abs.x < 0)
		return (FALSE);
	if (p[0]->abs.y >= CONST_HEINTH && p[1]->abs.y >= CONST_HEINTH && p[2]->abs.y >= CONST_HEINTH)
		return (FALSE);
	if (p[0]->abs.x >= CONST_WIDTH && p[1]->abs.x >= CONST_WIDTH && p[2]->abs.x >= CONST_WIDTH)
		return (FALSE);
	if (pic->near[p[0]->abs.y * CONST_WIDTH + p[0]->abs.x] > p[0]->abs.z
	&& pic->near[p[1]->abs.y * CONST_WIDTH + p[1]->abs.x] > p[1]->abs.z
	&& pic->near[p[2]->abs.y * CONST_WIDTH + p[2]->abs.x] > p[2]->abs.z)
		return (FALSE);
	return (TRUE);
}

void	ft_draw_traing(t_pict *pic, t_vektr **p, int grad, int color)
{
	t_vektr tmp1;
	t_vektr tmp2;
	t_line line;
	int delta;
	int y;

	if (!ft_need_print_traing(p, pic))
		return ;
	line.p1 = &tmp1;
	line.p2 = &tmp2;
	line.color = color;
	delta = ft_znak(p[0]->abs.y - p[1]->abs.y);
	y = p[1]->abs.y;
	while (y != p[0]->abs.y)
	{
		tmp1.abs.y = y;
		ft_vektr_interpolation_by_y(&tmp1, p[0], p[1], grad);
		tmp2.abs.y = y;
		ft_vektr_interpolation_by_y(&tmp2, p[0], p[2], grad);
		draw_line_img2(&line, pic, grad);
		y += delta;
	}
	//line.p1 = p[1];
	//line.p2 = p[2];
	//draw_line_img2(&line, pic, grad);
}



void	ft_print_plgn(t_plgn *plgn, t_pict *pic, int grad)
{
	t_vektr tmp;


	ft_sort_points_by_y(plgn);
	tmp.abs.y = plgn->p[1]->abs.y;
	ft_vektr_interpolation_by_y(&tmp, plgn->p[0], plgn->p[2], grad);
	//printf("%d_%d_%d\n", plgn->p[0]->abs.z, tmp.abs.z, plgn->p[2]->abs.z);
	//ft_print_rect2(pic, &(plgn->p[0]->abs), 5, plgn->p[0]->color);
	//ft_print_rect2(pic, &(plgn->p[1]->abs), 5, plgn->p[1]->color);
	//ft_print_rect2(pic, &(plgn->p[2]->abs), 5, plgn->p[2]->color);
	//ft_print_rect2(pic, &(tmp.abs), 5, 0xFFFFFF);
	plgn->p[3] = plgn->p[2];
	plgn->p[2] = &tmp;
	ft_draw_traing(pic, plgn->p, grad, plgn->color);
	ft_swap_points(&(plgn->p[3]), &(plgn->p[0]));
	ft_draw_traing(pic, plgn->p, grad, plgn->color);
	ft_swap_points(&(plgn->p[2]), &(plgn->p[3]));

}



void ft_print_poligons(t_vis *vis, t_plgn *plgn)
{
	if (vis->is_rotate_or_csale || vis->is_shift)
		ft_change_points5(vis, vis->points);
	vis->is_rotate_or_csale = FALSE;
	vis->is_shift = FALSE;
	while (plgn)
	{
		ft_print_plgn(plgn, &(vis->pic), vis->grad);
		plgn = plgn->next;
	}
}


t_plgn *ft_create_poligon(t_vektr *p1, t_vektr *p2, t_vektr *p3, int color)
{
	t_plgn *tmp;

	tmp = (t_plgn *)ft_memalloc(sizeof(t_plgn));
	if (!tmp)
		return (NULL);
	tmp->p[0] = p1;
	tmp->p[1] = p2;
	tmp->p[2] = p3;
	tmp->color = color;
	return (tmp);
}



void	ft_print_water_in_cell(void *param, int j, int i, int k)
{
	t_vis *vis;

	vis = (t_vis *)param;
	if (ft_is_water(flags[j][i][k]))
	{
		ft_change_points5(vis, parts[j][i][k]);
		if (!flags_surface[j][i][k])
			;//ft_print_points(vis, parts[j][i][k]);
		else
			ft_print_points2(vis, parts[j][i][k]);
	}
}



void	ft_print_all_water(t_vis *vis)
{
	t_point start;
	t_point end;

	vis->is_rotate_or_csale = TRUE;
	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube((void *)vis, &ft_print_water_in_cell, &start, &end);
	vis->is_rotate_or_csale = FALSE;
}





void	ft_refresh_picture(t_vis *vis)
{
	mlx_clear_window(vis->mlx, vis->win);
	ft_bzero((void *)vis->pic.addr, CONST_WIDTH * CONST_HEINTH * 4);
	ft_memset((void *)vis->pic.near, 0x80, CONST_WIDTH * CONST_HEINTH * 4);

	//ft_change_points5(vis);
	if (!vis->is_need_print_obstacles)
		ft_print_poligons(vis, vis->plgn);
	ft_print_all_water(vis);
	//ft_print_lines(vis, vis->lines);

	mlx_put_image_to_window(vis->mlx, vis->win, vis->img, 0, 0);
}






void	ft_print_int(void *param, int j, int i, int k)
{
	int ***arr;

	arr = (int ***)param;
	printf(" %2d", arr[j][i][k]);
	if (i == imax + 1)
		printf("\n");
}

void	ft_print_char(void *param, int j, int i, int k)
{
	char ***arr;

	arr = (char ***)param;
	printf(" %c", arr[j][i][k]);
	if (i == imax + 1)
		printf("\n");
}

void	ft_print_real(void *param, int j, int i, int k)
{
	REAL ***arr;
	char *str;

	if (sizeof(REAL) == sizeof(double))
		str = " %6.2lf";
	else if (sizeof(REAL) == sizeof(float))
		str = " %6.2f";
	else
		return ;
	arr = (REAL ***)param;
	printf(str, arr[j][i][k]);
	if (i == imax + 1)
		printf("\n");
}

void	ft_print_arr(void *arr, void (*f)(void *, int, int, int), int k)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, jmax + 1, 0, k);
	ft_fill_point(&end, 0, imax + 1, k);
	ft_cycle_cube(arr, f, &start, &end);
}




int loop_hook(void *param)
{
	t_vis *vis;

	vis = (t_vis *)param;

	if (!vis->pause)
	{
		ft_solver();
		ft_recalk_parts();
	}


	ft_print_arr(flags, &ft_print_int, 6);
	ft_print_arr(map, &ft_print_char, 6);
	//ft_print_arr(flags_surface, &ft_print_int, 6);
	ft_print_arr(press_p, &ft_print_real, 6);
	ft_print_arr(speed_v, &ft_print_real, 6);
	//ft_print_flags(fluid, fluid->flags);
	//
	//ft_print_fluid(fluid, fluid->speed_v);
	//printf("%d_%d\n", fluid->jmax, fluid->imax);
	/*int j = 1;
	while (j <= jmax)
	{
		printf("%s\n", map[j][2] + 1);
		j++;
	}*/
	//ft_print_fluid(fluid, fluid->speed_v);
	//ft_print_flags(fluid, fluid->flags);
	//ft_print_flags2(fluid, fluid->flags, C_X);
	ft_refresh_picture(vis);
	//ft_new_pos_of_points(vis);
	#ifdef SLEEP
	sleep(1);
	#endif
	return (0);
}


void	ft_del_each_vektrs(void *param, int j, int i, int k)
{
	ft_del_vektor(&(parts[j][i][k]));
}




void ft_del_all_print_error(char *msg_error)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 0, 0, 0);
	ft_fill_point(&end, jmax + 1, imax + 1, kmax + 1);
	if (parts)
		ft_cycle_cube(NULL, &ft_del_each_vektrs, &start, &end);
	if (vis)
	{
		free(vis->pic.addr);
		free(vis->pic.near);
		ft_del_vektor(&(vis->points));
		ft_del_lines(&(vis->lines));
		free(vis);
	}
	if (msg_error)
		ft_putendl_fd(msg_error, 2);
	ft_del_variable();
}


void	ft_create_stable_level_of_water(void *param, int j, int i, int k)
{
	int num;;

	num = PARTS_COUNT;

	if (map[j][i][k] == EMPTY && j < TEST_WATER_LEVEL// && j < TEST_WATER_LEVEL + 3
	&& i > TEST_WATER_WALL)
	{
		map[j][i][k] = WATER;
		ft_create_new_water_in_cell((void *)(&num), j, i, k);
	}
}


void	ft_create_first_water(void)
{

	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube(NULL, &ft_create_stable_level_of_water, &start, &end);
}


int main(int ac, char **av)
{
	vis = ft_memalloc(sizeof(t_vis));
	ft_create_xyz(vis);
	vis->ang_z = M_PI;
	vis->is_rotate_or_csale = TRUE;


	if (ft_read_ground_from_file2("text.txt") == FAIL)
		return (0);

	//создаем все массивы, проставляем начальные значения скоростей и давления
	//инициализируем все глобальные переменные
	ft_initialization();

	//заполняем 3д карту с 2д рельефа
	ft_fill_map_from_ground(ground);

	/*int num = PARTS_COUNT;
	map[2][2][2] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 2, 2, 2);
	map[2][3][2] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 2, 3, 2);
	map[2][2][3] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 2, 2, 3);
	map[2][3][3] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 2, 3, 3);
	map[3][2][2] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 3, 2, 2);
	map[4][2][2] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 4, 2, 2);*/
	ft_create_first_water();
	/*int num = PARTS_COUNT;
	map[4][2][2] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 4, 2, 2);*/
	ft_init_first_value();
	//ft_solver();

	//создаем модель для 3д карты
	ft_create_points_in_cells(vis);

	//создаем имейдж и z-буфер
	ft_create_img(vis);

	mlx_hook(vis->win, 2, 0, deal_key, (void *)vis);
	mlx_loop_hook(vis->mlx, loop_hook, (void *)vis);
	mlx_loop(vis->mlx);

	return (0);
}
