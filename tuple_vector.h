/*
 * tuple_vector.h
 *
 *  Created on: 23.10.2016
 *      Author: sini
 */

#ifndef __tuple_vector_h
#define __tuple_vector_h

#include <stdexcept>
#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>

#include <assert.h>
#include <pthread.h>

using namespace std;

/**
 * \brief key functor used to do numerical calculations with the given type K
 * 		since not all date/time types are readily convertible into numeric types
 * 		the user has to provide a template specialisation
 * \tparam K this should be the same key type as used for the tuple_vector definition
 */
template<typename K>
struct key {
    double operator()(const K &x) const;
};

/**
 * \brief This class represents a generic timeseries template.
 *
 * Keys and values can be chosen at will, where a key will usually be a date based entity,
 * while a value will be something else. Example:
 * \code
 * tuple_vector<time_t, double> ts;
 * \endcode
 * \author Sinisa Susnjar
 */

/*
template<typename K = time_t>
struct key {
    double operator()(const K &x) const { return x; }
};
*/

// declare a custom container based on a vector containing pair<K,V> tuples
template<typename K, typename V>
class tuple_vector : public vector<pair<K,V>> {
static constexpr bool debug = false;
public:
	typedef pair<K,V>											value_type;
	typedef typename vector<value_type>::iterator				iterator;
	typedef typename vector<value_type>::const_iterator			const_iterator;
	typedef typename vector<value_type>::size_type				size_type;

	tuple_vector() { }

	tuple_vector(size_type n) : vector<value_type>(n) { }

	~tuple_vector() { }

	/*
	inline void fast_add(const K &key, const V &value)
	{
		vector<value_type>::emplace_back(make_pair(key, value));
	}

	inline void add(const K &key, const V &value)
	{
		vector<value_type>::emplace_back(make_pair(key, value));
	}

	inline void add(const std::pair<K, V> &p)
	{
		vector<value_type>::emplace_back(p);
	}
	*/

	inline iterator at(const K &key)
	{
		auto pos = find(key);
		if (pos == vector<value_type>::end())
			throw std::out_of_range(__func__);
		return pos;
	}

	inline const_iterator at(const K &key) const
	{
		auto pos = find(key);
		if (pos == vector<value_type>::end())
			throw std::out_of_range(__func__);
		return pos;
	}

	/*
	inline const value_type &last() const {
		return *vector<value_type>::crbegin();
	}

	inline const value_type &first() const {
		return *vector<value_type>::cbegin();
	}
	*/

	/**
	 * \brief Gets the last key/value pair from the timeseries.
	 * \return Mutable reference to the last key/value pair of the timeseries.
	 */
	/*
	inline value_type &last() {
		return *vector<value_type>::rbegin();
	}

	inline value_type &first() {
		return *vector<value_type>::begin();
	}
	*/

	/*
	// const std::pair<K, V> &get(unsigned idx) const { assert(idx < m_vec->size()); return m_vec[idx]; }
	inline const value_type &get(unsigned idx) const {
		return at(idx);
	}
	inline value_type &get(unsigned idx) {
		return at(idx);
	}
	*/

	/**
	 * \brief Const array access operator.
	 * \param idx Index into the timeseries data.
	 * \return Const reference to the appropriate timeseries value.
	 */
	// const V &operator[](unsigned idx) const { assert(idx < m_vec->size()); return m_vec[idx].second; }
	/*
	inline const V &operator[](unsigned idx) const {
		return vector<value_type>::operator [](idx).second;
	}
	*/

	/**
	 * \brief Mutable array access operator.
	 * \param idx Index into the timeseries data.
	 * \return Mutable reference to the appropriate timeseries value.
	 */
	// V &operator[](unsigned idx)	{ assert(idx < m_vec->size()); return m_vec[idx].second; }
	/*
	inline V &operator[](unsigned idx)	{
		return vector<value_type>::operator [](idx).second;
	}
	*/

