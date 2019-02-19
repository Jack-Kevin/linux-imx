/*
 * Copyright (C) 2018 Droi Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/reboot.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/reboot.h>
#include <linux/kthread.h>

/**********************************************************
  *
  *   [Global Variable]
  *
  *********************************************************/
static struct pinctrl *droi_power_rst_ctrl = NULL;
static struct pinctrl_state *droi_power_rst_ctrl_high = NULL;
static struct pinctrl_state *droi_power_rst_ctrl_low = NULL;

static int droi_power1_num = -1;
static int droi_power2_num = -1;
static int droi_power3_num = -1;
static int droi_power4_num = -1;
static int droi_power5_num = -1;
static int droi_power6_num = -1;
static int droi_power7_num = -1;
static int droi_power8_num = -1;
static int droi_rst1_num = -1;
static int droi_rst2_num = -1;
static int droi_rst3_num = -1;
static int droi_rst4_num = -1;
static int droi_rst5_num = -1;
static int droi_rst6_num = -1;
static int droi_rst7_num = -1;
static int droi_rst8_num = -1;
static DEFINE_MUTEX(droi_power_rst_mutex);
static volatile unsigned short g_droi_power_rst_flag = 0;

static ssize_t show_droi_power_rst_access(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "0x%x\n", g_droi_power_rst_flag);
}

static ssize_t store_droi_power_rst_access(struct device *dev, struct device_attribute *attr,
				    const char *buf, size_t size)
{
	int ret = 0;
	char *pvalue, temp_buf[16];
	unsigned int flag_value = 0;

	if (buf != NULL && size != 0) {
		strncpy(temp_buf, buf, sizeof(temp_buf));
		temp_buf[sizeof(temp_buf) - 1] = 0;
		pvalue = temp_buf;
		ret = kstrtouint(pvalue, 0, &flag_value);
		if (ret) {
			pr_err("hejinlong wrong format!\n");
			return size;
		}

		mutex_lock(&droi_power_rst_mutex);
		g_droi_power_rst_flag = flag_value;
		if (g_droi_power_rst_flag & (0x1 << 0)) {
			gpio_set_value(droi_power1_num, 1);
			msleep(1000);
			gpio_set_value(droi_power1_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 1)) {
			gpio_set_value(droi_power2_num, 1);
			msleep(1000);
			gpio_set_value(droi_power2_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 2)) {
			gpio_set_value(droi_power3_num, 1);
			msleep(1000);
			gpio_set_value(droi_power3_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 3)) {
			gpio_set_value(droi_power4_num, 1);
			msleep(1000);
			gpio_set_value(droi_power4_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 4)) {
			gpio_set_value(droi_power5_num, 1);
			msleep(1000);
			gpio_set_value(droi_power5_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 5)) {
			gpio_set_value(droi_power6_num, 1);
			msleep(1000);
			gpio_set_value(droi_power6_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 6)) {
			gpio_set_value(droi_power7_num, 1);
			msleep(1000);
			gpio_set_value(droi_power7_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 7)) {
			gpio_set_value(droi_power8_num, 1);
			msleep(1000);
			gpio_set_value(droi_power8_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 8)) {
			gpio_set_value(droi_rst1_num, 1);
			msleep(1000);
			gpio_set_value(droi_rst1_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 9)) {
			gpio_set_value(droi_rst2_num, 1);
			msleep(1000);
			gpio_set_value(droi_rst2_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 10)) {
			gpio_set_value(droi_rst3_num, 1);
			msleep(1000);
			gpio_set_value(droi_rst3_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 11)) {
			gpio_set_value(droi_rst4_num, 1);
			msleep(1000);
			gpio_set_value(droi_rst4_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 12)) {
			gpio_set_value(droi_rst5_num, 1);
			msleep(1000);
			gpio_set_value(droi_rst5_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 13)) {
			gpio_set_value(droi_rst6_num, 1);
			msleep(1000);
			gpio_set_value(droi_rst6_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 14)) {
			gpio_set_value(droi_rst7_num, 1);
			msleep(1000);
			gpio_set_value(droi_rst7_num, 0);
		}
		if (g_droi_power_rst_flag & (0x1 << 15)) {
			gpio_set_value(droi_rst8_num, 1);
			msleep(1000);
			gpio_set_value(droi_rst8_num, 0);
		}
		mutex_unlock(&droi_power_rst_mutex);
	}
	return size;
}

