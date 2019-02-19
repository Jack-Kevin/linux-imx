/*
 * I2C slave mode nxpinfo
 */

#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/sysfs.h>

struct nxpinfo_data {
	bool first_write;
	spinlock_t buffer_lock;
	u8 buffer_idx;
	u8 buffer[];
};

static unsigned int buffer_size = 0;
static int i2c_slave_nxpinfo_slave_cb(struct i2c_client *client,
				     enum i2c_slave_event event, u8 *val)
{
	struct nxpinfo_data *nxpinfo = i2c_get_clientdata(client);

	switch (event) {
	case I2C_SLAVE_WRITE_RECEIVED:
		if (nxpinfo->first_write) {
			if (*val >= buffer_size || *val < 0)
				return -1;
			nxpinfo->buffer_idx = *val; // set idx first write byte
			nxpinfo->first_write = false;
		} else {
			if (nxpinfo->buffer_idx >= buffer_size)
				return -1;
			spin_lock(&nxpinfo->buffer_lock);
			nxpinfo->buffer[nxpinfo->buffer_idx++] = *val;
			spin_unlock(&nxpinfo->buffer_lock);
		}
		break;

	case I2C_SLAVE_READ_PROCESSED:
		nxpinfo->buffer_idx++; // read next;
	case I2C_SLAVE_READ_REQUESTED:
		if (nxpinfo->buffer_idx >= buffer_size) {
			*val = 0; // return 0 if overflow
			return -1;
		}
		spin_lock(&nxpinfo->buffer_lock);
		*val = nxpinfo->buffer[nxpinfo->buffer_idx];
		spin_unlock(&nxpinfo->buffer_lock);
		break;

	case I2C_SLAVE_STOP:
	case I2C_SLAVE_WRITE_REQUESTED:
		nxpinfo->first_write = true;
		break;

	default:
		break;
	}

	return 0;
}

static ssize_t slave_nxpinfo_show_pin(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct nxpinfo_data *nxpinfo = dev_get_drvdata(dev);
	int len = buffer_size;
	unsigned long flags;

	if (len < 0)
		return 0;

	spin_lock_irqsave(&nxpinfo->buffer_lock, flags);
	memcpy(buf, &nxpinfo->buffer[0], len);
	spin_unlock_irqrestore(&nxpinfo->buffer_lock, flags);

	buf[len] = '\n';
	len++;

	return len;
}

static ssize_t slave_nxpinfo_store_pin(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct nxpinfo_data *nxpinfo = dev_get_drvdata(dev);
	int offset, value;
	unsigned long flags;

	if (2 == sscanf(buf, "%d %d", &offset, &value)) {
		if (offset < buffer_size && offset >= 0) {
			spin_lock_irqsave(&nxpinfo->buffer_lock, flags);
			//memcpy(&nxpinfo->buffer[off], buf, count);
			nxpinfo->buffer[offset] = value;
			spin_unlock_irqrestore(&nxpinfo->buffer_lock, flags);
		}
	}

	return count;
}

static DEVICE_ATTR(slave_nxpinfo, 0664, slave_nxpinfo_show_pin, slave_nxpinfo_store_pin);

static int i2c_slave_nxpinfo_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct nxpinfo_data *nxpinfo;
	int ret;
	unsigned size = id->driver_data;

	nxpinfo = devm_kzalloc(&client->dev, sizeof(struct nxpinfo_data) + size, GFP_KERNEL);
	if (!nxpinfo)
		return -ENOMEM;

	nxpinfo->first_write = true;
	spin_lock_init(&nxpinfo->buffer_lock);
	i2c_set_clientdata(client, nxpinfo);

	ret = device_create_file(&(client->dev), &dev_attr_slave_nxpinfo);
	if (ret)
		return ret;

	ret = i2c_slave_register(client, i2c_slave_nxpinfo_slave_cb);
	if (ret) {
		device_remove_file(&(client->dev), &dev_attr_slave_nxpinfo);
		return ret;
	}

	buffer_size = size;
	return 0;
};

static int i2c_slave_nxpinfo_remove(struct i2c_client *client)
{
	struct nxpinfo_data *nxpinfo = i2c_get_clientdata(client);

	i2c_slave_unregister(client);
	device_remove_file(&(client->dev), &dev_attr_slave_nxpinfo);

	return 0;
}

static const struct i2c_device_id i2c_slave_nxpinfo_id[] = {
	{ "droi,slave_nxpinfo", 100 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, i2c_slave_nxpinfo_id);

static struct i2c_driver i2c_slave_nxpinfo_driver = {
	.driver = {
		.name = "i2c_slave_nxpinfo",
		.owner = THIS_MODULE,
	},
	.probe = i2c_slave_nxpinfo_probe,
	.remove = i2c_slave_nxpinfo_remove,
	.id_table = i2c_slave_nxpinfo_id,
};
module_i2c_driver(i2c_slave_nxpinfo_driver);

MODULE_AUTHOR("hejinlong <hejinlong@droi.com>");
MODULE_DESCRIPTION("I2C slave mode nxpinfo");
MODULE_LICENSE("GPL v2");
