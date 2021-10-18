## lambda function

The C++ concept of a lambda function originates in the lambda calculus and functional programming. A lambda is an unnamed function that is useful (in actual programming, not theory) for short snippets of code that are impossible to reuse and are not worth naming.

In C++ a lambda function is defined like this

```cpp
[]() { } // barebone lambda
```

or in all its glory

```cpp
[]() mutable -> T { } // T is the return type, still lacking throw()
```

`[]` is the capture list, `()` the argument list and `{}` the function body.

##### The capture list

The capture list defines what from the outside of the lambda should be available inside the function body and how. It can be either:

1. a value: [x]
2. a reference [&x]
3. any variable currently in scope by reference [&]
4. same as 3, but by value [=]

You can mix any of the above in a comma separated list `[x, &y]`.

##### The argument list

The argument list is the same as in any other C++ function.

##### The function body

The code that will be executed when the lambda is actually called.

##### Return type deduction

If a lambda has only one return statement, the return type can be omitted and has the implicit type of `decltype(return_statement)`.

##### Mutable

If a lambda is marked mutable (e.g. `[]() mutable { }`) it is allowed to mutate the values that have been captured by value.



how to capture with `std::move`:

```cpp
auto ptr = std::make_unique<int>(10);
auto lambda = [ptr = std::move(ptr)] {return *ptr;};
```

## rvalue and lvalue

##### Crude definition

1. **lvalue** : A value that resides in **memory (heap or stack)** and **addressable**.
2. **rvalue** : A value that's not lvalue. It resides only on the right side of an assignment expression such as a literal or a temporary which is intended to be **non-modifiable**.

A useful heuristic to determine whether an expression is an **lvalue** is to ask if you can take its address. If you can, it typically is. If you can't, it's usually an **rvalue**.

##### Reference

The references that we've been using are **lvalue** references - references to **lvalues**. The **lvalues** references can only be bound to **lvalues** but not **rvalues**.

```c++
int i;
int &r; = i;
int &r; = 7; // error: lvalue cannot be bound to rvalue 7
```

we can bind an rvalue to a **const lvalue** reference (const reference). It's legal since when a compiler sees **const**, it converts 7 to an lvalue, and then assign it to the reference

```
const int&r; = 7;  // OK
```

The above rules also apply to functions:

```c++
int square(int& x) { return x*x;  }
int i = 7;
square(i);  // OK
square(7);  // error, 7 is an rvalue and cannot be assigned to a reference

int square(const int& x) { return x*x;  }
square(i);  // OK
square(7);  // OK
```

## rvalue Reference & move semantics

### rvalue reference

The primary purpose of introducing an rvalue is to implement **move semantics**

The C++11 Standard introduces rvalue references which bind only to rvalues. They are declared with two ampersands:

```c++
int&& rvalue_ref = 99;
```

We have two functions: one that's taking an lvalue reference and the other one taking an rvalue:

```c++
#include <iostream>

void f(int& i) { std::cout << "lvalue ref: " << i << "\n"; }
void f(int&& i) { std::cout << "rvalue ref: " << i << "\n"; }

int main()
{
    int i = 77;
    f(i);    // lvalue ref called
    f(99);   // rvalue ref called

    f(std::move(i));  // rvalue ref called

    return 0;
}
```

The **move(i)** is calling a function taking rvalue. This is possible because **move() takes an lvalue and converts it into an rvalue**.



An expression is an **rvalue** if it results in a temporary object as shown in the example below. The **getValue()** is an **rvalue**.

```c++
#include <iostream>

int getValue ()
{
    int ii = 10;
    return ii;
}

int main()
{
    std::cout << getValue();
    return 0;
}
```

In C++0x, we could use a **normal** or **lvalue reference** to bind **temporary** object, but only if it was **const**.

In C++11, however, the **rvalue reference** lets us bind a mutable reference to an **rvalue**, but not an **lvalue**. 

```c++
// C++ 11
const int&& val = getValue(); // OK
int&& val = getValue(); //  OK
// C++ 0x
const int& val = getValue(); // OK
int& val = getValue(); // NOT OK
```

##### rvalue references are perfect for detecting whether a value is a temporary object or not.

```c++
void printReference (const int& value)
{
        cout << value;
}
 
void printReference (int&& value)
{
        cout << value;
}
```

The printReference() function taking a **const lvalue** reference will accept any argument whether it be an **lvalue** or an **rvalue**. However, the second overloaded printReference() taking only an **rvalue** reference.

Obviously, this update enables better adaption to **function overload**.



##### rvalue summary:

1. **int &&a;**: new (C++11) declaration rvalue reference.
2. non-const **lvalue** reference binds to an **object**.
3. **rvalue** reference binds to a **temporary object** which typically will not be used again.



