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
#define SIZE_OF_PTR 8

void	*ft_arr_get_addr(t_arr *arr, int num);

void	*ft_return_elem(void *elem)
{
	return (elem);
}

void	*ft_return_ptr(void *elem)
{
	return (*((void **)elem));
}

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
				elem = ft_arr_get_addr(*arr, i);
				(*arr)->func_del((*arr)->value(elem));
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

	if (elems_count != 0 && elems_count < ARR_FIRST_COUNT)
		elems_count = ARR_FIRST_COUNT;
	if (elem_size * elems_count < 0)
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
		arr->current = arr->elems - arr->elem_size;
		arr->value = &ft_return_elem;
	}
	return (arr);
}

t_arr	*ft_create_arr_of_ptr(int elems_count, void (*func_del)(void *))
{
	t_arr	*arr;

	arr = ft_create_arr(SIZE_OF_PTR, elems_count, func_del);
	if (!arr)
		return (NULL);
	arr->value = &ft_return_ptr;
	return (arr);
}

int		ft_realloc_arr(t_arr *arr, int new_count)
{
	void *tmp;

	if (!arr)
		return (FALSE);
	tmp = NULL;
	if (new_count > arr->elems_count)
		tmp = ft_memalloc(new_count * arr->elem_size);
	else
		return (TRUE);
	if (!tmp)
		return (FALSE);
	ft_memcpy(tmp, arr->elems, arr->elems_count * arr->elem_size);
	free(arr->elems);
	arr->elems = tmp;
	arr->elems_count = new_count;
	arr->current = arr->elems - arr->elem_size;
	return (TRUE);
}

void	*ft_arr_add(t_arr *arr, void *elem)
{
	void *tmp;

	if (!arr || !elem)
		return (NULL);
	if (arr->elems_used == arr->elems_count)
		if (!ft_realloc_arr(arr, arr->elems_count * ARR_REALLOC_COEF + 1))
			return (NULL);
	tmp = arr->elems + arr->elems_used * arr->elem_size;
	if (arr->value == ft_return_ptr)
		ft_memcpy(tmp, &elem, arr->elem_size);
	else
		ft_memcpy(tmp, elem, arr->elem_size);
	(arr->elems_used)++;
	return (tmp);
}

void	*ft_arr_get(t_arr *arr, int num)
{
	if (!arr || num < 0 || arr->elems_used - 1 < num)
		return (NULL);
	return (arr->value(arr->elems + arr->elem_size * num));
}

void	*ft_arr_get_addr(t_arr *arr, int num)
{
	if (!arr || num < 0 || arr->elems_used - 1 < num)
		return (NULL);
	return (arr->elems + arr->elem_size * num);
}

void	*ft_arr_get_next(t_arr *arr)
{
	(arr->next)++;
	if (arr->next < arr->elems_used)
		arr->current += arr->elem_size;
	else
	{
		arr->next = NEXT_START;
		arr->current = arr->elems - arr->elem_size;
		return (NULL);
	}
	return (arr->value(arr->current));
}

void	ft_del_elem(t_arr *arr, int num)
{
	void *dst;
	void *src;

	if (!arr || arr->elems_used == 0 || num < 0 || num >= arr->elems_used)
		return ;
	(arr->elems_used)--;
	dst = arr->elems + arr->elem_size * num;
	src = arr->elems + arr->elem_size * arr->elems_used;
	if (arr->func_del)
		arr->func_del(arr->value(dst));
	ft_memcpy(dst, src, arr->elem_size);
}

void	ft_del_elems_if(t_arr *arr, int (*need_del)(void *, void *), void *param)
{
	int i;
	void *elem;

	i = arr->elems_used - 1;
	elem = ft_arr_get_addr(arr, i);
	while (i >= 0)
	{
		if (need_del(arr->value(elem), param))
			ft_del_elem(arr, i);
		elem -= arr->elem_size;
		i--;
	}
}

void	ft_for_each_elem(t_arr *arr, void (*func)(void *, void *), void *param)
{
	int i;
	void *elem;

	if (!func)
		return ;
	elem = arr->elems;
	i = 0;
	while (i < arr->elems_used)
	{
		if ((arr->value(elem)))
			func(arr->value(elem), param);
		elem += arr->elem_size;
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
	elem = (void **)ft_arr_get_addr(thread->arr, i);
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
