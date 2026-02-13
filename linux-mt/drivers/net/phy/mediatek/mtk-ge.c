// SPDX-License-Identifier: GPL-2.0+
#include <linux/of.h>
#include <linux/bitfield.h>
#include <linux/module.h>
#include <linux/phy.h>

#include "mtk.h"

#define MTK_GPHY_ID_MT7530		0x03a29412
#define MTK_GPHY_ID_MT7531		0x03a29441

#define MTK_PHY_PAGE_EXTENDED_1			0x0001
#define MTK_PHY_AUX_CTRL_AND_STATUS		0x14
#define   MTK_PHY_ENABLE_DOWNSHIFT		BIT(4)

#define MTK_PHY_PAGE_EXTENDED_2			0x0002
#define MTK_PHY_PAGE_EXTENDED_3			0x0003
#define MTK_PHY_RG_LPI_PCS_DSP_CTRL_REG11	0x11

#define MTK_PHY_PAGE_EXTENDED_2A30		0x2a30

/* Registers on Token Ring debug nodes */
/* ch_addr = 0x1, node_addr = 0xf, data_addr = 0x17 */
#define SLAVE_DSP_READY_TIME_MASK		GENMASK(22, 15)

/* ch_addr = 0x1, node_addr = 0xf, data_addr = 0x18 */
/* EnabRandUpdTrig */
#define ENABLE_RANDOM_UPDOWN_COUNTER_TRIGGER	BIT(8)

/* Registers on MDIO_MMD_VEND1 */
#define MTK_PHY_GBE_MODE_TX_DELAY_SEL		0x13
#define MTK_PHY_TEST_MODE_TX_DELAY_SEL		0x14
#define   MTK_TX_DELAY_PAIR_B_MASK		GENMASK(10, 8)
#define   MTK_TX_DELAY_PAIR_D_MASK		GENMASK(2, 0)

#define MTK_PHY_MCC_CTRL_AND_TX_POWER_CTRL	0xa6
#define   MTK_MCC_NEARECHO_OFFSET_MASK		GENMASK(15, 8)

#define MTK_PHY_RXADC_CTRL_RG7			0xc6
#define   MTK_PHY_DA_AD_BUF_BIAS_LP_MASK	GENMASK(9, 8)

#define MTK_PHY_RG_LPI_PCS_DSP_CTRL_REG123	0x123
#define   MTK_PHY_LPI_NORM_MSE_LO_THRESH100_MASK	GENMASK(15, 8)
#define   MTK_PHY_LPI_NORM_MSE_HI_THRESH100_MASK	GENMASK(7, 0)

#define MTK_PHY_RG_DEV1E_REG189			0x189

#define MTK_PHY_RG_DEV1E_REG234			0x234
#define   MTK_PHY_TR_OPEN_LOOP_EN_MASK		GENMASK(0, 0)

#define MTK_PHY_RG_DEV1E_REG2D1			0x2d1

struct mtk_gephy_priv {
	unsigned long		led_state;
};

static void mtk_gephy_config_init(struct phy_device *phydev)
{
	/* Disable EEE */
	phy_write_mmd(phydev, MDIO_MMD_AN, MDIO_AN_EEE_ADV, 0);

	/* Enable HW auto downshift */
	phy_modify_paged(phydev, MTK_PHY_PAGE_EXTENDED_1,
			 MTK_PHY_AUX_CTRL_AND_STATUS,
			 0, MTK_PHY_ENABLE_DOWNSHIFT);

	/* Increase SlvDPSready time */
	mtk_tr_modify(phydev, 0x1, 0xf, 0x17, SLAVE_DSP_READY_TIME_MASK,
		      FIELD_PREP(SLAVE_DSP_READY_TIME_MASK, 0x5e));

	/* Adjust 100_mse_threshold */
	phy_modify_mmd(phydev, MDIO_MMD_VEND1,
		       MTK_PHY_RG_LPI_PCS_DSP_CTRL_REG123,
		       MTK_PHY_LPI_NORM_MSE_LO_THRESH100_MASK |
		       MTK_PHY_LPI_NORM_MSE_HI_THRESH100_MASK,
		       FIELD_PREP(MTK_PHY_LPI_NORM_MSE_LO_THRESH100_MASK,
				  0xff) |
		       FIELD_PREP(MTK_PHY_LPI_NORM_MSE_HI_THRESH100_MASK,
				  0xff));

	/* If echo time is narrower than 0x3, it will be regarded as noise */
	phy_modify_mmd(phydev, MDIO_MMD_VEND1,
		       MTK_PHY_MCC_CTRL_AND_TX_POWER_CTRL,
		       MTK_MCC_NEARECHO_OFFSET_MASK,
		       FIELD_PREP(MTK_MCC_NEARECHO_OFFSET_MASK, 0x3));
}

