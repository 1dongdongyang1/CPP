# C++虚表和虚基表

[toc]

## 1.虚表/虚函数表(vtable)

### 1.1概念

+ **虚表** 是一种内部数据结构，用于支持多态。
+ 当一个类包含虚函数时，编译器会为该类生成一个虚表，以存储指向虚函数的指针。
+ 每个含有虚函数的类都有一个虚表，虚表用于记录类的虚函数地址，方便在运行时通过基类指针或引用调用派生类的实现。

### 1.2工作机制

+ 每个含有虚函数的类对象中都会有一个指向虚表的指针，称为 **虚表指针（vptr）**。
+ 当通过基类指针调用虚函数时，编译器根据对象中的 `vptr` 找到相应的虚表，并从虚表中获得该虚函数的实际地址，保证调用正确的派生类函数。
+ 虚表在编译期生成，而 `vptr` 在对象创建时被初始化。

**示例**：

```cpp
class Base {
public:
    virtual void show() { std::cout << "Base show" << std::endl; }
};

class Derived : public Base {
public:
    void show() override { std::cout << "Derived show" << std::endl; }
};
```

在这个例子中，`Base` 和 `Derived` 各自都有一个虚表，用于存储 `show` 函数的地址。当通过 `Base*` 指针调用 `show` 时，会通过虚表找到 `Derived` 中的 `show` 函数，实现多态。

### 1.3虚表的特性

+ 虚表是由编译器自动创建和维护的，开发者无法直接访问。
+ 对象的内存中通常包含一个指向虚表的指针（vptr）。
+ 虚表只在包含虚函数的类中存在。

## 2.虚基表（vbtable）

### 2.1概念

+ **虚基表** 是编译器为了解决多重继承中的**虚基类**问题而引入的数据结构。
+ 它的目的是确保在多重继承中共享虚基类的唯一实例，从而避免重复继承带来的存储空间浪费和不一致的问题。

### 2.2工作机制

+ 当类通过多重继承包含同一个基类时，为了确保基类在派生类中只存在一个实例，可以将基类声明为虚基类。
+ 编译器会为包含虚基类的类生成一个虚基表，其中记录了虚基类实例的偏移量。
+ 虚基表（vbtable）通过这些偏移量来指示派生类如何访问唯一的虚基类实例。

**示例**：

```cpp
class A {
public:
    int value;
};

class B : virtual public A {
};

class C : virtual public A {
};

class D : public B, public C {
};
```

在这个例子中，`B` 和 `C` 都通过虚基类继承 `A`。因此 `D` 类中只会有一个 `A` 的实例。编译器会生成虚基表来记录 `A` 的地址偏移，以便 `B` 和 `C` 都能正确访问 `A` 的成员。

### 2.3虚基表的特性

+ 只有在虚基类（`virtual` 基类）存在时才会生成虚基表。
+ 虚基表中存储了虚基类实例的偏移量。
+ 虚基表用于解决多重继承下的虚基类共享问题。

## 3.深入认识虚基表

> 研究环境：vs2022  32位平台

### 3.1菱形继承对象模型

```cpp
class A {
public:
	int _a;
};

class B : public A {
public:
	int _b;
};

class C : public A {
public:
	int _c;
};

class D : public B, public C {
public:
	int _d;
};
```

上面是没有使用虚继承的菱形继承，导致D的对象成员里有两份A，导致了**数据冗余**和**二义性**。

**图示**：

![菱形继承对象模型](.\picture\菱形继承对象模型.png)

### 3.2菱形虚拟继承对象模型

使用虚继承：

```cpp
class A {
public:
	int _a;
};

class B :  public A {
public:
	int _b;
};

class C :  public A {
public:
	int _c;
};

class D : public B, public C {
public:
	int _d;
};
```

**图示**：

![菱形虚拟继承对象模型](.\picture\菱形虚拟继承对象模型.png)

## 4.深入认识虚表

### 4.1含有虚函数的类对象模型	

```cpp
class A
{
public:
	virtual void func1() { cout << "A::func1" << endl; }
	int _a;
};
```

**图示**：

![虚表模型](D:\a.blog\C++\picture\虚表模型.png)

### 4.2单继承中的虚表

```cpp
class Base {
public:
	virtual void func1() { cout << "Base::func1" << endl; }
	virtual void func2() { cout << "Base::func2" << endl; }
private:
	int a;
};
class Derive :public Base {
public:
	virtual void func1() { cout << "Derive::func1" << endl; }
	virtual void func3() { cout << "Derive::func3" << endl; }
	virtual void func4() { cout << "Derive::func4" << endl; }
private:
	int b;
};
```

如何观察到类对象的虚表呢？我们设计函数来实现。

