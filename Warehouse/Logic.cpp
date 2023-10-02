#include <algorithm>
#include "Logic.h"
#include <Windows.h>
namespace Warehouse {

    /**
     * Обновление остатков на складе
     * @param value кол-во товара. если надо убрать передаем отрицательное значение
     */
    void Warehouse::SetStocks(int value) {
        this->stock_+=value;
    }

    /**
     * Обновление вместимости склада
     * @param value
     */
    void Warehouse::UpdateCapacity(int value) {
        this->capacity_+=value;
    }





    /**
 * Приемка товара на склад
 * @param quantity
 * @param warehouse_id
 */
    void WarehousesContainer::ShipGoods(int quantity, int warehouse_id) {
        // Проверка на существование склада ОТКУДА и КУДА
        if (!DoesExist(warehouse_id)) {
            throw Exceptions::WarehouseIdNotFound();
        }
        Warehouse* warehouse = GetWarehouse(warehouse_id);
        /** проверяем, что на складе есть достаточно места
         *
         * объем поставки + кол-во товара на складе <= объем склада
         *
         * иначе сообщаем, что не хватает места на складе
         * */
        if (quantity+warehouse->GetStock() <= warehouse->GetCapacity()){
            warehouse->SetStocks(quantity);
            std::cout << "Приемка товара на склад: " << GetWarehouse(warehouse_id)->GetName() <<  ". Текущий запас: " << GetWarehouse(warehouse_id)->GetStock() << std::endl;
        }else{
            throw Exceptions::NoEnoughCapacity();
        }
    }


    /**
     * Отправка товаров между складами
     * @param quantity кол-во товара
     * @param warehouse_FROM_id откуда посылаем
     * @param warehouse_TO_id куда принимаем
     */
    void WarehousesContainer::ShipGoodsBetweenWarehouses(int quantity, int warehouse_FROM_id, int warehouse_TO_id) {
        if (!DoesExist(warehouse_FROM_id) || !DoesExist(warehouse_TO_id)) {
            throw Exceptions::WarehouseIdNotFound();
        }

        // Получение указателей на склады
        Warehouse* warehouse_FROM = GetWarehouse(warehouse_FROM_id);
        Warehouse* warehouse_TO = GetWarehouse(warehouse_TO_id);

        // Проверка наличия кол-ва товара на складе ОТКУДА
        if (warehouse_FROM->GetStock() < quantity) {
            std::cout << "Недостаточно товара на складе " << warehouse_FROM->GetName() << std::endl;
            return;
        }

        // Проверка наличия места на складе КУДА
        if (warehouse_TO->GetCapacity() < quantity) {
            std::cout << "Недостаточно места на складе " << warehouse_TO->GetName() << std::endl;
            return;
        }

        // Отгрузка товара со склада ОТКУДА
        if (quantity+warehouse_TO->GetStock() <= warehouse_TO->GetCapacity()){
            warehouse_TO->SetStocks(quantity);
            warehouse_FROM->SetStocks(-quantity);
            std::cout << "Товар отгружен со склада " << warehouse_FROM->GetName() << " >>> " << warehouse_TO->GetName() << ". Текущий запас: " << warehouse_FROM->GetStock() << std::endl;
        }else{
            throw Exceptions::NoEnoughCapacity();
        }
    }

    /**
     * Обновление остатков склада, внутри контейнера
     * @param value значение
     * @param warehouse_id айди склада
     */
    void WarehousesContainer::UpdateCapacity(int value, int warehouse_id) {
        if (DoesExist(warehouse_id)) {
            Warehouse *warehouse = GetWarehouse(warehouse_id);
            warehouse->UpdateCapacity(value);
        }else{
            throw Exceptions::WarehouseIdNotFound();
        }
    }



    /**
     * Методы для управления запасами на складе
     * @param quantity количество товара
     */
    void WarehousesContainer::ReceiveGoods(int quantity) {

        if (stock_ + quantity > capacity_) {
            std::cout << "Недостаточно места на складе!" << std::endl;
        } else {
            stock_ += quantity;
            std::cout << "Товар получен. Текущий запас: " << stock_ << std::endl;
        }
    }

    /**
     * Добавление склада в контейнер
     * @param warehouse элемент класса Склад
     */
    void WarehousesContainer::addWarehouse(const Warehouse &warehouse) {
        for (const auto& w : warehouses_) {
            if (w.GetId() == warehouse.GetId()) {
                throw Exceptions::SameIdError(); // Генерация исключения при обнаружении склада с таким же айди
            }
        }
        warehouses_.push_back(warehouse); //добавляем склад к нашему хранилищу
    }