static int mt7530_phy_config_init(struct phy_device *phydev)
{
	mtk_gephy_config_init(phydev);

	/* Increase post_update_timer */
	phy_write_paged(phydev, MTK_PHY_PAGE_EXTENDED_3,
			MTK_PHY_RG_LPI_PCS_DSP_CTRL_REG11, 0x4b);

	return 0;
}

static int mt7530_led_config_of(struct phy_device *phydev)
{
	struct device_node *np = phydev->mdio.dev.of_node;
	const __be32 *paddr;
	int len;
	int i;

	paddr = of_get_property(np, "mediatek,led-config", &len);
	if (!paddr)
		return 0;

	if (len < (2 * sizeof(*paddr)))
		return -EINVAL;

	len /= sizeof(*paddr);

	phydev_warn(phydev, "Configure LED registers (num=%d)\n", len);
	for (i = 0; i < len - 1; i += 2) {
		u32 reg;
		u32 val;

		reg = be32_to_cpup(paddr + i);
		val = be32_to_cpup(paddr + i + 1);

		phy_write_mmd(phydev, MDIO_MMD_VEND2, reg, val);
	}

	return 0;
}

static void mt7531_phy_line_driving(struct phy_device *phydev)
{
	/* For ADC timing margin window for LDO calibration */
	phy_write_mmd(phydev, MDIO_MMD_VEND1, 0xd3, 0x2222);

	/* Adjust AD sample timing */
	phy_write_mmd(phydev, MDIO_MMD_VEND1, 0xc2, 0x4444);

	/* Adjust Line driver current for different mode */
	phy_write_mmd(phydev, MDIO_MMD_VEND2, 0x271, 0x2ca5);

	/* Adjust Line driver current for different mode */
	phy_write_mmd(phydev, MDIO_MMD_VEND2, 0x272, 0xc6b);

	/* Adjust Line driver gain for 10BT from 1000BT calibration result */
	phy_write_mmd(phydev, MDIO_MMD_VEND2, 0x273, 0x3000);

	/* Adjust RX Echo path filter */
	phy_write_mmd(phydev, MDIO_MMD_VEND1, 0xfe, 0x2);

	/* Adjust RX HVGA bias current */
	phy_write_mmd(phydev, MDIO_MMD_VEND1, 0x41, 0x3333);

	/* Adjust TX class AB driver 1 */
	phy_write_mmd(phydev, MDIO_MMD_VEND2, 0x268, 0x384);

	/* Adjust TX class AB driver 2 */
	phy_write_mmd(phydev, MDIO_MMD_VEND2, 0x269, 0x1114);

	/* Adjust DAC delay for TX Pairs */
	phy_write_mmd(phydev, MDIO_MMD_VEND1, 0x13, 0x404);
	phy_write_mmd(phydev, MDIO_MMD_VEND1, 0x14, 0x404);

	/* Adjust DAC digital delay for TX Delay */
	phy_write_mmd(phydev, MDIO_MMD_VEND2, 0x44, 0xc0);

	/* Adjust Line driver compensation cap for stability concern due to
	 * increase current.
	 */
	phy_write_mmd(phydev, MDIO_MMD_VEND2, 0x26a, 0x3333);
}

static void mt7531_phy_eee(struct phy_device *phydev)
{
	/* Disable EEE */
	phy_write_mmd(phydev, MDIO_MMD_AN, 0x3c, 0x0);\

	/* Disable generate signal to clear the scramble_lock when lpi mode */
	phy_clear_bits_mmd(phydev, MDIO_MMD_VEND1, MTK_PHY_RG_DEV1E_REG189, BIT(1));

	/* Roll back EEE Slave Mode */
	phy_write_mmd(phydev, MDIO_MMD_VEND1, MTK_PHY_RG_DEV1E_REG2D1, 0x0);
	mtk_tr_write(phydev, 0x2, 0xd, 0x8, 0x1b);
	mtk_tr_write(phydev, 0x2, 0xd, 0xf, 0x0);
	mtk_tr_write(phydev, 0x2, 0xd, 0x10, 0x5000);
}

