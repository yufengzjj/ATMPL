# ATMPL

**all library components write in c++11 without any other dependency.**

at present, this ATMPL--Another Template Meta Programming Library has a little supported  

features as follow:  

### functional_helper  
include function traits and a enhanced bind template class.  

#####function_traits  
 it provide a template class named function_traits<template T>. 
you can obtain type information on compile-time like return type, 
arguments' type. it support function pointer,member function pointer, 
member object pointer,lambda and functor.(no support for bind expression,see next).  

#####bind--enhanced std::bind  
 this version of bind also supports whatever std::bind supports for binding. 
it allow you obtain type information of the bonded callable objects as function_traits provided. 
bind::underlying_type is the very original callable type. it can obtain very original callable type information 
of any recursive bind(not std::bind) you have made.  

  

all of it programmed with standard c++11. 
this library supports any compilers as long as they support c++11 standard.

  

examples: 
`void test(int,char*){}` 
`auto f1 = functional_helper::bind(test, _1,_2);` 
`auto f2 = functional_helper::bind(f1, _1, _2);` 
`auto test_lambda = [](int, char*)->void{};` 
obtain argument: 
`functional_helper::function_traits<decltype(test)>::arg<1>::type//char*` 
`decltype(f1)::underlying_type::arg<1>::type //char*` 
`decltype(f2)::underlying_type::arg<1>::type //char*` 
`decltype(test_lambda)::underlying_type::arg<1>::type //char*`  



### Any

Safe, generic container for single values of different value types. **similar to [boost.Any](http://www.boost.org/doc/libs/1_60_0/doc/html/any.html)**

example:


```c++
template<typename _T>
void pod_test(any &v)
{
  //print content in any container.
	std::cout << "any cast to " << typeid(_T).name() 
      << " : " << any::cast<_T>(v) <<std::endl;
}
any ret()
{
	int i = 1;
	return any(i);
}
any v(short(1));
pod_test<short>(v); // print 1

any v_int(int(2));
pod_test<int>(v_int);// print 2

any v_long(long(3));
pod_test<long>(v_long);// print 3

any v_longlong((long long)(4));
pod_test<long long>(v_longlong);// print 4

any v_char(char('5'));
pod_test<char>(v_char);// print 5

char str[] = "hello world";
any v_cptr(str);
pod_test<char const *>(v_cptr);// print "hello world"

any v_string(std::string("HI! string any cast"));
pod_test<std::string>(v_string);// print "HI! string any cast"

any any_0;
any any_1(ret());
pod_test<int>(any_1);// print 1

any any_2(any_1);
pod_test<int>(any_2);// print 1

any any_3 = ret();
pod_test<int>(any_3);// print 1

any any_4 = any_1;
pod_test<int>(any_4);// print 1

any any_5 = any(int(5));
pod_test<int>(any_5);// print 5

std::vector<any> any_vec;
any_vec.push_back('A');
any_vec.push_back(short(1));
any_vec.push_back(int(2));
any_vec.push_back(str);
any_vec.push_back(std::string("hello any,world"));
pod_test<char>(any_vec[0]);// print A
pod_test<short>(any_vec[1]);// print 1
pod_test<int>(any_vec[2]);// print 2
pod_test<char const *>(any_vec[3]);// print "hello world"
pod_test<std::string>(any_vec[4]);// print "hello any,world"
```
**all test on VS2015/GCC 5.3.0/Clang 3.8.0**