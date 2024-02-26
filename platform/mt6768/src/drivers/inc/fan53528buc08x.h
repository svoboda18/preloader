#ifndef _fan53528buc08x_SW_H_
#define _fan53528buc08x_SW_H_

#define fan53528BUC08X_ID		0x8108
/* Voltage setting */
#define fan53528BUC08X_VSEL0		0x00
#define fan53528BUC08X_VSEL1		0x01
/* Control register */
#define fan53528BUC08X_CONTROL		0x02
/* IC Type */
#define fan53528BUC08X_ID1		0x03
/* IC mask version */
#define fan53528BUC08X_ID2		0x04
/* Monitor register */
#define fan53528BUC08X_MONITOR		0x05

extern void fan53528buc08x_dump_register(void);
extern void fan53528buc08x_driver_probe(void);
/*extern int fan53528buc08x_vosel(unsigned long val);*/
extern int fan53528buc08x_set_voltage(unsigned long val);
extern unsigned long fan53528buc08x_get_voltage(void); // if return 0 --> get fail
extern int fan53528buc08x_enable(unsigned char en);
extern int fan53528buc08x_is_enabled(void); // if return -1 --> get fail
extern int fan53528buc08x_set_mode(unsigned char mode);
extern unsigned int fan53528buc08x_read_byte(unsigned char cmd, unsigned char *returnData);
extern int is_fan53528buc08x_exist(void);
extern int g_fan53528buc08x_hw_exist;
#endif				/* _fan53528buc08x_SW_H_ */
