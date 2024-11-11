# C++内存管理

[toc]

---

## 1.C++内存分区<p id="1.0"></p>

> 内存为什么要分区?
>
> 为了更高效地组织和管理计算机系统中的内存资源，以满足不同类型的数据和程序执行的需要

| 分区           | 作用                                                         |
| -------------- | ------------------------------------------------------------ |
| 栈(向下增长)   | 存储非静态局部变量，函数参数，返回值                         |
| 内存映射段     | 用于装载一个共享的动态内存库，用户可使用系统接口创建共享内存，做进程间通讯 |
| 堆(向上增长)   | 用于程序运行时动态内存分配                                   |
| 数据段(静态区) | 存储全局数据和静态数据                                       |
| 代码段(常量区) | 存储可执行的代码，只读常量                                   |

## 2.C++动态内存管理方式<p id="2.0"></p>

>1. C++可以继续使用C语言的动态内存管理方式
>
>   > C语言动态内存管理方式：
>   >
>   > 1. malloc：在堆上开辟一块连续的内存空间
>   > 2. calloc：在堆上开辟一块连续的内存空间，同时将开辟的空间初始化为0
>   > 3. realloc：重新调整已经分配的内存空间的大小
>   >    1. 扩展时，如果内存连续空间不足/收缩时，无法在原位置收缩，这种情况下，realloc会分配一个新的内存块，并把原内存块的数据复制到新内存块上，然后**释放**原内存块
>   > 4. free：释放开辟的内存块
>
>2. 但是在某些地方不好用，因此C++提出了自己的动态内存管理方式

### 2.1new/delete操作内置类型<p id="2.1"></p>

```cpp
int main()
{	
    //对比C语言，malloc/calloc/realloc/free都是函数，而new/delete是操作符
    int* p1 = new int;		//开辟一个4字节的整形空间
    int* p2 = new int(10);	//开辟一个4字节的整形空间，并初始化成10
    int* p3 = new int[10];	//开辟连续的10个4字节的整形空间
    
    delete p1;		//释放空间
    delete p2;
    delete[] p3;	//释放连续的空间
}
```

**注意**：

1. 申请和释放单个元素的空间，使用***new***和***delete***操作符
2. 申请和释放连续的空间，使用***new []***和***delete[]***
3. 需要匹配使用

> 不难发现，new/delete的对于内置类型的使用跟C语言的malloc/free并无太大区别，结合类与对象学习时的种种，推测出new/delete是为**自定义类型**服务的

### 2.2new/delete操作自定义类型<p id="2.2"></p>

```cpp
class A
{
public:
    A(int a = 0)
    	:_a(a)
	{
    	cout<<"A"<<endl;        
    }
    ~A()
    {
        cout<<"~A"<<endl;
    }
private:
    int _a;
};

int main()
{
    A* p1 = new A;	//先申请空间，然后调用构造函数
    delete p1;		//先调用析构函数，然后释放空间
    return 0;
}
```

从**操作自定义类型的角度**看`new/delete`与`malloc/free`的区别：对于自定义类型是否会调用构造/析构函数

> 对于这个区别，很好理解
>
> 默认成员函数是C++提出的，C++产生的相比于C语言新的东西，都会更贴合C++自身
>
> 因此new/delete在设计时就会考虑到构造/析构

**建议**：在C++中用`new/delete`来进行动态内存管理

## 3.operator new和operator delete函数<p id="3.0"></p>

> * `operator`是运算符重载的操作符
> * `new`是操作符
>
> 那么是否意味着`operator new`是对`new`的运算符重载呢？
>
> 答案是**否定的**

### 3.1operator new/operator delete与malloc/free的关系<p id="3.1"></p>

**operator new**和**operator delete**是系统提供的**全局函数**

```cpp
//使用operator new和malloc
int main()
{
    int* p1 = (int*)operator new(sizeof(int));
    int* p2 = (int*)malloc(sizeof(int));		//发现，operator new和malloc在申请空间上没有任何区别
    return 0;									//那operator new出现还有什么意义呢？
}

//为了找到operator new和malloc的不同，我们从operator new的源代码入手观察
void *__CRTDECL operator new(size_t size) _THROW1(_STD bad_alloc)
{
    void* p;
    while ((p = malloc(size)) == 0)	//从这里可以发现，operator new就是通过malloc实现的
    {
       	 if (_callnewh(size) == 0)	//发现，operator new在申请失败时会抛出异常，而malloc只会返回NULL
         {
             // 如果申请内存失败了，这里会抛出bad_alloc类型异常
             static const std::bad_alloc nomem;
             _RAISE(nomem);
         }
    }
    return (p);
}

```

`operator new`和`malloc`的区别是：`malloc`申请失败时**返回空指针**，`operator new`申请失败时**抛出异常**

> 失败抛异常是面向对象处理错误的方式，这种方式使得代码更加模板化，提高了代码的可读性和可维护性

那么就可以得出以下结论：

* `operator new` = `malloc` + 失败抛异常的处理方式

既然`operator new`和`malloc`有这样的关系，那么`operator delete`和`free`是否也存在这样的关系呢？

