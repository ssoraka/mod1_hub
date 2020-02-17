/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 17:13:40 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/25 17:13:40 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"



t_prog    g_compile[PROGRAMS_COUNT + 10] =
{
	{"kernel.cl", "test", 1, CELLS, -1, -1, PART_COUNT},
	{"clear_cell.cl", "clear_cell", 1, PARTS, -1, -1, PART_COUNT},
	{"", "", 0, 0, 0, 0, 0}
};

void	ft_error_and_exit(t_open_cl *cl, char *message)
{
	if (message)
		ft_putstr_fd(message, 2);
	ft_free_open_cl(&cl);
	exit (0);
}


void ft_init_parts(void *src)
{
	t_part *parts = (t_part *)src;

	int i = 0;
	while (i < PART_COUNT)
	{
		parts[i].pos.x = i * 0.87 + 10;
		parts[i].pos.y = 10;
		parts[i].pos.z = 10;
		parts[i].jik.x = parts[i].pos.x;
		parts[i].jik.y = parts[i].pos.y;
		parts[i].jik.z = parts[i].pos.z;
		i++;
	}
}


void ft_init_cells(void *src)
{
	t_cell *cell = (t_cell *)src;

	int i = 0;
	while (i < CELL_COUNT)
	{
		cell[i].obstacle = 100 + i;
		i++;
	}
}


int main(int argc, char **argv)
{
	t_open_cl *cl;

	//инициализируем сл и компилим программы
	if (!(cl = ft_init_open_cl()))
		ft_error_and_exit(cl, "init error\n");
	if (!ft_read_and_build_programs(cl))
		ft_error_and_exit(cl, "some error\n");

	char *src1 = ft_memalloc(PART_COUNT * sizeof(t_part));
	ft_init_parts(src1);
	char *src2 = ft_memalloc(CELL_COUNT * sizeof(t_cell));
	ft_init_cells(src2);

	printf("%d\n", (*((t_cell *)src2)).obstacle);

	//создаем буферы и копируем в них инфу
	if (!ft_create_buffers(cl, PARTS, (void *)src1, PART_COUNT * sizeof(t_part)))
		ft_error_and_exit(cl, "buffer error\n");
	if (!ft_create_buffers(cl, CELLS, (void *)src2, CELL_COUNT * sizeof(t_cell)))
		ft_error_and_exit(cl, "buffer error\n");


	//привязываем аргументы к программам

	if (!ft_set_kernel_arg(cl))
		ft_error_and_exit(cl, "set error\n");

	//запускаем программы
	if (!ft_run_kernels(cl))
		ft_error_and_exit(cl, "run error\n");


	//считываем буффер
	if (!ft_read_buffers(cl, PARTS, (void *)src1, PART_COUNT * sizeof(t_part)))
		ft_error_and_exit(cl, "read error\n");
	if (!ft_read_buffers(cl, CELLS, (void *)src2, CELL_COUNT * sizeof(t_cell)))
		ft_error_and_exit(cl, "read error\n");


	ft_free_open_cl(&cl);

	free((void *)src1);
	free((void *)src2);

	return (0);
}
