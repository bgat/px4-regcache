#if !defined(PX4_DEVICE_DRIVER__INCLUDED)
#define PX4_DEVICE_DRIVER__INCLUDED

#include <string>
#include <px4/regcache.h>


namespace px4 {
	class device {
	public:
		device( /* naked device makes no sense */ );
		virtual ~device() {}
		device(std::string &path) {};
		virtual int probe(void) { return 0; }
		// ...
	protected:
		regcache *io;
	private:
	};

	typedef device *(driver)(std::string &path);
	int register_driver(const std::string &name, driver *d);
	device *new_device(std::string &path);
};

#endif
