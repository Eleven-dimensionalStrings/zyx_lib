#pragma once
#include <memory>

class jarray1
{
public:
	jarray1()
	{
	}
	jarray1(std::initializer_list<int>li)
	{
		s = li.size();
		data = std::shared_ptr<int>(new int[li.size()], [](int*p) { delete[]p; });
		for (int i = 0; i < int(li.size()); ++i)
		{
			*(data.get() + i) = *(li.begin() + i);
		}
	}
	jarray1(int ts)
	{
		s = ts;
		data = std::shared_ptr<int>(new int[ts], [](int*p) { delete[]p; });
	}
	jarray1(int ts1, int ini) :jarray1(ts1)
	{
		memset(&*data, 0, ts1 * 4);
	}
	jarray1(jarray1&t)
	{
		data = t.data;
		s = t.s;
	}
	int& jarray1::operator[](int k);
	int size()
	{
		return s;
	}
	int* begin()
	{
		return &*data;
	}
	const int* const end()
	{
		return &*data + s;
	}
private:
	std::shared_ptr<int>data;
	int s;
};
int& jarray1::operator[](int k)
{
	return *(&*data + k);
}

class jarray2
{
public:
	jarray2()
	{
	}
	jarray2(int ts1, int ts2)
	{
		c* k = new c(ts1, ts2);
		s1 = ts1;
		data = std::shared_ptr<c>(k);
	}
	jarray2(int ts1, int ts2, int ini) :jarray2(ts1, ts2)
	{
		memset(data->v, 0, ts1*ts2 * 4);
	}
	jarray2(jarray2&t)
	{
		data = t.data;
		s1 = t.s1;
		data->s1 = t.data->s1;
		data->s2 = t.data->s2;
	}
	class c;
	c& jarray2::operator[](int k)
	{
		(*data).k1 = k;
		return (*data);
	}
	int size()
	{
		return s1;
	}
	class c
	{
		friend class jarray2;
		int s2, k1, s1;
		int* v;
	public:
		c(int ts1, int ts2)
		{
			v = new int[ts1*ts2];
			s1 = ts1;
			s2 = ts2;
			k1 = 0;
		}
		~c()
		{
			delete[]v;
		}
		int& c::operator[](int k)
		{
			return v[k1*s2 + k];
		}
		int size()
		{
			return s2;
		}
	};
private:
	std::shared_ptr<c>data;
	int s1;
};



class jarray3
{
public:
	class a
	{
	public:
		a()
		{
		}
		~a()
		{
			delete[] v;
		}
		int s;
		int * v;
		int index;
		int size()
		{
			return s;
		}
		int& a::operator[](int k)
		{
			return v[index + k];
		}
	};
	class b
	{
	public:
		b()
		{
			aa = new a();
		}
		~b()
		{
			delete aa;
		}
		int s;
		a* aa;
		int size()
		{
			return s;
		}
		a& b::operator[](int k)
		{
			aa->index += k*aa->s;
			return *aa;
		}
	};
	jarray3()
	{
	}
	jarray3(int ts1, int ts2, int ts3)
	{
		data = std::shared_ptr<b>(new b());
		s = ts1;
		data->s = ts2;
		data->aa->s = ts3;
		data->aa->v = new int[ts1*ts2*ts3];
	}
	jarray3(int ts1, int ts2, int ts3, int ini) :jarray3(ts1, ts2, ts3)
	{
		memset(data->aa->v, 0, ts1*ts2*ts3 * 4);
	}
	jarray3(jarray3&t)
	{
		data = t.data;
		s = t.s;
		data->s = t.data->s;
		data->aa->s = t.data->aa->s;
	}
	b& jarray3::operator[](int k)
	{
		data->aa->index = k*(data->aa->s)*(data->s);
		return (*data);
	}
	int size()
	{
		return s;
	}
	std::shared_ptr<b>data;
	int s;
};

