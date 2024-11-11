# C++继承

[toc]

## 1.初识继承

### 1.1基本概念

+ **继承**（Inheritance）是面向对象编程中的一个核心概念，允许一个类从另一个类获取属性和行为，实现代码复用和扩展。
+ 在C++中，**继承**是一种从已有类（称为基类或父类）中派生出新类（称为派生类或子类）的机制。派生类会继承基类的成员变量和成员函数，还可以增加自己的新成员或重写基类的成员。

### 1.2继承的语法

继承通过类声明中的“`:`”符号和继承访问控制符（`public`、`protected`、`private`）来实现：

```cpp
class 基类 {
    // 基类成员
};

class 派生类 : 继承访问控制符 基类 {
    // 派生类成员
};
```

### 1.3继承访问限定符

C++中有三种继承访问控制符，分别是`public`、`protected`和`private`。它们决定了基类中的成员在派生类中的可见性：

1. **public继承**：基类的`public`成员在派生类中仍然是`public`，`protected`成员保持为`protected`。
2. **protected继承**：基类的`public`和`protected`成员都变为`protected`。
3. **private继承**：基类的`public`和`protected`成员都变为`private`。

### 1.4继承的类型

C++支持多种继承方式：

1. **单继承**：一个类只继承一个基类。
2. **多继承**：一个类可以继承多个基类（例如`class D : public B, public C`），但多继承可能会带来复杂性，如菱形继承问题。
3. **多层继承**：派生类继续被其他类继承，形成继承链。

> C++是面向对象的先行语言，多继承是C++踩到的一个坑，后续推出的oo语言(比如**Java**)是没有多继承的，因为多继承会产生菱形继承，要解决菱形继承就需要引入虚基类，而虚基类的底层实现又十分复杂，这就降低了效率，而C++是十分重视效率的

## 2.继承中的细节

### 2.1基类和派生类的赋值转换

+ **派生类对象** 可以赋值给 **基类的对象** **/** **基类的指针** **/** **基类的引用**。这里有个形象的说法叫切片或者切割。寓意把派生类中父类那部分切来赋值过去。
+ 基类对象不能赋值给派生类对象。

**图示**：

![切片](D:\a.blog\C++\picture\切片.png)

**示例**：

```cpp
class Person
{
protected:
    string _name;	//姓名
    string _sex;	//性别
    int _age;		//年龄
};

class Stuudent : public Person
{
public:
    int _Num;		//学号
};

int main()
{
    Student st;
    Person p1 = st;		//对象
    Person* p2 = &st;	//指针
    Person& p3 = st;	//引用
    
    st = p1;	//error 基类的对象/指针/引用，都不能赋值给派生类
    return 0;
}
```

### 2.3继承中的作用域

1. 在继承体系中**基类**和**派生类**都有**独立的作用域**。
2. 子类和父类中有同名成员，**子类成员将屏蔽父类对同名成员的直接访问**，这种情况叫**隐藏**，也叫**重定义**。(在子类成员函数中，可以使用**基类::基类成员**显示访问父类的同名函数)
3. 需要注意的是如果是成员函数的隐藏，只需要函数名相同就构成隐藏。
4. 注意在实际中在**继承体系里面**最好**不要定义同名的成员**。

```cpp
//B中的fun和A中的fun不是构成重载，因为不是在同一作用域
//B中的fun和A中的fun构成隐藏，成员函数满足函数名相同就构成隐藏。
class A
{
public:
	void fun()
	{
		cout << "func()" << endl;
	}
};
class B : public A
{
public:
 	void fun(int i)
 	{
 		A::fun();
 		cout << "func(int i)->" <<i<<endl;
 	}
};
void Test()
{
    B b;
    b.fun(10);
}
```

### 2.4派生类类的默认成员函数

1. 派生类的构造函数必须调用基类的构造函数初始化基类的那一部分成员。如果基类没有默认的构造函数，则必须在派生类构造函数的初始化列表阶段显示调用。
2. 派生类对象初始化先调用基类构造再调派生类构造。
3. 派生类的拷贝构造函数必须调用基类的拷贝构造完成基类的拷贝初始化。
4. 派生类的operator=必须要调用基类的operator=完成基类的复制。
5. 派生类的析构函数会在被调用完成后自动调用基类的析构函数清理基类成员。因为这样才能保证派生类对象先清理派生类成员再清理基类成员的顺序。
6. 编译器会对析构函数名进行特殊处理，处理成destrutor()，如果父类析构函数不加virtual修饰，那么子类和父类的析构函数构成隐藏。

