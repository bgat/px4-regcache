#if !defined(__LINUX_REGIO__INCLUDED__)
#define __LINUX_REGIO__INCLUDED__

class i2c_regio : public px4::regio {
public:
	// @id: client identification string, in the form "<bus>.<addr>"
	i2c_regio(const char *id);
	~i2c_regio();
	bool is_ok() { return fd != -1 ? true : false; }
	bool read_reg(const px4::reg_t &reg, int &val);
	bool write_reg(const px4::reg_t &reg, int val) {
		// TODO: ...
		return false;
	}

private:
	i2c_regio(/* i2c clients need addresses specified */ ) {}
	int bus, addr;
	int fd;
};


// Linux userspace spidev register accessor
// TODO: provide an equivalent for NuttX
class spi_regio : public px4::regio {
public:
	// @id: client identification string, in the form "spi:<bus>.<slave>"
	spi_regio(const char *id) {
		// TODO: ...
		dev = id;
		fd = -1;
	}
	~spi_regio() {}

	// TODO: implement these
	bool read_reg(const px4::reg_t &reg, int &val) { return false; }
	bool write_reg(const px4::reg_t &reg, int val) { return false; }
private:
	spi_regio(/* spi clients need addresses specified */ ) {}
	int fd;
};

class mem_regio : public px4::regio {
public:
	// @id: client identification string, in the form "mem:<len>@<physaddr>"
	mem_regio(const char *id) {
		// TODO: ...
		dev = id;
		mem = NULL;
	}
	~mem_regio() {}

	// TODO: implement these
	bool read_reg(const px4::reg_t &reg, int &val) { return false; }
	bool write_reg(const px4::reg_t &reg, int val) { return false; }
private:
	mem_regio(/* mem clients need addresses specified */ ) {}
	void *mem;
};


#endif
