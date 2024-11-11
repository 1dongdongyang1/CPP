# list类的模拟实现

**目录**：

1. [结点](#1.0)
2. [框架](#2.0)
3. [迭代器](#3.0)
   1. [初次尝试](#3.1)
   2. [const迭代器](#3.2)

---

> [完整代码](https://github.com/1dongdongyang1/CPP/blob/master/STL/STL_list.h)

## 1.结点<p id="1.0"></p>

一个结点需要包含：**储存的数据，上一个结点的地址，下一个结点的地址。**

```c++
template<class T>
struct __list_node
{
    __list_node<T>* _prev;
    __list_node<T>* _next;
    T _data;

    __list_node(const T& x = T())	//T()是匿名类型，如果没有传参，编译器会调用相应类型的构造函数
        :_data(x)
        ,_prev(nullptr)
        ,_next(nullptr)
    {}
};
```

+ 由于我们后面会在类外访问，所以为了避免麻烦就写成`struct`,这样成员属性都是public的。
+ 使用模板，方便适配各种数据类型

## 2.框架<p id="2.0"></p>

list类包含**哨兵结点的指针**。

```c++
template<class T>
class list
{
public:
    typedef __list_node<T> Node;
    
    list()
    {
        _head = new Node;
        _head->_next = _head;
        _head->_prev = _head;
    }
    
private:
    Node* _head;
};    
```

+ 因为是双向循环链，所以哨兵结点的指针都指向自己

## 3.迭代器<p id="3.0"></p>

> list的迭代器的实现，是模拟list实现的重中之重

### 3.1初次尝试<p id="3.1"></p>

+ list迭代器的本质还是**指针**
+ 我们将指针的封装，内部进行**运算符重载**

```c++
template<class T>
struct __list_iterator
{
    typedef __list_node<T> Node;
    typedef __list_iterator<T> Self;
    Node* _node;

    __list_iterator(Node* node)
        :_node(node)
    {}

    T& operator*()
    {
        return _node->_data;
    }

    T* operator->()
    {
        return &_node->_data;
    }

    Self& operator++()
    {
        _node = _node->_next;
        return *this;
    }

    Self operator++(int)
    {
        Self tmp(*this);
        //_node = _node->_next;
        ++(*this);
        return tmp;
    }

    Self& operator--()
    {
        _node = _node->_prev;
        return *this;
    }

    Self operator--(int)
    {
        Self tmp(*this);
        //_node = _node->_next;
        --(*this);
        return tmp;
    }

    bool operator!=(const Self& it)const
    {
        return _node != it._node;
    }

    bool operator==(const Self& it)const
    {
        return _node == it._node;
    }
};
```

+ 就是将++/--的操作通过运算符重载，变成在结点间移动的操作。

### 3.2const迭代器<p id="3.2"></p>

+ list的迭代器不像vector/string类的迭代器，它不可以直接在迭代器的前面加const修饰
+ 指针被const修饰后，不能通过指针来修改数据，意味着`operator*`和`operator->`的作用失效，但其他函数还能继续使用
+ 如果再专门写一份const的迭代器，那么代码就太过冗余了

```c++
template<class T, class Ref, class Ptr>
struct __list_iterator
{
    typedef __list_node<T> Node;
    typedef __list_iterator<T, Ref, Ptr> Self;
    Node* _node;

    __list_iterator(Node* node)
        :_node(node)
    {}

    Ref operator*()
    {
        return _node->_data;
    }

    Ptr operator->()
    {
        return &_node->_data;
    }

    Self& operator++()
    {
        _node = _node->_next;
        return *this;
    }

    Self operator++(int)
    {
        Self tmp(*this);
        //_node = _node->_next;
        ++(*this);
        return tmp;
    }

    Self& operator--()
    {
        _node = _node->_prev;
        return *this;
    }

    Self operator--(int)
    {
        Self tmp(*this);
        //_node = _node->_next;
        --(*this);
        return tmp;
    }

    bool operator!=(const Self& it)const
    {
        return _node != it._node;
    }

    bool operator==(const Self& it)const
    {
        return _node == it._node;
    }
};
```

+ 这样，如果是const迭代器，就传const修饰的指针和引用。

