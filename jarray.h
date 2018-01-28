#pragma once
#include <initializer_list>
#include <utility>
#include <exception>
namespace LemonJavaArray
{
	//用于数组
	template<class T = int>
	class my_ptr
	{
	private:
		int* count;
		T* manage;
	public:
		operator bool()const;
		my_ptr();
		my_ptr(T* t);
		my_ptr(my_ptr&t);
		my_ptr(my_ptr&& t);
		~my_ptr();
		my_ptr<T>& operator=(my_ptr<T>&t);
		my_ptr<T>& operator=(my_ptr<T>&&t);
		T& operator*();
		T* operator->();
		T* get();
	};

	template<class T>
	class jarray1
	{
	public:
		jarray1();
		jarray1(std::initializer_list<T>li);
		jarray1(int ts);
		jarray1(int ts1, int ini);//ini为0则不初始化,否则用memset初始化为0
		jarray1(jarray1&t);
		jarray1(jarray1&&t);
		T& operator[](unsigned int k);
		unsigned int size();
		T* begin();//懒得写迭代器,反正没什么必要
		const T* const end();
		jarray1& operator=(jarray1& t);
		jarray1& operator=(jarray1&& t);
	private:
		my_ptr<T>data;
		unsigned int _size;
	};




	template<class T>
	class jarray2
	{
	public:
		class count
		{
			friend class jarray2;
			unsigned int s2, k1, s1;
			int* manage;
		public:
			count(int ts1, int ts2);
			~count();
			T& operator[](unsigned int k);
			unsigned int size();
		};
		jarray2();
		jarray2(unsigned int ts1, unsigned int ts2);
		jarray2(unsigned int ts1, unsigned int ts2, int ini);
		jarray2(jarray2& t);
		jarray2(jarray2&& t);
		count& operator[](unsigned int k);
		jarray2 operator=(jarray2& t);
		jarray2 operator=(jarray2&& t);
		unsigned int size();

		/*
		只用来完成
		jarray2 a(xxx,xxx);
		....
		a=0;
		这种操作
		*/
		jarray2(int t);
	private:
		my_ptr<count>data;
		int s1;
	};



	template<class T>
	LemonJavaArray::jarray2<T>::jarray2() :data(), s1(0)
	{
	}

	template<class T>
	LemonJavaArray::jarray2<T>::jarray2(unsigned int ts1, unsigned int ts2) : data(new count(ts1, ts2)), s1(ts1)
	{
	}

	template<class T>
	LemonJavaArray::jarray2<T>::jarray2(unsigned int ts1, unsigned int ts2, int ini) : jarray2(ts1, ts2)
	{
		if (ini)
			memset(data->manage, 0, ts1*ts2 * 4);
	}

	template<class T>
	LemonJavaArray::jarray2<T>::jarray2(jarray2 & t)
	{
		data = t.data;
		s1 = t.s1;
		if (t.data)
		{
			data->s1 = t.data->s1;
			data->s2 = t.data->s2;
		}
	}

	template<class T>
	LemonJavaArray::jarray2<T>::jarray2(jarray2 && t) :data(std::move(t.data)), s1(t.s1)
	{

	}

	template<class T>
	jarray2<T> LemonJavaArray::jarray2<T>::operator=(jarray2 & t)
	{
		data = t.data;
		s1 = t.s1;
		if (t.data)
		{
			data->s1 = t.data->s1;
			data->s2 = t.data->s2;
		}
		return *this;
	}

	template<class T>
	jarray2<T> jarray2<T>::operator=(jarray2<T> && t)
	{
		data = std::move(t.data);
		s1 = t.s1;
		return *this;
	}

	template<class T>
	unsigned int LemonJavaArray::jarray2<T>::size()
	{
		return s1;
	}

	template<class T>
	jarray2<T>::jarray2(int t) :data(), s1(0)
	{
		if (!t)
			throw std::exception("jarray2 obj can only use = 0 to clear. obj = x, which x != 0 is wrong.");
	}

