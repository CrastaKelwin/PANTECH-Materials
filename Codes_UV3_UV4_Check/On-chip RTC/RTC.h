#ifndef __RTC_H

#define __RTC_H

void UART1_ISR(void)__irq;
void RTC_ISR (void)__irq;
void RTC_Setup (char *);
void Initialize (void);

//char Key;

#endif