	/**
	 * \brief Gets a value given a key.
	 * \param key The key whose value should be found.
	 * \return Const reference to the appropriate timeseries value.
	 * \todo Do we really need an assert when accessing the data via the key?!?
	 */
	/*
	inline const V &get(const K &key) const {
		auto pos = lower_bound(key);
		assert(pos != vector<value_type>::cend());
		return pos->second;
	}

	inline V &get(const K &key) {
		auto pos = lower_bound(key);
		assert(pos != vector<value_type>::end());
		return pos->second;
	}
	*/

	/*
	inline const_iterator get_iter(const K &key) const {
		const_iterator pos = lower_bound(key);
		assert(pos != vector<value_type>::cend());
		return pos;
	}

	inline iterator get_iter(const K &key) {
		iterator pos = lower_bound(key);
		assert(pos != vector<value_type>::end());
		return pos;
	}
	*/

	/* is this still needed?
	size_t erase(const K &key, size_t idx) {
		assert(idx < m_vec->size());
		m_vec->erase(m_vec->begin() + idx);
		return m_map->erase(key);
	}
	*/

	/*
	inline void set(const K &key, const V &val) {
		auto pos = lower_bound(key);
		assert(pos != vector<value_type>::end());
		pos->second = val;
	}

	inline void set(size_t idx, const V &val) {
		vector<value_type>::operator [](idx) = val;
	}
	*/

	/**
	 * \brief Const array access operator.
	 * \param key The key whose value should be found.
	 * \return Const reference to the appropriate timeseries value.
	 */
	inline const V &operator[](const K &key) const { return at(key)->second; }

	/**
	 * \brief Mutable array access operator.
	 * \param key The key whose value should be found.
	 * \return Mutable reference to the appropriate timeseries value.
	 */
	inline V &operator[](const K &key)	{ return at(key)->second; }

	/**
	 * \brief Get all values from this timeseries.
	 * \retval v Vector that will contain all values from this timeseries.
	 */
	/*
	void values(std::vector<V> &v) const {
		v.clear();
		v.reserve(vector<value_type>::size());
		for (auto &i : *this) {
			v.emplace_back(i.second);
		}
	}
	*/

	/**
	 * \brief Comparison operator.
	 * \param op The timeseries which should be compared.
	 * \return True if the size() < op.size(), false otherwise.
	 */
	/*
	inline bool operator<(const tuple_vector<K, V> &op) const
	{
		return vector<value_type>::size() < op.vector<value_type>::size();
	}

	tuple_vector<K, V> &operator=(const tuple_vector<K, V> &op) {
		if (this != &op) {
			vector<value_type>::operator =(op);
		}
		return *this;
	}
	*/

	/*
	friend std::ostream &operator<< (std::ostream &os, const tuple_vector<K, V> &t) {
		auto pos = vector<value_type>::crbegin();
		os << "timeseries[" << std::hex << &t << std::dec << "] = {" << std::endl
			<< "  size = " << t.size() << std::endl
			<< "  last = " << pos->second << " @ " << pos->first << std::endl
			<< "}" << std::endl;
		return os;
	}
	*/


	/**
	 * \brief Checks the existence of a given key.
	 * \param key The key to be checked.
	 * \return True or false, depending if the key has been found or not.
	 */
	/*
	inline bool exists(const K &key) const
	{
		if (debug) std::cout << "tuple_vector::exists('" << key << "')" << std::endl;
		const_iterator pos = lower_bound(key);
		// typename std::map<datetime, double>::const_iterator pos = m_map->find(key);
		if (pos == vector<value_type>::cend()) {
			if (debug) std::cout << "pos == m_map.end() !!!" << std::endl;
			return false;
		}
		if (pos->first == key) {
			if (debug) std::cout << "pos->first == key !!!" << std::endl;
			return true;
		}
		return false;
	}
	*/


