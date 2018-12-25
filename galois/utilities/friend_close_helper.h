///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#ifndef GALOIS_UTILITIES_FRIEND_CLOSE_HELPER_H
#define GALOIS_UTILITIES_FRIEND_CLOSE_HELPER_H

#include <boost/atomic.hpp>
#include "singleton.hpp"

class FriendCloseHelperImpl : private boost::noncopyable{
public:
	static void registerSignals();
	static void static_signal_handler(int signum);
public:
	void signalHandler(int signum);

	inline bool IsRunning(){ return stopped_.load(boost::memory_order_relaxed) < 0; }
	inline bool IsStopping(){ return 0 == stopped_.load(boost::memory_order_relaxed); }
	inline void Stop(){ stopped_.store(1); }
	
protected:
	FriendCloseHelperImpl();
protected:
	///-1:Normal; 0:Cleaning; 1:Cleaned
	boost::atomic<int> stopped_;
};

typedef Galois::Utilities::Singleton<FriendCloseHelperImpl> FriendCloseHelper;


#endif