static int mt7531_phy_config_init(struct phy_device *phydev)
{
	u16 val[12] = { 0x0187, 0x01c9, 0x01c6, 0x0182,
			0x0208, 0x0205, 0x0384, 0x03cb,
			0x03c4, 0x030a, 0x000b, 0x0002 };
	int i;

	/* Set default MLT3 shaper first */
	for (i = 0; i < 12; i++)
		phy_write_mmd(phydev, MDIO_MMD_VEND1, i, val[i]);

	mtk_gephy_config_init(phydev);

	/* enable random update mechanism */
	phy_select_page(phydev, MTK_PHY_PAGE_EXTENDED_52B5);
	__mtk_tr_set_bits(phydev, 0x1, 0xf, 0x18,
			  ENABLE_RANDOM_UPDOWN_COUNTER_TRIGGER);
	phy_restore_page(phydev, MTK_PHY_PAGE_STANDARD, 0);

	/* Timing Recovery for GbE slave mode */
	mtk_tr_write(phydev, 0x1, 0xf, 0x1, 0x6fb90a);
	mtk_tr_write(phydev, 0x2, 0xd, 0x6, 0x2ebaef);

	/* TR_OPEN_LOOP_EN = 1 */
	phy_modify_mmd(phydev, MDIO_MMD_VEND1, MTK_PHY_RG_DEV1E_REG234,
		       MTK_PHY_TR_OPEN_LOOP_EN_MASK,
		       BIT(0));

	/* PHY link down power saving enable */
	phy_set_bits(phydev, 0x17, BIT(4));
	phy_modify_mmd(phydev, MDIO_MMD_VEND1, MTK_PHY_RXADC_CTRL_RG7,
		       MTK_PHY_DA_AD_BUF_BIAS_LP_MASK,
		       FIELD_PREP(MTK_PHY_DA_AD_BUF_BIAS_LP_MASK, 0x3));

	/* Set TX Pair delay selection */
	phy_modify_mmd(phydev, MDIO_MMD_VEND1, MTK_PHY_GBE_MODE_TX_DELAY_SEL,
		       MTK_TX_DELAY_PAIR_B_MASK | MTK_TX_DELAY_PAIR_D_MASK,
		       FIELD_PREP(MTK_TX_DELAY_PAIR_B_MASK, 0x4) |
		       FIELD_PREP(MTK_TX_DELAY_PAIR_D_MASK, 0x4));
	phy_modify_mmd(phydev, MDIO_MMD_VEND1, MTK_PHY_TEST_MODE_TX_DELAY_SEL,
		       MTK_TX_DELAY_PAIR_B_MASK | MTK_TX_DELAY_PAIR_D_MASK,
		       FIELD_PREP(MTK_TX_DELAY_PAIR_B_MASK, 0x4) |
		       FIELD_PREP(MTK_TX_DELAY_PAIR_D_MASK, 0x4));

	/* enable asymmetric pause cap */
	phy_set_bits(phydev, 0x4, ADVERTISE_PAUSE_ASYM);

	/* LED Config*/
	mt7530_led_config_of(phydev);

	mt7531_phy_line_driving(phydev);
	mt7531_phy_eee(phydev);

	return 0;
}

