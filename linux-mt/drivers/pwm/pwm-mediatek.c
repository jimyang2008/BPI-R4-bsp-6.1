// SPDX-License-Identifier: GPL-2.0
/*
 * MediaTek Pulse Width Modulator driver
 *
 * Copyright (C) 2015 John Crispin <blogic@openwrt.org>
 * Copyright (C) 2017 Zhi Mao <zhi.mao@mediatek.com>
 *
 */

#include <linux/err.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/types.h>

/* PWM registers and bits definitions */
#define PWMCON			0x00
#define PWMHDUR			0x04
#define PWMLDUR			0x08
#define PWMGDUR			0x0c
#define PWMWAVENUM		0x28
#define PWMDWIDTH		0x2c
#define PWM45DWIDTH_FIXUP	0x30
#define PWMTHRES		0x30
#define PWM45THRES_FIXUP	0x34
#define PWM_CK_26M_SEL		0x210

#define PWM_BREATHING_THRES_MAX	32
#define PWM_BREATHING_CON	0xb8
#define PWM_BREATHING_THRES_0	0xbc
#define PWM_BREATHING_THRES_1	0xd0

#define PWM_CLK_DIV_MAX		7

struct pwm_mediatek_of_data {
	unsigned int num_pwms;
	bool pwm45_fixup;
	bool has_ck_26m_sel;
	const unsigned int *reg_offset;
	int hw_breathing_light;
	int hw_breathing_light_thres_num;
};

/**
 * struct pwm_mediatek_chip - struct representing PWM chip
 * @chip: linux PWM chip representation
 * @regs: base address of PWM chip
 * @clk_top: the top clock generator
 * @clk_main: the clock used by PWM core
 * @clk_pwms: the clock used by each PWM channel
 * @clk_freq: the fix clock frequency of legacy MIPS SoC
 * @soc: pointer to chip's platform data
 */
struct pwm_mediatek_chip {
	struct pwm_chip chip;
	void __iomem *regs;
	struct clk *clk_top;
	struct clk *clk_main;
	struct clk **clk_pwms;
	const struct pwm_mediatek_of_data *soc;
	bool bw_mode[8];
};

static const unsigned int mtk_pwm_reg_offset_v1[] = {
	0x0010, 0x0050, 0x0090, 0x00d0, 0x0110, 0x0150, 0x0190, 0x0220
};

static const unsigned int mtk_pwm_reg_offset_v2[] = {
	0x0080, 0x00c0, 0x0100, 0x0140, 0x0180, 0x01c0, 0x0200, 0x0240
};

static const unsigned int mtk_pwm_reg_offset_v3[] = {
	0x0100, 0x0200, 0x0300, 0x0400, 0x0500, 0x600, 0x700, 0x0800
};

static inline struct pwm_mediatek_chip *
to_pwm_mediatek_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct pwm_mediatek_chip, chip);
}

static int pwm_mediatek_clk_enable(struct pwm_chip *chip,
				   struct pwm_device *pwm)
{
	struct pwm_mediatek_chip *pc = to_pwm_mediatek_chip(chip);
	int ret;

	ret = clk_prepare_enable(pc->clk_top);
	if (ret < 0)
		return ret;

	ret = clk_prepare_enable(pc->clk_main);
	if (ret < 0)
		goto disable_clk_top;

	ret = clk_prepare_enable(pc->clk_pwms[pwm->hwpwm]);
	if (ret < 0)
		goto disable_clk_main;

	return 0;

disable_clk_main:
	clk_disable_unprepare(pc->clk_main);
disable_clk_top:
	clk_disable_unprepare(pc->clk_top);

	return ret;
}

static void pwm_mediatek_clk_disable(struct pwm_chip *chip,
				     struct pwm_device *pwm)
{
	struct pwm_mediatek_chip *pc = to_pwm_mediatek_chip(chip);

	clk_disable_unprepare(pc->clk_pwms[pwm->hwpwm]);
	clk_disable_unprepare(pc->clk_main);
	clk_disable_unprepare(pc->clk_top);
}