	// insert element at end
	inline void emplace_back(const pair<K,V> &&p)
	{
		vector<value_type>::emplace_back(p);
		m_recompute_range = true;
	}
	inline void emplace_back(const pair<K,V> &p)
	{
		vector<value_type>::emplace_back(p);
		m_recompute_range = true;
	}
	size_type erase(const K &key)
	{
		auto pos = find(key);
		m_recompute_range = true;
		vector<value_type>::erase(pos);
		return 1;
	}
	iterator erase(const_iterator pos)
	{
		m_recompute_range = true;
		return vector<value_type>::erase(pos);
	}
	iterator erase(const_iterator start, const_iterator end)
	{
		m_recompute_range = true;
		return vector<value_type>::erase(start, end);
	}

	inline iterator lower_bound(const K &key)
	{
		return iterator(const_cast<value_type *>(_lower_bound(key)));
	}

	inline const_iterator lower_bound(const K &key) const
	{
		return const_iterator(_lower_bound(key));
	}	// lower_bound()

	// reset housekeeping variables
	void reset()
	{
		m_recompute_range = true;
		m_hits = 0;
		m_outofbound = 0;
		m_recompute = 0;
		m_resync = 0;
		m_avg_diff = 0;
	}

	inline iterator find(const K &key)
	{
		return iterator(const_cast<value_type *>(_find(key)));
	}

	inline const_iterator find(const K &key) const
	{
		return const_iterator(_find(key));
	}

	/*
	iterator find(const K &key)
	{
		value_type *rc = nullptr;

		if (m_recompute_range) {
			m_size = vector<value_type>::size();
			m_total_range = key<>()(vector<value_type>::back().first) - key<>()(vector<value_type>::front().first);
			m_element_range = m_total_range / (m_size-1);
			m_offset = 0;
			m_recompute_range = false;
			++m_recompute;
		}

		size_t idx = (key<>()(key) - (key<>()(vector<value_type>::front().first)) / m_element_range + m_offset;

		if (idx >= 0 && idx < m_size) {
			// cout << "key could be at index position " << idx << " (offset " << offset << ")" << endl;
			rc = &this->operator[](idx);
		} else {
			// idx is out of bounds, start from the beginning
			// rc = &this->operator[](0);
			if (idx > m_size)
				rc = &vector<value_type>::back();
			else
				rc = &vector<value_type>::front();
			m_offset = 0;
			++m_outofbound;
		}
		if (rc->first == key) {
			++m_hits;
			return iterator(rc);
		}

		// offset needs re-synch
		int old_offset = m_offset;
		if (rc->first > key)
			for (; rc->first > key; --rc, --m_offset);
		else
			for (; rc->first < key; ++rc, ++m_offset);
		m_resync += abs(m_offset - old_offset);

		// cout << key << ": off by: " << (m_offset-old_offset) << endl;

		if (rc->first == key) {
			return iterator(rc);
		}

		return vector<value_type>::end();
	}	// find()
	*/

	int resync() const { return m_resync; }
	int recompute() const { return m_recompute; }
	int outofbound() const { return m_outofbound; }
	int hits() const { return m_hits; }
	double avg_diff() const { return m_avg_diff; }

protected:
	inline const value_type *_find(const K &key) const
	{
		const value_type *rc = nullptr;

		if (m_recompute_range) {
			m_size = vector<value_type>::size();
			m_total_range = ::key<K>()(vector<value_type>::back().first) - ::key<K>()(vector<value_type>::front().first);
			m_element_range = m_total_range / (m_size-1);
			m_offset = 0;
			m_recompute_range = false;
			m_front = &vector<value_type>::front();
			m_back = &vector<value_type>::back();
			++m_recompute;
		}

		if (m_size == 0)
			return vector<value_type>::_M_impl._M_finish;

		size_t idx = (::key<K>()(key) - ::key<K>()(vector<value_type>::front().first)) / m_element_range + m_offset;

		if (idx >= 0 && idx < m_size) {
			rc = &vector<value_type>::operator[](idx);
		} else {
			if (idx > m_size)
				rc = &vector<value_type>::back();
			else
				rc = &vector<value_type>::front();
			m_offset = 0;
			++m_outofbound;
		}
		if (rc->first == key) {
			++m_hits;
			return rc;
		}
		const value_type *old_rc = rc;
		if (rc->first > key)
			for (; rc->first > key && rc > m_front; --rc);
		else
			for (; rc->first < key && rc < m_back; ++rc);
		m_avg_diff *= m_resync;
		m_avg_diff += rc - old_rc;
		m_offset += rc - old_rc;
		++m_resync;
		m_avg_diff /= m_resync;
		m_offset = (double)m_offset + m_avg_diff + .5;

		if (rc->first == key)
			return rc;

		return vector<value_type>::_M_impl._M_finish;
	}	// _find()