```cpp
//为了解决这个疑问，我们继续来看源代码

//free的实现
#define   free(p)               _free_dbg(p, _NORMAL_BLOCK)

//operator delete的实现
void operator delete(void *pUserData)
{
     _CrtMemBlockHeader * pHead;
     RTCCALLBACK(_RTC_Free_hook, (pUserData, 0));
     if (pUserData == NULL)
         return;
     _mlock(_HEAP_LOCK); 
     __TRY
         pHead = pHdr(pUserData);
         _ASSERTE(_BLOCK_TYPE_IS_VALID(pHead->nBlockUse));
         _free_dbg( pUserData, pHead->nBlockUse );		//注意到这一行代码，发现operator delete是通过free实现的
     __FINALLY
         _munlock(_HEAP_LOCK);
     __END_TRY_FINALLY
     return;
}
```

不难发现，`operator delete`与`free`是等价的，那么为什么还会出现`operator delete`呢？

因为`operator new`需要一个函数来释放空间，`free`可以释放，但是这很牛头人，所以引入了`operator delete`来释放`operator new`申请的空间

目前我们得出以下结论：

* `operator new` = `malloc` + 失败抛异常的处理方式
* `operator delete` = `free`

### 3.2operator new/operator delete与new/delete的关系<p id="3.2"></p>

> 在最开始，我们设想operator new与new可能是运算符重载的关系，但是并不是，结合operator new与malloc的关系，由此我们可以得到它们的关系

* `new` = `operator new` + 构造函数
* `delete` = `operator delete` + 析构函数

由此，我们得到了较为完整的关系：

* `new` = `operator new` + 构造函数 = `malloc` + 失败抛异常的处理方式 + 构造函数
* `delete` = `operator delete` + 析构函数 = `free` + 析构函数

### 3.3operator new/operator delete与operator new[]/operator delete[]的关系<p id="3.3"></p>

> new是申请单个元素的空间的，new []是申请连续的空间的
>
> 由此，operator new/operator new []也是同理

```cpp
//这里我想说的是，对于自定义类型不要用new/operator new硬开连续的空间
int main()
{			//为了方便，我用int代替自定义类型
    int* p1 = (int*)operator new[](10*sizeof(int));	//正常开
    int* p2 = (int*)operator new(10*sizeof(int));//硬开
    
    operator delete[](p1);	//正常释放	会自动调用析构函数
    
    // 释放动态分配的数组内存
    for (int i = 0; i < 10; ++i) {
       	p2[i].~int(); // 调用对象的析构函数，需要每个元素都自己手动调用一遍
    }
    operator delete(p2);		//很麻烦的
}
```

**结论**：多多注意**匹配**

## 4.new和delete的实现原理<p id="4.0"></p>

> 因为在处理内置类型上，没有太大区别，这里只讨论自定义类型

* ***new***的原理
  * 先调用`operator new`函数申请空间
  * 再在申请的空间上执行构造函数，完成对象的构造

* ***delete***的原理
  * 先在空间上执行析构函数，完成对象中资源的清理工作
  * 再调用`operator delete`函数释放空间
* ***new [N]***的原理
  * 先调用`operator new[]`函数，`operator new[]`实际调用`operator new`函数完成对连续的N个对象空间的申请
  * 再在申请的空间上执行构造函数，执行N次
* ***delete[]***的原理
  * 先在空间上执行N次析构函数，完成对象中资源的清理工作
  * 再调用`operator delete[]`函数释放空间，`operator delete[]`实际调用`operator delete`函数完成空间的释放

## 5.malloc/free与new/delete的区别<p id="5.0"></p>

**共同点**：都是在堆上申请空间，并需要程序员手动释放

**区别**：

1. 概念性质：malloc/free是函数，new/delete是操作符
2. 使用效果：new会调用构造函数初始化，失败了抛异常，malloc失败了返回0；free会调用析构函数
3. 使用方法：malloc参数传字节数，返回值是void*需要强转，new后面跟申请对象的类型，返回值是类型的指针，多个对象时，需在[]指定对象的个数

## 6.定位new表达式<p id="6.0"></p>

**概念**：在已分配的原始内存空间在调用**构造函数**初始化一个对象

> **构造函数**是不能被显式调用的，**析构函数**可以，而定位new可以看作是程序员调用构造函数的一种方式

**使用场景**：内存池

**使用格式**：new(指针)type(参数)	指针指向内存空间，参数可有可无

```cpp
class A
{
public:
    A(int a = 0)
        :_a(a)
    {
    	cout<<"A()"<<endl;   
    }
    ~A()
    {
        cout<<"~A()"<<endl;
    }
private:
    int _a;
};

int main()
{	//想要出现满足定位new的条件，我们可以用malloc/operator new来申请空间，再用定位new来调用构造函数
    A* p1 = (A*)malloc(sizeof(A));
    //p1->A();	是不行的
    new(p1)A;	//参数可有可无
    p1->~A();	//可以手动调析构
    free(p1);
    
    A* p2 = (A*)operator new(sizeof(A));
    new(p2)A(10);	//想传参得这样传
    p2->~A();
    free(p2);
    return 0;
}
```

## 7.内存泄漏<p id="7.0"></p>

**概念**：因为疏忽或错误造成程序未能释放已经不再使用的内存的情况

> 内存泄漏并不是指内存在物理上的消失，而是应用程序分配某段内存后，因为设计错误，失去了对该段内存的控制，因而造成了内存的浪费

**危害**：内存泄漏对长期运行的程序影响很大，如操作系统、后台服务等等，出现内存泄漏会导致响应越来越慢，最终卡死。

**分类**：

* 堆内存泄漏
* 系统资源泄漏

**如何避免**：

* 事前预防，如智能指针
* 事后查错，如泄漏检测工具 
