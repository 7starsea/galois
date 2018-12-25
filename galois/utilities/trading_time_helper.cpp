///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#include "trading_time_helper.h"

#include <iostream>
#include <time.h>
#include <utility>
#include <string.h>

#include <boost/predef.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <galois/utilities/local_time_helper.h>

#if BOOST_OS_WINDOWS
#include <windows.h>
#else
#include <sys/time.h>
#endif


namespace Galois{
	namespace Utilities{

	TradingTimeHelper::TradingTimeHelper(const char * trading_times)
		:TradingTimeParser(trading_times) {}

	bool TradingTimeHelper::_beforeTrading(int t)const{
	   int t1 = original_intervals_.begin()->first,
				t2 = original_intervals_.rbegin()->second;
														///detect near which point
	   return ( t < t1 ) || ( crossed_day() && t > t2 &&  ( t - t2 > t1 - t + 86400 )  &&  t-86400 < t1  );
	}
	bool TradingTimeHelper::_afterTrading(int t)const{
	   int t1 = original_intervals_.begin()->first,
				t2 = original_intervals_.rbegin()->second;
														///detect near which point
	   return ( t > t2 && !crossed_day() ) || ( crossed_day() && t > t2 &&  ( t - t2 < t1 - t + 86400 )  );
	}

	int TradingTimeHelper::_beforeEndTrading(int t)const{
		t = _adjust_time(t);
		int result = -1;
		int t1 = original_intervals_.begin()->first,
			t2 = original_intervals_.rbegin()->second;
		if( t1 <= t && t <= t2){
			result = t2 - t;
		}
		return result;
	}
	int TradingTimeHelper::_afterBegTrading(int t)const{
		t = _adjust_time(t);
		int result = -1;
		int t1 = original_intervals_.begin()->first,
			t2 = original_intervals_.rbegin()->second;
		if( t1 <= t && t <= t2){
			result = t - t1;
		}
		return result;
	}
	int TradingTimeHelper::_currentOpenTime(int t)const{
		t = _adjust_time(t);
		int result = -1;
		for(int i=0;i<(int)original_intervals_.size();++i){
			if( t >= original_intervals_[i].first && t <= original_intervals_[i].second ){
				result = t - original_intervals_[i].first;
				break;
			}
		}
		return result;
	}

	int TradingTimeHelper::_nextClosingTime(int t)const{
		t = _adjust_time(t);
		int result = -1;
		for(int i=0;i<(int)original_intervals_.size();++i){
			if( t >= original_intervals_[i].first && t <= original_intervals_[i].second ){
				result = original_intervals_[i].second - t;
				break;
			}
		}
		return result;
	}

	int TradingTimeHelper::_prevTradingTime(int t)const{
		t = _adjust_time(t);
		int result = -1;
		int size = (int) original_intervals_.size();
		if( t > original_intervals_[size-1].second ){
			result = t - original_intervals_[size-1].second;
		}else{
			for(int i=0; i<size-1; ++i){
				if( t > original_intervals_[i].second && t < original_intervals_[i+1].first ){
					result = t - original_intervals_[i].second;
					break;
				}
			}
		}
		return result;
	}

	int TradingTimeHelper::_nextTradingTime(int t)const{
		t = _adjust_time(t);
		int result = -1;
		if( t < original_intervals_[0].first ){
			result = original_intervals_[0].first - t;
		}else{
			for(int i=1; i<(int)original_intervals_.size();++i){
				if( t > original_intervals_[i-1].second && t < original_intervals_[i].first ){
					result = original_intervals_[i].first - t;
					break;
				}
			}
		}
		return result;
	}


	
	std::string TradingTimeHelper::RtnTradingDay()const{

		LocalTimeHelper & local_time_helper = LocalTimeHelper::create();
		tm info = local_time_helper.local_time();

		int now = info.tm_hour * 3600 + info.tm_min * 60 + info.tm_sec;

		boost::gregorian::date cur_trading_date = boost::gregorian::day_clock::local_day();
		if( now  > original_intervals_.rbegin()->second && !this->_afterTrading(now) ){
			cur_trading_date +=  boost::gregorian::days(1);
		}

		switch (cur_trading_date.day_of_week())
		{
		case boost::date_time::Saturday:
			cur_trading_date +=  boost::gregorian::days(2);
			break;
		case boost::date_time::Sunday:
			cur_trading_date +=  boost::gregorian::days(1);
			break;
		default:
			break;
		}

		return boost::gregorian::to_iso_string( cur_trading_date );//RealTimeObservingDate;
	}


	///thread-unsafe b/c localtime and time function
	bool TradingTimeHelper::RtnObservingTime(char * observer_time)const{
		LocalTimeHelper & local_time_helper = LocalTimeHelper::create();
		tm info = local_time_helper.local_time();
		LocalTimeHelper::to_update_time(info, observer_time);

		int now = info.tm_hour * 3600 + info.tm_min * 60 + info.tm_sec;

		return _include(now);
	}
	///thread-unsafe b/c localtime and time function
	bool TradingTimeHelper::RtnObservingTime(const int before_trade, const int after_trade,  char * observer_time)const{
		LocalTimeHelper & local_time_helper = LocalTimeHelper::create();
		tm info = local_time_helper.local_time();
		LocalTimeHelper::to_update_time(info, observer_time);

		int now = info.tm_hour * 3600 + info.tm_min * 60 + info.tm_sec;

		return _include(now) || _include(now - before_trade) || _include(now + after_trade);
	}

	bool TradingTimeHelper::RtnObservingTime(int & update_milli, char * observer_time)const{
		LocalTimeHelper & local_time_helper = LocalTimeHelper::create();
		/// const tm info = local_time_helper.local_time();
		
#if BOOST_OS_WINDOWS
		SYSTEMTIME tp;
		GetLocalTime(&tp);
		/// LONG time_ms = (tp.wSecond * 1000) + tp.wMilliseconds;
		update_milli = tp.wMilliseconds;
		//// const tm info = local_time_helper.local_time(tp.wHour * 3600 + tp.wMinute * 60 + tp.wSecond);
		const tm info = { tp.wSecond, tp.wMinute, tp.wHour, tp.wDay, tp.wMonth, tp.wYear, tp.wDayOfWeek, 0, 0 };
#else
		struct timeval tp;
		gettimeofday(&tp, NULL);
		///	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		update_milli = tp.tv_usec / 1000;
		const tm info = local_time_helper.local_time(tp.tv_sec);
#endif

		LocalTimeHelper::to_update_time(info, observer_time);
		int now = info.tm_hour * 3600 + info.tm_min * 60 + info.tm_sec;
		return _include(now);
	}

}//namespace Utilities
}///namespace Galois