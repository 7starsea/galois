///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
/////////////////////////////////////////////////////////////////////////
///@file UnifiedHighResolutionTimer.hpp
///@brief定义并实现一个基于Windows API的高精度时钟，可提供秒，毫秒及微秒级的计时精度
///@author Veidt provides the window version
///@date 20150414
///@author Aimin provides the Linux and MacOS version
///@date 20160719
///	APIs
///	void start();
///	double seconds_elapsed() ;
///	double milliseconds_elapsed();
///	double microseconds_elapsed();
///每次计时开始，调用该对象的start函数，计时结束时：
///调用seconds_elapsed函数获取以从上一次start开始以秒为单位的时间消耗
///调用milliseconds_elapsed函数获取以从上一次start开始以毫秒为单位的时间消耗(ms=1/1000 s)
///调用microseconds_elapsed函数获取以从上一次start开始以微秒为单位的时间消耗(us=1/1000 ms = 1/10^6 s)
/////////////////////////////////////////////////////////////////////////
#ifndef GALOIS_UTILITIES_HIGH_RESOLUTION_TIMER_H
#define GALOIS_UTILITIES_HIGH_RESOLUTION_TIMER_H

#include <boost/predef.h>



namespace Galois{
	namespace Utilities{

#if BOOST_OS_WINDOWS
#include <Windows.h>
	class HighResolutionTimer
	{
	public:
		const static char version = 'W';	///Windows
	public:
		HighResolutionTimer(){
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			high_res_frequency_ = (double)frequency.QuadPart;
			QueryPerformanceCounter(&start_time_);
		}

		void start(){
			QueryPerformanceCounter(&start_time_);
		}

		double seconds_elapsed() {
			//LARGE_INTEGER now;
			QueryPerformanceCounter(&end_time_);
			return (end_time_.QuadPart - start_time_.QuadPart)/high_res_frequency_;
		}

		double milliseconds_elapsed() {
			//LARGE_INTEGER now;
			QueryPerformanceCounter(&end_time_);
			return 1000*(end_time_.QuadPart - start_time_.QuadPart)/high_res_frequency_;
		}

		double microseconds_elapsed()  {
			//LARGE_INTEGER now;
			QueryPerformanceCounter(&end_time_);
			return 1000000*(end_time_.QuadPart - start_time_.QuadPart)/high_res_frequency_;
		}

	private:
		LARGE_INTEGER start_time_;
		LARGE_INTEGER end_time_;
		double high_res_frequency_;
	};


#elif BOOST_OS_LINUX
	#include <time.h>

	// CLOCK_REALTIME, a system-wide realtime clock.
	// CLOCK_PROCESS_CPUTIME_ID, high-resolution timer provided by the CPU for each process.
	// CLOCK_THREAD_CPUTIME_ID, high-resolution timer provided by the CPU for each of the threads.

	class HighResolutionTimer
	{
	public:
		const static char version = 'L';	///Linux
	public:
		HighResolutionTimer(const clockid_t clk_id = CLOCK_REALTIME )
			:clk_id_(clk_id){
			clock_gettime(clk_id_, &start_time_);
		}
		void start(){
			clock_gettime(clk_id_, &start_time_);
		}
		double seconds_elapsed() {
			clock_gettime(clk_id_, &end_time_);
			double elapsedTime = (end_time_.tv_sec - start_time_.tv_sec)
								+(end_time_.tv_nsec - start_time_.tv_nsec)/1000000000.0;
			return elapsedTime;
		}
		double milliseconds_elapsed() {
			clock_gettime(clk_id_, &end_time_);
			double elapsedTime = (end_time_.tv_sec - start_time_.tv_sec) * 1000.0
								+(end_time_.tv_nsec - start_time_.tv_nsec)/1000000.0;
			return elapsedTime;
		}
		double microseconds_elapsed()  {
			clock_gettime(clk_id_, &end_time_);
			double elapsedTime = (end_time_.tv_sec - start_time_.tv_sec) * 1000000.0
								+(end_time_.tv_nsec - start_time_.tv_nsec)/1000.0;
			return elapsedTime;
		}
	private:
		const clockid_t clk_id_;
		timespec  start_time_;
		timespec  end_time_;

