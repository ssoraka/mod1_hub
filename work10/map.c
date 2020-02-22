/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 10:38:09 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/17 10:38:10 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"
#include <math.h>

/*
считываем текстовый документ
делим сплитом по пробелам
делим по запятым
наполняем

*/


char	*ft_read_string_from_file(char *name)
{
	int		fd;
	char	*str;

	str = NULL;
	if ((fd = open(name, O_RDWR)) == -1)
		return (NULL);
	get_next_line(fd, &str);
	close(fd);
	return (str);
}


int		ft_find_point(t_map *map, char **args)
{
	int i;
	//тут надо сделать проверку точек на интовые значения...
	if (args[0][0] != '(' || args[2][ft_strlen(args[2]) - 1] != ')')
		return (FALSE);
	i = map->count;
	map->p[i].x = ft_atoi(args[0] + 1);
	map->p[i].y = ft_atoi(args[1]);
	map->p[i].z = ft_atoi(args[2]);
	map->p_max.x = ft_max(map->p_max.x, map->p[i].x);
	map->p_max.y = ft_max(map->p_max.y, map->p[i].y);
	map->p_max.z = ft_max(map->p_max.z, map->p[i].z);
	map->p_min.x = ft_min(map->p_min.x, map->p[i].x);
	map->p_min.y = ft_min(map->p_min.y, map->p[i].y);
	map->p_min.z = ft_min(map->p_min.z, map->p[i].z);
	return (TRUE);
}




int		ft_find_points(t_map *map, char **arr)
{
	char	**tmp;
	int		valid;

	map->count = 0;
	while (arr[map->count] && map->count < MAX_POINT)
	{
		valid = FALSE;
		tmp = ft_strsplit(arr[map->count], ',');
		if (tmp && tmp[0] && tmp[1] && tmp[2] && !tmp[3]
		&& ft_find_point(map, tmp))
			valid = TRUE;
		ft_str_arr_free(&tmp);
		if (!valid)
			return (FALSE);
		(map->count)++;
	}
	return (TRUE);
}






void	ft_create_first_and_last_points(t_map *map)
{
	int delta_x;
	int delta_y;
	int delta_z;
	int delta;

	delta_x = (map->p_max.x - map->p_min.x) / (IMAX * MAP_KOEF);
	delta_y = (map->p_max.y - map->p_min.y) / (JMAX * MAP_KOEF);
	delta_z = (map->p_max.z - map->p_min.z) / (KMAX * MAP_KOEF);
	delta = ft_max(delta_x, delta_y);
	delta = ft_max(delta, delta_z);
	delta = ft_max(delta, 1);
	map->delta = delta * 2;
	ft_fill_point(&(map->first),
	(map->p_min.y + map->p_max.y) / 2 - delta * JMAX,
	(map->p_min.x + map->p_max.x) / 2 - delta * IMAX,
	(map->p_min.z + map->p_max.z) / 2 - delta * KMAX);
}



REAL	ft_sigmoida(int i, int k)
{
	REAL answer;
	int x;

	x = ft_min(i, IMAX - 1 - i);
	x = ft_min(x, KMAX - 1 - k);
	x = ft_min(x, k);
	x = -x * x;
	answer = 1 - exp2((REAL)x);
	return (answer);
}



void	ft_superposition(void *param, int j, int i, int k)
{
	t_map *map;
	double e;
	double dist;
	double all_dist;
	double sigma;
	int num;

	map = (t_map *)param;
	e = 0;
	all_dist = 0;
	num = 0;
	//2.0 - отвечает за пологость, 0,8 - защита от касания гор потолка
	while (num < map->count)
	{
		dist = ft_power(map->p[num].x - map->first.x - map->delta * i, 2)
		+ ft_power(map->p[num].y - map->first.y - map->delta * k, 2);
		sigma = map->delta * map->delta * (IMAX);
		all_dist += 1.0 / (1.0 + dist);
		e += exp2(-dist / (sigma * 2.0)) * map->p[num].z / (1.0 + dist);
		num++;
	}
	//map->arr[k][i] = (int)(e * ft_sigmoida(i, k) / all_dist / map->p_max.z * KMAX * MAP_KOEF) + 0;// / all_dist); // / map->delta
	map->arr[k][i] = (int)(e * ft_sigmoida(i, k) / all_dist / map->p_max.z * MAP_HEIGTH2) + 0;
}


void	ft_fill_map_arr(t_map *map)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 0, I0, K0);
	ft_fill_point(&end, 0, IMAX, KMAX);
	ft_cycle_cube((void *)map, &ft_superposition, &start, &end);
}

int		**ft_create_map(t_map *map)
{
	map->arr = NULL;
	ft_create_first_and_last_points(map);
	/*printf("%d\n", map->delta);
	printf("%d\n", map->first.x);
	printf("%d\n", map->first.y);
	printf("%d\n", map->first.x + map->delta * 100);
	printf("%d\n", map->first.y + map->delta * 100);
	exit(0);*/
	if (!(map->arr = (int **)ft_mem_arr_new(KMAX + 2, IMAX + 2, sizeof(int))))
		return (NULL);
	ft_fill_map_arr(map);
	//потом надо будет сетку сделать для поверхности
	//с интерполяцией и всеми фичами...

	return (map->arr);
}


int		**ft_read_ground_from_file3(char *name)
{
	char	*str;
	char	**arr;
	int		**ground;
	t_map	map;

	ground = NULL;
	str = NULL;
	arr = NULL;
	ft_bzero((void *)&map, sizeof(t_map));
	ft_fill_point(&(map.p_min), 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF);
	if ((str = ft_read_string_from_file(name))
	&& (arr = ft_strsplit(str, ' '))
	&& ft_find_points(&map, arr))
		ground = ft_create_map(&map);
	free(str);
	ft_str_arr_free(&arr);

	//imax = ft_strlen(ground[1]);
	//jmax = MAP_HEIGTH;
	//kmax = k;

	return (ground);
}
