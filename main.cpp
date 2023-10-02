#include <iostream>
#include "Creatures/Human.h"
#include <Windows.h>
#include "Warehouse/Logic.h"
#include <random>

// проверка корректности введенных данные (не завершится, пока не будет введено число)
void inInt(int& n, const std::string& textOut) {
    bool validInput = false;
    do {
        std::cout << textOut;
        std::string input;
        //очищаем буффер CIN
        _flushall();
        std::cin >> input;

        try {
            n = std::stoi(input);
            validInput = true;
        } catch (std::exception& e) {
            std::cout << "Неверный ввод. Пожалуйста, введите число!\n";
        }
    } while (!validInput);
}

void enter_var(std::string &NAME, int &CAPACITY, int &ID) {
    std::cout << "Введите название склада: ";
    std::cin >> NAME;
    _flushall();
    inInt(CAPACITY, "Введите вместимость склада: ");
    inInt(ID,  "Введите ID склада: ");

    std::cout << std::endl;
}


int main() {
    // Устанавливаем кодовую страницу 65001 (UTF-8)
    // вывод русских символов в консоль
    SetConsoleOutputCP(CP_UTF8);

    // Создание объекта контейнера для складов где лежат складики
    Warehouse::WarehousesContainer warehouseContainer;

    int choice = -1;
    while (choice != 0) {
        std::cout << "Выберите операцию:\n";
        std::cout << "0 - Выход\n";
        std::cout << "1 - Добавить склад\n";
        std::cout << "2 - Сделать поставку на склад\n";
        std::cout << "3 - Обновить размер склада\n";
        std::cout << "4 - Перевезти товары между складами\n";
        std::cout << "5 - Добавить работника на склад\n";
        std::cout << "6 - Уволить работника со склада\n";
        std::cout << "7 - Вывести все склады и их данные\n";

        std::string input;
        std::cin >> input;

        try {
            choice = std::stoi(input);
        } catch (std::exception& e) {
            std::cout << "Неверный ввод. Пожалуйста, введите число!\n";
            continue;
        }

        switch (choice) {
            case 0:
                std::cout << "Программа завершена.\n";
                break;
            case 1: {
                std::string WH_Name;
                int CAPACITY, ID;
                enter_var(WH_Name, CAPACITY, ID);
                Warehouse::Warehouse warehouse(WH_Name, CAPACITY, ID);
                try {
                    warehouseContainer.addWarehouse(warehouse);
                    std::cout << "Склад успешно добавлен.\n";
                }
                catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            }
            case 2: {
                int quantity, warehouseID;
                try {
                    inInt(warehouseID,  "Введите ID склада для поставки: ");
                    warehouseContainer.GetWarehouse(warehouseID);
                    inInt(quantity,  "Введите количество товаров для поставки: ");
                    warehouseContainer.ShipGoods(quantity, warehouseID);
                }
                catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            }
            case 3: {
                int newCapacity, warehouseID;
                try {
                    inInt(warehouseID, "Введите ID склада: ");
                    warehouseContainer.GetWarehouse(warehouseID);
                    inInt(newCapacity, "Введите новый размер склада: ");
                    warehouseContainer.UpdateCapacity(newCapacity, warehouseID);
                }
                catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            }
            case 4: {
                int quantity, sourceWarehouseID, destinationWarehouseID;
                inInt(quantity,   "Введите количество товаров для перевозки: ");
                try {
                    inInt(sourceWarehouseID,   "Введите ID склада-источника: ");
                    warehouseContainer.GetWarehouse(sourceWarehouseID);
                    inInt(destinationWarehouseID,   "Введите ID склада-назначения: ");
                    warehouseContainer.GetWarehouse(destinationWarehouseID);
                    warehouseContainer.ShipGoodsBetweenWarehouses(quantity, sourceWarehouseID, destinationWarehouseID);
                }
                catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            }
            case 5: {
                std::string workerName, workerSurname;
                int workerId, warehouseID;
                std::cout << "Введите имя работника: ";
                std::cin >> workerName;
                std::cout << "Введите фамилию работника: ";
                std::cin >> workerSurname;
                inInt(workerId,   "Введите ID работника: ");
                inInt(warehouseID,   "Введите ID склада, на котором будет работать: ");
                try {
                    WarehouseWorker worker(workerName, workerSurname, workerId, "");
                    warehouseContainer.addWorker(worker, warehouseID);
                    std::cout << "Работник успешно добавлен на склад.\n";
                }
                catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            }
            case 6: {
                int workerId, warehouseID;
                inInt(workerId,   "Введите ID работника, которого нужно уволить: ");
                inInt(warehouseID,   "Введите ID склада, с которого нужно уволить работника: ");

                try {
                    warehouseContainer.dismissWorker(workerId, warehouseID);
                    std::cout << "Работник успешно уволен со склада.\n";
                }
                catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            }
            case 7:
                // Code for displaying all warehouses and their data
                warehouseContainer.DisplayWarehouses();
                break;
            default:
                std::cout << "Неверный выбор операции.\n";
                break;
        }
    }
    return 0;
}