static inline void pwm_mediatek_writel(struct pwm_mediatek_chip *chip,
				       unsigned int num, unsigned int offset,
				       u32 value)
{
	writel(value, chip->regs + chip->soc->reg_offset[num] + offset);
}

static void pwm_mediatek_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct pwm_mediatek_chip *pc = to_pwm_mediatek_chip(chip);
	u32 value;

	value = readl(pc->regs);
	value |= BIT(pwm->hwpwm);
	writel(value, pc->regs);
}

static void pwm_mediatek_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct pwm_mediatek_chip *pc = to_pwm_mediatek_chip(chip);
	u32 value;

	value = readl(pc->regs);
	value &= ~BIT(pwm->hwpwm);
	writel(value, pc->regs);
}

static inline int pwm_bl_set_thres(struct pwm_mediatek_chip *chip, int pwm,
				    int idx, int value, int scale)
{
	u32 offset;
	u32 shift;
	u32 tmp;

	value = value / scale;
	if ((idx < 0) || (idx >= PWM_BREATHING_THRES_MAX))
		return -EINVAL;
	if ((value < 0) || (value > 255))
		return -EINVAL;
	if (idx < 4) {
		offset = PWM_BREATHING_THRES_0;
		shift = (idx % 4) * 8;
	} else {
		offset = PWM_BREATHING_THRES_1 + ((idx / 4) - 1) * 4;
		shift = (idx % 4) * 8;
	}
	tmp = readl(chip->regs + mtk_pwm_reg_offset_v3[pwm] + offset);
	tmp &= ~(0xff << shift);
	tmp |= (value << shift);
	writel(tmp, chip->regs + mtk_pwm_reg_offset_v3[pwm] + offset);

	return 0;
}

static inline int pwm_config_bl_thres(struct pwm_mediatek_chip *chip, int pwm,
				      int max_counter, int num_thres)
{
	/* use x_scale to make the wavefrom display smoothly */
	const int x_scale = 1000;
	int c, x;
	int ret;
	int i;
	/*
	 *  [Breathing Light Pattern]
         *  - x         : max_counter / (num_thres / 2)
         *  - 100% duty : (num_thres / 2) * x
         *  - 0% duty   : 0 * x
         *  - per period: T(0)~T(num_thres)
	 *                (If period = 10^9 (ns), 1T = 10^9 (ns) / num_thres)
         *
	 *  | num_thres | T0 T1 T2 T3 T4 T5 T6 T7 T8 T9 10 11 12 13 14 15 ... |
	 *  |-----------|-----------------------------------------------------|
         *  | 8         | 4x 3x 2x 1x 0x 1x 2x 3x (repeat) .................. |
	 *  | 16        | 8x 7x 6x 5x 4x 3x 2x 1x 0x 1x 2x 3x 4x 5x 6x 7x ... |
	 *  | 24        | 12x ............................................... |
	 *  | 32 (max)  | 16x ............................................... |
	 */
	int pattern[PWM_BREATHING_THRES_MAX];

	if ((num_thres < 2) || (num_thres % 2))
		return -EINVAL;

	c = num_thres / 2;
	x = (max_counter * x_scale) / (num_thres / 2);

	/* create breathing ligh pattern according to previous table */
	/* use (x_scale * max_counter) instead of (x * num_thres) */
	pattern[0] = max_counter * x_scale;
	/* caculate each pattern according to (i) */
	for (i = 1 ; i < c ; i++)
		pattern[i] = x * (c - i);
	for (i = c ; i < num_thres ; i++)
		pattern[i] = x * (i - c);
	for (i = 0 ; i < num_thres ; i++) {
		ret = pwm_bl_set_thres(chip, pwm, i, pattern[i], x_scale);
		if (ret)
			return ret;
	}

	/* enable breathing light mode */
	writel(((num_thres - 1) << 8 | 0x1),
	       chip->regs + mtk_pwm_reg_offset_v3[pwm] + PWM_BREATHING_CON);

	return 0;
}

