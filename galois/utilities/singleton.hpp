///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#ifndef GALOIS_UTILITIES_SINGLETON_HPP
#define GALOIS_UTILITIES_SINGLETON_HPP
#include <memory>
#include <utility>

#include <boost/config.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/core/ignore_unused.hpp>
/*!
 * Aim: Provide a generic C++ singleton abstract class
 * @class template<typename T> Singleton; 
 * The parameter (class) T must be derived from boost::noncopyable
 *      and the constructor of T must be protected
 * @example:
 *      class LoggerImpl : private boost::noncopyable{
 *          public:
 *              void log(const std::string & msg);
 *          protected:
 *              LoggerImpl(const std::string & logger_file_name);
 *      };
 *
 *      typedef Singleton<LoggerImpl> Logger;
 *		Logger & my_logger = Logger::create( std::string("Log_file_name") );
 *		my_logger.log("This is a test message.");
 *
 */

namespace Galois{	namespace Utilities{

#ifndef BOOST_NO_CXX11_VARIADIC_TEMPLATES

        template<typename T>
        class Singleton : public T{
        public:
            BOOST_MPL_ASSERT(( boost::is_base_of<boost::noncopyable, T> ));
			
			static Singleton<T>& create(){
				static Singleton<T> my_instance;
				return my_instance;
			}

            template <typename... Args>
            static Singleton<T>&  create(Args&&... args){
                static Singleton<T> my_instance( std::forward<Args>(args)...);
                return my_instance;
            } 
        public:
            ~Singleton(){}

        protected:
			///Singleton():T(){}

            template <typename... Args>
            Singleton(Args&&... args): T(std::forward<Args>(args)...) {}
        };
#else


	template<typename T>
	class Singleton : public T
	{
	public:
		BOOST_MPL_ASSERT(( boost::is_base_of<boost::noncopyable, T> ));

		static Singleton<T>& create(){
			static Singleton<T> my_instance;
			return my_instance;
		}

		template<typename T1>
		static Singleton<T>& create(T1 && t1){
			static Singleton<T> my_instance( std::forward<T1>(t1) );
			return my_instance;
		}

		template<typename T1, typename T2>
		static Singleton<T>& create(T1 && t1, T2 && t2){
			static Singleton<T> my_instance( std::forward<T1>(t1), std::forward<T2>(t2) );
			return my_instance;
		}

		template<typename T1, typename T2, typename T3>
		static Singleton<T>& create(T1 && t1, T2 && t2, T3 && t3){
			static Singleton<T> my_instance( std::forward<T1>(t1), std::forward<T2>(t2), std::forward<T3>(t3) );
			return my_instance;
		}

		template<typename T1, typename T2, typename T3, typename T4>
		static Singleton<T>& create(T1 && t1, T2 && t2, T3 && t3, T4 && t4){
			static Singleton<T> my_instance( std::forward<T1>(t1), std::forward<T2>(t2), std::forward<T3>(t3), std::forward<T4>(t4) );
			return my_instance;
		}
	public:
		~Singleton(){}

	protected:
		Singleton():T(){}

		template<typename T1>
		Singleton(T1 && t1): T(std::forward<T1>(t1)){}

		template<typename T1, typename T2>
		Singleton(T1 && t1, T2 && t2): T(std::forward<T1>(t1), std::forward<T2>(t2)){}  

		template<typename T1, typename T2, typename T3>
		Singleton(T1 && t1, T2 && t2, T3 && t3): T(std::forward<T1>(t1), std::forward<T2>(t2), std::forward<T3>(t3)){}  

		template<typename T1, typename T2, typename T3, typename T4>
		Singleton(T1 && t1, T2 && t2, T3 && t3, T4 && t4): T(std::forward<T1>(t1), std::forward<T2>(t2), std::forward<T3>(t3), std::forward<T4>(t4)){}  


	};
#endif


    }///namespace Utilities 
}///namespace Galois
        
#endif
