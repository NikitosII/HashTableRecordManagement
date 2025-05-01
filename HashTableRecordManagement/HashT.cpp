#include <iostream>
#include "HashT.h"
#include <regex>
#include "Windows.h"
#include <fstream>
#include <sstream>

// Конструктор
Hashmap::Hashmap() {
    size = 10;
    items.resize(size);
    for (int i = 0; i < size; i++)
        items[i] = nullptr;
}

// Деструктор
Hashmap::~Hashmap() {
    items.clear();
}

// Хеш-функция
long Hashmap::hashFunc(std::string data) {
    long sum = 0;
    for (int i = 0; i < data.length(); i++) {
        sum += data[i];
    }
    return (sum % size);
}

// Вставка элемента
void Hashmap::insertItem(std::string key, std::string data, std::string key2) {
    int index = hashFunc(data);
    Item* current = items[index];

    if (current == nullptr || current->deleted == true) {
        items[index] = new Item(key, data, key2);
    }
    else if (current->key == key) {
        current->data = data;
        current->key2 = key2;
    }
    else {
        long i = 1;
        long newIndex = (index + i * i) % size;
        while (items[newIndex] != nullptr && items[newIndex]->deleted != true) {
            i++;
            newIndex = (index + i * i) % size;
        }
        items[newIndex] = new Item(key, data, key2);
        return;
    }
}

// Поиск элемента по номеру договора
std::string Hashmap::getItem(std::string data) {
    int index = hashFunc(data);
    Item* current = items[index];

    if (current->data == data) { // если сразу нашли на своём месте
        return current->key;
    }
    else {
        long i = 1;
        long newIndex = (index + i * i) % size;

        while (items[newIndex] != nullptr && items[newIndex]->deleted != true) {
            if (items[newIndex]->data == data) {
                return items[newIndex]->key;
            }
            i++;
            newIndex = (index + i * i) % size;
        }
    }
    return " "; // ключ не найден
}

// Удаление элемента по номеру договора
void Hashmap::deleteItem(std::string data) {
    long index = hashFunc(data);
    Item* current = items[index];

    if (current->data == data) { // если сразу нашли на своём месте
        delete current;
        items[index] = new Item("", "", "", true);
        return;
    }
    else {
        long i = 1;
        long newIndex = (index + i * i) % size;

        while (items[newIndex] != nullptr) {
            if (items[newIndex]->data == data) {
                delete items[newIndex];
                items[newIndex] = new Item("", "", "", true);
                return;
            }
            i++;
            newIndex = (index + i * i) % size;
        }
    }
}

// Вывод всех элементов
void Hashmap::displayHashmap() {
    for (int i = 0; i < size; i++) {
        Item* current = items[i];

        if (current != nullptr && current->deleted != true) {
            std::cout << current->key << " - " << current->data << " - " << current->key2 << std::endl;
        }
    }
}

// Проверка номера договора
bool Hashmap::isValidData(const std::string& data) {
    if (data.length() != 5 || !std::all_of(data.begin(), data.end(), ::isdigit)) {
        return false;
    }
    return true;
}

// Проверка ФИО
bool Hashmap::isValidKey(const std::string& key) {
    for (char c : key) {
        if (std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Проверка даты
bool Hashmap::isValidKey2(const std::string& key2) {
    std::regex pattern("^(\\d{2})\\.(\\d{2})\\.(\\d{4})$"); // формат даты
    std::smatch matches; // объект используется для хранения результатов сопоставления регулярного выражения с key2

    if (!std::regex_match(key2, matches, pattern)) {
        return false;
    } // если совпадает, передаём в matches

    int day = std::stoi(matches[1]);
    int month = std::stoi(matches[2]);
    int year = std::stoi(matches[3]);

    // Проверка на високосный
    bool LeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    // номер месяца проверка
    if (month < 1 || month > 12) {
        return false;
    }

    int days_month;
    switch (month) {
    case 2:
        days_month = LeapYear ? 29 : 28;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        days_month = 30;
        break;
    default:
        days_month = 31;
    }
    // проверки на номер дня
    if (day < 1 || day > days_month) {
        return false;
    }

    return true;
}

// Загрузка данных из файла
void Hashmap::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, data, key2;
        if (std::getline(iss, key, ';') && std::getline(iss, data, ';') && std::getline(iss, key2, ';')) {
            if (isValidKey(key) && isValidData(data) && isValidKey2(key2)) {
                insertItem(key, data, key2);
            }
            else {
                std::cerr << "Некорректная запись: " << line << std::endl;
            }
        }
    }
    file.close();
}

// Сохранение данных в файл
void Hashmap::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для записи: " << filename << std::endl;
        return;
    }

    for (int i = 0; i < size; i++) {
        Item* current = items[i];
        if (current != nullptr && !current->deleted) {
            file << current->key << ";" << current->data << ";" << current->key2 << std::endl;
        }
    }
    file.close();
}