### Move semantics

The main usage of **rvalue** references is to create a **move constructor** and **move assignment operator**.

Here is the typical class definition using move semantics:

```c++
#include <iostream>
#include <algorithm>

class A
{
public:

    // Simple constructor that initializes the resource.
    explicit A(size_t length)
        : mLength(length), mData(new int[length]){
        std::cout << "A(size_t). length = "
		<< mLength << "." << std::endl;
    }

    // Destructor.
    ~A()
    {
	std::cout << "~A(). length = " << mLength << ".";

	if (mData != NULL) {
            std::cout << " Deleting resource.";
	    delete[] mData;  // Delete the resource.
	}

	std::cout << std::endl;
    }

    // Copy constructor.
    A(const A& other)
	    : mLength(other.mLength), mData(new int[other.mLength])
    {
	std::cout << "A(const A&). length = "
		<< other.mLength << ". Copying resource." << std::endl;

	std::copy(other.mData, other.mData + mLength, mData);
    }

    // Copy assignment operator.
    A& operator=(const A& other)
    {
	std::cout << "operator=(const A&). length = "
	         << other.mLength << ". Copying resource." << std::endl;

	if (this != &other;) {
	    delete[] mData;  // Free the existing resource.
	    mLength = other.mLength;
            mData = new int[mLength];
            std::copy(other.mData, other.mData + mLength, mData);
	}
	return *this;
    }

    // Move constructor.
    A(A&& other) : mData(NULL), mLength(0)
    {
        std::cout << "A(A&&). length = " 
             << other.mLength << ". Moving resource.\n";

        // Copy the data pointer and its length from the 
        // source object.
        mData = other.mData;
        mLength = other.mLength;

        // Release the data pointer from the source object so that
        // the destructor does not free the memory multiple times.
        other.mData = NULL;
        other.mLength = 0;
    }

    // Move assignment operator.
    A& operator=(A&& other)
    {
        std::cout << "operator=(A&&). length = " 
             << other.mLength << "." << std::endl;

        if (this != &other;) {
          // Free the existing resource.
          delete[] mData;

          // Copy the data pointer and its length from the 
          // source object.
          mData = other.mData;
          mLength = other.mLength;

          // Release the data pointer from the source object so that
          // the destructor does not free the memory multiple times.
          other.mData = NULL;
          other.mLength = 0;
       }
       return *this;
    }

    // Retrieves the length of the data resource.
    size_t Length() const
    {
        return mLength;
    }

private:
    size_t mLength; // The length of the resource.
    int* mData;     // The resource.
};

#include <vector>

int main()
{
   // Create a vector object and add a few elements to it.
   std::vector<A> v;
   v.push_back(A(25));
   v.push_back(A(75));

   // Insert a new element into the second position of the vector.
   v.insert(v.begin() + 1, A(50));
   return 0;
}
```

##### How it works

```C++
#include <vector>

int main()
{
    std::vector<A> v;
    A aObj(25);       // lvalue
    v.push_back(aObj);  // push_back(const T&)
    v.push_back(std::move(aObj));  //calls push_back(T&&)
}
```

##### Swapping objects using move()

```C++
/* move.cpp */
#include <iostream>
using namespace std;

class A
{
  public:
    // constructor
    explicit A(size_t length)
        : mLength(length), mData(new int[length]) {}

    // move constructor
    A(A&& other)
    {
      mData = other.mData;
      mLength = other.mLength;
      other.mData = nullptr;
      other.mLength = 0;
    }

    // move assignment
    A& operator=(A&& other) noexcept
    {
      mData =  other.mData;
      mLength = other.mLength;
      other.mData = nullptr;
      other.mLength = 0;
      return *this;
    }

    size_t getLength() { return mLength; }

    
    void swap(A& other)
    {
      A temp = move(other);
      other = move(*this);
      *this = move(temp);
    }

    int* get_mData() { return mData; }

  private:
    int *mData;
    size_t mLength;
};

int main()
{
  A a(11), b(22);
  cout << a.getLength() << ' ' << b.getLength() << endl;
  cout << a.get_mData() << ' ' << b.get_mData() << endl;
  swap(a,b);
  cout << a.getLength() << ' ' << b.getLength() << endl;
  cout << a.get_mData() << ' ' << b.get_mData() << endl;
  return 0;
}
```

Output:

```
$ g++ -std=c++11 -o move move.cpp
$ ./move
11 22
0x1870010 0x1870050
22 11
0x1870050 0x1870010
```

## Scopes

A scope in any programming is a region of the program where a defined variable can have its existence and beyond that variable it cannot be accessed. 

There are three places where variables can be declared in C programming language:

- Inside a function or a block which is called **local** variables.
- Outside of all functions which is called **global** variables.
- In the definition of function parameters which are called **formal** parameters.

##### Initializing Local and Global Variables

When a local variable is defined, it is not initialized by the system, you must initialize it yourself. Global variables are initialized automatically by the system when you define them as follows −

| Data Type | Initial Default Value |
| :-------: | :-------------------: |
|    int    |           0           |
|   char    |         '\0'          |
|   float   |           0           |
|  double   |           0           |
|  pointer  |         NULL          |

## Storage duration

Each variable in C/C++ has one of the following three storage durations:

- **Static**. For a variable with a static storage duration, its storage size and address are determined at compile time (before the program starts running); the lifetime of its storage is the entire program execution time. A variable declared at file/namespace scope has a static storage duration.
- **Automatic**. A local variable declared at block scope normally has an automatic storage duration. Local variables are stored in a run-time stack. Allocating storage for local variables usually takes just one machine instruction. Each time a function is called, a stack frame (a block of memory in the stack) is allocated for the function’s local variables, and the stack frame is deallocated when the function returns. Thus, for a variable with an automatic storage duration, the lifetime of its storage begins upon entry into the block immediately enclosing the object’s declaration and ends upon exit from the block.
- **Dynamic**. A local variable declared at block scope can have a dynamic storage duration if its storage is allocated by calling an allocation function, such as malloc() in C or the operator new in C++. Dynamic memory allocation allows a user to manage memory very economically. The drawback is that it is much slower than automatic allocation because it typically involves tens or hundreds of instructions. For a variable with a dynamic storage duration, the lifetime of its storage lasts until the memory is deallocated explicitly—say, by a free function in C or the operator delete in C++.

##### Storage class specifiers

The storage class specifiers are a part of the *decl-specifier-seq* of a name's [declaration syntax](https://en.cppreference.com/w/cpp/language/declarations). Together with the [scope](https://en.cppreference.com/w/cpp/language/scope) of the name, they control two independent properties of the name: its *storage duration* and its *linkage*.

| `auto` - automatic storage duration.                   | (until C++11)|
| ------------------------------------------------------------ | ----------------- |
| `register` - automatic storage duration. Also hints to the compiler to place the object in the processor's register. (deprecated) | (until C++17)|
| `static` - *static* or *thread* storage duration and *internal* linkage. | |
| `extern` - *static* or *thread* storage duration and *external* linkage. | |
| `thread_local` - *thread* storage duration. | (since C++11) |
| `mutable` - does not affect storage duration or linkage. | |

Only one storage class specifier may appear in a declaration except that `thread_local` may be combined with `static` or with `extern` (since C++11).

## Smart pointer

Smart pointers are class objects that behave like built-in pointers but also manage objects that you create with new so that you don't have to worry about when and whether to delete them - the smart pointers automatically delete the  managed object for you at the appropriate time.

C++11's smart pointer facility, which consists:

- **unique_ptr**, only one smart pointer owns the object at a time; when the owning smart pointer is destroyed, then the owned object is automatically destroyed.
- **shared_ptr**, shared ownership. Any number of these smart pointers  jointly own the object. The owned object is destroyed only when its last owning smart pointer is destroyed.
- **weak_ptr** doesn't own an object at all, and so plays no role in when or whether the object gets deleted. Rather, a **weak_ptr** merely observes objects being managed by **shared_ptrs**, and provides facilities for determining whether the observed object still exists or not.

## Function objects