```cpp
//定义函数指针类型，方便声明
typedef void(*VFPTR) ();
void PrintVTable(VFPTR vTable[])
{
	// 依次取虚表中的虚函数指针打印并调用。调用就可以看出存的是哪个函数
	cout << " 虚表地址>" << vTable << endl;
	for (int i = 0; vTable[i] != nullptr; ++i)
	{
		printf(" 第%d个虚函数地址 :0X%x,->", i, vTable[i]);
		VFPTR f = vTable[i];
		f();
	}
	cout << endl;
}
int main()
{
	Base b;
	Derive d;
	// 思路：取出b、d对象的头4bytes，就是虚表的指针，前面我们说了虚函数表本质是一个存虚函数
	// 指针的指针数组，这个数组最后面放了一个nullptr
		// 1.先取b的地址，强转成一个int*的指针
		// 2.再解引用取值，就取到了b对象头4bytes的值，这个值就是指向虚表的指针
		// 3.再强转成VFPTR*，因为虚表就是一个存VFPTR类型(虚函数指针类型)的数组。
		// 4.虚表指针传递给PrintVTable进行打印虚表
		// 5.需要说明的是这个打印虚表的代码经常会崩溃，因为编译器有时对虚表的处理不干净，虚表最
		//   后面没有放nullptr，导致越界，这是编译器的问题。我们只需要点目录栏的 - 生成 - 清理解决方案，再
		//	 编译就好了。
	VFPTR * vTableb = (VFPTR*)(*(int*)&b);
	PrintVTable(vTableb);
	VFPTR* vTabled = (VFPTR*)(*(int*)&d);
	PrintVTable(vTabled);
	return 0;
}
```

运行代码，我们得到

```shell
 虚表地址>00029B34
 第0个虚函数地址 :0X212b7,->Base::func1
 第1个虚函数地址 :0X21113,->Base::func2

 虚表地址>00029B64
 第0个虚函数地址 :0X21244,->Derive::func1
 第1个虚函数地址 :0X21113,->Base::func2
 第2个虚函数地址 :0X21230,->Derive::func3
 第3个虚函数地址 :0X2116d,->Derive::func4
```

发现：

+ 如果一个类通过单继承继承了一个基类，并且重写了基类的虚函数，那么派生类的虚表通常会**直接覆盖**基类的虚表。
+ 这种情况下，派生类会沿用基类的虚表布局，只是将重写的虚函数地址替换到对应的位置。
+ 因此，单继承的虚表是继承和延续的，不需要重新创建。

### 4.3多继承中的虚表

```cpp
class Base1 {
public:
	virtual void func1() { cout << "Base1::func1" << endl; }
	virtual void func2() { cout << "Base1::func2" << endl; }
private:
	int b1;
};
class Base2 {
public:
	virtual void func1() { cout << "Base2::func1" << endl; }
	virtual void func2() { cout << "Base2::func2" << endl; }
private:
	int b2;
};
class Derive : public Base1, public Base2 {
public:
	virtual void func1() { cout << "Derive::func1" << endl; }
	virtual void func3() { cout << "Derive::func3" << endl; }
private:
	int d1;
};
typedef void(*VFPTR) ();
void PrintVTable(VFPTR vTable[])
{
	cout << " 虚表地址>" << vTable << endl;
	for (int i = 0; vTable[i] != nullptr; ++i)
	{
		printf(" 第%d个虚函数地址 :0X%x,->", i, vTable[i]);
		VFPTR f = vTable[i];
		f();
	}
	cout << endl;
}
```

发现：

+ 在多继承的情况下，派生类会从多个基类继承虚表。这种情况下，派生类的对象布局中会包含**多个虚表指针（vfptr）**，每个指针对应一个基类的虚表。
+ 如果派生类重写了基类的虚函数或者定义了新的虚函数，则会为派生类**创建一个新的虚表**。这个新虚表不仅包含派生类自己的虚函数，还包含各个基类的虚函数（按照每个基类独立的虚表）。
+ 换句话说，**多继承的派生类会拥有多个虚表**，每个虚表分别管理来自不同基类的虚函数，以确保不同路径的虚函数调用可以正确绑定到对应的实现。

### 4.4菱形继承中的虚表

```cpp
//在原先代码的基础上加入了Base0，让Base1和Base2对Base0进行public继承
class Base0
{
public:
	virtual void func1() = 0;
};
class Base1 : public Base0
{
public:
	virtual void func1() { cout << "Base1::func1" << endl; }
	virtual void func2() { cout << "Base1::func2" << endl; }
private:
	int b1;
};
class Base2 : public Base0
{
public:
	virtual void func1() { cout << "Base2::func1" << endl; }
	virtual void func2() { cout << "Base2::func2" << endl; }
private:
	int b2;
};
class Derive : public Base1, public Base2 {
public:
	virtual void func1() { cout << "Derive::func1" << endl; }
	virtual void func3() { cout << "Derive::func3" << endl; }
private:
	int d1;
};
```

+ Base0对Base1，Base2是单继承，Base1和Base2将沿用Base0的虚表。
+ Base1和Base2对Derive是多继承，Derive将会创建新的虚表

## 5.菱形虚拟继承中的虚表虚基表对象模型

```cpp
class Base0
{
public:
	virtual void func1() = 0;
private:
	int b0 = 0;
};
class Base1 :  public Base0
{
public:
	virtual void func1() { cout << "Base1::func1" << endl; }
	virtual void func2() { cout << "Base1::func2" << endl; }
private:
	int b1 = 1;
};
class Base2 :  public Base0
{
public:
	virtual void func1() { cout << "Base2::func1" << endl; }
	virtual void func2() { cout << "Base2::func2" << endl; }
private:
	int b2 = 2;
};
class Derive : public Base1, public Base2 {
public:
	virtual void func1() { cout << "Derive::func1" << endl; }
	virtual void func3() { cout << "Derive::func3" << endl; }
private:
	int d1 = 3;
};
```

**图示**：

![菱形虚拟继承虚表对象模型](.\picture\菱形虚拟继承虚表对象模型.png)

发现：

+ Base1和Base2本应该继承Base0的虚表，但是虚拟继承后，Base1和Base2创建自己的虚表。
+ 虚基表存的是偏移量

