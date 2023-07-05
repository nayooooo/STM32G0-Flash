#include <stdio.h>

#if 1

#include "usart.h"

static UART_HandleTypeDef* const serial = &huart2;
static const uint32_t time_Out = 0XFFFFFF;

#if (__ARMCC_VERSION >= 6010050)				/* 使用AC6编译器 */
__asm (".global __use_no_semihosting\n\t");		/* 声明不使用半主机模式 */
__asm (".global __ARM_use_no_argv \n\t");		/* AC6不需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */
#else											/* 使用AC5编译器 */
#pragma import(__use_no_semihosting)

struct __FILE
{
	int handle;
}
#endif

// 不使用半主机模式，至少需要重定义__ttywrch\_sys_exit\_sys_command_string函数，以同时兼容AC6和AC5模式
int _ttywrch(int ch)
{
	ch = ch;
	return ch;
}

// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}

char *_sys_command_string(char *cmd, int len)
{
	return NULL;
}

// FILE在stdio.h里面定义
FILE __stdout;
#endif  /* 代码法 */

#if 1

// 重定义fputc函数
int fputc(int ch, FILE* f)
{
	HAL_UART_Transmit(serial, (const uint8_t*)&ch, 1, time_Out);
	return ch;
}

#endif // 重映射printf至串口打印