void ___SimpleJarrayDeleter(int** p)
{
	delete[](*p);
	delete p;
}
class jarrayn
{
public:
	jarrayn()
	{
		/*
		int* a = new int[1];
		int* b = new int[1];
		int **c = new int*(a);
		int ** d = new int*(b);
		p = std::shared_ptr<int*>(c, ___SimpleJarrayDeleter);
		s = std::shared_ptr<int*>(d, ___SimpleJarrayDeleter);
		*/
		d = 0;
	}
	jarrayn(int ts)
	{
		d = 1;
		int* a = new int[ts];
		int* b = new int[1];
		int **c = new int*(a);
		int ** d = new int*(b);
		p = std::shared_ptr<int*>(c, ___SimpleJarrayDeleter);
		s = std::shared_ptr<int*>(d, ___SimpleJarrayDeleter);
		(*s)[0] = ts;
	}
	jarrayn(int ts, int in)
	{
		d = 1;
		int* a = new int[ts];
		int* b = new int[1];
		int **c = new int*(a);
		int ** d = new int*(b);
		p = std::shared_ptr<int*>(c, ___SimpleJarrayDeleter);
		s = std::shared_ptr<int*>(d, ___SimpleJarrayDeleter);
		(*s)[0] = ts;
		if (in)
			memset(*p, 0, (*s)[0] * 4);
	}
	jarrayn(std::initializer_list<int>li)
	{
		int size = 1;
		d = li.size() - 1;
		int * a = new int[d];
		int **c = new int*(a);
		s = std::shared_ptr<int*>(c, ___SimpleJarrayDeleter);
		for (int i = 0; i < d; ++i)
		{
			size *= *(li.begin() + i);
			(*s)[i] = *(li.begin() + i);
		}
		int * b = new int[size];
		int ** d = new int*(b);
		p = std::shared_ptr<int*>(d, ___SimpleJarrayDeleter);
		if (*(li.end() - 1))
			memset(&(*p)[0], 0, size * 4);
	}
	jarrayn(jarrayn& t)
	{
		d = t.d;
		p = t.p;
		s = t.s;
	}
	int* get()
	{
		return *p;
	}
	int& get(int index)
	{
		return (*p)[index];
	}
	int& get(std::initializer_list<int>in)
	{
		int index = 0;
		index += *(in.begin());
		for (int i = 1; i < d; ++i)
		{
			index *= (*s)[i];
			index += *(in.begin() + i);
		}
		return (*p)[index];
	}
	int size()
	{
		return (*s)[0];
	}
	int size(std::initializer_list<int>in)
	{
		return (*s)[in.size()];
	}
	int dimension()
	{
		return d;
	}
private:
	int d;
	std::shared_ptr<int*>p;
	std::shared_ptr<int*>s;
};

template<class T> class my_ptr
{
private:
	int flag;
	int* c;
	T* v;
public:
	my_ptr()
	{
		flag = 0;
	}
	my_ptr(my_ptr&t)
	{
		flag = 1;
		v = t.v;
		c = t.c;
		++(*c);
	}
	my_ptr& my_ptr::operator=(my_ptr&t)
	{
		if (!flag)
			flag = 1;
		else
		{
			--(*c);
			if (*c == 0)
				delete[] v;
		}
		v = t.v;
		c = t.c;
		++(*c);
		return *this;
	}
	my_ptr(T* t)
	{
		flag = 1;
		v = t;
		c = new int(1);
	}
	~my_ptr()
	{
		if (flag)
		{
			--(*c);
			if ((*c) == 0)
			{
				delete c;
				delete[] v;
			}
		}
	}
	T* my_ptr::operator*()
	{
		return v;
	}
};

template<class T> class my_array
{
public:
	my_array()
	{
	}
	my_array(std::initializer_list<T>li)
	{
		T* k = new T(li.size());
		s = li.size();
		data = my_ptr<T>(k);
		for (int i = 0; i < int(li.size()); ++i)
		{
			(*data)[i] = *(li.begin() + i);
		}
	}
	my_array(int ts)
	{
		s = ts;
		data = my_ptr<T>(new T[ts]);
	}
	my_array(int ts1, int ini) :my_array(ts1)
	{
		memset(*data, 0, ts1 * sizeof(T));
	}
	my_array(my_array&t)
	{
		data = t.data;
		s = t.s;
	}
	T& my_array::operator[](int k)
	{
		return (*data)[k];
	}
	int size()
	{
		return s;
	}
	T* begin()
	{
		return *data;
	}
	const T* const end()
	{
		return *data + s;
	}
private:
	my_ptr<T>data;
	int s;
};
