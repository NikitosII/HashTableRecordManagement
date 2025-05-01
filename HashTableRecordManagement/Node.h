#pragma once
#include <iostream>

// Структура элемента таблицы 
class Item {
public:
    std::string data; //номер договора
    std::string key; //ФИО
    std::string key2; //дата
    Item* next;
    bool deleted;

    Item(std::string key, std::string data, std::string key2) {
        this->key = key;
        this->key2 = key2;
        this->data = data;
        this->deleted = false;
        this->next = nullptr;
    }

    Item(std::string key, std::string data, std::string key2, bool del) {
        this->key = key;
        this->key2 = key2;
        this->data = data;
        this->next = nullptr;
        this->deleted = del;
    }
};
