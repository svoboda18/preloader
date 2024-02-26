#ifndef _PL_REGULATOR_H_
#define _PL_REGULATOR_H_

struct consumer {
	const char *name;
};

struct regulator {
    const char *name;
    unsigned char id;
    struct regulator_ctrl *reg_ops;
};

enum {
    REGULATOR_MODE_FAST,
    REGULATOR_MODE_NORMAL,
};

struct regulator *regulator_get(struct consumer *dev, const char *id);
int regulator_enable(struct regulator *regulator);
int regulator_disable(struct regulator *regulator);
int regulator_set_voltage(struct regulator *regulator, int min_uV, int max_uV);
int regulator_get_voltage(struct regulator *regulator);
int regulator_is_enabled(struct regulator *regulator);
int regulator_set_mode(struct regulator *regulator, unsigned int mode);
unsigned int regulator_get_mode(struct regulator *regulator);
void regulator_put(struct regulator *regulator);

#endif
