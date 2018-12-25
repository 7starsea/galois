///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#ifndef GAUSS_UTILITIES_TRADING_TIME_PARSER_H
#define GAUSS_UTILITIES_TRADING_TIME_PARSER_H
#include <string>
#include <vector>

namespace Galois{
	namespace Utilities{
				
	class TradingTimeParser{
	public:
		/// @param trading_time its format is like [T1, T2]; [T3, T4]; [T5, T6]; ...
		///              and the format of each time point [T1, T2, ...] is HH:MM:SS
		/// 格式为 [T1, T2]; [T3, T4]; [T5, T6]; ...
		/// 其中时间点 T1, T2, T3, T4, T5, T6等的格式为 HH:MM:SS
		TradingTimeParser(const char * trading_times);

		/// @brief return bool indicating whether the trading_time is parsed or not
		/// 所否交易时间是否通过解析
		inline bool IsValidTradingTime()const{return is_parsed_;}
		inline bool crossed_day() const{return crossed_day_; }
		/// @brief 返回总交易秒数
		inline int total_seconds() const { return total_seconds_; }
		///Arg time format: HH:MM:SS

		///返回当前时间的秒数，time format: HH:MM:SS, reture: HH * 3600 + MM * 3600 + SS
		inline int to_seconds(const char * time)const{return _to_seconds(time);}

		////时间time是否属于交易时间段
		inline bool include(const char*time) const{return this->_include(  _to_seconds(time) );}
		/////Example: include( 11 * 3600 + 23 * 60 + 10 ) = include( "11:23:10" )
		inline bool include(int t)const{return _include(t);}	

		
		/// @brief return relative time seconds; 
		///        parameter t is coming from _to_seconds(char * time)
		inline int relativeTime(const char*time)const{return _adjust_time(_to_seconds(time));}
		inline int relativeTime(int t)const{ return _adjust_time(t); }

		/// @brief unfold trading_times as a list in seconds, return the index of the given time in that list
		int relativeIndex(const char * time)const;
		
		inline std::string RtnBeginTradingTime()const{  return original_times_[0].first; }
		inline std::string RtnEndTradingTime()const{  return original_times_.rbegin()->second; }

		inline int GetBeginTradingTime()const{ return original_intervals_[0].first; }
		inline int GetEndTradingTime()const{ return original_intervals_.rbegin()->second; }

	public:
		const std::string trading_time_;	/// 交易时间
	private:
		int current_date_;		/// 相对时间的起始秒数
		int total_seconds_;		/// 总交易时间秒数
		bool crossed_day_;		/// 交易时间段是否跨了两天
		bool is_parsed_;		/// 所给交易时间段格式是否正确
	protected:

		std::vector< std::pair<std::string, std::string> > original_times_;		/// 原始交易时间段
		std::vector< std::pair<int, int> > original_intervals_;					/// 经过解析的原始交易时间段

		int _to_seconds( const char * time )const;

		/// @brief param int should come from _to_seconds( const char * time )
		bool _include(int)const;
		/// @brief return relative time seconds; parameter t is coming from _to_seconds(char * time)
		int _adjust_time(int t)const;
	private:
		int __to_seconds( std::vector<char> & buf)const;
		void __adjust_intervals();
	};


}//namespace Utilities
}///namespace Galois

#endif
