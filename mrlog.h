#ifndef MRLOG_H_
#define MRLOG_H_
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
namespace mrlog
{
	#if defined(__unix__) || defined(__APPLE__)
		#define _BEGIN_INFO_ "\033[1;32m"
		#define _BEGIN_ERROR_ "\033[1;31m"
		#define _BEGIN_DEBUG_ "\033[1;33m"
		#define _END_COLOR_ "\033[0m"

		#define MRERROR _BEGIN_ERROR_ "[ERROR] "
		#define MRDEBUG _BEGIN_DEBUG_ "[DEBUG] "
		#define MRINFO _BEGIN_INFO_ "[INFO ] "
		#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1)    \
		: __FILE__)
	#else
		#define _BEGIN_INFO_ "LOG_COLOR_G"
		#define _BEGIN_ERROR_ "LOG_COLOR_R"
		#define _BEGIN_DEBUG_ "LOG_COLOR_Y"
		#define _END_COLOR_ "LOG_COLOR_W"

		#define MRERROR _BEGIN_ERROR_ "[ERROR] "
		#define MRDEBUG _BEGIN_DEBUG_ "[DEBUG] "
		#define MRINFO _BEGIN_INFO_ "[INFO ] "
		#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1)  \
			: __FILE__)
	#endif // __unix__, __APPLE__

	class MRLog {
	public:
		template<class T>
		MRLog(const T &options) {
			my_cout_ << options;
		}
		~MRLog(){};
		template<class T>
		inline MRLog &operator << (const T &x) {
			return *this;
		}
	private:
		std::ostringstream my_cout_;
	};
}
#endif//MRLOG_H_
