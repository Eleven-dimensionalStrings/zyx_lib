#pragma once
#include <initializer_list>
#include <utility>
#include <exception>

template<class T>
class my_ptr
{
public:
	int* count;
	T* manage;
	operator bool()const;
	my_ptr();
	my_ptr(T*);
	my_ptr(my_ptr&);
	my_ptr(my_ptr&&);
	virtual ~my_ptr();
	my_ptr<T>& operator=(my_ptr<T>&);
	my_ptr<T>& operator=(my_ptr<T>&&);
	T& operator*();
	T* operator->();
	T* get();
	T& operator[](size_t);
};


template<class T>
class shared_array
{
public:
	shared_array();
	shared_array(std::initializer_list<T>li);
	shared_array(int count);
	shared_array(int count, int val);
	shared_array(shared_array&t);
	shared_array(shared_array&&t);
	T& operator[](size_t k);
	size_t size();
	T* begin();
	T* end();
	shared_array& operator=(shared_array& t);
	shared_array& operator=(shared_array&& t);
private:
	my_ptr<T>data;
	size_t _size;
};




template<class T>
my_ptr<T>::operator bool() const
{
	return manage;
}

template<class T>
inline my_ptr<T>::my_ptr() : count(0), manage(nullptr)
{
}


template<class T>
my_ptr<T>& my_ptr<T>::operator=(my_ptr<T>& t)
{
	if (t.count)
		++(*(t.count));
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
	return *this;
}

template<class T>
inline my_ptr<T>& my_ptr<T>::operator=(my_ptr<T>&& t)
{
	if (t.count)
		++(*(t.count));
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
my_ptr<T>::my_ptr(T* t) :count(new int(1)), manage(t)
{

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
inline T & my_ptr<T>::operator[](size_t s)
{
	return manage[s];
}


template<class T>
shared_array<T>::shared_array() :data(), _size(0)
{
}
template<class T>
shared_array<T>::shared_array(std::initializer_list<T> li) : data(new int[li.size()]),
_size(li.size())
{
	for (size_t i = 0; i < li.size(); ++i)
	{
		data[i] = *(li.begin() + i);
	}
}
template<class T>
shared_array<T>::shared_array(int count) :data(new T[count]), _size(count)
{
}
template<class T>
shared_array<T>::shared_array(int count, int val) : shared_array(count)
{
	for (int i = 0; i < count; i++)
	{
		data[i] = count;
	}
}
template<class T>
shared_array<T>::shared_array(shared_array & t) :data(t.data), _size(t.size())
{
	data = t.data;
	_size = t._size;
}
template<class T>
inline shared_array<T>::shared_array(shared_array<T> && t) :data(std::move(t.data)), _size(t.size())
{
}
template<class T>
T& shared_array<T>::operator[](size_t k)
{
	return data[k];
}
template<class T>
size_t shared_array<T>::size()
{
	return _size;
}
template<class T>
T * shared_array<T>::begin()
{
	return data.get();
}
template<class T>
T *shared_array<T>::end()
{
	return data.get() + _size;
}
template<class T>
inline shared_array<T> & shared_array<T>::operator=(shared_array<T> & t)
{
	data = t.data;
	_size = t._size;
	return *this;
}
template<class T>
inline shared_array<T> & shared_array<T>::operator=(shared_array<T> && t)
{
	data = std::move(t.data);
	_size = t._size;
	return *this;
}



