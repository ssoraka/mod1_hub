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



void pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data)
{
	printf("!!!%s!!!\n", errinfo);
}

void pfn_notify2(cl_program program, void *user_data)
{
	printf("!!!Что-то не так с программой!!!\n");
}

#define PARAM_PLATFORM_COUNT 5

cl_platform_info	ft_platform_info(int param_number)
{
	if (param_number == 0)
		return (CL_PLATFORM_PROFILE);
	if (param_number == 1)
		return (CL_PLATFORM_VERSION);
	if (param_number == 2)
		return (CL_PLATFORM_NAME);
	if (param_number == 3)
		return (CL_PLATFORM_VENDOR);
	return (CL_PLATFORM_EXTENSIONS);
}

#define PARAM_DEVICE_COUNT 14

cl_device_info	ft_device_info(int param_number)
{
	if (param_number == 0)
		return (CL_DEVICE_TYPE);
	if (param_number == 1)
		return (CL_DEVICE_VENDOR_ID);
	if (param_number == 2)
		return (CL_DEVICE_MAX_COMPUTE_UNITS);
	if (param_number == 3)
		return (CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
	if (param_number == 4)
		return (CL_DEVICE_MAX_WORK_GROUP_SIZE);
	if (param_number == 5)
		return (CL_DEVICE_MAX_CLOCK_FREQUENCY);
	if (param_number == 6)
		return (CL_DEVICE_ADDRESS_BITS);
	if (param_number == 7)
		return (CL_DEVICE_MAX_MEM_ALLOC_SIZE);
	if (param_number == 8)
		return (CL_DEVICE_IMAGE_MAX_BUFFER_SIZE);
	if (param_number == 9)
		return (CL_DEVICE_NAME);
	if (param_number == 10)
		return (CL_DEVICE_VENDOR);
	if (param_number == 11)
		return (CL_DRIVER_VERSION);
	if (param_number == 12)
		return (CL_DEVICE_VERSION);
	return (CL_DEVICE_MAX_WORK_ITEM_SIZES);
}

void	ft_print_device_info(int i, void *param_value)
{
	if (i == 0)
		printf("%d_%d\n", i, *((cl_uint *)param_value));
	if (i == 1)
		printf("%d_%d\n", i, *((cl_uint *)param_value));
	if (i == 2)
		printf("%d_%d\n", i, *((cl_uint *)param_value));
	if (i == 3)
		printf("%d_%d\n", i, *((cl_uint *)param_value));
	if (i == 4)
		printf("%d_%zu\n", i, *((size_t *)param_value));
	if (i == 5)
		printf("%d_%d\n", i, *((cl_uint *)param_value));
	if (i == 6)
		printf("%d_%d\n", i, *((cl_uint *)param_value));
	if (i == 7)
		printf("%d_%llu\n", i, *((cl_ulong *)param_value));
	if (i == 8)
		printf("%d_%zu\n", i, *((size_t *)param_value));
	if (i == 9)
		printf("%d_%s\n", i, (char *)param_value);
	if (i == 10)
		printf("%d_%s\n", i, (char *)param_value);
	if (i == 11)
		printf("%d_%s\n", i, (char *)param_value);
	if (i == 12)
		printf("%d_%s\n", i, (char *)param_value);
}



void ft_context_error(cl_int errcode_ret)
{
	if (errcode_ret == CL_INVALID_PLATFORM)
		printf("%s\n", "CL_INVALID_PLATFORM");
	if (errcode_ret == CL_INVALID_PROPERTY)
		printf("%s\n", "CL_INVALID_PROPERTY");
	if (errcode_ret == CL_INVALID_VALUE)
		printf("%s\n", "CL_INVALID_VALUE");
	if (errcode_ret == CL_INVALID_DEVICE)
		printf("%s\n", "CL_INVALID_DEVICE");
	if (errcode_ret == CL_DEVICE_NOT_AVAILABLE)
		printf("%s\n", "CL_DEVICE_NOT_AVAILABLE");
	if (errcode_ret == CL_OUT_OF_RESOURCES)
		printf("%s\n", "CL_OUT_OF_RESOURCES");
	if (errcode_ret == CL_OUT_OF_HOST_MEMORY)
		printf("%s\n", "CL_OUT_OF_HOST_MEMORY");
}

void ft_queue_error(cl_int errcode_ret)
{
	if (errcode_ret == CL_INVALID_CONTEXT)
		printf("%s\n", "CL_INVALID_CONTEXT");
	if (errcode_ret == CL_INVALID_DEVICE)
		printf("%s\n", "CL_INVALID_DEVICE");
	if (errcode_ret == CL_INVALID_VALUE)
		printf("%s\n", "CL_INVALID_VALUE");
	if (errcode_ret == CL_INVALID_QUEUE_PROPERTIES)
		printf("%s\n", "CL_INVALID_QUEUE_PROPERTIES");
	if (errcode_ret == CL_OUT_OF_RESOURCES)
		printf("%s\n", "CL_OUT_OF_RESOURCES");
	if (errcode_ret == CL_OUT_OF_HOST_MEMORY)
		printf("%s\n", "CL_OUT_OF_HOST_MEMORY");
}


#define PROGRAMM_SIZE 3000

int is_read_programm(char **buffer, char *filename)
{
	int fd;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (FALSE);
	if (read(fd, NULL, 0) == -1)
		return (FALSE);
	if (!(*buffer = ft_memalloc(PROGRAMM_SIZE)))
		return (FALSE);
	read(fd, *buffer, PROGRAMM_SIZE);
	close(fd);
	return (TRUE);
}


void	ft_error_and_exit(t_open_cl *cl, char *message)
{
	if (message)
		ft_putstr_fd(message, 2);
	clFlush(cl->queue);
	clFinish(cl->queue);
	clReleaseCommandQueue(cl->queue);
	clReleaseMemObject(cl->buffer);
	clReleaseProgram(cl->program);
	clReleaseKernel(cl->kernel);
	clRetainContext(cl->context);
	ft_memdel((void **)&cl->prog_text);
	exit(0);
}







#define ELEM_COUNT 32

int main(int argc, char **argv)
{

	t_open_cl cl;
	ft_bzero((void *)&cl, sizeof(t_open_cl));

	cl_uint num_entries = 100;

	if (clGetPlatformIDs(num_entries, &cl.platform, &cl.num_platform) != CL_SUCCESS)
		ft_error_and_exit(&cl, "platform error!!!\n");


	/*
	//вывод парамеров платформы
	cl_platform_info param_name;
	size_t param_value_size = 10000;
	void *param_value = ft_memalloc(param_value_size);
	size_t param_value_size_ret = 0;

	int i;

	i = 0;
	while (i < PARAM_PLATFORM_COUNT)
	{
		param_name = ft_platform_info(i);
		if (clGetPlatformInfo(platform, param_name, param_value_size, param_value, &param_value_size_ret) != CL_SUCCESS)
			ft_putstr("Error!!!\n");
		printf("%s\n", (char *)param_value);
		i++;
	}*/




	cl.device_type = CL_DEVICE_TYPE_GPU;
	cl_uint num_entries2 = 10;

	if (clGetDeviceIDs(cl.platform, cl.device_type, num_entries2, &cl.device, &cl.num_devices) != CL_SUCCESS)
		ft_error_and_exit(&cl, "device error!!!\n");

	/*
	//вывод парамеров девайса
	cl_device_info param_name2;
	i = 0;
	while (i < PARAM_DEVICE_COUNT)
	{
		param_name2 = ft_device_info(i);
		if (clGetDeviceInfo(devices, param_name2, param_value_size, param_value, &param_value_size_ret) != CL_SUCCESS)
			ft_putstr("Error!!!\n");
		ft_print_device_info(i, param_value);
		i++;
	}*/

	cl_context_properties properties = CL_CONTEXT_PLATFORM;// хз, не понял, как с этим работать
	//cl_context_properties properties = (cl_context_properties)platform;

	void *user_data = (void *)"";

	cl.context = clCreateContext(NULL, cl.num_devices, &cl.device, &pfn_notify, user_data, &cl.errcode_ret);
	if (cl.errcode_ret != CL_SUCCESS)
	{
		ft_context_error(cl.errcode_ret);
		ft_error_and_exit(&cl, "context error!!!\n");
	}

	cl_command_queue_properties	qproperties = 0;//CL_QUEUE_PROPERTIES;

	cl.queue = clCreateCommandQueue(cl.context, cl.device, qproperties, &cl.errcode_ret);
	if (cl.errcode_ret != CL_SUCCESS)
	{
		ft_queue_error(cl.errcode_ret);
		ft_error_and_exit(&cl, "queue error!!!\n");
	}


	cl_mem_flags flags = CL_MEM_READ_WRITE;
	size_t size = sizeof(cl_int) * ELEM_COUNT;


	cl.buffer = clCreateBuffer(cl.context, flags, size, NULL, &cl.errcode_ret);
	if (cl.errcode_ret != CL_SUCCESS)
		ft_error_and_exit(&cl, "buffer error!!!\n");



	if (!is_read_programm(&cl.prog_text, "kernel.cl"))
		ft_error_and_exit(&cl, "prog_text error!!!\n");


	cl_uint count = 1; //количество строк в массиве программ
	size_t lengths[count]; //массив длина программ
	lengths[0] = ft_strlen(cl.prog_text);

	cl.program = clCreateProgramWithSource(cl.context, count, (const char **)&cl.prog_text, lengths, &cl.errcode_ret);
	if (cl.errcode_ret != CL_SUCCESS)
		ft_error_and_exit(&cl, "program error!!!\n");



	char *options = NULL;
	user_data = (void *)"Мои данные на случай ошибки";

	//if (clBuildProgram(cl.program, cl.num_devices, &cl.device, options, &pfn_notify2, user_data) != CL_SUCCESS)

	if (clBuildProgram(cl.program, cl.num_devices, &cl.device, NULL, NULL, NULL) != CL_SUCCESS)
		ft_error_and_exit(&cl, "build error!!!\n");

	/*
	//вывод логов компиляции программы
	cl_program_build_info param_name1 = CL_PROGRAM_BUILD_LOG;
	size_t param_value_size = 10000;
	void *param_value = ft_memalloc(param_value_size);
	size_t param_value_size_ret = 0;

	if (clGetProgramBuildInfo(cl.program, cl.device, param_name1, param_value_size, param_value, &param_value_size_ret) != CL_SUCCESS)
	 	ft_putstr("Error!!!\n");
	printf("%s\n", (char *)param_value);
	*/



	char *kernel_name = "test";

	cl.kernel = clCreateKernel(cl.program, kernel_name, &cl.errcode_ret);
	if (cl.errcode_ret != CL_SUCCESS)
		ft_error_and_exit(&cl, "kernel error!!!\n");



	if (clSetKernelArg(cl.kernel, 0, sizeof(cl_mem), (void *)&cl.buffer) != CL_SUCCESS)
	 	ft_error_and_exit(&cl, "kernel arg error!!!\n");




	cl_bool blocking_write = CL_FALSE;
	size_t offset = 0;
	void *ptr = ft_memalloc(size);

	cl_uint num_events_in_wait_list = 0;
	cl_event *event_wait_list = NULL;


	//запись в буффер
	if (clEnqueueWriteBuffer(cl.queue, cl.buffer, blocking_write, offset, size, ptr,
	0, NULL, NULL) != CL_SUCCESS)
		ft_error_and_exit(&cl, "write buffer error!!!\n");



	size_t global_work_item = ELEM_COUNT;// = 1 * 1000;
	cl_uint work_dim = 1; //<= CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS = 4099
	size_t global_work_size[1] = { ELEM_COUNT };
	cl_event event = NULL;
	cl_event event1 = NULL;
	//printf("%d\n", CL_KERNEL_WORK_GROUP_SIZE);

	if (clEnqueueNDRangeKernel(cl.queue, cl.kernel, work_dim, NULL, global_work_size,
	NULL, 0, NULL, &event) != CL_SUCCESS)
		ft_error_and_exit(&cl, "write buffer error!!!\n");

	clFinish(cl.queue);



	/*
	cl_event ev1 = clCreateUserEvent(ctx, NULL);
	clEnqueueWriteBuffer(cq, buf1, CL_FALSE, ..., 1, &ev1, NULL);
	clEnqueueWriteBuffer(cq, buf2, CL_FALSE,...);
	clSetUserEventStatus(ev1, CL_COMPLETE);
	clReleaseMemObject(buf2);
	cl_int clWaitForEvents (cl_uint num_events, const cl_event *event_list);
	cl_int clSetEventCallback (cl_event event, cl_int command_exec_callback_type,
	void (CL_CALLBACK *pfn_event_notify)(cl_event event, cl_int event_command_exec_status,
	void *user_data), void *user_data);
	*/


	cl_bool blocking_read = CL_FALSE;
	void *ptr2 = ft_memalloc(size);
	if (clEnqueueReadBuffer(cl.queue, cl.buffer, blocking_read, offset, size, ptr2,
	0, NULL, NULL) != CL_SUCCESS)
		ft_error_and_exit(&cl, "read buffer error!!!\n");

	int n = 0;
	int *p = ptr2;
	while (n < ELEM_COUNT)
	{
		printf("%d_%d\n",n, p[n]);
		n++;
	}



	ft_memdel(&ptr2);
	ft_memdel(&ptr);

	ft_error_and_exit(&cl, "close!!!\n");


	/*flags = CL_MEM_READ_WRITE;
	printf("%s\n", ft_bytes_to_bits((void *)&flags, sizeof(cl_mem_flags), " "));
	flags = CL_MEM_WRITE_ONLY;
	printf("%s\n", ft_bytes_to_bits((void *)&flags, sizeof(cl_mem_flags), " "));
	flags = CL_MEM_READ_ONLY;
	printf("%s\n", ft_bytes_to_bits((void *)&flags, sizeof(cl_mem_flags), " "));
	flags = CL_MEM_USE_HOST_PTR;
	printf("%s\n", ft_bytes_to_bits((void *)&flags, sizeof(cl_mem_flags), " "));
	flags = CL_MEM_ALLOC_HOST_PTR;
	printf("%s\n", ft_bytes_to_bits((void *)&flags, sizeof(cl_mem_flags), " "));
	flags = CL_MEM_COPY_HOST_PTR;
	printf("%s\n", ft_bytes_to_bits((void *)&flags, sizeof(cl_mem_flags), " "));*/
	return (0);
}