static int pwm_mediatek_config(struct pwm_chip *chip, struct pwm_device *pwm,
			       int duty_ns, int period_ns)
{
	struct pwm_mediatek_chip *pc = to_pwm_mediatek_chip(chip);
	/* The source clock is divided by 2^clkdiv or iff the clksel bit
	 * is set by (2^clkdiv*1625)
	 */
	u32 clkdiv = 0, clksel = 0, cnt_period, cnt_duty,
	    reg_width = PWMDWIDTH, reg_thres = PWMTHRES;
	unsigned long clk_rate;
	u64 resolution;
	u32 max_cnt_period = 8192;
	int thres_num;
	int ret;

	ret = pwm_mediatek_clk_enable(chip, pwm);
	if (ret < 0)
		return ret;

	clk_rate = clk_get_rate(pc->clk_pwms[pwm->hwpwm]);
	if (!clk_rate) {
		ret = -EINVAL;
		goto out;
	}

	/* Make sure we use the bus clock and not the 26MHz clock */
	if (pc->soc->has_ck_26m_sel)
		writel(0, pc->regs + PWM_CK_26M_SEL);

	if (pc->bw_mode[pwm->hwpwm]) {
		thres_num = pc->soc->hw_breathing_light_thres_num;
		period_ns = period_ns / thres_num;
		max_cnt_period = 255;
	}

	/* Using resolution in picosecond gets accuracy higher */
	resolution = (u64)NSEC_PER_SEC * 1000;

	/* Calculate resolution based on current clock frequency */
	do_div(resolution, clk_rate);

	/* Using resolution to calculate cnt_period which represents
	 * the effective range of the PWM period counter
	 */
	cnt_period = DIV_ROUND_CLOSEST_ULL((u64)period_ns * 1000, resolution);
	if (!cnt_period)
		return -EINVAL;


	while (cnt_period > max_cnt_period) {
		/* Using clkdiv to reduce clock frequency and calculate
		 * new resolution based on new clock speed
		 */
		resolution *= 2;
		clkdiv++;
		if (clkdiv > PWM_CLK_DIV_MAX && !clksel) {
			/* Using clksel to divide the pwm source clock by
			 * an additional 1625, and recalculate new clkdiv
			 * and resolution
			 */
			clksel = 1;
			clkdiv = 0;
			resolution = (u64)NSEC_PER_SEC * 1000 * 1625;
			do_div(resolution,
				clk_get_rate(pc->clk_pwms[pwm->hwpwm]));
		}
		/* Calculate cnt_period based on resolution */
		cnt_period = DIV_ROUND_CLOSEST_ULL((u64)period_ns * 1000,
						   resolution);
	}

	if (clkdiv > PWM_CLK_DIV_MAX) {
		dev_err(chip->dev, "period of %d ns not supported\n", period_ns);
		ret = -EINVAL;
		goto out;
	}

	if (pc->soc->pwm45_fixup && pwm->hwpwm > 2) {
		/*
		 * PWM[4,5] has distinct offset for PWMDWIDTH and PWMTHRES
		 * from the other PWMs on MT7623.
		 */
		reg_width = PWM45DWIDTH_FIXUP;
		reg_thres = PWM45THRES_FIXUP;
	}

	/* Calculate cnt_duty based on resolution */
	cnt_duty = DIV_ROUND_CLOSEST_ULL((u64)duty_ns * 1000, resolution);

	if (clksel)
		pwm_mediatek_writel(pc, pwm->hwpwm, PWMCON, BIT(15) | BIT(3) |
				    clkdiv);
	else
		pwm_mediatek_writel(pc, pwm->hwpwm, PWMCON, BIT(15) | clkdiv);

	pwm_mediatek_writel(pc, pwm->hwpwm, reg_width, cnt_period - 1);


	/* use array of bw_thres instead of normal thres in bw_mode */
	if (pc->bw_mode[pwm->hwpwm])
		pwm_config_bl_thres(pc, pwm->hwpwm, cnt_period, thres_num);
	else {
		if (cnt_duty) {
		        pwm_mediatek_writel(pc, pwm->hwpwm, reg_thres, cnt_duty - 1);
		        pwm_mediatek_enable(chip, pwm);
		} else {
		        pwm_mediatek_disable(chip, pwm);
		}
	}

out:
	pwm_mediatek_clk_disable(chip, pwm);

	return ret;
}

