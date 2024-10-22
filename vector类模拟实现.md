# vector类模拟实现

**目录**：

1. [vector的个人理解](#1.0)
2. [vector类常见接口介绍](#2.0)
3. [vector类的模拟实现](#3.0)
   1. [成员变量](#3.1)
   2. [默认成员函数](#3.2)
   3. [增删改+拷贝升级](#3.3)

---

## 1.vector的个人理解<p id="1.0"></p>

> 对比上一节的**string**，vector可以存字符串类型，也可以存其他类型，但string只能存字符串类型；
>
> vector类似**数组**，但是相较于数组，vector不止可以存入`int`变量，还能调整容量的大小，这是数组所没有的

## 2.vector类常见接口介绍<p id="2.0"></p>

> [vector类文档查阅](https://cplusplus.com/reference/vector/vector/)

## 3.vector类的模拟实现<p id="3.0"></p>

### 3.1成员变量<p id="3.1"></p>

不同于string类只接受字符串类型，vector可以存入不同的类型，因此在模拟实现vector类时，实现的是vector的**模板类**。

```cpp
namespace ddy		//在ddy的命名空间内，vector可以直接叫vector
{
    template<class T>	//这里的class不是类，等同于typename
    class vector
    {
    public:
        typedef T* iterator;
    private:
        //在这里的写法采用SGI版STL3.0的写法
        iterator _start;
        iterator _finish;				//相当于string实现时的	_size
        iterator _end_of_storage;		//					_capacity		
    }
}
```

> [SGI STL30源码](https://github.com/Challechang/stl30)

### 3.2默认成员函数<p id="3.2"></p>

```cpp
//类内
//构造函数
vector()						//能力有限，先模拟实现无参构造
    :_start(nullptr)
    ,_finish(nullptr)
    ,_end_of_storage(nullptr)
{}

//析构函数
~vector()
{
    delete[] _start;
    _start = _finish = _end_of_storage;
}

//辅助接口
size_t size()const					//因为不改变，所以加上const修饰，这样非const函数可以调用，const函数也可以调用
{
    return _finish - _start;
}

size_t capacity()const
{
    return _end_of_storage - _start;
}

T& operator[](size_t i)
{
    return _start[i];
}

//拷贝构造(传统写法)
vector(const vector<T>& v)
{
    _start = new T[v.capacity()];
    _finish = _start;
    _end_of_storage = _start + v.capacity();
    
    for(size_t i = 0 ; i < v.size(); i++)
    {
    	*_finish = v[i];
        _finish++;
    }
}

//赋值运算符重载(传统写法)
vector<T>& operator=(const vector<T>& v)
{
    if(this!=&v)
    {
        delete[] _start;
        _start = new T[v.capacity()];
        _finish = _start;
        for(size_t i = 0; i < size(); i++)
        {
            *_finish = v[i];
            _finish++;
        }
        _end_of_storage = _start + v.capacity();
    }
}
```

### 3.3增删改+拷贝升级<p id="3.3"></p>

```cpp
//类内
//辅助接口
iterator begin()		//vector在增删时，传入的是迭代器
{
    return _start;
}

iterator end()
{
    return _finish;
}

void reserve(size_t n)		//扩容函数
{
    if(n>capacity())
    {
        size_t sz = size();		//扩容时，会造成迭代器失效，需要提前保存数据的大小
        T* tmp = new T[n];
        if(_start)
        {
            for(size_t i = 0; i<sz;i++)
            {
                tmp[i] = _start[i];
            }
            delete[] _start;
        }
        _finish = _start + sz;
        _end_of_storage = _start + n;
    }
}

void swap(vector<T>& v)			//交换函数
{
    ::swap(_start,v._start);
    ::swap(_finish,v._finish);
    ::swap(_end_of_storage,v._end_of_storage);
}

//增
void push_back(const T& x)
{
    if(_finish = _end_of_storage)
    {
        size_t newcapacity = capacity()==0?2:2*capacity();
        reserve(newcapacity);
    }
    *_finish = x;
    _finish++;
}

void insert(iterator pos, const T& x)
{
    assert(pos<=_finish);
    if(_finish = _end_of_storage)
    {
        size_t n = pos - _start;		//只要扩容，指针就会失效，需要提前记录
        size_t newcapacity = capacity() == 0 ? 2 : 2 * capacity();
        reserve(newcapacity);
        pos = _start + n;
    }
    iterator end = _finish - 1;
    while(end>=pos)
    {
        *(end+1) = *end;
        end--;
    }
    *pos = x;
    _finish++;
}

//复用insert实现push_back
void push_back(const T& x)
{
    insert(_finish,x);
}

//删
void pop_back()
{
    assert(_start<_finish);
    _finish--;
}

iterator erase(iterator pos)
{
    assert(pos<=_finish);
    size_t start = pos;
    while(start<_finish)
    {
        *start = *(start + 1);
        start++;
    }
    _finish--;
    return pos;
}

//改
void resize(size_t n , const T& val = T())
{
    if(n < _finish - _start)
    {
        _finish = n + _start;
    }
    else
    {
        if(n>_end_of_storage - _start)
            reserve(n);
        for(size_t i = size(); i < n; i++)
        {
            _start[i] = val;
        }
        _finish = _start + n;
    }
}

//拷贝构造(现代写法)
vector(const vector<T>& v)
    :_start(nullptr)
    ,_finish(nullptr)
    ,_end_of_storage(nullptr)
{
    reserve(v.capacity());
   	for(const auto& e : v)
    {
        push_back(e);
    }
}

//赋值运算符重载(现代写法)
vector<T>& operator=(vector<T> v)
{
    swap(v);
    return *this;
}
```

