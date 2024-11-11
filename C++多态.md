# C++多态

[toc]

## 1.概念

+ **多态**（Polymorphism）是面向对象编程的核心特性之一，指的是在父类的指针或引用指向子类对象时，可以通过父类接口调用子类特有的实现，从而实现“**一类多形**”的效果。
+ 简单来说，多态让同一个接口或方法可以表现出不同的行为。

**多态的两种类型**：

+ 编译时多态（静态多态）
+ 运行时多态（动态多态）

## 2.编译时多态（静态多态）

+ 编译时多态指的是函数重载（函数名相同但参数不同）和运算符重载，通过不同参数或操作符调用不同的实现。
+ 编译时多态在编译阶段就能确定调用哪个函数，不涉及运行时的动态绑定。

```cpp
class Example {
public:
    void func(int x) { cout << "Integer function: " << x << endl; }
    void func(double x) { cout << "Double function: " << x << endl; }
};

Example e;
e.func(5);       // 调用 int 参数的 func
e.func(5.5);     // 调用 double 参数的 func
```

## 3.运行时多态（动态多态）

### 3.1构成条件

1. 必须通过基类的**指针**或**引用**调用虚函数
2. 被调用的函数必须是虚函数，且派生类必须对基类的虚函数进行重写

### 3.2虚函数

**虚函数**：被关键字***virtual***修饰的函数就是虚函数

```cpp
class Person {
public:
 	virtual void BuyTicket() { cout << "买票-全价" << endl;}
};
```

### 3.3虚函数的重写

**虚函数的重写(覆盖)**：派生类中有一个跟基类**完全相同**(返回值类型/函数名/参数)的虚函数，称子类的虚函数**重写**了父类的虚函数。

```cpp
class Person {
public:
	virtual void BuyTicket() { cout << "买票-全价" << endl; }
};
class Student : public Person {
public:
	virtual void BuyTicket() { cout << "买票-半价" << endl; }
};
void Func(Person& p)
{
	p.BuyTicket();
}
int main()
{
	Person ps;
	Student st;
	Func(ps);
	Func(st);
	return 0;
}
```

虚函数重写的两个**例外**：

1. **协变**(基类与派生类函数的返回值类型不同)

   + 派生类重写基类虚函数时，与基类虚函数返回值类型不同。
   + 基类虚函数返回基类对象的指针或者引用，派生类虚函数返回派生类对象的指针或者引用时，称为**协变**。

   ```cpp
   class A {};
   class B : public A {};
   class Person {
   public:
   	virtual A* f() { return new A; }
   };
   class Student : public Person {
   public:
   	virtual B* f() { return new B; }
   };
   ```

2. **析构函数的重写**(基类与派生类析构函数的名字不同)

   + 虽然我们写的析构函数名字不同，但是编译器会将析构函数名称统一处理成**destructor**，这样它们就同名了，进而达成虚函数重写的条件。

   ```cpp
   class Person {
   public:
   	virtual ~Person() { cout << "~Person()" << endl; }
   };
   class Student : public Person {
   public:
   	virtual ~Student() { cout << "~Student()" << endl; }
   };
   // 只有派生类Student的析构函数重写了Person的析构函数，下面的delete对象调用析构函
   //数，才能构成多态，才能保证p1和p2指向的对象正确的调用析构函数。
   int main()
   {
   	Person* p1 = new Person;
   	Person* p2 = new Student;
   	delete p1;
   	delete p2;
   	return 0;
   }
   ```

### 3.4 override 和 final (C++11)

1. **final：修饰虚函数，表示该虚函数不能再被重写**

   ```cpp
   class Car
   {
   public:
   	virtual void Drive() final {}
   };
   class Benz :public Car
   {
   public:
   	virtual void Drive() { cout << "Benz-舒适" << endl; }	//会报错，无法完成重写
   };
   ```

   > **final**除了可以修饰虚函数，还可以修饰**类**，`final`修饰类后，则该类将无法继承
   >
   > ```cpp
   > class Base final {
   >     // 类的内容
   > };
   > 
   > // 错误：无法继承带有 final 的类
   > class Derived : public Base {
   >     // ...
   > };
   > ```
   >
   > 在上面的例子中，`Base` 类被标记为 `final`，因此任何尝试从 `Base` 继承的行为都会导致编译错误。

2. **override: 检查派生类虚函数是否重写了基类某个虚函数，如果没有重写编译报错。**

   ```cpp
   class Car {
   public:
   	virtual void Drive() {}
   };
   class Benz :public Car {
   public:
   	virtual void Drive() override { cout << "Benz-舒适" << endl; }	//避免函数名粗心写错
   };
   ```

### 3.5重载，重写，重定义的对比

![三重对比](D:\a.blog\C++\picture\三重对比.png)

## 4.抽象类

### 4.1概念

+ 在 C++ 中，**抽象类** 是包含至少一个**纯虚函数**的类。
+ 抽象类不能直接实例化，而是用作基类，为派生类提供接口和行为约定。
+ 抽象类用于定义一个统一接口，并强制所有派生类实现该接口中的特定功能。

### 4.2定义抽象类

要创建一个抽象类，可以在基类中声明至少一个纯虚函数。纯虚函数的定义方式是在函数声明后加上 `= 0`：

```cpp
class Shape {
public:
    // 纯虚函数：每个派生类都必须实现它
    virtual void draw() = 0;  
    virtual double area() = 0;
};
```

在上面的例子中，`Shape` 类包含两个纯虚函数 `draw()` 和 `area()`，因此 `Shape` 是一个抽象类，不能被直接实例化。任何从 `Shape` 派生的类都必须实现这两个函数。

### 4.3实现抽象类的派生类

派生类必须实现所有的纯虚函数，才能实例化对象。例如：

```cpp
class Circle : public Shape {
public:
    Circle(double r) : radius(r) {}

    void draw() override {
        // 实现绘制圆形的代码
    }

    double area() override {
        return 3.14159 * radius * radius;
    }

private:
    double radius;
};
```

在 `Circle` 类中，`draw()` 和 `area()` 函数被实现，因此 `Circle` 可以实例化。如果某个派生类没有实现基类的所有纯虚函数，则它也会被视为抽象类，不能实例化。

### 4.4接口继承和实现继承

+ 普通函数的继承是一种**实现继承**，派生类继承了基类函数，可以使用函数，继承的是函数的实现。
+ 虚函数的继承是一种**接口继承**，派生类继承的是基类虚函数的接口，目的是为了重写，达成多态，继承的是接口。
+ 所以如果不实现多态，不要把函数定义成虚函数。



