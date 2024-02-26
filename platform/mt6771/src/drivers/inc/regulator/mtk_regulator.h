#ifndef __SSPM_MTK_REGULATOR_H
#define __SSPM_MTK_REGULATOR_H

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#undef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

struct mtk_regulator {
    const char *name;
    unsigned char id;
    struct regulator_ctrl *reg_ops;
};

enum {
    REGULATOR_MODE_FAST,
    REGULATOR_MODE_NORMAL,
};

extern int mtk_regulator_get(const char *id, struct mtk_regulator *mreg);
extern int mtk_regulator_enable(struct mtk_regulator *mreg, unsigned char enable);
extern int mtk_regulator_is_enabled(struct mtk_regulator *mreg);
extern int mtk_regulator_set_voltage(struct mtk_regulator *mreg, int min_uv, int max_uv);
extern int mtk_regulator_get_voltage(struct mtk_regulator *mreg);
extern int mtk_regulator_set_mode(struct mtk_regulator *mreg, unsigned char mode);
extern int mtk_regulator_get_mode(struct mtk_regulator *mreg);
#ifdef LDO_VOTRIM_SUPPORT
extern int mtk_regulator_set_votrim(struct mtk_regulator *mreg, int trim_uv);
extern int mtk_regulator_get_votrim(struct mtk_regulator *mreg);
#endif

#endif /* __SSPM_MTK_REGULATOR_H */
