/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 19:05:18 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/04 19:05:19 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	ft_init_variable(void)
{
	g_cell = NULL;
	g_parts = NULL;
	g_iparts = NULL;
	g_cl = NULL;
}

void	ft_init_delta_xyz(void)
{
	imax = IMAX;
	jmax = JMAX;
	kmax = KMAX;

	g.x = CONST_GX;
	g.y = CONST_GY;
	g.z = CONST_GZ;
}

void ***ft_cube_arr(int jmax, int imax, int kmax, int size)
{
	void ***arr;
	int j;

	if (!(arr = (void ***)ft_memalloc(sizeof(void **) * (jmax + 1))))
		return (NULL);
	j = 0;
	while (j < jmax)
	{
		if (!(arr[j] = (void **)ft_mem_arr_new(imax, kmax, size)))
		{
			ft_del_cube_arr(&arr);
			break ;
		}
		j++;
	}
	return (arr);
}

void	ft_init_parts_arr_and_cell(void)
{
	if (!(vis = ft_create_img()))
		ft_del_all(NULL);
	if (!(g_parts = ft_init_all_clear_parts()))
		ft_del_all(NULL);
	if (!(g_iparts = ft_create_arr(sizeof(t_ipart), g_parts->elems_count, NULL)))
		ft_del_all(NULL);
	if (!(g_cell = ft_memalloc(sizeof(t_cell) * (CELL_COUNT + 1))))
		ft_del_all(NULL);

	//инициализируем сл и компилим программы
	if (!(g_cl = ft_init_open_cl()))
		ft_del_all("init error\n");
	if (!ft_read_and_build_programs(g_cl, g_compile))
		ft_del_all("some error\n");
}


void	ft_initialization_of_global_variable(void)
{
	ft_init_variable();
	ft_init_delta_xyz();
	ft_init_parts_arr_and_cell();
}

void	ft_del_cube_arr(void ****arr)
{
	void	***tmp;
	int		i;

	i = 0;
	tmp = *arr;
	while (tmp[i])
	{
		ft_str_arr_free((char ***)(tmp + i));
		i++;
	}
	ft_memdel((void *)arr);
}




void	ft_del_all(char *message)
{
	if (message)
		ft_putstr_fd(message, 2);
	ft_del_arr(&g_parts);
	ft_del_arr(&g_iparts);
	ft_str_arr_free((char ***)(&ground));
	ft_memdel((void **)&g_cell);
	ft_free_open_cl(&g_cl);
	ft_img_destroy(&vis);
	exit(0);
}
