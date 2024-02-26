#ifndef _fan53555_SW_H_
#define _fan53555_SW_H_

#define FAN53555UC04X_ID	0x840F
/* Voltage setting */
#define FAN53555_VSEL0		0x00
#define FAN53555_VSEL1		0x01
/* Control register */
#define FAN53555_CONTROL	0x02
/* IC Type */
#define FAN53555_ID1		0x03
/* IC mask version */
#define FAN53555_ID2		0x04
/* Monitor register */
#define FAN53555_MONITOR	0x05

extern void fan53555_dump_register(void);
extern void fan53555_driver_probe(void);


#endif				/* _fan53555_SW_H_ */
