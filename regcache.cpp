#include <cstring>
#include <cstdlib>

#include <px4/regcache.h>

static const char *i2c_id = "i2c:";
static const char *spi_id = "spi:";
static const char *mem_id = "mem:";

// @path is:
//    "drivername@bustype:busnum.addr"
//
// i.e.:
//    "mpu9250@i2c:2.0x69"
//

void px4::regcache::bus(int &id, int &cli)
{
	char *next;
	id = strtol(addr(), &next, 0);
	cli = strtol(++next, NULL, 0);
}

const char *px4::regcache::bus() const
{
	const char *ret = strchr(path.c_str(), '@');
	if (!ret) return path.c_str();;
	return ++ret;
}

const char *px4::regcache::addr() const
{
	const char *ret = strchr(bus(), ':');
	if (!ret) return bus();
	return ++ret;
}

px4::regcache::regcache(const std::string &p, px4::reg_t *map) : path(p) {
	dev = NULL;
	this->map = map;

	if (strstr(path.c_str(), i2c_id)) {
		int id, cli;
		bus(id, cli);
		dev = new px4::i2c_regio(id, cli);
	}
	else if (strstr(path.c_str(), spi_id))
		dev = new px4::spi_regio(addr());
	else if (strstr(path.c_str(), mem_id))
		dev = new px4::mem_regio(addr());
}
