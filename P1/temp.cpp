#include <iostream>
#include <vector>
#include<list>
using namespace std;

class A {
public:
A() {cout << 0;}
~A() {cout << 9;}
};

class Embedded {
public:
Embedded() {cout << 1;}
~Embedded() {cout << 2;}
};
class Grandparent {
public:
Grandparent() {cout << 3;}
~Grandparent() {cout << 4;}
};
class Parent: public Grandparent {
A a;
public:
Parent() {cout << 5;}
~Parent() {cout << 6;}
private:
Embedded embed;
};
class Child: public Parent {
public:
Child() {cout << 7;}
~Child() {cout << 8;}
};

// int& foo(){
//     int x = 42;
//     return x;
// }

class HD{
private:
    int p = 18;
public:
    HD(int val) : p(val) {}
    void display() {
        cout << "Program: " << p << endl;
    }
};

class Galaxy{
private:
    HD something;
public:
    void display() {
        cout << "galaxy ship" << endl;
    }
};

class pay{
public:
    virutal void foo(){
        cout << "A" << endl;
    }
};

class hay{
public:
    void foo(){
        cout << "B" << endl;
    }
};

class child : public hay, public pay{
public:

};



int main() {
    // int x = 6;
    // int arr[] = {2, 3, 5, 7, 11, 13, 17, 19};
    // int* p = arr + 1;
    // cout << *p << endl;
    // int* q = p + x;
    // cout  << "A: " << *q << endl;
    // *p = x;
    // p++;
    // cout << "B: " << *p << endl;
    // Dog fido("fido");
    // fido.eat();
    // Galaxy n;
    // HD hd;
    // hd.display();
    //n.display();
    child a;
    a.foo();
}
