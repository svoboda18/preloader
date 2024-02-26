#ifndef _rt5738_SW_H_
#define _rt5738_SW_H_

#define RT5738_ID		0x0000
/* Voltage setting */
#define RT5738_VSEL0		0x00
#define RT5738_VSEL1		0x01
/* Control register */
#define RT5738_CONTROL		0x02
/* IC Type */
#define RT5738_ID1		0x03
/* IC mask version */
#define RT5738_ID2		0x04
/* Monitor register */
#define RT5738_MONITOR		0x05
#define RT5738_CTRL2		0x06
#define RT5738_CTRL3		0x07
#define RT5738_CTRL4		0x08

extern void rt5738_dump_register(void);
extern void rt5738_driver_probe(void);
/*extern int rt5738_vosel(unsigned long val);*/
extern int rt5738_set_voltage(unsigned long val);
extern unsigned long rt5738_get_voltage(void); // if return 0 --> get fail
extern int rt5738_enable(unsigned char en);
extern int rt5738_is_enabled(void); // if return -1 --> get fail
extern int rt5738_set_mode(unsigned char mode);
extern unsigned int rt5738_read_byte(unsigned char cmd, unsigned char *returnData);
extern int is_rt5738_exist(void);
extern int g_rt5738_hw_exist;
#endif				/* _rt5738_SW_H_ */