	template<class T>
	typename jarray2<T>::count& LemonJavaArray::jarray2<T>::operator[](unsigned int k)
	{
		(*data).k1 = k;
		return (*data);
	}
	template<class T>
	LemonJavaArray::jarray2<T>::count::count(int ts1, int ts2) :s2(ts2), k1(0), s1(ts1), manage(new int[ts2*ts1])
	{
	}
	template<class T>
	LemonJavaArray::jarray2<T>::count::~count()
	{
		delete[] manage;
	}
	template<class T>
	T& jarray2<T>::count::operator[](unsigned int k)
	{
		return manage[k1*s2 + k];
	}
	template<class T>
	unsigned int jarray2<T>::count::size()
	{
		return s2;
	}



	template<class T>
	my_ptr<T>::operator bool() const
	{
		return count;
	}

	template<class T>
	inline my_ptr<T>::my_ptr() : count(0), manage(nullptr)
	{
	}

	template<class T>
	my_ptr<T>& my_ptr<T>::operator=(my_ptr<T>& t)
	{
		if (count)
		{
			--(*count);
			if (*count == 0)
			{
				delete manage;
				delete count;
			}
		}
		manage = t.manage;
		count = t.count;
		if (count)
			++(*count);
		return *this;
	}

	template<class T>
	inline my_ptr<T>& my_ptr<T>::operator=(my_ptr<T>&& t)
	{

		if (count)
		{
			--(*count);
			if (*count == 0)
			{
				delete manage;
				delete count;
			}
		}
		manage = t.manage;
		count = t.count;
		t.count = 0;
		t.manage = nullptr;
		return *this;
	}

	template<class T>
	my_ptr<T>::my_ptr(my_ptr<T>&t) : count(t.count), manage(t.manage)
	{
		if (count)
			++(*count);
	}

	template<class T>
	my_ptr<T>::my_ptr(my_ptr<T> && t) : count(t.count), manage(t.manage)
	{
		t.count = 0;
		t.manage = nullptr;
	}

	template<class T>
	my_ptr<T>::my_ptr(T* t)
	{
		manage = t;
		count = new T(1);
	}

	template<class T>
	my_ptr<T>::~my_ptr()
	{
		if (count)
		{
			--(*count);
			if (*count == 0)
			{
				delete[] manage;
				delete count;
			}
		}
	}

	template<class T>
	T& my_ptr<T>::operator*()
	{
		return *manage;
	}
	template<class T>
	T* my_ptr<T>::operator->()
	{
		return manage;
	}
	template<class T>
	T * my_ptr<T>::get()
	{
		return manage;
	}
	template<class T>
	jarray1<T>::jarray1() :data(), _size(0)
	{
	}
	template<class T>
	jarray1<T>::jarray1(std::initializer_list<T> li) : data(new int[li.size()]),
		_size(li.size())
	{
		for (unsigned int i = 0; i < li.size(); ++i)
		{
			*(data.get() + i) = *(li.begin() + i);
		}
	}
	template<class T>
	jarray1<T>::jarray1(int ts) :data(new T[ts]), _size(ts)
	{
	}
	template<class T>
	jarray1<T>::jarray1(int ts1, int ini) : jarray1(ts1)
	{
		if (ini)
			memset(&*data, 0, ts1 * sizeof(T));
	}
	template<class T>
	jarray1<T>::jarray1(jarray1 & t)
	{
		data = t.data;
		_size = t._size;
	}
	template<class T>
	inline jarray1<T>::jarray1(jarray1<T> && t) :data(std::move(t.data)), _size(t._size)
	{
	}
	template<class T>
	T& jarray1<T>::operator[](unsigned int k)
	{
		return *(&*data + k);
	}
	template<class T>
	unsigned int jarray1<T>::size()
	{
		return _size;
	}
	template<class T>
	T * jarray1<T>::begin()
	{
		return &*data;
	}
	template<class T>
	const T * const jarray1<T>::end()
	{
		return &*data + _size;
	}
	template<class T>
	inline jarray1<T> & jarray1<T>::operator=(jarray1<T> & t)
	{
		data = t.data;
		_size = t._size;
		return *this;
	}
	template<class T>
	inline jarray1<T> & jarray1<T>::operator=(jarray1 && t)
	{
		data = std::move(t.data);
		_size = t._size;
		return *this;
	}
}