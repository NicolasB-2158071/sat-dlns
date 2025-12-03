#include "dimacs.hpp"
#include "dlns.hpp"

#include <stdexcept>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        throw std::runtime_error{"Usage ./dlns ~.cnf"};
    }

    try
    {
        DLNS solver{};
        Dimacs{}.parse(argv[1], solver);
        solver.solve({});
        std::cout << solver.output();

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what();
    }
}