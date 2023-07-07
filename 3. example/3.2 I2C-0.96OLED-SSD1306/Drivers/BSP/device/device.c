#include "device.h"

char *hw_strncpy(char *dst, const char *src, hw_size_t n)
{
	if (n != 0) {
		char *d = dst;
		const char *s = src;
		
		do {
			if ((*d++ = *s++) == 0) {
				while (--n != 0) {
					*d++ = 0;
				}
				break;
			}
		} while (--n != 0);
	}
	
	return dst;
}

hw_uint32_t hw_strncmp(const char *cs, const char *ct, hw_size_t count)
{
	register signed char __res = 0;
	
	while (count) {
		if ((__res = *cs - *ct++) != 0 || !*cs++) {
			break;
		}
		count--;
	}
	
	return __res;
}

hw_err_t hw_object_init(hw_object_t obj, const char *name, hw_uint16_t flags)
{
	if (obj == HW_NULL)
		return -HW_ERROR;
	
	hw_strncpy(obj->name, name, HW_NAME_MAX);
	obj->flag = flags;
	obj->type = HW_NULL;
	
	return HW_EOK;
}

hw_err_t hw_device_register(hw_device_t dev, const char *name, hw_uint16_t flags)
{
	if (dev == HW_NULL)
		return -HW_ERROR;
	
	hw_object_init(&(dev->parent), name, HW_NULL);
	dev->flag = flags;
	dev->ref_count = 0;
	dev->open_flag = 0;
	
	return HW_EOK;
}

hw_err_t hw_device_add_method(hw_device_t dev, const char *type, void(*method)(void))
{
	if (dev == HW_NULL)
		return -HW_ERROR;
	
	if (hw_strncmp(type, "init", 4) == 0) {
		dev->init = (init_func)method;
		goto HW_DEVICE_ADD_SUCC;
	}
	if (hw_strncmp(type, "open", 4) == 0) {
		dev->open = (open_func)method;
		goto HW_DEVICE_ADD_SUCC;
	}
	if (hw_strncmp(type, "close", 5) == 0) {
		dev->close = (close_func)method;
		goto HW_DEVICE_ADD_SUCC;
	}
	if (hw_strncmp(type, "read", 4) == 0) {
		dev->read = (read_func)method;
		goto HW_DEVICE_ADD_SUCC;
	}
	if (hw_strncmp(type, "write", 5) == 0) {
		dev->write = (write_func)method;
		goto HW_DEVICE_ADD_SUCC;
	}
	if (hw_strncmp(type, "control", 7) == 0) {
		dev->control = (control_func)method;
		goto HW_DEVICE_ADD_SUCC;
	}
	
	if (0) {
	HW_DEVICE_ADD_SUCC:
		return HW_EOK;
	}
	return HW_EINVAL;
}









