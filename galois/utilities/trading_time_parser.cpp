///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#include "trading_time_parser.h"

#include <iostream>
#include <time.h>
#include <utility>
#include <string.h>


namespace Galois{
	namespace Utilities{

TradingTimeParser::TradingTimeParser(const char * trading_times)
    :trading_time_(trading_times)
	, current_date_(0), total_seconds_(-1)
	, crossed_day_(false),is_parsed_(true)
{
	std::size_t i=0, n = strlen(trading_times);

	std::vector<char> first, second;
	std::vector<char> * expected = NULL;
//	bool parsed = true;
	while(i<n && is_parsed_){
		char c = trading_times[i];++i;
		if( ' ' == c || '\t' == c ){
			////do nothing
		}else if('[' == c || '(' == c){
			////expect first number
			expected = &first;
			if( second.size() ){
				is_parsed_ = false;
			}
		}else if( ']' == c || ')' == c){
			////a interval 
			if( first.size() && second.size() ){
				first.push_back('\0'); second.push_back('\0');
				int v1 = __to_seconds( first ), v2 = __to_seconds( second );
				
				if(v1 < v2){
					original_times_.push_back( std::pair<std::string, std::string>( reinterpret_cast<char*> (&first[0]), reinterpret_cast<char*> (&second[0])  ) );
					original_intervals_.push_back( std::pair<int, int>(v1, v2) );
				}else{
					is_parsed_ = false;
				}
				
			}else{
				is_parsed_ = false;
			}
			first.resize(0); second.resize(0); 
		}else if( (48<=c && c<=57) || ':' == c ){ //HH:MM:SS
			if( expected ){
				expected->push_back( c );
			}else{
				is_parsed_ = false;
			}
		}else if( ',' == c){
			////expect second number
			expected = &second;
			if( ! first.size() ) is_parsed_ = false;
		}else if( ';' == c ){
			////expect a new interval
			expected = NULL;
			if( first.size() || second.size() ) is_parsed_ = false;
		}else{
			is_parsed_ = false;
		}	
	}
	if( ! (is_parsed_ && original_intervals_.size()) ){
	//	std::cout<<"Failed to parse data range:" <<intervals<<std::endl;
		std::cerr<<"===> Failed to parse trading time range:" <<trading_times <<std::endl;
//		exit(-1);
	}

	__adjust_intervals();
}

void TradingTimeParser::__adjust_intervals(){
	current_date_  = 2 * 86400; //= 2 * 3600 * 24
	
	total_seconds_ = 0;
	int index = 0;
	while( index < (int)original_intervals_.size() ){
		std::pair<int, int> & interval = original_intervals_[index];
		interval.first += current_date_;
		interval.second += current_date_;

		total_seconds_ += interval.second - interval.first + 1;

		if( interval.first > interval.second ){
            if( crossed_day_ ){
				std::cout<< "A valid trading time range could not cross two days!!!" <<std::endl;
				exit(-1);
			}else{
                crossed_day_ = true;
				for(int i = 0; i < index; ++i){
                    original_intervals_[i].first -= 86400;// 3600 * 24;
                    original_intervals_[i].second -=86400;// 3600 * 24;
				}
                original_intervals_[index].first -= 86400;// 3600 * 24;
			}
		}
        if(index){
            if(interval.first < original_intervals_[index-1].second){
                if( crossed_day_ ){
                    std::cout<< "A valid trading time range could not cross two days!!!" <<std::endl;
                    exit(-1);
                }else{
                    crossed_day_ = true;
                    for(int i = 0; i < index; ++i){
                        original_intervals_[i].first -= 86400; //=3600 * 24;
                        original_intervals_[i].second -= 86400; //=3600 * 24;
                    }
                }
            }
        }
		++index;
	}

	if(!crossed_day_){
		index = 0;
		while( index < (int)original_intervals_.size() ){
			std::pair<int, int> & interval = original_intervals_[index];
			interval.first -= current_date_;
			interval.second -= current_date_;

			++index;
		}
		current_date_ = 0;
	}
 
}


	int TradingTimeParser::_adjust_time(int t)const{
		t += current_date_;

		int t2 = original_intervals_.rbegin()->second;
		if( crossed_day_ &&  t > t2 ){
			int t1 = original_intervals_.begin()->first;
			if( t - t2 > t1 - t + 86400 )
				t -= 86400; //=3600 * 24;
		}
		return t;
	}

	bool TradingTimeParser::_include(int t)const{
		t = _adjust_time(t);
		bool result = false;
		for(std::size_t i=0; i < original_intervals_.size(); ++i){
			if( t >= original_intervals_[i].first && t <= original_intervals_[i].second ){
				result = true;
				break;
			}
		}
		return result;
	}

	int TradingTimeParser::relativeIndex(const char * time)const{
		int t = _adjust_time( _to_seconds(time));

		int count_index = 0, index = -1;
		for(std::size_t i=0; i < original_intervals_.size(); ++i){
			if( t >= original_intervals_[i].first && t <= original_intervals_[i].second ){
				index = count_index + t - original_intervals_[i].first;
				break;
			}
			count_index += (original_intervals_[i].second - original_intervals_[i].first) + 1;
		}
		return index;
	}

	int TradingTimeParser::_to_seconds( const char * time )const{
		//// fmt: HH:MM:SS
		/// char(0) = 48
		bool flag = true;
		int tmp = (int)time[0] - 48, tmp1 = (int)time[1] - 48;
		flag &= tmp1 >= 0 && ((tmp >= 0 && tmp <= 1 && tmp1 <= 9) || (2 == tmp && tmp1 <= 4));
		int hour = 10 * tmp + tmp1;

		tmp = (int)time[3] - 48; tmp1 = (int)time[4] - 48;
		flag &= (tmp >= 0 && tmp <= 5 && tmp1 >= 0 && tmp1 <= 9) || (6 == tmp && 0 == tmp1);
		int minute = 10 * tmp + tmp1;

		tmp = (int)time[6] - 48; tmp1 = (int)time[7] - 48;
		flag &= (tmp >= 0 && tmp <= 5 && tmp1 >= 0 && tmp1 <= 9) || (6 == tmp && 0 == tmp1);
		int second = 10 * tmp + tmp1;

		return flag ? hour * 3600 + minute * 60 + second : -1;
		
	}

	int TradingTimeParser::__to_seconds( std::vector<char> & buf)const{
		if( buf.size() < 8 ){
			std::cout<< "Invalid Time: "<< reinterpret_cast<char*> (&buf[0]) <<"! Correct time format is: HH:MM:SS, e.g. 09:30:00\n";
			exit(-1);
		}

		return _to_seconds( reinterpret_cast<char*> (&buf[0]) );//int(Utilities::toSeconds( reinterpret_cast<char*> (&buf[0]) ) + 0.5);
	}


}//namespace Utilities
}///namespace Galois