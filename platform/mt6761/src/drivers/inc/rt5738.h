#ifndef _rt5738_SW_H_
#define _rt5738_SW_H_

#define CFG_PRELOADER

#ifdef CFG_PRELOADER
#include <typedefs.h>
#endif

#define RT5738_VDD2_ID		0x0000
#define RT5738_VDDQ_ID		0x0000

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
/* Hidden mode */
#define	RT5738_CTRL5		0x09

enum {
	RT5738_VDD2,
	RT5738_VDDQ,
	RT5738_MAX,
};

struct rt5738_chip {
	kal_uint8 id;
	char *name;
	kal_uint16 i2c_channel;
	kal_uint8 slave_addr;
	kal_uint8 buck_ctrl;
	kal_uint8 mode_shift;
	kal_uint8 en_shift;
	kal_uint32 chip_id;
};

extern void rt5738_dump_register(int id);
extern void rt5738_driver_probe(void);
extern int rt5738_set_voltage(int id, unsigned long val);
extern unsigned long rt5738_get_voltage(int id); // if return 0 --> get fail
extern int rt5738_enable(int id, unsigned char en);
extern int rt5738_is_enabled(int id); // if return -1 --> get fail
extern int rt5738_set_mode(int id, unsigned char mode);
extern unsigned char rt5738_get_mode(int id);
extern int is_rt5738_exist(int id);
extern int g_rt5738_hw_exist[RT5738_MAX];
/*extern unsigned char rt5738_get_vsel0(int id);*/
/*extern void rt5738_monitor(void);*/
#endif				/* _rt5738_SW_H_ */
