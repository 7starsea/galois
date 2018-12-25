///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#include "friend_close_helper.h"

#include <iostream>
#include <chrono>		///std::chrono::seconds
#include <thread>
#include <csignal>
#include <boost/predef.h>

void FriendCloseHelperImpl::registerSignals(){
	void(*handler) (int) = FriendCloseHelperImpl::static_signal_handler;
	std::signal(SIGINT, handler);
	std::signal(SIGTERM, handler);
	std::signal(SIGABRT, handler);
	//	std::signal(SIGILL, handler); 
	//	std::signal(SIGSEGV, handler); 
#if BOOST_OS_WINDOWS
	std::signal(SIGBREAK, handler);
#else
	std::signal(SIGILL, handler);
#endif

}

void FriendCloseHelperImpl::static_signal_handler(int signum){
	FriendCloseHelper & close_helper = FriendCloseHelper::create();
	close_helper.signalHandler(signum);
}


FriendCloseHelperImpl::FriendCloseHelperImpl()
	:stopped_(-1){
	FriendCloseHelperImpl::registerSignals();
}


void FriendCloseHelperImpl::signalHandler( int signum )
{
	(void)(signum);
	std::cout<<"Please waiting for the program to clean up the resources..."<<std::endl;
	if(-1 == stopped_.load() ) stopped_.store(0);
}