static int pwm_mediatek_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			      const struct pwm_state *state)
{
	int err;

	if (state->polarity != PWM_POLARITY_NORMAL)
		return -EINVAL;

	if (!state->enabled) {
		if (pwm->state.enabled) {
			pwm_mediatek_disable(chip, pwm);
			pwm_mediatek_clk_disable(chip, pwm);
		}

		return 0;
	}

	err = pwm_mediatek_config(pwm->chip, pwm, state->duty_cycle, state->period);
	if (err)
		return err;

	if (!pwm->state.enabled)
		err = pwm_mediatek_clk_enable(chip, pwm);

	return err;
}

static const struct pwm_ops pwm_mediatek_ops = {
	.apply = pwm_mediatek_apply,
	.owner = THIS_MODULE,
};

static int pwm_mediatek_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct pwm_mediatek_chip *pc;
	unsigned int i;
	int ret;

	pc = devm_kzalloc(&pdev->dev, sizeof(*pc), GFP_KERNEL);
	if (!pc)
		return -ENOMEM;

	pc->soc = of_device_get_match_data(&pdev->dev);

	pc->regs = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(pc->regs))
		return PTR_ERR(pc->regs);

	pc->clk_pwms = devm_kmalloc_array(&pdev->dev, pc->soc->num_pwms,
				    sizeof(*pc->clk_pwms), GFP_KERNEL);
	if (!pc->clk_pwms)
		return -ENOMEM;

	pc->clk_top = devm_clk_get(&pdev->dev, "top");
	if (IS_ERR(pc->clk_top))
		return dev_err_probe(&pdev->dev, PTR_ERR(pc->clk_top),
				     "Failed to get top clock\n");

	pc->clk_main = devm_clk_get(&pdev->dev, "main");
	if (IS_ERR(pc->clk_main))
		return dev_err_probe(&pdev->dev, PTR_ERR(pc->clk_main),
				     "Failed to get main clock\n");

	for (i = 0; i < pc->soc->num_pwms; i++) {
		char name[8];
		char bw_name[32];

		snprintf(name, sizeof(name), "pwm%d", i + 1);

		pc->clk_pwms[i] = devm_clk_get(&pdev->dev, name);
		if (IS_ERR(pc->clk_pwms[i]))
			return dev_err_probe(&pdev->dev, PTR_ERR(pc->clk_pwms[i]),
					     "Failed to get %s clock\n", name);

		if (pc->soc->hw_breathing_light) {
			snprintf(bw_name, sizeof(bw_name),
				 "mediatek,pwm%d-breathing-light", i);
			pc->bw_mode[i] = of_property_read_bool(np, bw_name);
		}
	}

	pc->chip.dev = &pdev->dev;
	pc->chip.ops = &pwm_mediatek_ops;
	pc->chip.npwm = pc->soc->num_pwms;

	ret = devm_pwmchip_add(&pdev->dev, &pc->chip);
	if (ret < 0)
		return dev_err_probe(&pdev->dev, ret, "pwmchip_add() failed\n");

	return 0;
}

static const struct pwm_mediatek_of_data mt2712_pwm_data = {
	.num_pwms = 8,
	.pwm45_fixup = false,
	.has_ck_26m_sel = false,
	.reg_offset = mtk_pwm_reg_offset_v1,
};

static const struct pwm_mediatek_of_data mt6795_pwm_data = {
	.num_pwms = 7,
	.pwm45_fixup = false,
	.has_ck_26m_sel = false,
	.reg_offset = mtk_pwm_reg_offset_v1,
};

static const struct pwm_mediatek_of_data mt7622_pwm_data = {
	.num_pwms = 6,
	.pwm45_fixup = false,
	.has_ck_26m_sel = true,
	.reg_offset = mtk_pwm_reg_offset_v1,
};

static const struct pwm_mediatek_of_data mt7623_pwm_data = {
	.num_pwms = 5,
	.pwm45_fixup = true,
	.has_ck_26m_sel = false,
	.reg_offset = mtk_pwm_reg_offset_v1,
};

