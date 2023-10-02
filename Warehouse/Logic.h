#ifndef WAREHOUSE_LOGIC_H
#define WAREHOUSE_LOGIC_H

#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include "../Exceptions/Exceptions.h"
#include "../Creatures/Human.h"

namespace Warehouse {
    class Warehouse{
    protected:
        /**
         * защищенные параметры класса
         * видно только из методов. напрямую получить нельзя
         * полезно в случае разраничения ответственности работы сервисных классов
         * когда метод не должен получать инофрмацию о содержимом класса,
         * а только работать со своей прописанной логикой.
         */
        std::string address;      /** адрес **/
        std::string name_;        /** название склада **/
        int id{};                   /** айди склада в системе **/
        int capacity_{};            /** вместимость склада **/
        int stock_{};               /** сколько на складе сейчас товара (остатки) **/
        std::vector<WarehouseWorker> workers_; /** работники склада **/
        std::vector<Supplier> suppliers_;      /** поставщики склада **/
    private:
        WarehouseWorker* DoesExist(int workerId);
        std::vector<WarehouseWorker> GetAllWorkers() { return this->workers_;};
    public:
        // Конструктор
        /**
         * Конструктор создания класса - Склад. Имеет параметры:
         * @param name название склада
         * @param capacity вместимость склада
         * @param _id айди склада в системе
         */
        Warehouse(std::string name, int capacity, int _id): name_(std::move(name)),
        capacity_(capacity), stock_(0), id(_id) {}

        Warehouse();

        // Методы доступа к полям класса
        std::string GetName() const { return name_; }
        int GetCapacity() const { return capacity_; }
        int GetStock() const { return stock_; }
        int GetId() const { return id; };

        void SetStocks(int value);
        void UpdateCapacity(int value);

        void addWorker(const WarehouseWorker& worker);

        void dismissWorker(int workerId);

    };
    class WarehousesContainer: public Warehouse{
    protected:
        std::vector<Warehouse> warehouses_;

    public:

        bool DoesExist(int warehouse_id);

        // поиск и добавления складов в контейнер
        Warehouse* GetWarehouse(int warehouse_id);
        void addWarehouse(const Warehouse& warehouse);

        //методы работы с работниками внутри контейнера
        void addWorker(const WarehouseWorker& worker, int warehouseId);
        void dismissWorker(int workerId, int warehouseId);

        void DisplayWarehouses();

        //методы работы со складами внутри контейнера
        void ReceiveGoods(int quantity);
        void ShipGoodsBetweenWarehouses(int quantity, int warehouse_FROM_id, int warehouse_TO_id);
        void ShipGoods(int quantity, int warehouse_id);
        void UpdateCapacity(int value, int warehouse_id);
    };
}

#endif