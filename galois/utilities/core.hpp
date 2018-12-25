///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#ifndef GALOIS_UTILITIES_CORE_HPP
#define GALOIS_UTILITIES_CORE_HPP
#include <string>
#include <type_traits>
#include <string.h> ///memset
#include <assert.h>	///assert
#include <vector>
#include <algorithm> 
#include <boost/filesystem.hpp>

#ifdef _WIN32
#include <io.h> 
#define access    _access_s
#else
#include <unistd.h>
#endif


namespace Galois{
	namespace Utilities{

		inline bool check_file_exists(const std::string & file_name){
			 return access( file_name.c_str(), 0 ) == 0;
		}

		inline void ensure_exists_dir(const std::string & dir){
			boost::filesystem::is_directory(dir) || boost::filesystem::create_directories(dir);
		}

		struct is_space{
			bool operator()(char c){
				return c ==' ' || c == '\r' || c == '\n' || c == '\t' || c == '\v' || c == '\f';
			}
		};

		inline void remove_white_space( std::string & s){
			s.erase(std::remove_if(s.begin(), s.end(), is_space() ), s.end());
		}


		namespace details {
			///only for positive integer
			//template<typename BufferType, typename DataType, typename std::enable_if<std::is_integral<DataType>::value && !std::is_floating_point<DataType>::value>::type >
			template<typename BufferType, typename DataType>
			void _to_str(BufferType & buffer, DataType j){
				static_assert( std::is_integral<DataType>::value, "DataType must be integral: (unsigned) int, short, long!" );
				if( j > 0 ){
					const std::size_t beg = buffer.size();
					while( j ){
						buffer.push_back( char(j%10 + 48) );	/// char('0') == int(48)
						j = j/10;
					}
					std::reverse( buffer.begin() + beg, buffer.end() );
				}else if( j == 0){
					buffer.push_back( '0' );
				}
			}

			template<typename DataType>
			std::size_t _to_str(char * buffer, std::size_t N, DataType j){
				static_assert( std::is_integral<DataType>::value, "DataType must be integral: (unsigned) int, short, long!" );
				std::size_t next = 0;
				if( j > 0 ){
					while( j && next < N){
						buffer[next++] = char(j%10 + 48);/// char('0') == int(48)
						j = j/10;
					}
					buffer[next] = 0;
					std::reverse( buffer, buffer + next);
				}else if( j == 0 && N){
					buffer[0] = '0';
					buffer[1] = 0;
					next = 1;
				}
				return next;
			}
		}


		/// Mainly used in convert local_id to order_ref
		/// N is the buffer length;
		/// M is the length of the return buffer
		/// Padding is the character for leading padding, generally zero
		/// Assuming that j is non-negative; otherwise no change on buffer
		template<std::size_t N, std::size_t M, char Padding>
		inline void OrderRefConversionFromInt(char (& buffer)[N], int j){
			///maximum int : 2147483647, length: 10;
			assert(( N > M )); assert(( M >= 10 ));
			if( j >= 0){
				memset(buffer, int(Padding), M); memset(buffer + M, 0, N-M);
				int k = 1;
				while(j){
					buffer[M-k] = char(j%10 + 48);	/// char('0') == int(48)
					j = j/10;
					++k;
				}
			}
		}


		////BufferType should support method: push_back; usuage: std::string, vector<char>
		template<typename BufferType, typename DataType>
		void to_str(BufferType & buffer, DataType j, int precision = 0){
			static_assert( std::is_arithmetic<DataType>::value, "DataType must be arithmetic!" );
			if( j < 0){
				j = -j;
				buffer.push_back('-');
			}

			long int i = (long int)j;
			details::_to_str( buffer, i);
			j -= i;
			if(std::is_floating_point<DataType>::value && precision > 0){
				buffer.push_back('.');
				for(int k = 1;k<= precision; ++k){
					j *= 10; 
					///if(k == precision) j += .5;	////四舍五入
					i = (long int) j; 
					buffer.push_back(  char(i + 48) );
					j -= i;
				}
			}
		}

		template<typename DataType>
		void to_str(char * buffer,  DataType j, std::size_t N, int precision = 0){
			static_assert( std::is_arithmetic<DataType>::value, "DataType must be arithmetic!" );
			////N is usually the size of buffer including \0
			std::size_t next = strlen(buffer); N-=1;
			if( j < 0){
				j = -j;
				if(next < N){
					buffer[next] ='-'; buffer[++next]=0;
				}
			}

			long int i = (long int)j;
			next += details::_to_str(buffer+next, N-next, i);
			j -= i;
			if(std::is_floating_point<DataType>::value && precision > 0){
				if(next < N) buffer[next++] = '.';
				for(int k = 1;k<= precision; ++k){
					j *= 10; 
					///if(k == precision) j += .5;	////四舍五入
					i = (long int) j; 
					if(next < N) buffer[next++] = char(i + 48);
					j -= i;
				}
				buffer[next] = 0;
			}
		}



}//namespace Utilities
}///namespace Galois

#endif