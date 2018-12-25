///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#include "trading_day.h"
#include <time.h> 
#include <boost/date_time/gregorian/gregorian.hpp>

std::string current_trading_day(){
	time_t rawtime = 0;
	::time(&rawtime);
	struct tm * timeinfo = ::localtime(&rawtime);
	tm info;
	std::memcpy(&info, timeinfo, sizeof(info));

	boost::gregorian::date cur_trading_date = boost::gregorian::day_clock::local_day();
	if (info.tm_hour > 17){ /// 17 = 5pm, then settlement_day +1
		cur_trading_date += boost::gregorian::days(1);
	}

	switch (cur_trading_date.day_of_week())
	{
	case boost::date_time::Saturday:
		cur_trading_date += boost::gregorian::days(2);
		break;
	case boost::date_time::Sunday:
		cur_trading_date += boost::gregorian::days(1);
		break;
	default:
		break;
	}

	return boost::gregorian::to_iso_string(cur_trading_date);//RealTimeObservingDate;
}
