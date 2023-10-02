#ifndef WAREHOUSE_HUMAN_H
#define WAREHOUSE_HUMAN_H

#include <iostream>
#include <string>
#include <utility>

class Human {
protected:
    /**
     * protected - означает, что получить доступ к этим значениям у объекта класса нельзя
     * напрямую, только обращаясь к заранее прописанным методам, например, GetName()
     */
    std::string name_;
    std::string surname_;
    int id_{};
    std::string unique_id_;

public:
    // Конструктор
    /**
     * Конструктор создания человека, по сути, абстрактный класс, необходимый для классов WarehouseWorker и Supplier
     * @param name имя человека
     * @param surname фамилия человека
     *
     * std::move просто поправил Clang-Tide - перемещаем кусочек памяти, чтобы избежать копирование
     */
    Human(std::string name, std::string surname)
            : name_(std::move(name)), surname_(std::move(surname)) {}

    // Методы доступа к полям
    std::string GetName() const { return name_; }
    std::string GetSurname() const { return surname_; }
};

class WarehouseWorker : public Human {
private:
    int id_;
    std::string unique_id_; // string
                            // си. указатель на кусок памяти,
                            // где будет лежать уникальный айди.
                            // Стринг - потому что чаще всего уникальный айди в таких системах
                            // это хэш-код.
public:
    /**
     * конструктор класс (создает элемент класса WarehouseWorker) - работника склада
     * @param name имя работника
     * @param surname фамилия работника
     * ^^^^
     * наследуется от Human
     *
     *
     *
     * собственные поля
     * @param id айди на текущем складе
     * @param unique_id айди в общей системе
     */
    WarehouseWorker(std::string name, std::string surname, int id, std::string unique_id)
            : Human(std::move(name), std::move(surname)), id_(id), unique_id_(unique_id) {}

    int GetId() const { return id_; }
    std::string GetUniqueId() const { return unique_id_; }
    // Другие методы для работника склада
};

class Supplier : public Human {
private:
    int id_;
    const char * unique_id_;
public:
    // Конструктор
    /**
     * конструктор класса Supplier (поставщик) - создает "карточку" поставщика
     * @param name имя
     * @param surname фамилия
     * @param id айди на складе
     * @param unique_id айди в системе
     */
    Supplier(std::string name, std::string surname, int id, const char *unique_id)
            : Human(std::move(name), std::move(surname)), id_(id), unique_id_(unique_id) {}
    int GetId() const { return id_; }
    const char * GetUniqueId() const { return unique_id_; }
    // Другие методы для поставщика товара
};

#endif
