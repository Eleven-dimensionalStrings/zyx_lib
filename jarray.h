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
		b* k = new b(li.size());
		s = li.size();
		data = std::shared_ptr<b>(k);
		for (int i = 0; i < int(li.size()); ++i)
		{
			((*data).v)[i] = *(li.begin() + i);
		}
	}
	jarray1(int s)
	{
		b* k = new b(s);
		s = s;
		data = std::shared_ptr<b>(k);
	}
	jarray1(int ts1, int ini) :jarray1(ts1)
	{
		memset(data->v, 0 , ts1 * 4);
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
private:
	class b
	{
	public:
		int* v;
		b(int s)
		{
			v = new int[s];
		}
		~b()
		{
			delete[]v;
		}
	};
	std::shared_ptr<b>data;
	int s;
};
int& jarray1::operator[](int k)
{
	return ((*data).v)[k];
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
