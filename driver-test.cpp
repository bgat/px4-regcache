#include <iostream>
#include <string>
#include <px4/device_driver.h>


int main(void)
{
	// these would normally be entries in a device-tree, ini file, or whatever...
	std::string mag("hmc5883@i2c:2.0x34");
	std::string imu("mpu9250@i2c:2.0x69");

	px4::device *d;

	d = px4::new_device(mag);
	if (d) delete d;
	d = px4::new_device(imu);
	if (d) delete d;

	return 0;		
}
