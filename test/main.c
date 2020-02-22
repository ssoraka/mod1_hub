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
	{"clear_cell.cl", "clear_cell", 1, CELLS, -1, -1, CELL_COUNT},
	{"add_part_in_cell.cl", "add_part_in_cell", 2, PARTS, CELLS, -1, PART_COUNT},
	{"up_part_in_cell.cl", "up_part_in_cell", 1, CELLS, -1, -1, CELL_COUNT},
	{"find_neighbors.cl", "find_neighbors", 2, PARTS, CELLS, -1, PART_COUNT},
	{"density_press.cl", "density_press", 1, PARTS, -1, -1, PART_COUNT},
	{"delta_speed.cl", "delta_speed", 1, PARTS, -1, -1, PART_COUNT},
	{"delta_coord.cl", "delta_coord", 1, PARTS, -1, -1, PART_COUNT},
	{"change_cell.cl", "change_cell", 3, PARTS, CELLS, INTERFACE, PART_COUNT},
	{"print_parts.cl", "print_parts", 2, PARTS, -1, -1, PART_COUNT}, //надо бы это в сунуть в другую функцию...
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
		parts[i].pos.y = 2;
		parts[i].pos.x = 2 + i * 0.870;
		parts[i].pos.z = 2;
		parts[i].jik.y = parts[i].pos.y;
		parts[i].jik.x = parts[i].pos.x;
		parts[i].jik.z = parts[i].pos.z;
		parts[i].n.count = 0;
		parts[i].speed.x = i * 0.01;
		i++;
	}
}


void ft_init_cells(void *src)
{
	t_cell *cell = (t_cell *)src;

	int i = 0;
	while (i < CELL_COUNT)
	{
		//cell[i].obstacle = 100 + i;
		i++;
	}
}


int main(int argc, char **argv)
{
	t_open_cl *cl;

	//инициализируем сл и компилим программы
	if (!(cl = ft_init_open_cl()))
		ft_error_and_exit(cl, "init error\n");
	if (!ft_read_and_build_programs(cl, g_compile))
		ft_error_and_exit(cl, "some error\n");

	char *src1 = ft_memalloc(PART_COUNT * sizeof(t_part));
	ft_init_parts(src1);
	char *src2 = ft_memalloc(CELL_COUNT * sizeof(t_cell));
	ft_init_cells(src2);
	char *src3 = ft_memalloc(PART_COUNT * sizeof(t_ipart));

	printf("%d\n", (*((t_cell *)src2)).obstacle);

	//создаем буферы и копируем в них инфу
	if (!ft_create_buffers(cl, PARTS, (void *)src1, PART_COUNT * sizeof(t_part)))
		ft_error_and_exit(cl, "buffer error\n");
	if (!ft_create_buffers(cl, CELLS, (void *)src2, CELL_COUNT * sizeof(t_cell)))
		ft_error_and_exit(cl, "buffer error\n");
	if (!ft_create_buffers(cl, INTERFACE, (void *)src3, PART_COUNT * sizeof(t_ipart)))
		ft_error_and_exit(cl, "buffer error\n");


	//привязываем аргументы к программам

	if (!ft_set_kernel_arg(cl, g_compile))
		ft_error_and_exit(cl, "set error\n");

	//запускаем программы
	if (!ft_run_kernels(cl))
		ft_error_and_exit(cl, "run error\n");


	//считываем буффер
	if (!ft_read_buffers(cl, PARTS, (void *)src1, PART_COUNT * sizeof(t_part)))
		ft_error_and_exit(cl, "read error\n");
	if (!ft_read_buffers(cl, CELLS, (void *)src2, CELL_COUNT * sizeof(t_cell)))
		ft_error_and_exit(cl, "read error\n");
	if (!ft_read_buffers(cl, INTERFACE, (void *)src3, PART_COUNT * sizeof(t_ipart)))
		ft_error_and_exit(cl, "read error\n");


	ft_free_open_cl(&cl);

	free((void *)src1);
	free((void *)src2);
	free((void *)src3);

	return (0);
}
