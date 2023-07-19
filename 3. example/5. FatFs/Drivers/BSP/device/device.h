#ifndef __DEVICE_H
#define __DEVICE_H

#include <stdint.h>
#include <string.h>

#define HW_NULL					(0)

#define HW_EOK					(0)               /**< There is no error */
#define HW_ERROR				(1)               /**< A generic error happens */
#define HW_ETIMEOUT				(2)               /**< Timed out */
#define HW_EFULL				(3)               /**< The resource is full */
#define HW_EEMPTY				(4)               /**< The resource is empty */
#define HW_ENOMEM				(5)               /**< No memory */
#define HW_ENOSYS				(6)               /**< No system */
#define HW_EBUSY				(7)               /**< Busy */
#define HW_EIO					(8)               /**< IO error */
#define HW_EINTR				(9)               /**< Interrupted system call */
#define HW_EINVAL				(10)              /**< Invalid argument */

#define HW_NAME_MAX				(8)

typedef long					hw_base_t;
typedef hw_base_t				hw_err_t;
typedef hw_base_t				hw_off_t;

typedef unsigned int			hw_size_t;

typedef uint8_t					hw_uint8_t;
typedef uint16_t				hw_uint16_t;
typedef uint32_t				hw_uint32_t;
typedef uint64_t				hw_uint64_t;
typedef int8_t					hw_int8_t;
typedef int16_t					hw_int16_t;
typedef int32_t					hw_int32_t;
typedef int64_t					hw_int64_t;

struct hw_object
{
	char name[HW_NAME_MAX];
	hw_uint8_t type;
	hw_uint8_t flag;
};
typedef struct hw_object *hw_object_t;

typedef struct hw_device *hw_device_t;
	
typedef	hw_err_t	(*init_func)	(hw_device_t dev);
typedef	hw_err_t	(*open_func)	(hw_device_t dev, hw_uint16_t oflag);
typedef	hw_err_t	(*close_func)	(hw_device_t dev);
typedef	hw_size_t	(*read_func)	(hw_device_t dev, hw_off_t ops, void *buffer, hw_size_t size);
typedef	hw_size_t	(*write_func)	(hw_device_t dev, hw_off_t ops, const void *buffer, hw_size_t size);
typedef	hw_err_t	(*control_func)	(hw_device_t dev, int cmd, void *args);
struct hw_device
{
	struct hw_object			parent;
	
	hw_uint16_t					flag;
	hw_uint16_t					open_flag;
	
	hw_uint8_t					ref_count;
	hw_uint8_t					device_id;
	
	init_func					init;
	open_func					open;
	close_func					close;
	read_func					read;
	write_func					write;
	control_func				control;
	
	void						*user_data;
};

char *hw_strncpy(char *dst, const char *src, hw_size_t n);
hw_uint32_t hw_strncmp(const char *cs, const char *ct, hw_size_t count);

hw_err_t hw_object_init(hw_object_t obj, const char *name, hw_uint16_t flags);

hw_err_t hw_device_register(hw_device_t dev, const char *name, hw_uint16_t flags);
hw_err_t hw_device_add_method(hw_device_t dev, const char *type, void(*method)(void));

#endif  /* __DEVICE_H */









