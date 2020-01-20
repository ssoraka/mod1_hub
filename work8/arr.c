/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

#define ARR_REALLOC_COEF 1.5
#define ARR_FIRST_COUNT 16
#define NEXT_START -1

void	ft_del_arr(t_arr **arr)
{
	int		i;
	void	*elem;

	if (arr && *arr)
	{
		if ((*arr)->func_del)
		{
			i = (*arr)->elems_used - 1;
			while (i >= 0)
			{
				elem = ft_arr_get(*arr, i);
				(*arr)->func_del(elem);
				i--;
			}
		}
		free((*arr)->elems);
		free(*arr);
	}
	*arr = NULL;
}

t_arr	*ft_create_arr(int elem_size, int elems_count, void (*func_del)(void *))
{
	t_arr	*arr;

	if (elems_count < ARR_FIRST_COUNT)
		elems_count = ARR_FIRST_COUNT;
	if (elem_size * elems_count <= 0)
		return (NULL);
	arr = ft_memalloc(sizeof(t_arr));
	if (arr)
	{
		arr->elems_count = elems_count;
		arr->elems_used = 0;
		arr->elem_size = elem_size;
		arr->func_del = func_del;
		arr->next = NEXT_START;
		if (!(arr->elems = ft_memalloc(elem_size * elems_count)))
			ft_del_arr(&arr);
	}
	return (arr);
}

t_arr	*ft_realloc_arr(t_arr **arr)
{
	void *tmp;
	int count;

	if (!arr || !(*arr))
		return (NULL);
	tmp = NULL;
	count = (*arr)->elems_count * ARR_REALLOC_COEF;
	if (count > (*arr)->elems_count)
		tmp = ft_memalloc(count * (*arr)->elem_size);
	if (!tmp)
		return (NULL);
	ft_memcpy(tmp, (*arr)->elems, (*arr)->elems_count * (*arr)->elem_size);
	free((*arr)->elems);
	(*arr)->elems = tmp;
	(*arr)->elems_count = count;
	return (*arr);
}

void	*ft_arr_add(t_arr **arr, void *elem)
{
	void *tmp;

	if (!arr || !(*arr) || !elem)
		return (NULL);
	if ((*arr)->elems_used == (*arr)->elems_count)
		if (!ft_realloc_arr(arr))
		{
			ft_del_all_print_error("need more memory");
			return (NULL);
		}
	tmp = (*arr)->elems + (*arr)->elems_used * (*arr)->elem_size;
	ft_memcpy(tmp, elem, (*arr)->elem_size);
	((*arr)->elems_used)++;
	return (tmp);
}

void	*ft_arr_get(t_arr *arr, int num)
{
	if (!arr || num < 0 || arr->elems_used - 1 < num)
		return (NULL);
	return (arr->elems + arr->elem_size * num);
}

void	*ft_arr_get_next(t_arr *arr)
{
	if (!arr || arr->next < NEXT_START || arr->elems_used - 1 < arr->next)
	{
		arr->next = NEXT_START;
		return (NULL);
	}
	(arr->next)++;
	return (arr->elems + arr->elem_size * arr->next);
}

void	ft_del_elem(t_arr *arr, int num)
{
	void *dst;
	void *src;

	if (!arr || arr->elems_used == 0 || num < 0 || arr->elems_used - 1 < num)
		return ;
	(arr->elems_used)--;
	dst = arr->elems + arr->elem_size * num;
	src = arr->elems + arr->elem_size * arr->elems_used;
	if (arr->func_del)
		arr->func_del(dst);
	ft_memcpy(dst, src, arr->elem_size);
}

void	ft_del_elems(t_arr *arr, int (*need_del)(void *))
{
	int i;
	void *elem;

	i = arr->elems_used - 1;
	while (i >= 0)
	{
		elem = ft_arr_get(arr, i);
		if (need_del(elem))
			ft_del_elem(arr, i);
		i--;
	}
}

void	ft_for_each_elem(t_arr *arr, void (*func)(void *, void *), void *param)
{
	int i;
	void *elem;

	if (!func)
		return ;
	i = 0;
	while (i < arr->elems_used)
	{
		elem = ft_arr_get(arr, i);
		if (elem)
			func(elem, param);
		i++;
	}
}

void	ft_for_each_ptr(t_arr *arr, void (*func)(void *, void *), void *param)
{
	void **elem;
	int i;

	if (!func)
		return ;
	elem = (void **)ft_arr_get(arr, 0);
	i = 0;
	while (i < arr->elems_used)
	{
		if (*elem)
			func(*elem, param);
		elem++;
		i++;
	}
}

typedef struct		s_thread
{
	int				start;
	int				end;
	struct s_arr	*arr;
	void			*param;
	void			(*func)(void *, void *);
}					t_thread;


void	*ft_use_thread(void *param)
{
	t_thread *thread;
	void **elem;
	int i;

	thread = param;
	i = thread->start;
	elem = (void **)ft_arr_get(thread->arr, i);
	while (thread->start < thread->end)
	{
		if (*elem)
			thread->func(*elem, thread->param);
		elem++;
		(thread->start)++;
	}
	pthread_exit(0);
	return (NULL);
}

void	ft_for_each_ptr2(t_arr *arr, void (*func)(void *, void *), void *param)
{
	pthread_t tid[THREAD_COUNT]; /* идентификатор потока */
	pthread_attr_t attr; /* атрибуты потока */
	t_thread thrd[THREAD_COUNT];
	int i;
	int count;

	pthread_attr_init(&attr);
	/* создаем новый поток */
	count = arr->elems_used / THREAD_COUNT;
	i = -1;
	while (++i < THREAD_COUNT)
	{
		thrd[i].arr = arr;
		thrd[i].param = param;
		thrd[i].start = count * i;
		thrd[i].end = count * (i + 1);
		if (i == THREAD_COUNT - 1)
			thrd[i].end = arr->elems_used;
		thrd[i].func = func;
		pthread_create(&tid[i], &attr, ft_use_thread, &thrd[i]);
	}
	/* ждем завершения исполнения потока */
	i = -1;
	while (++i < THREAD_COUNT)
		pthread_join(tid[i],NULL);
}
