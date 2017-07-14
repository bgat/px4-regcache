#if !defined (__REGCACHE_H_INCLUDED__)
#define __REGCACHE_H_INCLUDED__

#include <cstring>

namespace px4 {

	// TODO: can we optimize for 8-bit vs. 32-bit registers?
	typedef struct {int offset; int ioperm; int reserved; } reg_t;
	enum ioperm {READ = (1 << 0), WRITE = (1 << 1)};

	class regio {
	public:
		regio() {dev = NULL;}
		virtual ~regio() {}

		virtual bool is_ok() { return true; }

		inline bool is_readable(const reg_t &reg) const { return reg.ioperm & ioperm::READ ? true : false; }
		inline bool is_writable(const reg_t &reg) const { return reg.ioperm & ioperm::WRITE ? true : false; }
		inline int mask_reserved(const reg_t &reg, int &val) { return val & ~reg.reserved; }

		// TODO: how to handle errno?
		bool read(const reg_t &reg, int &val) {
			if (false == is_readable(reg))
				return false;
			if (false == read_reg(reg, val))
				return false;
			val = mask_reserved(reg, val);
			return true;
		}
		bool write(const reg_t &reg, int val) {
			if (false == is_writable(reg))
				return false;
			if (false == write_reg(reg, mask_reserved(reg, val)))
				return false;
			return true;
		}

		// TODO: buffering register values for save/restore, read-from-write-only, etc.
	protected:
		// these get overloaded to support specific bus and OS types
		virtual bool read_reg(const reg_t &reg, int &val) { return false; }
		virtual bool write_reg(const reg_t &reg, int val) { return false; }
		const void *dev;
	};

	class regcache {
	public:
		regcache(const char *id, reg_t *map);
		~regcache() { delete dev; }

		inline bool read(const reg_t &reg, int &val) { return dev->read(reg, val); }
		inline bool read(int idx, int &val) { return read(map[idx], val); }
		inline bool write(const reg_t &reg, int val) { return dev->write(reg, val); }
		inline bool write(int idx, int val) { return write(map[idx], val); }

		bool is_ok() { return dev->is_ok(); }

		// TODO: operator[]?

		// helper to crack apart <bus>.<addr> strings
		static bool id2busaddr(const char *id, int &bus, int &addr) {
			char *next;
			bus = strtol(id, &next, 0);
			addr = strtol(++next, NULL, 0);
			return true;
		}

	private:
		regcache(/* a register cache with no registers makes no sense */) {};

		const reg_t *map;
		regio *dev;
	};

#if defined(__linux__)
#include <px4/linux-regio.h>
#else
#error "I don't know how to do regcache i/o for this target"
#endif
}

#endif