	inline const value_type *_lower_bound(const K &key) const
	{
		const value_type *rc = nullptr;

		if (m_recompute_range) {
			m_size = vector<value_type>::size();
			m_total_range = ::key<K>()(vector<value_type>::back().first) - ::key<K>()(vector<value_type>::front().first);
			m_element_range = m_total_range / (m_size-1);
			m_offset = 0;
			m_recompute_range = false;
			m_front = &vector<value_type>::front();
			m_back = &vector<value_type>::back();
			++m_recompute;
		}

		if (m_size == 0)
			return vector<value_type>::_M_impl._M_finish;

		size_t idx = (::key<K>()(key) - ::key<K>()(vector<value_type>::front().first)) / m_element_range + m_offset;

		if (idx >= 0 && idx < m_size) {
			rc = &vector<value_type>::operator[](idx);
		} else {
			if (idx > m_size)
				rc = &vector<value_type>::back();
			else
				rc = &vector<value_type>::front();
			m_offset = 0;
			++m_outofbound;
		}
		/*
		if (rc->first >= key) {
			++m_hits;
			// return iterator(rc);
			return rc;
		}
		*/
		const value_type *old_rc = rc;
		if (rc->first > key) {
			/*
			if (rc->first > key) {
				cout << "rc->first > key" << endl;
			} else {
				cout << "! rc->first > key" << endl;
			}
			*/
			/*
			for (; rc->first > key && rc > m_front; --rc) {
				// if (debug) rc->first.dump(cout);
			}
			*/
			/*
			if (rc == m_front && rc->first < key)
				return vector<value_type>::_M_impl._M_finish;
				*/
			if (rc->first < key)
				++rc;
		} else {
			/*
			if (rc->first < key) {
				cout << "rc->first < key" << endl;
			} else {
				cout << "! rc->first < key" << endl;
			}
			*/
			/*
			for (; rc->first < key && rc < m_back; ++rc) {
				// if (debug) rc->first.dump(cout);
			}
			*/
			/*
			if (rc == m_back && rc->first < key)
				return vector<value_type>::_M_impl._M_finish;
				*/
			if (rc->first < key)
				--rc;
		}
		m_avg_diff *= m_resync;
		m_avg_diff += rc - old_rc;
		m_offset += rc - old_rc;
		++m_resync;
		m_avg_diff /= m_resync;
		m_offset = (double)m_offset + m_avg_diff + .5;

		if (rc->first >= key)
			return rc;
			// return iterator(rc);
		if (debug) cout << "end!" << endl;

		return vector<value_type>::_M_impl._M_finish;
		// return vector<value_type>::end();
	}	// _lower_bound()

private:
	mutable size_t m_size = 0;
	mutable double m_total_range = 0;
	mutable double m_element_range = 0;
	mutable int m_offset = 0;
	mutable bool m_recompute_range = false;
	mutable int m_resync = 0;
	mutable int m_recompute = 0;
	mutable int m_outofbound = 0;
	mutable int m_hits = 0;
	mutable const value_type *m_last = nullptr;
	mutable const value_type *m_front = nullptr;
	mutable const value_type *m_back = nullptr;
	mutable double m_avg_diff = 0;
};

#endif /* __tuple_vector_h */