```cpp
class Person
{
public:
	Person(const char* name = "ddy1")
		:_name(name)
	{
		cout << "Person()" << endl;
	}

	Person(const Person& p)
		:_name(p._name)
	{
		cout << "Person(const Person& p)" << endl;
	}

	Person& operator=(const Person& p)
	{
		cout << "Person& operator=(const Person& p)" << endl;
		if (this != &p)	_name = p._name;
		return *this;
	}

	~Person()
	{
		cout << "~Person()" << endl;
	}
protected:
	string _name;
};

class Student : public Person
{
public:
	Student(const char* name = "ddy2", int num = 000)
		:Person(name)
		,_num(num)
	{
		cout << "Student(const char* name , int num = 000)" << endl;
	}

	Student(const Student& s)
		:Person(s)
		, _num(s._num)
	{
		cout << "Student(const Student& s)" << endl;
	}

	Student& operator=(const Student& s)
	{
		cout << "Student& operator=(const Student& s)" << endl;
		if (this != &s)
		{
			Person::operator=(s);	//赋值重载构成隐藏，需要使用作用域限定符
			_num = s._num;
		}
		return *this;
	}

	~Student()
	{
		cout << "~Student" << endl;
	}
private:
	int _num;
};
	

int main()
{
	Student s;
	return 0;
}
```

### 2.5继承与友元

> [点我了解友元](https://blog.csdn.net/2301_80030944/article/details/139469139)

**友元关系不能继承**，也就是说基类友元不能访问子类私有和保护成员。

```cpp
#include <iostream>
#include <string>
using namespace std;

class Student;	//声明Student类，非常重要，如果不加，编译器就会报非常奇怪的错，chatgpt和copilot都检测不出来
class Person
{
public:
	friend void Display(const Person& p, const Student& s);
protected:
	string _name; // 姓名
};
class Student : public Person
{
public:
	//friend void Display(const Person& p, const Student& s);	加上这句后，友元函数就能访问Student的私有成员
protected:
	int _stuNum; // 学号
};
void Display(const Person& p, const Student& s)
{
	cout << p._name << endl;
	cout << s._stuNum << endl;
}
void main()
{
	Person p;
	Student s;
	Display(p, s);
}
```

### 2.6继承与静态成员

**基类**定义了***static***静态成员，则整个继承体系里面只有一个这样的成员。无论派生出多少个子类，都只有一个static成员实例 。

### 2.7菱形继承和菱形虚拟继承

**菱形继承：菱形继承是多继承的一种特殊情况。**

**图示**：

![菱形继承](D:\a.blog\C++\picture\菱形继承.png)

**菱形继承的问题**：

+ 从上面可以看到，**Assistant**的对象中，**Person**成员会有两份。
+ 这样的结果，让菱形继承有**数据冗余**和**二义性**的问题。

```cpp
class Person
{
public:
	string _name; // 姓名
};
class Student : public Person
{
protected:
	int _num; //学号
};
class Teacher : public Person
{
protected:
	int _id; // 职工编号
};
class Assistant : public Student, public Teacher
{
protected:
	string _majorCourse; // 主修课程
};
void Test()
{
	Assistant a;
	a._name = "peter";	//error  这样会有二义性无法明确知道访问的是哪一个_name
	// 需要显示指定访问哪个父类的成员可以解决二义性问题，但是数据冗余问题无法解决
	a.Student::_name = "xxx";
	a.Teacher::_name = "yyy";
}
```

+ **虚拟继承**可以解决菱形继承的二义性和数据冗余的问题。
+ 如上面的继承关系，在Student和Teacher的继承Person时使用虚拟继承，即可解决问题。
+ 需要注意的是，虚拟继承不要在其他地方使用。

```cpp
class Person
{
public:
	string _name; // 姓名
};
class Student : virtual public Person
{
protected:
	int _num; //学号
};
class Teacher : virtual public Person
{
protected:
	int _id; // 职工编号
};
class Assistant : public Student, public Teacher
{
protected:
	string _majorCourse; // 主修课程
};
void Test()
{
	Assistant a;
	a._name = "peter";
}
```

+ 这里的**Person**类被虚拟继承了，因此**Person**可以被称为**虚基类**。
+ **虚基类**保证无论通过多少条继承路径继承，派生类中对基类的成员只有一份拷贝。

### 2.8继承和组合

+ public继承是一种**is-a**的关系。也就是说每个派生类对象都是一个基类对象。
+ 组合是一种**has-a**的关系。假设B组合了A，每个B对象中都有一个A对象。
+ 优先使用对象组合，而不是类继承
  + 继承允许你根据基类的实现来定义派生类的实现。这种通过生成派生类的复用通常被称为**白箱复用**(white-box reuse)。术语“白箱”是相对可视性而言：在继承方式中，基类的内部细节对子类可见 。继承一定程度破坏了基类的封装，基类的改变，对派生类有很大的影响。派生类和基类间的依赖关系很强，耦合度高。
  + 对象组合是类继承之外的另一种复用选择。新的更复杂的功能可以通过组装或组合对象来获得。对象组合要求被组合的对象具有良好定义的接口。这种复用风格被称为**黑箱复用**(black-box reuse)，因为对象的内部细节是不可见的。对象只以“黑箱”的形式出现。组合类之间没有很强的依赖关系，耦合度低。优先使用对象组合有助于你保持每个类被封装。
  + 实际尽量多去用组合。组合的耦合度低，代码维护性好。不过继承也有用武之地的，有些关系就适合继承那就用继承，另外要实现多态，也必须要继承。类之间的关系可以用继承，可以用组合，就用组合。

