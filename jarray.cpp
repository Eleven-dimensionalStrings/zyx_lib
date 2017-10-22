#include "jarray.h"

namespace LemonJavaArray
{
	LemonJavaArray::jarray2::jarray2() :data()
	{
	}

	LemonJavaArray::jarray2::jarray2(int ts1, int ts2)
	{
		c* k = new c(ts1, ts2);
		s1 = ts1;
		data = my_ptr<c>(k);
	}

	LemonJavaArray::jarray2::jarray2(int ts1, int ts2, int ini) :jarray2(ts1, ts2)
	{
		if (ini)
			memset(data->v, 0, ts1*ts2 * 4);
	}

	LemonJavaArray::jarray2::jarray2(jarray2 & t)
	{
		data = t.data;
		s1 = t.s1;
		data->s1 = t.data->s1;
		data->s2 = t.data->s2;
	}

	LemonJavaArray::jarray2::jarray2(jarray2 && t) :data(t.data), s1(t.s1)
	{
		data->s1 = t.data->s1;
		data->s2 = t.data->s2;
	}

	jarray2 LemonJavaArray::jarray2::operator=(jarray2 & t)
	{
		data = t.data;
		s1 = t.s1;
		data->s1 = t.data->s1;
		data->s2 = t.data->s2;
		return *this;
	}

	jarray2 jarray2::operator=(jarray2 && t)
	{
		data = t.data;
		s1 = t.s1;
		data->s1 = t.data->s1;
		data->s2 = t.data->s2;
		return *this;
	}

	int LemonJavaArray::jarray2::size()
	{
		return s1;
	}

	jarray2::c& LemonJavaArray::jarray2::operator[](int k)
	{
		(*data).k1 = k;
		return (*data);
	}
	LemonJavaArray::jarray2::c::c(int ts1, int ts2) :s2(ts2), k1(0), s1(ts1), v(new int[ts2*ts1])
	{
	}
	LemonJavaArray::jarray2::c::~c()
	{
		delete[] v;
	}
	int& jarray2::c::operator[](int k)
	{
		return v[k1*s2 + k];
	}
	int jarray2::c::size()
	{
		return s2;
	}



	template<class T>
	inline my_ptr<T>::my_ptr() :flag(0), c(nullptr), v(nullptr)
	{
	}

	template<class T>
	my_ptr<T>& my_ptr<T>::operator=(my_ptr<T>& t)
	{

		if (!flag)
			flag = 1;
		else
		{
			//std::cout << (*c) << std::endl;
			--(*c);
			if (*c == 0)
			{
				delete v;
				delete c;
			}
		}
		v = t.v;
		c = t.c;
		++(*c);
		return *this;
	}

	template<class T>
	my_ptr<T>::my_ptr(my_ptr<T>&t) :flag(1), c(t.c), v(t.v)
	{
		++(*c);
	}

	template<class T>
	my_ptr<T>::my_ptr(my_ptr<T> && t) :flag(1), c(t.c), v(t.v)
	{
		++(*c);
	}

	template<class T>
	my_ptr<T>::my_ptr(T* t)
	{
		flag = 1;
		v = t;
		c = new int(1);
	}

	template<class T>
	my_ptr<T>::~my_ptr()
	{
		if (flag)
		{
			--(*c);
			if (*c == 0)
			{
				delete v;
				delete c;
			}
		}
	}

	template<class T>
	T& my_ptr<T>::operator*()
	{
		return *v;
	}
	template<class T>
	T* my_ptr<T>::operator->()
	{
		return v;
	}
	template<class T>
	T * my_ptr<T>::get()
	{
		return v;
	}
	jarray1::jarray1()
	{
	}
	jarray1::jarray1(std::initializer_list<int> li)
	{
		s = static_cast<int>(li.size());
		data = my_ptr<int>(new int[li.size()]);
		for (int i = 0; i < int(li.size()); ++i)
		{
			*(data.get() + i) = *(li.begin() + i);
		}
	}
	jarray1::jarray1(int ts)
	{
		s = ts;
		data = my_ptr<int>(new int[ts]);
	}
	jarray1::jarray1(int ts1, int ini) :jarray1(ts1)
	{
		if (ini)
			memset(&*data, 0, ts1 * 4);
	}
	jarray1::jarray1(jarray1 & t)
	{
		data = t.data;
		s = t.s;
	}
	int& jarray1::operator[](int k)
	{
		return *(&*data + k);
	}
	int jarray1::size()
	{
		return s;
	}
	int * jarray1::begin()
	{
		return &*data;
	}
	const int * const jarray1::end()
	{
		return &*data + s;
	}
}