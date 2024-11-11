# C++模板进阶

**目录**：

1. [非类型模板参数](#1.0)
2. [模板的特化](#2.0)
   1. [函数模板特化](#2.1)
   2. [类模板特化](#2.2)
3. [模板总结](#3.0)

---

## 1.非类型模板参数<p id="1.0"></p>

模板参数分为**类型模板参数**和**非类型模板参数**

+ **类型模板参数**：出现在模板参数列表中，跟在**class**或者**typename**之后的参数类型名称。

  ```cpp
  template<class T>	//这里的T就是类型模板参数
  ```

+ **非类型模板参数**：就是用一个**常量**作为类(函数)模板的一个参数，在类(函数)模板中可将该参数当成常量来使用。

  ```cpp
  template<class T1, int T2>	//T2为非类型模板参数 
  ```

**示例**：

```cpp
namespace ddy
{
    template<class T, size_t N = 10>
    class Array
    {
    public:
        T& operator[](size_t index){return _array[index];}
        const T& operator[](size_t index)const{return _array[index];}
        
        size_t size()const{return _size;}
        bool empty()const{return 0 == _size;}
	private:
        T _array[N];
        size_t _size;
    };
}
```

+ 上面是STL中容器**Array**的模拟实现，其中**N**为非类型模板参数。
+ **N**被看作为常量，因为数组的大小需要常量来定义。
+ 浮点数、类对象以及字符串是不允许作为非类型模板参数的。
+ 非类型的模板参数必须在编译期就能确认结果。

## 2.模板的特化<p id="2.0"></p>

**概念**：

+ 使用模板可以实现一些与类型无关的代码，但对于一些特殊类型的可能会得到一些错误的结果，对于这些特殊类型我们需要特殊处理
+ 这个特殊处理就是模板的**特化**，即：**在原模板类的基础上，针对特殊类型所进行特殊化的实现方式。**

### 2.1函数模板特化<p id="2.1"></p>

**步骤**：

1. 必须要先有一个基础的函数模板。
2. 关键字template后面接一对空的尖括号<>。
3. 函数名后跟一对尖括号，尖括号中指定需要特化的类型。
4. 函数形参表: 必须要和模板函数的基础参数类型完全相同，如果不同编译器可能会报一些奇怪的错误。

```cpp
//函数模板 -- 参数匹配
template<class T>
bool IsEqual(T& left, T& right)
{
    return left == right;
}

//对IsEqual函数模板进行特化
template<>
bool IsEqual<char*>(char* &left, char* & right)
{
    return strcmp(left,right)==0;
}
```

+ 一般情况下如果函数模板遇到不能处理或者处理有误的类型，为了实现简单通常都是将该函数直接给出。如下：

  ```cpp
  bool IsEqual(char* &left, char* & right)
  {
      return strcmp(left,right)==0;
  }
  ```

  + 该种实现简单明了，代码的可读性高，容易书写。
  + 因为对于一些参数类型复杂的函数模板，特化时不容易出，因此**函数模板不建议特化**。

### 2.2类模板特化<p id="2.2"></p>

#### 2.2.1全特化

**概念**：全特化即是将模板参数列表中**所有**参数都确定。

```cpp
//原模版
template<class T1, class T2>
class Data
{
public:
    Data(){cout<<"原模版：Data<T1,T2>"<< endl;}
private:
    T1 _d1;
    T2 _d2;
};

//全特化
template<>
class Data<int, char>
{
public:
    Data(){cout<<"全特化：Data<int,char>"<< endl;}
}

int main()
{
    Data<int,int> d1;	//原模版
    Data<int,char> d2;	//全特化
    return 0;
}
```

#### 2.2.2偏特化

**概念**：任何针对模版参数进一步进行条件限制设计的特化版本。

```cpp
//原模版
template<class T1, class T2>
class Data
{
public:
    Data(){cout<<"原模版：Data<T1,T2>"<< endl;}
private:
    T1 _d1;
    T2 _d2;
};
```

偏特化有以下两种表现方式：

+ 部分特化：将模板参数类表中的一部分参数特化。

  ```cpp
  //第二个参数特化为int
  template<class T>
  class Data<T,int>
  {
  public:
      Data(){cout<<"半特化：Data<T，int>"<< endl;}
  private:
      T _d1;
      int _d2;
  };
  ```

+ 参数更进一步的限制：偏特化并不仅仅是指特化部分参数，而是针对模板参数更进一步的条件限制所设计出来的一个特化版本。

  ```cpp
  //两个参数偏特化为指针类型
  template<class T1, class T2>
  class Data<T1*,T2*>
  {
  public:
      Data(){cout<<"Data<T1*,T2*>"<< endl;}
  private:
      T1 _d1;
      T2 _d2;
  };
  
  //两个参数偏特化为引用类型
  template<class T1, class T2>
  class Data<T1&,T2&>
  {
  public:
      Data(){cout<<"Data<T1&,T2&>"<< endl;}
  private:
      T1 _d1;
      T2 _d2;
  };
  ```

  

## 3.模板总结<p id="3.0"></p>

**优点**：

+ 模板复用了代码，节省资源，更快的迭代开发，C++的**标准模板库(STL)**因此产生
+ 增强了代码的灵活性

**缺陷**：

+ 模板会导致代码膨胀问题，也会导致编译时间变长
+ 出现模板编译错误时，错误信息非常凌乱，不易定位错误

