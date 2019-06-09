#ifndef MW_EXCEPTION_H
#define MW_EXCEPTION_H

#include <stdexcept>

namespace mw {

	class Exception : public std::runtime_error {
	public:
		inline Exception(const std::string& what_arg) : std::runtime_error(what_arg) {
		}
		
		inline Exception(const char* what_arg) : std::runtime_error(what_arg) {
		}
	};

} // Namespace mw.

#endif // MW_EXCEPTION_H
