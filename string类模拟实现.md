# string类

**文章目录**

1. [为什么要学习string类](#1.0)
2. [string类常见接口介绍](#2.0)
3. [string类的模拟实现](#3.0)
   1. [成员变量](#3.1)
   2. [构造析构函数](#3.2)
   3. [拷贝构造和赋值运算符重载(深浅拷贝问题)](#3.3)
   4. [简单接口实现](#3.4)
   5. [增删查改](#3.5)

---

## 1.为什么要学习string类<p id="1.0"></p>

> C++的出现更多是为了解决C语言做的不好或无法解决的问题的

C语言中，字符串是以'\0'结尾的一些字符的集合，为了操作方便，C标准库中提供了一些str系列的库函数，但是这些库函数与字符串是**分离开的**，不符合面向对象的思想，而且底层空间需要用户自己去管理，稍不留神就可能越界访问

> 这就是C语言解决的不够好的地方，C++为了解决这个问题，在标准库中提供了string类，我们学习使用string类，方便操作字符串

## 2.string类常见接口介绍<p id="2.0"></p>

> [string类文档查阅](https://cplusplus.com/reference/string/string/?kw=string)

## 3.string类的模拟实现<p id="3.0"></p>

### 3.1成员变量<p id="3.1"></p>

1. 如果**string**类的成员变量是**字符串**，那么这些字符串是存在代码段的**只读常量**，是不可修改的，满足不了STL库内string类的功能性
2. 如果**string**类的成员变量是**数组**，这样string类的内容就可以改变，但是数组是存储在栈区的，如果字符串非常多，那么栈区的空间就不足了
3. 联想到用C语言模拟实现数据结构中的栈，只要把存储位置开到堆区，就可以解决不能修改变量和空间不足的问题了

```cpp
//结合前面学到的命名空间，在这里我们新创建一个命名空间，就可以用string重新命名一个属于自己的string类
using namespace ddy
{
    class string
    {
    private:
        char* _str;			//指针，指向堆区开辟的空间
    };						
}
```

### 3.2构造析构函数<p id="3.2"></p>

> 上文里，我们定义了指针变量，对于真正使用的空间，我们还没有开辟出来，一般开辟空间的事情都在构造函数里完成，在析构函数里释放空间

```cpp
//类内	为后续方便，用类内表示已经在string类的内部
//构造函数
string()	//无参
    :_str(new char[1])	//开1字节的空间存放'\0'
    {
        _str[0]='\0';
    }

string(const char* str)		//带参
    :_str(new char[strlen(str)+1])	//这里多加的1同样是为了存放'\0'
    {
        strcpy(_str,str);
    }

//用缺省参数将无参和带参合在一起
string(const char* str = "")	//注意这里使用的"",等同于'\0'
    :_str(new char[strlen(str)+1])
    {
        strcpy(_str,str);
    }

//析构函数
~string()
{
    delete[] _str;
    _str = nullptr;
}
```

### 3.3拷贝构造和赋值运算符重载(深浅拷贝问题)<p id="3.3"></p>

**浅拷贝**：直接复制下来

```cpp
//类内
//拷贝构造
string(const string& s)
{
    _str = s._str;	//直接复制下来，这就是浅拷贝
}					//但是这样写出的拷贝构造函数是正确的吗？
					//_str存放的是开辟的空间的地址，复制后两个变量指向同一个地址，这正确吗？
```

**深拷贝**：开辟等大的空间，将复制对象空间的内容复制到开辟的空间内

```cpp
//类内
//拷贝构造
string(const string& s)
    :_str(new char[strlen(s._str)+1])		//这样拷贝下的数据才是正确的，有两份相同大小的空间，空间内的内容是相同的
{
	strcpy(_str,s._str);
}

//运算符重载
string& operator=(const string& s)
{
    if(this!=&s)	//放在自己赋值给自己   eg: s1 = s1;
    {
        delete[] _str;	//释放原有空间
        _str = new char[strlen(s._str)+1];
        strcpy(_str,s._str);
    }
    return *this;
}
```

上述是拷贝构造和赋值运算符重载的传统写法，自己调用**new**和**delete**函数，来开辟和释放空间，纯手动

```cpp
//类内
//拷贝构造(现代写法)
string(const string& s)
{
    string tmp(s._str);		//构造内容为s._str的新对象
    swap(_str,s_str);		//用swap函数交换_str与s._str指向地址的内容
}							//tmp由编译器调用构造函数开辟新空间，出作用域时又由编译器调用析构函数释放空间

//赋值运算符重载(现代写法)
string& operator=(string s)	//这里不使用引用传参就是为了让编译器自己拷贝构造一个新对象
{							//构造和析构都由编译器来调用，解放双手，避免手工调用出错
    swap(_str,s._str);		
    return *this;			
}
```

### 3.4简单接口实现<p id="3.4"></p>

```cpp
//类内
size_t size()
{
    return strlen(_str);
}

char& operator[](size_t i)
{
	return _str[i];
}

const char* c_str()
{
    return _str;
}
```

### 3.5增删查改<p id="3.5"></p>

> 不同于上述的内容，为了满足增删查改模拟实现的方便，将会增加两个新的成员变量，**_size**(已经存储的有效字符)，**_capacity**(可存储有效字符的总量)，同时，也会将上面所有的函数升级匹配新的成员变量

```cpp
#include<assert.h>
using namespace ddy
{
    class string
    {
    private:
        char* _str;		
        size_t _size;		//已经有了多少有效字符
        size_t _capacity;	//能存多少有效字符
        
        static size_t npos;		
    public:
        //构造析构
        string(const char* str = "")
        {
            _size = strlen(str);
            _capacity = _size;
            _str = new char[_capacity+1];
            strcpy(_str,str);
        }
        
        ~string()
        {
            delete[] _str;
            _str = nullptr;
            _size = _capacity = 0;
        }
        
        //拷贝构造和赋值运算符重载(现代写法)
        void swap(string& s)
        {
            ::swap(_str,s._str);			//::swap表示全局的swap
            ::swap(_size,s._size);
            ::swap(_capacity,s._capacity);
        }
        
        string(const string& s)
            :_str(nullptr)
            ,_size(0)
            ,_capacity(0)
        {
            string tem(s._str);		//构造
            swap(tem);						//上面实现的swap
        }
        
        string& operator=(string s)		
        {
            swap(s);
            return *this;
        }
        
        //简单接口
        size_t size()
        {
            return _size;
        }
        
        size_t capacity()
        {
            return _capacity;
        }
        
        char& operator[](size_t i)
        {
            return _str[i];
        }
        
        const char* c_str()
        {
            return _str;
        }
        
        /********************************************************************************************/
        //增
        //在增大的时候，空间不够是会发生的，为此，我们将扩容独立出来
        void reserve(size_t n)
        {
			if(n>_capacity)		//避免他人调用时传入小于已经空间的情况
            {
                char* newstr = new char[n+1];
                strcpy(newstr,_str);
                delete[] _str;
                _str = newstr;
                _capacity = n;
            }
        }
        
        void push_back(char ch)		//增加单个字符
        {
            if(_size==_capacity)
            {
				size_t newcapacity = _capacity == 0 ? 2 : _capacity * 2;
                reserve(newcapacity);
            }
            _str[_size] = ch;
            _size++;
            _str[_size]	= '\0';		//注意在末尾加上\0
     	}
            
        void append(const char* str)	//增加字符串
        {
            size_t len = strlen(str);
            if(len+_size>_capacity)
            	reserve(len+_size);
            strcat(_str+_size,str);
            _size=_capacity;
        }
        
        string& operator+=(char ch)			//对于string类，通常喜欢依靠+=来实现字符的增加，这里复用上面的函数完成重载
        {
            push_back(ch);
            return *this;
        }
        
        string& operator(const char* str)
        {
            append(str);
            return *this;
        }
        
        string& insert(size_t pos, char ch)
        {
            assert(pos <= _size);			//当pos=_size时，即使尾插，因此，push_back可复用insert函数实现
            if (_size == _capacity)
            {
                size_t newcapacity = _capacity == 0 ? 2 : _capacity * 2;
                reserve(newcapacity);
            }
            int end = _size;
            while(end>=(int)pos)	//注意，这里的比较将pos强转成int类型
            {
                _str[end+1]=_str[end];
                end--;
            }
            _str[pos] = ch;
            _size++;
            return *this;
        }
        
        string& insert(size_t pos, const char* str)
        {
            assert(pos <= _size);			//当pos=_size时，即使尾插，因此，append可复用insert函数实现
            size_t len = strlen(str);
            if (_size + len > _capacity)
            {
                reserve(_size + len);
            }
            int end = _size;
            while (end >= (int)pos)		//这里也进行强转
            {
                _str[end + len] = _str[end];
                end--;
            }
            _size=_capacity;
            for (size_t i = 0; i < len; i++)
            {
                _str[pos + i] = str[i];
            }
            return *this;
        }
        
        //删
        string& erase(size_t pos, size_t len = npos)
        {
            assert(pos<_size);
            if(len>=_size-pos)
            {
                _str[pos] = '\0';
                _size = pos;
            }
            else
            {
                size_t i = pos + len;
                while (i <= _size)
                {
                                _str[i - len] = _str[i];
                                i++;
                }
                _size -= len;
            }
            return *this;
        }
        
        //查
        size_t find(char ch, size_t pos = 0)
        {
            for (size_t i = pos; i < _size; i++)
            {
                if (_str[i] == ch)
                    return i;
            }
            return npos;
        }

        size_t find(const char* str, size_t pos = 0)
        {
            char* p = strstr(_str, str);
            if (!p)
                return npos;
            else
                return p - _str;
        }
        
        //改
        void resize(size_t n, char ch = '\0')		//改容量大小
        {
            if (n < _size)
            {
                _str[n] = '\0';
                _size = n;
            }
            else
            {
                if (n > _capacity)
                    reserve(n);
                for (size_t i = _size; i < n; i++)
                {
                    _str[i] = ch;
                }
                _size = n;
                _str[_size] = '\0';
            }
        }
    };		
    
    size_t string::npos = -1;	//静态成员变量在类外定义
}
```



