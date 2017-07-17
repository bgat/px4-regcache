#include <iostream>
#include <px4/device_driver.h>

// a.k.a. "hmc5883 device driver"
class hmc5883 : public px4::device {
public:
	hmc5883(std::string &path) : device(path) {
		std::cerr << "hmc5883 ctor" << std::endl;
		io = new px4::regcache(path, NULL); // TODO: regmap
		if (io->is_ok() == true) probe();
	}
	int probe() { return 0; } // TODO:
	~hmc5883() { delete io; }
	// ...
	static device *clone(std::string &path) { return new hmc5883(path); }
private:
	hmc5883( /* a naked driver makes no sense */ );
};

static int unused = register_driver("hmc5883", hmc5883::clone);

