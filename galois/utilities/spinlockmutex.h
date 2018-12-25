///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#ifndef GALOIS_UTILITIES_SPINLOCK_MUTEX_H
#define GALOIS_UTILITIES_SPINLOCK_MUTEX_H


#include <boost/atomic/atomic_flag.hpp>

namespace Galois{
	namespace Utilities{
		/// Provide a spin mutex; see https://en.wikipedia.org/wiki/Spinlock 
		/// Spin mutex is better than std::mutex in trading system for saving wakeup time from thread
		class SpinLockMutex {
		public:
			SpinLockMutex(){}
			inline bool try_lock(){
				return false == locked_.test_and_set(boost::memory_order_acquire);
			}
			inline void lock() {
				while (locked_.test_and_set(boost::memory_order_acquire));
			}
			inline void unlock() {
				locked_.clear(boost::memory_order_release);
			}
		protected:
			boost::atomic_flag locked_;

			BOOST_DELETED_FUNCTION(SpinLockMutex(const SpinLockMutex& rhs))
			BOOST_DELETED_FUNCTION(SpinLockMutex& operator=(const SpinLockMutex& rhs))
		};


}//namespace Utilities
}///namespace Galois
#endif