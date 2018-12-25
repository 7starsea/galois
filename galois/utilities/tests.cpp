///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#include <iostream>
#include <boost/predef.h>

#include "local_time_helper.h"
#include "trading_time_helper.h"

#if BOOST_OS_WINDOWS
#include <windows.h>
#else
#include <sys/time.h>
#endif


#if BOOST_OS_UNIX || BOOST_OS_LINUX || BOOST_OS_MACOS
#include <time.h>   // Needed for struct timespec on linux and unix and macos
//nanosleep((const struct timespec[]){{0, milli * 1000000L}},  NULL);
inline void UnifiedSleep(int milli){ const struct timespec tim = { milli/1000, (milli%1000) * 1000000L };   nanosleep(&tim, NULL); }
#elif BOOST_OS_WINDOWS	////on windows
#include <Windows.h>
inline void UnifiedSleep(int milli){ Sleep(milli); }
#elif __cplusplus >= 199711L
#include <thread>         // include std::this_thread::sleep_for and std::chrono::seconds
inline void UnifiedSleep(int milli){ std::this_thread::sleep_for(std::chrono::milliseconds(milli)); }
#endif

using Galois::Utilities::LocalTimeHelper;
using Galois::Utilities::TradingTimeHelper;

void local_time_helper_test(){
	LocalTimeHelper & local_time_helper = LocalTimeHelper::create();

	char observer_time[9], observer_time2[9];
	int update_milli = 0;
	for (int i = 0; i < 10; ++i){
#if BOOST_OS_WINDOWS
		SYSTEMTIME tp;
		GetLocalTime(&tp);
		/// LONG time_ms = (tp.wSecond * 1000) + tp.wMilliseconds;
		update_milli = tp.wMilliseconds;
		//// const tm info = local_time_helper.local_time(tp.wHour * 3600 + tp.wMinute * 60 + tp.wSecond);
		const tm info ={tp.wSecond, tp.wMinute, tp.wHour, tp.wDay, tp.wMonth, tp.wYear, tp.wDayOfWeek, 0, 0};
#else
		struct timeval tp;
		gettimeofday(&tp, NULL);
		///	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		update_milli = tp.tv_usec / 1000;
		const tm info = local_time_helper.local_time(tp.tv_sec);
#endif

		LocalTimeHelper::to_update_time(info, observer_time);


		const tm info2 = local_time_helper.local_time();
		LocalTimeHelper::to_update_time(info2, observer_time2);

		std::cout << observer_time << " " << observer_time2 <<" "<< update_milli<< std::endl;
		UnifiedSleep(1000 * 2);
	}
}


void TradingTimeHelper_test(){
	TradingTimeHelper timer_helper("[09:30:00, 15:00:00]");
	char observer_time[9], observer_time2[9];
	int update_milli = 0;

	for (int i = 0; i < 10; ++i){
		timer_helper.RtnObservingTime(0, 0, observer_time);
		timer_helper.RtnObservingTime(update_milli, observer_time2);

		std::cout << observer_time << " " << observer_time2 << " " << update_milli << std::endl;
		UnifiedSleep(1000 * 2);
	}
}


int main(){
///	local_time_helper_test();

	TradingTimeHelper_test();
}