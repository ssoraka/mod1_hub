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


char *g_names[PROGRAMS_COUNT] = {
	"kernel.cl",
	"kernel2.cl"
};

char *g_kernel[PROGRAMS_COUNT] = {
	"test",
	"test2"
};


void	ft_error_and_exit(t_open_cl *cl, char *message)
{
	if (message)
		ft_putstr_fd(message, 2);
	ft_free_open_cl(&cl);
	exit (0);
}





#define ELEM_COUNT 32

int main(int argc, char **argv)
{
	t_open_cl *cl;

	//инициализируем сл и компилим программы
	if (!(cl = ft_init_open_cl()))
		ft_error_and_exit(cl, "init error\n");
	if (!ft_read_and_build_programs(cl))
		ft_error_and_exit(cl, "some error\n");

	char *src1 = ft_memalloc(CELL_COUNT * sizeof(t_cell));
	char *src2 = ft_memalloc(PART_COUNT * sizeof(t_part));

	//создаем муферы и копируем в них инфу
	if (!ft_create_buffers(cl, CELLS, (void *)src1, CELL_COUNT * sizeof(t_cell)))
		ft_error_and_exit(cl, "buffer error\n");
	if (!ft_create_buffers(cl, PARTS, (void *)src2, PART_COUNT * sizeof(t_part)))
		ft_error_and_exit(cl, "buffer error\n");

	//привязываем аргументы к программам

	if (!ft_set_kernel_arg(cl))
		ft_error_and_exit(cl, "set error\n");

	//запускаем программы
	if (!ft_run_kernels(cl))
		ft_error_and_exit(cl, "run error\n");


	//считываем буффер
	if (!ft_read_buffers(cl, CELLS, (void *)src1, CELL_COUNT * sizeof(t_cell)))
		ft_error_and_exit(cl, "read error\n");
	if (!ft_read_buffers(cl, PARTS, (void *)src2, PART_COUNT * sizeof(t_part)))
		ft_error_and_exit(cl, "read error\n");

	ft_free_open_cl(&cl);

	free((void *)src1);
	free((void *)src2);

	return (0);
}