// Изменение размера таблицы 
void Hashmap::resize() {
    int newSize = size * 2;
    std::vector<Item*> newItems(newSize, nullptr);

    int oldSize = size;
    size = newSize;

    for (int i = 0; i < oldSize; ++i) {
        Item* current = items[i];
        if (current != nullptr && !current->deleted) {
            int index = hashFunc(current->data);
            while (newItems[index] != nullptr) {
                int j = 1;
                index = (index + j * j) % size;
            }
            newItems[index] = current;
        }
    }

    items.clear();
    items = newItems;
}


void Hashmap::menu(const std::string& filename) {
    int choice;
    int Level = 0; // подсчет элементов
    std::string key, data, key2;

    do {
        std::cout << "•1 Добавление записи\n";
        std::cout << "•2 Поиск\n";
        std::cout << "•3 Удаление записи\n";
        std::cout << "•4 Просмотр всех имеющихся записей\n";
        std::cout << "•5 Поиск по фрагменту\n";
        std::cout << "•6 Сохранить данные в файл\n";
        std::cout << "•0 Выход\n";
        std::cout << "Введите номер пункта(0-7): ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            int subChoice;
            std::cout << "•1 Добавить запись из файла\n";
            std::cout << "•2 Добавить запись через консоль\n";
            std::cout << "Введите номер пункта(1-2): ";
            std::cin >> subChoice;
            switch (subChoice) {
            case 1: {
                loadFromFile(filename);
                break;
            }
            case 2:{
                do {
                    std::cout << "Enter ФИО: ";
                    std::cin.ignore();
                    std::getline(std::cin, key);
                } while (!isValidKey(key));

                do {
                    std::cout << "Enter номер договора (из 5 цифр): ";
                    std::cin >> data;
                } while (!isValidData(data));

                do {
                    std::cout << "Enter др (дд.мм.гггг): ";
                    std::cin >> key2;
                } while (!isValidKey2(key2));

                // Если все данные корректны, вставляем
                insertItem(key, data, key2);
                Level++;

                if (Level >= size * 0.5) {
                    std::cout << "ресайз";
                    resize();
                    system("pause");
                }
                break;
            }
            }
            system("cls");
            break;
        }
        case 2: {
            std::cout << "Введите данные для поиска: ";
            std::cin >> data;
            std::cout << "Результат: " << getItem(data) << std::endl;
            system("pause");
            system("cls");
            break;
        }
        case 3: {
            std::cout << "Введите данные для удаления (номер договора): ";
            std::cin >> data;
            deleteItem(data);
            Level--;
            system("cls");
            break;
        }
        case 4: {
            displayHashmap();
            system("pause");
            system("cls");
            break;
        }
        case 5: {
            std::string fragment;
            std::cout << "Введите фрагмент даты: ";
            std::cin >> fragment;
            std::vector<std::string> results = search(fragment);
            if (!results.empty()) {
                std::cout << "Найденные записи:\n";
                for (const std::string& result : results) {
                    std::cout << result << std::endl;
                }
            }
            else {
                std::cout << "Ничего не найдено.\n";
            }
            system("pause");
            system("cls");
            break;
        }
        case 6: {
            saveToFile(filename);
            std::cout << "Данные сохранены в файл: " << filename << std::endl;
            system("pause");
            system("cls");
            break;
        }
        case 0: {
            break;
        }
        default: {
            std::cout << "Неверный выбор.\n";
            system("pause");
            system("cls");
            break;
        }
        }
    } while (choice != 0);
}

/////////////////////////////////////////////////// Функции для поиска с KMP

// вычисляет префиксные функции
std::vector<int> Hashmap::prefix_function(const std::string& pattern) {
    int n = pattern.length();
    std::vector<int> pi(n, 0);
    int k = 0;
    for (int i = 1; i < n; ++i) {
        while (k > 0 && pattern[k] != pattern[i]) {
            k = pi[k - 1];
        }
        if (pattern[k] == pattern[i]) {
            ++k;
        }
        pi[i] = k;
    }
    return pi;
}

// Поиск подстроки
std::vector<int> Hashmap::KMPSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> positions;
    std::vector<int> pi = prefix_function(pattern);
    int n = text.length(); // Длина текста
    int m = pattern.length(); // Длина шаблона
    int q = 0;
    for (int i = 0; i < n; ++i) {
        while (q > 0 && pattern[q] != text[i]) {
            q = pi[q - 1];
        }
        if (pattern[q] == text[i]) { // Если символы совпадают, увеличиваем q
            ++q;
        }
        if (q == m) { // Если весь шаблон совпал
            positions.push_back(i - m + 1); // Добавляем позицию вхождения в вектор позиций
            q = pi[q - 1]; // Возвращаемся к началу шаблона для поиска следующего вхождения
        }
    }
    return positions;
}

// ищет все записи в хэш-таблице
std::vector<std::string> Hashmap::search(const std::string& fragment) {
    std::vector<std::string> results; // Вектор для хранения найденных
    for (int i = 0; i < size; ++i) {
        if (items[i] != nullptr) {
            std::vector<int> positions = KMPSearch(items[i]->key2, fragment);
            if (!positions.empty()) { // Если найден
                results.push_back(items[i]->key);
            }
        }
    }
    return results;
}


