#ifndef _MY_UART_H_
#define _MY_UART_H_

void uart_init(void);
void uart_action(const char* pc_msg[]);
void uart_send(const char* pc_msg);

#endif  //_MY_UART_H_