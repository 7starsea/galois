///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#ifndef GALOIS_UTILITIES_TRADING_TIME_HELPER_H
#define GALOIS_UTILITIES_TRADING_TIME_HELPER_H
#include <string>
#include <vector>

#include "trading_time_parser.h"

namespace Galois{
	namespace Utilities{

	class TradingTimeHelper : public TradingTimeParser{
	public:
		/// @brief constructor
		/// @param see \ref TradingTimeParser for the format of trading_times
		TradingTimeHelper(const char * trading_times);


		////返回当前交易日期
		std::string RtnTradingDay()const;

		///将当前最新时间以 HH:MM:SS 格式存入 observer_time
		///假设当前交易时间为T，如果有某个时间点(T，T+before_trade, T-after_trade)在交易时间段，则返回true，否则返回false

		bool RtnObservingTime(char * observer_time)const;
        bool RtnObservingTime(const int before_trade, const int after_trade, char * observer_time)const;
		bool RtnObservingTime(int & update_milli,  char * observer_time)const;


		///Arg time format: HH:MM:SS

		///如处于交易日开盘前，返回true，否则返回false. 注意: 交易日中的暂停交易时间将返回false
		inline bool beforeTrading(const char*time)const{return _beforeTrading( _to_seconds(time) );}
		inline bool beforeTrading(int t)const{ return _beforeTrading(t);}

		///如交易日收盘后, 返回true，否则返回false. 注意: 交易日中的暂停交易时间将返回false
		inline bool afterTrading(const char*time)const{return _afterTrading( _to_seconds(time) );}
		inline bool afterTrading(int t)const{ return _afterTrading( t); } 

		///离今天收盘还有多少时间: 如time不在开盘前或收盘后，则返回 -1
		inline int beforeEndTrading(const char * time)const{return _beforeEndTrading( _to_seconds(time) );}
		inline int beforeEndTrading(int t)const{return _beforeEndTrading(t);}

		///离今天开盘已有多少时间: 如time不在开盘前或收盘后，则返回 -1
		inline int afterBegTrading(const char * time)const{return _afterBegTrading(_to_seconds(time) );}
		inline int afterBegTrading(int t)const{return _afterBegTrading(t);}
				
		///如在交易时间，则返回离当前交易时间段的开始交易时间点之差; 否则返回 -1
		inline int currentOpenTime(const char *time)const{return _currentOpenTime(_to_seconds(time) );}
		inline int currentOpenTime(int t)const{return _currentOpenTime(t );}

		///如在交易时间，则返回离当前交易时间段的结束交易时间点之差; 否则返回 -1
		inline int nextClosingTime(const char *time)const{ return _nextClosingTime(_to_seconds(time) );}
		inline int nextClosingTime(int t)const{return _nextClosingTime( t);}


		///如不在交易时间，则返回离下一个开始交易时间点之差; 否则返回 -1
		inline int nextTradingTime(const char *time)const{ return _nextTradingTime(_to_seconds(time) );}
		inline int nextTradingTime(int t)const{return _nextTradingTime(t);}

		///如不在交易时间，则返回离上一个结束交易时间点之差; 否则返回 -1
		inline int prevTradingTime(const char *time)const{ return _prevTradingTime(_to_seconds(time) );}
		inline int prevTradingTime(int t)const{return _prevTradingTime(t);}
	
	protected:
		/// @brief all param int should come from _to_seconds( const char * time ), that's HH * 3600 + MM * 60 + SS 

		bool _beforeTrading(int)const;
		bool _afterTrading(int)const;
		int _beforeEndTrading(int)const;
		int _afterBegTrading(int)const;

		int _currentOpenTime(int t)const;
		int _nextClosingTime(int t)const;
		int _nextTradingTime(int t)const;
		int _prevTradingTime(int t)const;
	};


}//namespace Utilities
}///namespace Galois

#endif
