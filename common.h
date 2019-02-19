#ifndef _my_ptime_h
#define _my_ptime_h

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

/// dummy helper class to make boost::posix_time::ptime work with run_test()
class my_ptime : public ptime {
public:
	my_ptime(int n = 0) : ptime(boost::gregorian::date(1970, Jan, 1)) { }
	my_ptime &operator++(int) {
		*this += seconds(1);
		return *this;
	}
};

/// sample key functor specialisation for ptime
template<> double key<my_ptime>::operator()(const my_ptime &x) const { return (x-ptime(date(1900, Jan, 1))).total_milliseconds(); }

/// sample key functor specialisation for time_t
template<> double key<time_t>::operator()(const time_t &x) const { return x; }

#endif