Class template `std::function` is a general-purpose polymorphic function wrapper. Instances of `std::function` can store, copy, and invoke any [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable) *target* -- functions, [lambda expressions](https://en.cppreference.com/w/cpp/language/lambda), [bind expressions](https://en.cppreference.com/w/cpp/utility/functional/bind), or other function objects, as well as pointers to member functions and pointers to data members.

```c++
#include <functional>
#include <iostream>
 
struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_+i << '\n'; }
    int num_;
};
 
void print_num(int i)
{
    std::cout << i << '\n';
}
 
struct PrintNum {
    void operator()(int i) const
    {
        std::cout << i << '\n';
    }
};
 
int main()
{
    // store a free function
    std::function<void(int)> f_display = print_num;
    f_display(-9);
 
    // store a lambda
    std::function<void()> f_display_42 = []() { print_num(42); };
    f_display_42();
 
    // store the result of a call to std::bind
    std::function<void()> f_display_31337 = std::bind(print_num, 31337);
    f_display_31337();
 
    // store a call to a member function
    std::function<void(const Foo&, int)> f_add_display = &Foo::print_add;
    const Foo foo(314159);
    f_add_display(foo, 1);
    f_add_display(314159, 1);
 
    // store a call to a data member accessor
    std::function<int(Foo const&)> f_num = &Foo::num_;
    std::cout << "num_: " << f_num(foo) << '\n';
 
    // store a call to a member function and object
    using std::placeholders::_1;
    std::function<void(int)> f_add_display2 = std::bind( &Foo::print_add, foo, _1 );
    f_add_display2(2);
 
    // store a call to a member function and object ptr
    std::function<void(int)> f_add_display3 = std::bind( &Foo::print_add, &foo, _1 );
    f_add_display3(3);
 
    // store a call to a function object
    std::function<void(int)> f_display_obj = PrintNum();
    f_display_obj(18);
 
    auto factorial = [](int n) {
        // store a lambda object to emulate "recursive lambda"; aware of extra overhead
        std::function<int(int)> fac = [&](int n){ return (n < 2) ? 1 : n*fac(n-1); };
        // note that "auto fac = [&](int n){...};" does not work in recursive calls
        return fac(n);
    };
    for (int i{5}; i != 8; ++i) { std::cout << i << "! = " << factorial(i) << ";  "; }
}
```

## Exceptions

### noexcept

`noexcept `exists in two forms since C++11: as a specifier and as an operator. The C++ core guidelines uses the specifier.

##### noexcept as specifier

By declaring a function, a method, or a lambda-function as noexcept, you specify that these does not throw an exception and if they throw, you do not care and let the program just crash.

```c++
void func1() noexcept;        // does not throw
void func2() noexcept(true);  // does not throw
void func3() throw();         // does not throw

void func4() noexcept(false); // may throw
```

##### noexcept as operator

The `noexcept` operator checks at compile-time if an expression does not throw an exception. The `noexcept`operator does not evaluate the expression. It can be used in a noexcept specifier of a function template to declare that the function may throw exceptions depending on the current type.

##### What does it *mean* that exceptions are “slow”?

Mainly it means that a `throw` can take a Very Long Time™ compared to e.g. an `int` assignment, due to the search for handler.

## Object oriented programming

### explicit

The `explicit` keyword is an optional decoration for constructors that take exactly one argument. It only applies to single-argument constructors since those are the only constructors that can be used in type casting.

### =default

A default constructor is a constructor which can be called with no arguments (either defined with an empty parameter list, or with default arguments provided for every parameter).

### member initializer list

- [ ] TODO

### Virtual functions

A pure virtual function is a virtual function in C++ for which we need not to write any function definition and only we have to declare it. It is declared by assigning 0 in the declaration.

### Abstract classes

An abstract class is a class in C++ which have at least one pure virtual function.

- Abstract class can have normal functions and variables along with a pure virtual function.
- Abstract class cannot be instantiated, but pointers and references of Abstract class type can be created.
- Abstract classes are mainly used for Upcasting, so that its derived classes can use its interface.
- If an Abstract Class has derived class, they must implement all pure virtual functions, or else they will become Abstract too.
- We can't create object of abstract class as we reserve a slot for a pure virtual function in Vtable, but we don’t put any address, so Vtable will remain incomplete.

### Friend

**Friend functions** of the class are granted permission to access private and protected members of the class in C++. They are defined globally outside the class' scope. Friend functions are not member functions of the class.

Having friends in programming is more-or-less considered "dirty" and easy to abuse. It breaks the relationships between classes and undermines some fundamental attributes of an OO language.

Another disadvantage is the appearance of objects with the same name, but different scope.

### Override

override is a feature to use without moderation! Every time you define a method in the derived class that overrides a `virtual` method in the base class, you should tag it override.


### Final

`final` came along with `override` in the C++11 standard. Like `override`, you can tag your virtual member function with `final`, which prevents any derived class of `Derived` to override the member function `f`.

## Data Structure

### B-trees

A B-tree of order **m** is a tree which satisfies the following properties:

1. Every node has at most *m* children.
2. Every non-leaf node (except root) has at least ⌈*m*/2⌉ child nodes, at most m-1 child nodes.
3. The root has at least two children if it is not a leaf node.
4. A non-leaf node with *k* children contains *k* − 1 keys.
5. All leaves appear in the same level and carry no information.

![img](https://pic2.zhimg.com/80/v2-2c2264cc1c6c603dfeca4f84a2575901_720w.jpg)

### advantages in database

In particular, a B-tree:

- keeps keys in sorted order for sequential traversing
- uses a hierarchical index to minimize the number of disk reads
- uses partially full blocks to speed up insertions and deletions
- keeps the index balanced with a recursive algorithm

In addition, a B-tree minimizes waste by making sure the interior nodes are **at least half full**. A B-tree can handle an arbitrary number of insertions and deletions.

