#include <iostream>

#include <px4/regcache.h>
#include <px4/linux-regio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>

using namespace px4;

i2c_regio::i2c_regio(int bus, int addr) {
	int ret;
	char path[256];
	snprintf(path, sizeof(path), "/dev/i2c-%d", bus);
	fd = open(path, O_RDWR);
	if (fd == -1) {
		std::cerr << "error " << fd << " opening " << path << std::endl;
		return;
	}
	ret = ioctl(fd, I2C_SLAVE, addr);
	if (ret)
		goto err_open;
	return;
err_open:
	std::cerr << "error " << ret << " in i2c_regio::i2c_regio(" << bus << ", " << addr << ")" << std::endl;
	close(fd);
	fd = -1;
	return;
}

i2c_regio::~i2c_regio() { if (fd != -1) close(fd); }

bool i2c_regio::read_reg(const px4::reg_t &reg, int &val) {
	union i2c_smbus_data data;
	struct i2c_smbus_ioctl_data args;
	int ret;

	__u8 *buf = &data.block[1];
	data.block[0] = reg.width;

	args.read_write = I2C_SMBUS_READ;
	args.command = reg.offset;
	args.data = &data;
	args.size = I2C_SMBUS_BLOCK_DATA;

	ret = ioctl(fd, I2C_SMBUS, &args);
	if (ret || !data.block[0])
		return false;

	val = *buf++;

	// note: this will merely truncate, rather than clobber memory,
	// if the register size is larger than sizeof(int)
	while (--data.block[0])
		val = (val << 8) + *buf++;

	return true;
}

bool i2c_regio::write_reg(const px4::reg_t &reg, int val) { return false; }
