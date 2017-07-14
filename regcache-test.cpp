#include <iostream>

#include <px4/regcache.h>

int main(int argc, const char *argv[], const char *env[])
{
	enum myregindex {SOMEDATA = 0, CHIP_ID = 1};
	int val;
	bool err;
	
	// TODO: how to make this initialization order-independent?
	px4::reg_t regs[] = {
		[SOMEDATA] = {42, px4::ioperm::READ | px4::ioperm::WRITE, 0},
		[CHIP_ID] = {0, px4::ioperm::READ, 6},
	};
	px4::regcache i("i2c:2.0x68", regs);
	px4::regcache s("spi:1.2", regs);

	std::cerr << "i.is_ok() returns " << i.is_ok() << std::endl;
	std::cerr << "s.is_ok() returns " << s.is_ok() << std::endl;

	err = i.read(CHIP_ID, val);
	err = i.read(regs[CHIP_ID], val);
	err = i.write(SOMEDATA, val);
	err = i.write(regs[SOMEDATA], val);
	
	err = s.read(CHIP_ID, val);
	err = s.read(regs[CHIP_ID], val);
	err = s.write(SOMEDATA, val);
	err = s.write(regs[SOMEDATA], val);
	
	return 0;
}