    /**
         * Функция получения ссылки на объект класса Warehouse(склад). Ссылка используется чтобы избежать
         * перевыделения памяти на объект склада.
         *
         * Получаем айди скалада во входных параметрах --> Проверяем, что такой вообще существует, иначе выкидываем ошибку,
         * прописанную в кастомных ошибках класса Exceptions::WarehouseIdNotFound() --> такой склад точно есть, значит просто
         * ищем его в списке актуальных складов.
         *
         * Можно оптимизировать: метод GetWarehouse может внутри себя проверять существование склада
         * (убрать DoesExist, изменив GetWarehouse, либо сразу же там же возвращать ссылку на Warehouse, если такой нашли)
         * Но я решил разграничить этот функционал, чтобы было понятнее, а также для работы методов ниже.
         *
         * @param warehouse_id получаемый айди склада
         * @return ссылка на объект класса Warehouse - (по сути сам склад)
         */
    Warehouse *WarehousesContainer::GetWarehouse(int warehouse_id) {
        if (DoesExist(warehouse_id)){
            for (auto& warehouse : warehouses_) {
                if (warehouse.GetId() == warehouse_id) {
                    return &warehouse;
                }
            }
        }else{
            throw Exceptions::WarehouseIdNotFound();
        }
    }



    /**
 * проверка на существование некоторого склада с принимаемым айди
 * @param warehouse_id принимаемое значение айди, проверяем далее, содержит ли список
 * актуальных-рабочих складов склад с таким айди
 * @return TRUE-ЕСЛИ СУЩЕСТВУЕТ; FALSE-ЕСЛИ СКЛАД НЕ НАЙДЕН
 */
    bool WarehousesContainer::DoesExist(int warehouse_id) {
        for (auto& warehouse : warehouses_) {
            if (warehouse.GetId() == warehouse_id) {
                return true;
            }
        }
        return false;
    }

    void WarehousesContainer::addWorker(const WarehouseWorker &worker, int warehouseId) {
        if (DoesExist(warehouseId)){
            for (auto& warehouse : warehouses_) {
                if (warehouse.GetId() == warehouseId) {
                    warehouse.addWorker(worker);
                }
            }
        }else{
            throw Exceptions::WarehouseIdNotFound();
        }
    }

    void WarehousesContainer::dismissWorker(int workerId, int warehouseId) {
        if (DoesExist(warehouseId)){
            for (auto& warehouse : warehouses_) {
                if (warehouse.GetId() == warehouseId) {
                    warehouse.dismissWorker(workerId);
                }
            }
        }else{
            throw Exceptions::WarehouseIdNotFound();
        }
    }


    /**
     * конструктор, тривиальное создание
     */

    Warehouse::Warehouse() = default;

    void Warehouse::addWorker(const WarehouseWorker& worker) {
        std::cout<<"На склад " << this->name_ << " был добавлен работник: " << worker.GetName() << " " << worker.GetSurname() << ". ID: " << worker.GetId() << "\n";
        this->workers_.push_back(worker);
    }

    /**
     *  находим внутри итератора вектора текущего работника встроенной в итератор функцией find_if
     *  если не нашли it == workers_[end()] (конец кусков памяти, выделенного на вектор)
     *  тогда сообщаем, что работник не найден.
     *  иначе, мы нашли работника с таким айди. удаляем его.
     * @param workerId айди работника
     */
    void Warehouse::dismissWorker(int workerId) {
        auto it = std::find_if(workers_.begin(), workers_.end(), [workerId](const WarehouseWorker& worker) {
            return worker.GetId() == workerId;
        });

        if (it == workers_.end()) {
            throw Exceptions::WorkerNotFound();
        }

        std::cout << "Cо склада " << this->name_ << " был уволен " << it->GetName() << ". ID:"<<it->GetId() << "\n";
        workers_.erase(it);
    }


    /**
     * Поиск работника по айди.
     * @param workerId айди работника для поиска
     * @return *worker(указатель на работника внутри склада, если такой найден) иначе NULLPTR
     */
    WarehouseWorker *Warehouse::DoesExist(int workerId) {
        for(auto worker: this->workers_){
            if(worker.GetId() == workerId) return &worker;
        }
        return nullptr;
    }

    void WarehousesContainer::DisplayWarehouses() {
        SetConsoleOutputCP(CP_UTF8);
        if (this->warehouses_.empty()) {
            std::cout << "Нет доступных складов для отображения.\n";
        } else {
            std::cout << "Список складов:\n";
            for (const auto& warehouse : warehouses_) {

                std::cout << "-------------------\n";
                std::cout << "Название склада: " << warehouse.GetName() << std::endl;
                std::cout << "Вместимость склада: " << warehouse.GetCapacity() << std::endl;
                std::cout << "ID склада: " << warehouse.GetId() << std::endl;
                std::cout << "-------------------\n";
            }
        }
    }
}