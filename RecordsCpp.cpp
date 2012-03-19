#include "stdafx.h"

#include <cassert>
#include <string>
#include <type_traits>
#include <cstring>

using namespace std;

// Obvious way
namespace Obvious {
	struct Person {
		const string Name;
		const int Salary;

		bool operator==(const Person& other) const { return Salary == other.Salary && Name == other.Name;}
		bool operator!=(const Person& other) const { return !(*this == other);}
	};
}

// Non obvious way, but better syntax for copy & modify
namespace Mutable {
	struct Person {
		string Name;
		int Salary;

		bool operator==(const Person& other) const { return Salary == other.Salary && Name == other.Name;}
		bool operator!=(const Person& other) const { return !(*this == other);}
	};
}

namespace Immutable {
	typedef const Mutable::Person Person;
}

namespace Pod {
	struct Person {
		char Name[20];
		int Salary;

		bool operator==(const Person& other) const { return Salary == other.Salary && !strcmp(Name,other.Name);}
		bool operator!=(const Person& other) const { return !(*this == other);}
	};	
}

namespace Mutable {

    struct foo {
        int value1;
        int value2;
        int value3; 
        int value4;
        int value5;
        int value7;
        int value6;
        int value8;
        int value9;
    };
}

typedef const Mutable::foo foo;

foo increment_value7( const foo& f )
{
     foo tmp = { f.value1, f.value2, f.value3, f.value4, f.value5, f.value6, f.value7+1, f.value8 };
     return tmp;
}

template<class T>
T rise_salary1(const T& p) {
	T ret = { p.Name, p.Salary + 1000 };
	return ret;
}

template<class T>
T rise_salary2(const T& p) {
	T ret(p);
	ret.Salary += 1000;
	return ret;
}

template<class T>
T rise_salary3(T p) {
	p.Salary += 1000;
	return p;
}

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define MAKE_UNIQUE(x) CONCATENATE(x, __LINE__)

#define RECORD1(n, t1, f1)																	\
	struct n {                                                                              \
		const t1 f1;																		\
																							\
		bool operator==(const n& other) const { return f1 == other.f1;}						\
		bool operator!=(const n& other) const { return !(*this == other);}					\
	};

#define RECORD2(n, t1, f1, t2, f2)															\
	struct n {                                                                              \
		const t1 f1;																		\
		const t2 f2;																		\
																							\
		bool operator==(const n& other) const { return f1 == other.f1 && f2 == other.f2;}	\
		bool operator!=(const n& other) const { return !(*this == other);}					\
	};

#define RECORD3(n, t1, f1, t2, f2, t3, f3)													\
	struct n {                                                                              \
		const t1 f1;																		\
		const t2 f2;																		\
		const t3 f3;																		\
																							\
		bool operator==(const n& other) const { return f1 == other.f1 && f2 == other.f2 && f3 == other.f3;}	\
		bool operator!=(const n& other) const { return !(*this == other);}					\
	};

RECORD1(Person1,
		int,		Salary);

RECORD2(Person2,
	      string,	Name,
	      int,		Salary);

RECORD3(Person3,
	      string,	Name,
	      int,		Salary,
		  double,	Quantity);

Immutable::Person rise_salary_m(const Immutable::Person& p) {
	Mutable::Person ret(p);
	ret.Salary += 1000;
	return ret;		
}

int main(int argc, char* argv[])
{
	assert(!is_trivial			<Obvious::Person>::value);
	assert(!is_standard_layout	<Obvious::Person>::value); // gcc disagrees
	assert(!is_trivial			<Mutable::Person>::value);
	assert(!is_standard_layout	<Mutable::Person>::value);  // gcc disagrees
	assert(!is_trivial			<Immutable::Person>::value);
	assert(!is_standard_layout	<Immutable::Person>::value); // gcc disagrees
	assert(is_trivial			<Pod::Person>::value);
	assert(is_standard_layout	<Pod::Person>::value);

	const int startSalary	= 10000;
	const int endSalary		= 11000;

	Obvious::Person		op = {"Bob", startSalary};
	Immutable::Person	ip = {"Ron", startSalary};
	Mutable::Person		mp = {"Dod", startSalary};
	Pod::Person			pp = {"Dod", startSalary};

	assert(rise_salary1(op).Salary == endSalary);
	assert(rise_salary1(ip).Salary == endSalary);
	assert(rise_salary1(mp).Salary == endSalary);
	
	assert(rise_salary2(mp).Salary == endSalary);
	assert(rise_salary2(pp).Salary == endSalary);
	assert(rise_salary3(mp).Salary == endSalary);
	assert(rise_salary3(pp).Salary == endSalary);

	assert(rise_salary_m(ip).Salary == endSalary);
	assert(rise_salary_m(ip).Salary != ip.Salary);
	
	auto opc = op;
	assert(opc == op);

	auto ppc = pp;
	assert(ppc == pp);

	return 0;
}

