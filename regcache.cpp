#include <iostream>
#include <cstring>

#include <px4/regcache.h>

static const char *i2c_id = "i2c:";
static const char *spi_id = "spi:";
static const char *mem_id = "mem:";

px4::regcache::regcache(const char *id, px4::reg_t *map) {
	this->map = map;
	dev = NULL;
	if (strstr(id, i2c_id))
		dev = new px4::i2c_regio(id + strspn(id, i2c_id));
	else if (strstr(id, spi_id))
		dev = new px4::spi_regio(id + strspn(id, spi_id));
	else if (strstr(id, mem_id))
		dev = new px4::mem_regio(id + strspn(id, mem_id));
	else
		std::cerr << "regcache::regcache(): unknown register i/o type: " << id << std::endl;
}
