#include <regulator_core.h>
#include <regulator.h>

static struct regulator mtk_regulator_table[MTK_REGULATOR_MAX_NR];

int mtk_simple_regulator_register(struct regulator *mreg)
{
    if (mreg == 0)
        return -1;
    memcpy(&mtk_regulator_table[mreg->id], mreg,
                sizeof(struct regulator));
    dbg_print("register %s id %d OK\n", mreg->name, mreg->id);
    return 0;
}

static int mtk_regulator_enable(struct regulator *mreg, unsigned char enable)
{
    int ret = 0;

    ret = mreg->reg_ops->enable(mreg->id, enable);
    return ret;
}

static int mtk_regulator_is_enabled(struct regulator *mreg)
{
    int ret = 0;

    ret = mreg->reg_ops->is_enabled(mreg->id);
    return ret;
}

static int mtk_regulator_set_voltage(struct regulator *mreg, int min_uv, int max_uv)
{
    int ret = 0;

    ret = mreg->reg_ops->set_voltage(mreg->id, min_uv, max_uv);
    return ret;
}

static int mtk_regulator_get_voltage(struct regulator *mreg)
{
    int ret = 0;

    ret = mreg->reg_ops->get_voltage(mreg->id);
    return ret;
}

static int mtk_regulator_set_mode(struct regulator *mreg, unsigned char mode)
{
    int ret = 0;
    ret = mreg->reg_ops->set_mode(mreg->id, mode);
    return ret;
}

static int mtk_regulator_get_mode(struct regulator *mreg)
{
    int ret = 0;
    ret = mreg->reg_ops->get_mode(mreg->id);
    return ret;
}

struct regulator *regulator_get(struct consumer *dev, const char *id)
{
    int i = 0;

    for (i = 0; i < MTK_REGULATOR_MAX_NR; i++) {
        if (mtk_regulator_table[i].name != 0) {
            if (strcmp(mtk_regulator_table[i].name, id) == 0) {
                /*memcpy(mreg, &mtk_regulator_table[i],
                    sizeof(struct regulator));*/
    		dbg_print("get %s ,id = %d success\n", id, i);
		return &mtk_regulator_table[i];
                /*return 0;*/
            }
        }
    }
    dbg_print("get %s fail\n", id);
    return 0;
}

int regulator_enable(struct regulator *regulator)
{
	int ret = 0;

	ret = mtk_regulator_enable(regulator, 1);
	if (ret)
		print("regulator enable failed\n");

	return ret;
}

int regulator_disable(struct regulator *regulator)
{
	int ret = 0;

	ret = mtk_regulator_enable(regulator, 0);
	if (ret)
		print("regulator disable failed\n");

	return ret;
}

int regulator_set_voltage(struct regulator *regulator, int min_uV, int max_uV)
{
	int ret = 0;

	ret  = mtk_regulator_set_voltage(regulator, min_uV, max_uV);
	if (ret)
		print("regulator_set_voltage failed\n");

	return ret;
}

int regulator_get_voltage(struct regulator *regulator)
{
	int ret = 0;

	ret = mtk_regulator_get_voltage(regulator);

	return ret;
}

int regulator_is_enabled(struct regulator *regulator)
{
	int ret = 0;

	ret = mtk_regulator_is_enabled(regulator);

	return ret;
}

int regulator_set_mode(struct regulator *regulator, unsigned int mode)
{
	int ret = 0;

	ret = mtk_regulator_set_mode(regulator, mode);

	return ret;
}

unsigned int regulator_get_mode(struct regulator *regulator)
{
	int ret = 0;

	ret = mtk_regulator_get_mode(regulator);

	return ret;
}

void regulator_put(struct regulator *regulator)
{
	/* Specify this regulator slot is free now */
}


#ifdef LDO_VOTRIM_SUPPORT
int mtk_regulator_set_votrim(struct regulator *mreg, int trim_uv)
{
    int ret = 0;
    ret = mreg->reg_ops->set_votrim(mreg->id, trim_uv);
    return ret;
}

int mtk_regulator_get_votrim(struct regulator *mreg)
{
    int ret = 0;
    ret = mreg->reg_ops->get_votrim(mreg->id);
    return ret;
}
#endif /*--LDO_VOTRIM_SUPPORT--*/
