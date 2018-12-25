///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#ifndef GALOIS_UTILITIES_LOCAL_TIME_HELPER_H
#define GALOIS_UTILITIES_LOCAL_TIME_HELPER_H

#include <time.h> 

#include <galois/utilities/singleton.hpp>
#include <galois/utilities/spinlockmutex.h>

namespace Galois{
	namespace Utilities{
		/*!
		本文件的核心目录是
		提供一个获取本地时间线程安全的LocalTimeHelper类，此类无copy构造函数，且只能有一个实例，获取实例方法
		LocalTimeHelper & local_time_helper = LocalTimeHelper::create();
		*/

		///定义 LocalTimeHelper 类的具体实现类 LocalTimeHelperImpl
		class LocalTimeHelperImpl : private boost::noncopyable{
		public:
			///将结构体info里面的时间以 HH:MM:SS 格式写到 observer_time 里
			///要求: observer_time 至少能容纳 9 个字节的，否则将出现不可预料的错误
			static void to_update_time(const tm & info, char * observer_time);
		public:
			///线程安全的返回存储当前时间信息的结构体 tm
			tm local_time();
			tm local_time(const time_t sec_since_epoch);

			///将当前时间以 HH:MM:SS 格式写到 observer_time 里
			///要求: observer_time 至少能容纳 9 个字节的，否则将出现不可预料的错误
			void update_time(char * observer_time);
		protected:
			LocalTimeHelperImpl(){}
		protected:
			
			Galois::Utilities::SpinLockMutex mutex_;		///用于保证线程安全的锁
		};
		///定义 LocalTimeHelper 类
		typedef Galois::Utilities::Singleton<LocalTimeHelperImpl> LocalTimeHelper;
	}//namespace Utilities
}///namespace Galois

#endif