static int mt7531_phy_probe(struct phy_device *phydev)
{
	struct mtk_gephy_priv *priv;

	priv = devm_kzalloc(&phydev->mdio.dev, sizeof(struct mtk_gephy_priv),
			    GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	phydev->priv = priv;

	mtk_phy_leds_state_init(phydev);

	return 0;
}

static int mt753x_phy_led_blink_set(struct phy_device *phydev, u8 index,
				    unsigned long *delay_on,
				    unsigned long *delay_off)
{
	struct mtk_gephy_priv *priv = phydev->priv;
	bool blinking = false;
	int err = 0;

	err = mtk_phy_led_num_dly_cfg(index, delay_on, delay_off, &blinking);
	if (err < 0)
		return err;

	err = mtk_phy_hw_led_blink_set(phydev, index, &priv->led_state,
				       blinking);
	if (err)
		return err;

	return mtk_phy_hw_led_on_set(phydev, index, &priv->led_state,
				     MTK_GPHY_LED_ON_MASK, false);
}

static int mt753x_phy_led_brightness_set(struct phy_device *phydev,
					 u8 index, enum led_brightness value)
{
	struct mtk_gephy_priv *priv = phydev->priv;
	int err;

	err = mtk_phy_hw_led_blink_set(phydev, index, &priv->led_state, false);
	if (err)
		return err;

	return mtk_phy_hw_led_on_set(phydev, index, &priv->led_state,
				     MTK_GPHY_LED_ON_MASK, (value != LED_OFF));
}

static const unsigned long supported_triggers =
	BIT(TRIGGER_NETDEV_FULL_DUPLEX) |
	BIT(TRIGGER_NETDEV_HALF_DUPLEX) |
	BIT(TRIGGER_NETDEV_LINK)        |
	BIT(TRIGGER_NETDEV_LINK_10)     |
	BIT(TRIGGER_NETDEV_LINK_100)    |
	BIT(TRIGGER_NETDEV_LINK_1000)   |
	BIT(TRIGGER_NETDEV_RX)          |
	BIT(TRIGGER_NETDEV_TX);

static int mt753x_phy_led_hw_is_supported(struct phy_device *phydev, u8 index,
					  unsigned long rules)
{
	return mtk_phy_led_hw_is_supported(phydev, index, rules,
					   supported_triggers);
}

static int mt753x_phy_led_hw_control_get(struct phy_device *phydev, u8 index,
					 unsigned long *rules)
{
	struct mtk_gephy_priv *priv = phydev->priv;

	return mtk_phy_led_hw_ctrl_get(phydev, index, rules, &priv->led_state,
				       MTK_GPHY_LED_ON_SET,
				       MTK_GPHY_LED_RX_BLINK_SET,
				       MTK_GPHY_LED_TX_BLINK_SET);
};

static int mt753x_phy_led_hw_control_set(struct phy_device *phydev, u8 index,
					 unsigned long rules)
{
	struct mtk_gephy_priv *priv = phydev->priv;

	return mtk_phy_led_hw_ctrl_set(phydev, index, rules, &priv->led_state,
				       MTK_GPHY_LED_ON_SET,
				       MTK_GPHY_LED_RX_BLINK_SET,
				       MTK_GPHY_LED_TX_BLINK_SET);
};

static struct phy_driver mtk_gephy_driver[] = {
	{
		PHY_ID_MATCH_EXACT(MTK_GPHY_ID_MT7530),
		.name		= "MediaTek MT7530 PHY",
		.config_init	= mt7530_phy_config_init,
		/* Interrupts are handled by the switch, not the PHY
		 * itself.
		 */
		.config_intr	= genphy_no_config_intr,
		.handle_interrupt = genphy_handle_interrupt_no_ack,
		.suspend	= genphy_suspend,
		.resume		= genphy_resume,
		.read_page	= mtk_phy_read_page,
		.write_page	= mtk_phy_write_page,
	},
	{
		PHY_ID_MATCH_EXACT(MTK_GPHY_ID_MT7531),
		.name		= "MediaTek MT7531 PHY",
		.probe		= mt7531_phy_probe,
		.config_init	= mt7531_phy_config_init,
		/* Interrupts are handled by the switch, not the PHY
		 * itself.
		 */
		.config_intr	= genphy_no_config_intr,
		.handle_interrupt = genphy_handle_interrupt_no_ack,
		.suspend	= genphy_suspend,
		.resume		= genphy_resume,
		.read_page	= mtk_phy_read_page,
		.write_page	= mtk_phy_write_page,
		.led_blink_set	= mt753x_phy_led_blink_set,
		.led_brightness_set = mt753x_phy_led_brightness_set,
		.led_hw_is_supported = mt753x_phy_led_hw_is_supported,
		.led_hw_control_set = mt753x_phy_led_hw_control_set,
		.led_hw_control_get = mt753x_phy_led_hw_control_get,
	},
};

module_phy_driver(mtk_gephy_driver);

static struct mdio_device_id __maybe_unused mtk_gephy_tbl[] = {
	{ PHY_ID_MATCH_EXACT(MTK_GPHY_ID_MT7530) },
	{ PHY_ID_MATCH_EXACT(MTK_GPHY_ID_MT7531) },
	{ }
};

MODULE_DESCRIPTION("MediaTek Gigabit Ethernet PHY driver");
MODULE_AUTHOR("DENG, Qingfang <dqfext@gmail.com>");
MODULE_LICENSE("GPL");

MODULE_DEVICE_TABLE(mdio, mtk_gephy_tbl);
