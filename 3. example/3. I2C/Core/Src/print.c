#include <stdio.h>

#if 1

#include "usart.h"

static UART_HandleTypeDef* const serial = &huart2;
static const uint32_t time_Out = 0XFFFFFF;

#if (__ARMCC_VERSION >= 6010050)				/* ʹ��AC6������ */
__asm (".global __use_no_semihosting\n\t");		/* ������ʹ�ð�����ģʽ */
__asm (".global __ARM_use_no_argv \n\t");		/* AC6����Ҫ����main����Ϊ�޲�����ʽ�����򲿷����̿��ܳ��ְ�����ģʽ */
#else											/* ʹ��AC5������ */
#pragma import(__use_no_semihosting)

struct __FILE
{
	int handle;
}
#endif

// ��ʹ�ð�����ģʽ��������Ҫ�ض���__ttywrch\_sys_exit\_sys_command_string��������ͬʱ����AC6��AC5ģʽ
int _ttywrch(int ch)
{
	ch = ch;
	return ch;
}

// ����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}

char *_sys_command_string(char *cmd, int len)
{
	return NULL;
}

// FILE��stdio.h���涨��
FILE __stdout;
#endif  /* ���뷨 */

#if 1

// �ض���fputc����
int fputc(int ch, FILE* f)
{
	HAL_UART_Transmit(serial, (const uint8_t*)&ch, 1, time_Out);
	return ch;
}

#endif // ��ӳ��printf�����ڴ�ӡ
