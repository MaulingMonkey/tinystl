/*-
 * Copyright 2012 Matthew Endsley
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TINYSTL_STRING_H
#define TINYSTL_STRING_H

#include <TINYSTL/stddef.h>

namespace tinystl {

	class string
	{
	public:
		string();
		string(const string& other);
		string(const char* sz);
		string(const char* sz, size_t len);
		~string();

		string& operator=(const string& other);

		const char* c_str() const;
		size_t size() const;

		void reserve(size_t size);
		void resize(size_t size);

		void append(const char* first, const char* last);

		void swap(string& other);

	private:
		typedef char* pointer;
		pointer m_first;
		pointer m_last;
		pointer m_capacity;
	};

	inline string::string()
		: m_first(0)
		, m_last(0)
		, m_capacity(0)
	{
		resize(0);
	}

	inline string::string(const string& other)
		: m_first(0)
		, m_last(0)
		, m_capacity(0)
	{
		reserve(other.size());
		append(other.m_first, other.m_last);
	}

	inline string::string(const char* sz)
		: m_first(0)
		, m_last(0)
		, m_capacity(0)
	{
		size_t len = 0;
		for (const char* it = sz; *it; ++it)
			++len;

		reserve(len);
		append(sz, sz + len);
	}

	inline string::string(const char* sz, size_t len)
		: m_first(0)
		, m_last(0)
		, m_capacity(0)
	{
		reserve(len);
		append(sz, sz + len);
	}

	inline string::~string()
	{
		delete[] m_first;
	}

	inline string& string::operator=(const string& other)
	{
		string(other).swap(*this);
		return *this;
	}

	inline const char* string::c_str() const
	{
		return m_first;
	}

	inline size_t string::size() const
	{
		return (size_t)(m_last - m_first);
	}

	inline void string::reserve(size_t capacity)
	{
		if (m_first + capacity + 1 <= m_capacity)
			return;

		const size_t size = (size_t)(m_last - m_first);
		const size_t current = (size_t)(m_capacity - m_first);

		pointer newfirst = new char[capacity + 1];
		for (pointer it = m_first, newit = newfirst, end = m_last; it != end; ++it, ++newit)
			*newit = *it;
		delete[] m_first;

		m_first = newfirst;
		m_last = newfirst + size;
		m_capacity = m_first + capacity;
	}

	inline void string::resize(size_t size)
	{
		reserve(size);
		for (pointer it = m_last, end = m_first + size + 1; it < end; ++it)
			*it = 0;

		m_last += size;
	}

	inline void string::append(const char* first, const char* last)
	{
		const size_t newsize = (size_t)((m_last - m_first) + (last - first) + 1);
		if (m_first + newsize > m_capacity)
			reserve((newsize * 3) / 2);

		for (; first != last; ++m_last, ++first)
			*m_last = *first;
		*m_last = 0;
	}

	inline void string::swap(string& other)
	{
		const pointer tfirst = m_first, tlast = m_last, tcapacity = m_capacity;
		m_first = other.m_first, m_last = other.m_last, m_capacity = other.m_capacity;
		other.m_first = tfirst, other.m_last = tlast, other.m_capacity = tcapacity;
	}

	inline bool operator==(const string& lhs, const string& rhs)
	{
		typedef const char* pointer;

		const size_t lsize = lhs.size(), rsize = rhs.size();
		if (lsize != rsize)
			return false;

		pointer lit = lhs.c_str(), rit = rhs.c_str();
		pointer lend = lit + lsize;
		while (lit != lend)
		{
			if (*lit++ != *rit++)
				return false;
		}

		return true;
	}
}

#endif
