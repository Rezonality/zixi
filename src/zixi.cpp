#include <zest/logger/logger.h>

namespace Zest {
#undef ERROR
#ifdef _DEBUG
Logger logger = { true, LT::DBG };
#else
Logger logger = { true, LT::INFO };
#endif
bool Log::disabled = false;
}
