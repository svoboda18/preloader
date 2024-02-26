#ifndef _fan53526_SW_H_
#define _fan53526_SW_H_

#define FAN53526_ID		0x8108
/* Voltage setting */
#define FAN53526_VSEL0		0x00
#define FAN53526_VSEL1		0x01
/* Control register */
#define FAN53526_CONTROL	0x02
/* IC Type */
#define FAN53526_ID1		0x03
/* IC mask version */
#define FAN53526_ID2		0x04
/* Monitor register */
#define FAN53526_MONITOR	0x05

extern void fan53526_dump_register(void);
extern void fan53526_driver_probe(void);
/*extern int fan53526_vosel(unsigned long val);*/
extern int fan53526_is_enabled(void);
extern int fan53526_enable(unsigned char en);
extern int fan53526_set_voltage(unsigned long val);
extern int fan53526_set_mode(unsigned char mode);
extern unsigned int fan53526_get_voltage(void);
extern int fan53526_enable(unsigned char en);
extern unsigned int fan53526_read_byte(unsigned char cmd, unsigned char *returnData);
extern int is_fan53526_exist(void);
extern int g_fan53526_hw_exist;
#endif				/* _fan53526_SW_H_ */
