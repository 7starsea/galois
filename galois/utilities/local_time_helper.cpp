///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#include "local_time_helper.h"
#include <cstring>  /// std::memcpy

namespace Galois{
	namespace Utilities{
		///时间格式转化
		void LocalTimeHelperImpl::to_update_time(const tm & info, char * observer_time){
			////format: "%H:%M:%S" / "HH:MM:SS"      /// char('0') == int(48)
			observer_time[0] = char(info.tm_hour/10 + 48);
			observer_time[1] = char(info.tm_hour%10 + 48);
			observer_time[2] = ':';

			observer_time[3] = char(info.tm_min/10 + 48);
			observer_time[4] = char(info.tm_min%10 + 48);
			observer_time[5] = ':';

			observer_time[6] = char(info.tm_sec/10 + 48);
			observer_time[7] = char(info.tm_sec%10 + 48);
			observer_time[8] = '\0';
		}
		
		///获取本地时间
		tm LocalTimeHelperImpl::local_time(){
			mutex_.lock();
				time_t rawtime = 0;
				::time (&rawtime);

				struct tm * timeinfo = ::localtime(&rawtime);
				tm info;
				std::memcpy(&info, timeinfo, sizeof(info));
			mutex_.unlock();

			return info;
		}

		tm LocalTimeHelperImpl::local_time(const time_t sec_since_epoch){
			mutex_.lock();
				struct tm * timeinfo = ::localtime(&sec_since_epoch);
				tm info;
				std::memcpy(&info, timeinfo, sizeof(info));
			mutex_.unlock();
			return info;
		}

		///获取本地时间
		void LocalTimeHelperImpl::update_time(char * observer_time){
			const struct tm info = LocalTimeHelperImpl::local_time();
			LocalTimeHelperImpl::to_update_time(info, observer_time);
		}
		
	}//namespace Utilities
}///namespace Galois
