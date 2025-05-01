#pragma once
#include <iostream>
#include <vector>
#include "Node.h"

class Hashmap {
private:
    int size;
    std::vector<Item*> items;

public:
    Hashmap();
    ~Hashmap();

    long hashFunc(std::string data); // хэш-функция
    void resize(); // ресайз

    void insertItem(std::string key, std::string data, std::string key2); // добавление
    std::string getItem(std::string data); // поиск
    void displayHashmap(); // вывод
    void deleteItem(std::string data); // удаление 
    void menu(const std::string& filename);

    // проверки 
    bool isValidData(const std::string& data); 
    bool isValidKey(const std::string& key);
    bool isValidKey2(const std::string& key);

    // Поиск по фрагменту, Кнута-Морриса-Пратта
    std::vector<int> prefix_function(const std::string& pattern);
    std::vector<int> KMPSearch(const std::string& text, const std::string& pattern);
    std::vector<std::string> search(const std::string& fragment);

    // Сохранение/запись данных файла
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);
};
