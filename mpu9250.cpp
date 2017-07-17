#include <iostream>
#include <px4/device_driver.h>

// a.k.a. "mpu9250 device driver"
class mpu9250 : public px4::device {
public:
	mpu9250(std::string &path) : device(path) {
		std::cerr << "mpu9250 ctor" << std::endl;
		io = new px4::regcache(path, NULL); // TODO: regmap
		if (io->is_ok() == true) probe();
	}
	
	int probe() { return 0; } // TODO:
	~mpu9250() { delete io; }
	// ...
	static device *clone(std::string &path) { return new mpu9250(path); }
};
static int unused2 = register_driver("mpu9250", mpu9250::clone);
