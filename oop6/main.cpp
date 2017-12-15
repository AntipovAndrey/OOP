#include <iostream>
#include <vector>
#include "vector.h"

struct Foo {
    Foo(int a) : b(a) { std::cout << "constr " << a << " \n"; }

    Foo &operator=(Foo &&o)  noexcept {
        b = (o.b);
        std::cout << " c Foo && \n";
    };

    Foo &operator=(const Foo &o) {
        b = (o.b);
        std::cout << " c const Foo & \n";
    };

    Foo(const Foo &o)  noexcept  : b(o.b) { std::cout << "const Foo & \n"; };

    Foo(Foo &&o) noexcept  : b(o.b) { std::cout << "Foo && \n"; };

    int b;
};




int main() {
/*
    using namespace andrey;
    {
        std::cout << "std::vector\n";
        std::vector<Foo> f;
        std::cout << "~~call push_back\n";
        int i = 3;
        Foo foo(i);
        f.push_back(foo);

        std::cout << "~~call emplace_back\n";
        int i1 = 6;
        f.emplace_back(i1);
        for (auto &it : f) {
            std::cout << it.b << " ";
        }
        std::cout << std::endl;
    }
       std::cout << "-----------\n";
       {
           std::cout << "andrey::Vector\n";

           Vector<Foo> f;
           std::cout << "~~call push_back\n";
           int i = 3;
           Foo foo(i);
           f.push_back(foo);

           std::cout << "~~call emplace_back\n";
           int i1 = 6;
           f.emplace_back(i1);

           for (auto &it : f) {
               std::cout << it.b << " ";
           }
       }*/

    andrey::Vector<int> v;

    return 0;
}


