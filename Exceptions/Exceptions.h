#ifndef WAREHOUSE_EXCEPTIONS_H
#define WAREHOUSE_EXCEPTIONS_H

#include <exception>

namespace Exceptions{
    class WarehouseIdNotFound : public std::exception {
    public:
        const char* what() const noexcept override {
            return "[!] Склад с таким айди не найден !";
        }
    };
    class NoEnoughCapacity : public std::exception {
    public:
        const char* what() const noexcept override {
            return "[!] На складе не хватает вместимости !";
        }
    };
    class SameIdError : public std::exception {
    public:
        const char* what() const noexcept override {
            return "[!] Склад с таким айди уже существует !";
        }
    };
    class WorkerNotFound: public std::exception {
    public:
        const char* what() const noexcept override {
            return "[!] Работник с таким айди не найден !";
        }
    };
}

#endif