static const struct pwm_mediatek_of_data mt7628_pwm_data = {
	.num_pwms = 4,
	.pwm45_fixup = true,
	.has_ck_26m_sel = false,
	.reg_offset = mtk_pwm_reg_offset_v1,
};

static const struct pwm_mediatek_of_data mt7629_pwm_data = {
	.num_pwms = 1,
	.pwm45_fixup = false,
	.has_ck_26m_sel = false,
	.reg_offset = mtk_pwm_reg_offset_v1,
};

static const struct pwm_mediatek_of_data mt7981_pwm_data = {
	.num_pwms = 3,
	.pwm45_fixup = false,
	.has_ck_26m_sel = true,
	.reg_offset = mtk_pwm_reg_offset_v2,
};

static const struct pwm_mediatek_of_data mt7986_pwm_data = {
	.num_pwms = 2,
	.pwm45_fixup = false,
	.has_ck_26m_sel = true,
	.reg_offset = mtk_pwm_reg_offset_v1,
};

static const struct pwm_mediatek_of_data mt7987_pwm_data = {
	.num_pwms = 3,
	.pwm45_fixup = false,
	.has_ck_26m_sel = false,
	.reg_offset = mtk_pwm_reg_offset_v3,
	.hw_breathing_light = 1,
	.hw_breathing_light_thres_num = 32,
};

static const struct pwm_mediatek_of_data mt7988_pwm_data = {
	.num_pwms = 8,
	.pwm45_fixup = false,
	.has_ck_26m_sel = false,
	.reg_offset = mtk_pwm_reg_offset_v2,
};

static const struct pwm_mediatek_of_data mt8183_pwm_data = {
	.num_pwms = 4,
	.pwm45_fixup = false,
	.has_ck_26m_sel = true,
	.reg_offset = mtk_pwm_reg_offset_v1,
};

static const struct pwm_mediatek_of_data mt8365_pwm_data = {
	.num_pwms = 3,
	.pwm45_fixup = false,
	.has_ck_26m_sel = true,
	.reg_offset = mtk_pwm_reg_offset_v1,
};

static const struct pwm_mediatek_of_data mt8516_pwm_data = {
	.num_pwms = 5,
	.pwm45_fixup = false,
	.has_ck_26m_sel = true,
	.reg_offset = mtk_pwm_reg_offset_v1,
};

static const struct of_device_id pwm_mediatek_of_match[] = {
	{ .compatible = "mediatek,mt2712-pwm", .data = &mt2712_pwm_data },
	{ .compatible = "mediatek,mt6795-pwm", .data = &mt6795_pwm_data },
	{ .compatible = "mediatek,mt7622-pwm", .data = &mt7622_pwm_data },
	{ .compatible = "mediatek,mt7623-pwm", .data = &mt7623_pwm_data },
	{ .compatible = "mediatek,mt7628-pwm", .data = &mt7628_pwm_data },
	{ .compatible = "mediatek,mt7629-pwm", .data = &mt7629_pwm_data },
	{ .compatible = "mediatek,mt7981-pwm", .data = &mt7981_pwm_data },
	{ .compatible = "mediatek,mt7986-pwm", .data = &mt7986_pwm_data },
	{ .compatible = "mediatek,mt7987-pwm", .data = &mt7987_pwm_data },
	{ .compatible = "mediatek,mt7988-pwm", .data = &mt7988_pwm_data },
	{ .compatible = "mediatek,mt8183-pwm", .data = &mt8183_pwm_data },
	{ .compatible = "mediatek,mt8365-pwm", .data = &mt8365_pwm_data },
	{ .compatible = "mediatek,mt8516-pwm", .data = &mt8516_pwm_data },
	{ },
};
MODULE_DEVICE_TABLE(of, pwm_mediatek_of_match);

static struct platform_driver pwm_mediatek_driver = {
	.driver = {
		.name = "pwm-mediatek",
		.of_match_table = pwm_mediatek_of_match,
	},
	.probe = pwm_mediatek_probe,
};
module_platform_driver(pwm_mediatek_driver);

MODULE_AUTHOR("John Crispin <blogic@openwrt.org>");
MODULE_LICENSE("GPL v2");