static DEVICE_ATTR(droi_power_rst_access, S_IWUSR | S_IRUGO, show_droi_power_rst_access, store_droi_power_rst_access);

static int droi_power_rst_user_space_probe(struct platform_device *dev)
{
	int ret_device_file = 0;
	int ret = -1;

	pr_debug("%s!\n", __func__);

	droi_power_rst_ctrl = devm_pinctrl_get(&dev->dev);
	if (IS_ERR(droi_power_rst_ctrl)) {
		dev_err(&dev->dev, "Cannot find droi_power_rst_ctrl!");
	}

	droi_power_rst_ctrl_high = pinctrl_lookup_state(droi_power_rst_ctrl, "power_rst1");
	if (IS_ERR(droi_power_rst_ctrl_high)) {
		ret = PTR_ERR(droi_power_rst_ctrl_high);
		printk("%s : pinctrl err, droi_power_rst_ctrl_high ret=%d\n", __func__, ret);
	}

	droi_power_rst_ctrl_low = pinctrl_lookup_state(droi_power_rst_ctrl, "power_rst0");
	if (IS_ERR(droi_power_rst_ctrl_low)) {
		ret = PTR_ERR(droi_power_rst_ctrl_low);
		printk("%s : pinctrl err, droi_power_rst_ctrl_low ret=%d\n", __func__, ret);
	} else {
		pinctrl_select_state(droi_power_rst_ctrl, droi_power_rst_ctrl_low);
	}

	ret_device_file = device_create_file(&(dev->dev), &dev_attr_droi_power_rst_access);

	droi_power1_num = of_get_named_gpio(dev->dev.of_node, "cpu1-power-gpios", 0);
	if (!gpio_is_valid(droi_power1_num)) {
		dev_err(dev, "no droi_power1_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_power1_num, GPIOF_OUT_INIT_LOW,
					"droi_power1_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_power2_num = of_get_named_gpio(dev->dev.of_node, "cpu2-power-gpios", 0);
	if (!gpio_is_valid(droi_power2_num)) {
		dev_err(dev, "no droi_power2_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_power2_num, GPIOF_OUT_INIT_LOW,
					"droi_power2_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_power3_num = of_get_named_gpio(dev->dev.of_node, "cpu3-power-gpios", 0);
	if (!gpio_is_valid(droi_power3_num)) {
		dev_err(dev, "no droi_power3_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_power3_num, GPIOF_OUT_INIT_LOW,
					"droi_power3_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_power4_num = of_get_named_gpio(dev->dev.of_node, "cpu4-power-gpios", 0);
	if (!gpio_is_valid(droi_power4_num)) {
		dev_err(dev, "no droi_power4_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_power4_num, GPIOF_OUT_INIT_LOW,
					"droi_power4_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_power5_num = of_get_named_gpio(dev->dev.of_node, "cpu5-power-gpios", 0);
	if (!gpio_is_valid(droi_power5_num)) {
		dev_err(dev, "no droi_power5_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_power5_num, GPIOF_OUT_INIT_LOW,
					"droi_power5_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_power6_num = of_get_named_gpio(dev->dev.of_node, "cpu6-power-gpios", 0);
	if (!gpio_is_valid(droi_power6_num)) {
		dev_err(dev, "no droi_power6_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_power6_num, GPIOF_OUT_INIT_LOW,
					"droi_power6_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_power7_num = of_get_named_gpio(dev->dev.of_node, "cpu7-power-gpios", 0);
	if (!gpio_is_valid(droi_power7_num)) {
		dev_err(dev, "no droi_power7_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_power7_num, GPIOF_OUT_INIT_LOW,
					"droi_power7_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_power8_num = of_get_named_gpio(dev->dev.of_node, "cpu8-power-gpios", 0);
	if (!gpio_is_valid(droi_power8_num)) {
		dev_err(dev, "no droi_power8_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_power8_num, GPIOF_OUT_INIT_LOW,
					"droi_power8_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_rst1_num = of_get_named_gpio(dev->dev.of_node, "cpu1-rst-gpios", 0);
	if (!gpio_is_valid(droi_rst1_num)) {
		dev_err(dev, "no droi_rst1_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_rst1_num, GPIOF_OUT_INIT_LOW,
					"droi_rst1_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_rst2_num = of_get_named_gpio(dev->dev.of_node, "cpu2-rst-gpios", 0);
	if (!gpio_is_valid(droi_rst2_num)) {
		dev_err(dev, "no droi_rst2_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_rst2_num, GPIOF_OUT_INIT_LOW,
					"droi_rst2_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_rst3_num = of_get_named_gpio(dev->dev.of_node, "cpu3-rst-gpios", 0);
	if (!gpio_is_valid(droi_rst3_num)) {
		dev_err(dev, "no droi_rst3_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_rst3_num, GPIOF_OUT_INIT_LOW,
					"droi_rst3_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_rst4_num = of_get_named_gpio(dev->dev.of_node, "cpu4-rst-gpios", 0);
	if (!gpio_is_valid(droi_rst4_num)) {
		dev_err(dev, "no droi_rst4_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_rst4_num, GPIOF_OUT_INIT_LOW,
					"droi_rst4_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_rst5_num = of_get_named_gpio(dev->dev.of_node, "cpu5-rst-gpios", 0);
	if (!gpio_is_valid(droi_rst5_num)) {
		dev_err(dev, "no droi_rst5_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_rst5_num, GPIOF_OUT_INIT_LOW,
					"droi_rst5_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_rst6_num = of_get_named_gpio(dev->dev.of_node, "cpu6-rst-gpios", 0);
	if (!gpio_is_valid(droi_rst6_num)) {
		dev_err(dev, "no droi_rst6_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_rst6_num, GPIOF_OUT_INIT_LOW,
					"droi_rst6_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_rst7_num = of_get_named_gpio(dev->dev.of_node, "cpu7-rst-gpios", 0);
	if (!gpio_is_valid(droi_rst7_num)) {
		dev_err(dev, "no droi_rst7_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_rst7_num, GPIOF_OUT_INIT_LOW,
					"droi_rst7_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	droi_rst8_num = of_get_named_gpio(dev->dev.of_node, "cpu8-rst-gpios", 0);
	if (!gpio_is_valid(droi_rst8_num)) {
		dev_err(dev, "no droi_rst8_num pin available\n");
		return -ENODEV;
	}
	ret = devm_gpio_request_one(&(dev->dev), droi_rst8_num, GPIOF_OUT_INIT_LOW,
					"droi_rst8_num");
	if (ret < 0)
		dev_err(dev, "hejinlong(%s,%d) devm_gpio_request_one err!\n", __func__, __LINE__);

	return 0;
}

static int droi_power_rst_user_space_remove(struct platform_device *dev)
{
	pr_debug("%s!\n", __func__);
	device_remove_file(&(dev->dev), &dev_attr_droi_power_rst_access);

	return 0;
}

#if 0
struct platform_device droi_power_rst_user_space_device = {
	.name = "droi_power_rst-user",
	.id = -1,
};
#endif

struct of_device_id droi_power_rst_of_match[] = {
	{ .compatible = "droi,power_rst_gpio" },
	{},
};
MODULE_DEVICE_TABLE(of, droi_power_rst_of_match);

static struct platform_driver droi_power_rst_user_space_driver = {
	.probe = droi_power_rst_user_space_probe,
	.remove = droi_power_rst_user_space_remove,
	.driver = {
		   .name = "droi_power_rst_gpio-user",
		   .of_match_table = droi_power_rst_of_match,
		   .owner = THIS_MODULE,
	},
};

static int __init droi_power_rst_init(void)
{
	int ret = 0;

	/* droi_power_rst user space access interface */
#if 0
	ret = platform_device_register(&droi_power_rst_user_space_device);
	if (ret) {
		pr_err("[%s] Unable to device register(%d)\n", __func__, ret);
		return ret;
	}
#endif
	ret = platform_driver_register(&droi_power_rst_user_space_driver);
	if (ret) {
		pr_err("[%s] Unable to register driver (%d)\n", __func__, ret);
		return ret;
	}

	return 0;
}

static void __exit droi_power_rst_exit(void)
{
}

module_init(droi_power_rst_init);
module_exit(droi_power_rst_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Droi power rst Driver");
MODULE_AUTHOR("hejinlong<hejinlong@droi.com>");
