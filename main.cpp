#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;
mutex m1;
mutex m2;

template<typename Function>
void swap_lock_lock(mutex &m1, mutex &m2, Function process) {
    lock(m1,m2);
    lock_guard<mutex> l1(m1, adopt_lock);
    lock_guard<mutex> l2(m2, adopt_lock);
    process();
}

template<typename Function>
void swap_scoped_lock(mutex &m1, mutex &m2, Function process) {
    scoped_lock L{ m1,m2 };
    process();
}

template<typename Function>
void swap_unique_lock(mutex &m1, mutex &m2, Function process) {
    unique_lock<mutex> L1{ m1, defer_lock };
    unique_lock<mutex> L2{ m2, defer_lock };
    lock(L1,L2);
    process();
}



void deadlock_func_1()
{
    swap_lock_lock(m1,m2, [&]()
    {
        cout << "swap mutex lock guards func 1..." << endl;
        this_thread::sleep_for(1000ms);
        cout << "print A" << endl;
    });
    swap_scoped_lock(m1,m2, [&]()
    {
        cout << "swap mutex scoped lock func 1..." << endl;
        this_thread::sleep_for(1000ms);
        cout << "print B" << endl;
    });
    swap_unique_lock(m1,m2, [&]()
    {
        cout << "swap mutex unique locks func 1..." << endl;
        this_thread::sleep_for(1000ms);
        cout << "print C" << endl;
    });
}

void deadlock_func_2()
{
    swap_lock_lock(m1,m2, [&]()
    {
        cout << "swap mutex lock guards func 2..." << endl;
        this_thread::sleep_for(500ms);
        cout << "print A" << endl;
    });
    swap_scoped_lock(m1,m2, [&]()
    {
        cout << "swap mutex scoped lock func 2..." << endl;
        this_thread::sleep_for(500ms);
        cout << "print B" << endl;
    });
    swap_unique_lock(m1,m2, [&]()
    {
        cout << "swap mutex unique locks func 2..." << endl;
        this_thread::sleep_for(500ms);
        cout << "print C" << endl;
    });
}

int main()
{
	thread t1(deadlock_func_1);
	thread t2(deadlock_func_2);
	t1.join();
	t2.join();

	return 0;
}