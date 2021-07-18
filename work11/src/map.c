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

#include "../includes/ft_mod1.h"
#include <math.h>

/*
считываем текстовый документ
делим сплитом по пробелам
делим по запятым
наполняем

*/

REAL	ft_return_heigth(REAL value)
{
	value = value * (JMAX - J0) * HEIGTH_KOEF / (MAP_HEIGTH2) + 1;
	return (value);
}


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
	map->p[i].y = ft_atoi(args[2]);//высота
	map->p[i].x = ft_atoi(args[0] + 1);
	map->p[i].z = ft_atoi(args[1]);
	map->p_max.y = ft_max(map->p_max.y, map->p[i].y);
	map->p_max.x = ft_max(map->p_max.x, map->p[i].x);
	map->p_max.z = ft_max(map->p_max.z, map->p[i].z);
	map->p_min.y = ft_min(map->p_min.y, map->p[i].y);
	map->p_min.x = ft_min(map->p_min.x, map->p[i].x);
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
		ft_mem_arr_free((void ***)&tmp);
		if (!valid)
			return (FALSE);
		(map->count)++;
	}
	return (TRUE);
}



void	ft_create_first_and_last_points(t_map *map)
{
	int delta_y;
	int delta_x;
	int delta_z;
	int delta;

	delta_y = (map->p_max.y - map->p_min.y) / ((JMAX + J0) * MAP_KOEF);
	delta_x = (map->p_max.x - map->p_min.x) / ((IMAX + I0) * MAP_KOEF);
	delta_z = (map->p_max.z - map->p_min.z) / ((KMAX + K0) * MAP_KOEF);
	delta = ft_max(delta_x, delta_y);
	delta = ft_max(delta, delta_z);
	delta = ft_max(delta, 1);
	map->delta = delta;
	ft_fill_point(&(map->first),
	((map->p_min.y + map->p_max.y) - delta * (JMAX + J0)) / 2,
	((map->p_min.x + map->p_max.x) - delta * (IMAX + I0)) / 2,
	((map->p_min.z + map->p_max.z) - delta * (KMAX + K0)) / 2);
}



REAL	ft_sigmoida(int i, int k)
{
	REAL answer;
	int x;

	x = ft_min(i - I0, IMAX - i);
	x = ft_min(x, KMAX - k);
	x = ft_min(x, k - K0);
	x = -x * x;
	answer = 1 - exp2((REAL)x);
	return (answer);
}

void	ft_superposition2(t_map *map, int i, int k)
{
	double e;
	double dist;
	double all_dist;
	double sigma;
	int num;

	e = 0;
	all_dist = 0;
	num = 0;
	//2.0 - отвечает за пологость, 0,8 - защита от касания гор потолка
	sigma = map->delta * map->delta * ft_max(IMAX, KMAX);
	while (num < map->count)
	{
		dist = ft_power(map->p[num].x - (map->first.x + map->delta * i), 2)
		+ ft_power(map->p[num].z - (map->first.z + map->delta * k), 2);
		all_dist += 1.0 / (1.0 + dist);
		e += exp2(-dist / (sigma * 2.0)) * map->p[num].y / (1.0 + dist) ;
		num++;
	}
	map->arr[k][i] += (int)((e * ft_sigmoida(i, k)) * MAP_HEIGTH2
	/ all_dist / (map->p_max.y - map->p_min.y));
}


void	ft_fill_map_arr(t_map *map)
{
	int i;
	int k;

	k = 0;
	while (k <= KMAX + 1)
	{
		i = 0;
		while (i <= IMAX + 1)
		{
			map->arr[k][i] = -map->p_min.y * MAP_HEIGTH2 / (map->p_max.y - map->p_min.y);
			if (k > K0 && k < KMAX && i > I0 && i < IMAX)
				ft_superposition2(map, i, k);
			i++;
		}
		k++;
	}
}


int		**ft_create_map(t_map *map)
{
	map->arr = NULL;
	ft_create_first_and_last_points(map);
	if (!(map->arr = (int **)ft_mem_arr_new(KMAX + 2, IMAX + 2, sizeof(int))))
		return (NULL);
	ft_fill_map_arr(map);
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
	ft_fill_point(&(map.p_min), 0, 0x7FFFFFFF, 0x7FFFFFFF);
	if ((str = ft_read_string_from_file(name))
	&& (arr = ft_strsplit(str, ' '))
	&& ft_find_points(&map, arr))
		ground = ft_create_map(&map);
	free(str);
	ft_mem_arr_free((void ***)&arr);
	return (ground);
}
