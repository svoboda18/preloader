#include <typedefs.h>
#include <pmic.h>
#include <regulator/mtk_regulator_core.h>
#include <regulator/mtk_regulator.h>

static struct mtk_regulator mtk_regulator_table[MTK_REGULATOR_MAX_NR];

int mtk_regulator_get(const char *id, struct mtk_regulator *mreg)
{
    int i = 0;

    for (i = 0; i < MTK_REGULATOR_MAX_NR; i++) {
        if (mtk_regulator_table[i].name != 0) {
            if (strcmp(mtk_regulator_table[i].name, id) == 0) {
                memcpy(mreg, &mtk_regulator_table[i],
                    sizeof(struct mtk_regulator));
                return 0;
            }
        }
    }
    pal_log_err("get %s fail\n", id);
    return -1;
}

int mtk_simple_regulator_register(struct mtk_regulator *mreg)
{
    if (mreg == 0)
        return -1;
    memcpy(&mtk_regulator_table[mreg->id], mreg,
                sizeof(struct mtk_regulator));
    pal_log_info("register %s OK\n", mreg->name);
    return 0;
}

int mtk_regulator_enable(struct mtk_regulator *mreg, unsigned char enable)
{
    int ret = 0;

    ret = mreg->reg_ops->enable(mreg->id, enable);
    return ret;
}

int mtk_regulator_is_enabled(struct mtk_regulator *mreg)
{
    int ret = 0;

    ret = mreg->reg_ops->is_enabled(mreg->id);
    return ret;
}

int mtk_regulator_set_voltage(struct mtk_regulator *mreg, int min_uv, int max_uv)
{
    int ret = 0;

    ret = mreg->reg_ops->set_voltage(mreg->id, min_uv, max_uv);
    return ret;
}

int mtk_regulator_get_voltage(struct mtk_regulator *mreg)
{
    int ret = 0;

    ret = mreg->reg_ops->get_voltage(mreg->id);
    return ret;
}

int mtk_regulator_set_mode(struct mtk_regulator *mreg, unsigned char mode)
{
    int ret = 0;
    ret = mreg->reg_ops->set_mode(mreg->id, mode);
    return ret;
}

int mtk_regulator_get_mode(struct mtk_regulator *mreg)
{
    int ret = 0;
    ret = mreg->reg_ops->get_mode(mreg->id);
    return ret;
}

#ifdef LDO_VOTRIM_SUPPORT
int mtk_regulator_set_votrim(struct mtk_regulator *mreg, int trim_uv)
{
    int ret = 0;
    ret = mreg->reg_ops->set_votrim(mreg->id, trim_uv);
    return ret;
}

int mtk_regulator_get_votrim(struct mtk_regulator *mreg)
{
    int ret = 0;
    ret = mreg->reg_ops->get_votrim(mreg->id);
    return ret;
}
#endif /*--LDO_VOTRIM_SUPPORT--*/