	//	double high_res_frequency_;
	};

#elif BOOST_OS_MACOS  /////for MAC OS in my case
	#include <sys/time.h> 
	#include <mach/clock.h>
	#include <mach/mach.h>
#ifndef _CLOCKID_T
#define	_CLOCKID_T
typedef	int	clockid_t;	/* clock identifier type */  
#endif	/* ifndef _CLOCKID_T */

#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC				((clockid_t)0)
	/* system-wide monotonic clock (aka system time) */
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME				((clockid_t)-1)
	/* system-wide real time clock */
#endif

#ifndef CLOCK_PROCESS_CPUTIME_ID
#define CLOCK_PROCESS_CPUTIME_ID	((clockid_t)-2)
	/* clock measuring the used CPU time of the current process */
#endif

#ifndef CLOCK_THREAD_CPUTIME_ID
#define CLOCK_THREAD_CPUTIME_ID		((clockid_t)-3)
	/* clock measuring the used CPU time of the current thread */
#endif

	class HighResolutionTimer
	{
	public:
		const static char version = 'M';	///MAC
	public:
		HighResolutionTimer(const clockid_t clk_id = CLOCK_REALTIME )	
			:clock_service_(clock_serv_t())
		{
		//	if( clk_id == CLOCK_REALTIME)
				host_get_clock_service(mach_host_self(),	SYSTEM_CLOCK,  &clock_service_);
		//	else
		//		host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &clock_service_);
		
	//		clock_serv_t cclock;
	//		mach_timespec_t mts;
			clock_get_time(clock_service_, &start_time_);
	//  mach_port_deallocate(mach_task_self(), cclock);
		
		}

		~HighResolutionTimer(){
			mach_port_deallocate(mach_task_self(), clock_service_);
		}
		void start(){
			clock_get_time(clock_service_, &start_time_);
		}
		double seconds_elapsed() {
			clock_get_time(clock_service_, &end_time_);
			double elapsedTime = (end_time_.tv_sec - start_time_.tv_sec) 
								+ (end_time_.tv_nsec - start_time_.tv_nsec) / 1000000000.0; 
			 return elapsedTime;
		}
		double milliseconds_elapsed() {
			clock_get_time(clock_service_, &end_time_);
			double elapsedTime = (end_time_.tv_sec - start_time_.tv_sec) * 1000.0
								+ (end_time_.tv_nsec - start_time_.tv_nsec) / 1000000.0; 
			 return elapsedTime;
		}
		double microseconds_elapsed()  {
			clock_get_time(clock_service_, &end_time_);
			double elapsedTime = (end_time_.tv_sec - start_time_.tv_sec) * 1000000.0
								+ (end_time_.tv_nsec - start_time_.tv_nsec)/1000.0; 
			 return elapsedTime;
		}
	private:
		clock_serv_t clock_service_;

		mach_timespec_t  start_time_;
		mach_timespec_t	 end_time_;

	//	double high_res_frequency_;
	};
#else		////for all other systems
	#include <sys/time.h> 
	class HighResolutionTimer
	{
	public:
		const static char version = 'O';	///Other
	public:
		HighResolutionTimer( )	{
			gettimeofday(&start_time_, NULL);
		}
		void start(){
			gettimeofday(&start_time_, NULL);
		}
		double seconds_elapsed() {
			gettimeofday(&end_time_, NULL);
			double elapsedTime = (end_time_.tv_sec - start_time_.tv_sec) 
								+ (end_time_.tv_usec - start_time_.tv_usec) / 1000000.0; 
			 return elapsedTime;
		}
		double milliseconds_elapsed() {
			gettimeofday(&end_time_, NULL);
			double elapsedTime = (end_time_.tv_sec - start_time_.tv_sec) * 1000.0
								+ (end_time_.tv_usec - start_time_.tv_usec) / 1000.0; 
			 return elapsedTime;
		}
		double microseconds_elapsed()  {
			gettimeofday(&end_time_, NULL);
			double elapsedTime = (end_time_.tv_sec - start_time_.tv_sec) * 1000000.0
								+ (end_time_.tv_usec - start_time_.tv_usec); 
			 return elapsedTime;
		}
	private:
		timeval  start_time_;
		timeval	 end_time_;
	};
#endif

}//namespace Utilities
}///namespace Galois
#endif
