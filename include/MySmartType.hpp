#pragma once

#include <iostream>
#include <algorithm>
#include <concepts>
#include <vector>
#include <string_view>
#include <string>
#include <cctype>
#include <cwctype>
#include <unordered_map>

// Базовая пассивная политика
template<typename T>
struct DefaultPolicy {
    static constexpr const T& on_get(const T& value) { return value; }

    static constexpr T on_set(const T& /*current*/, const T& newValue) { return newValue; }
    static constexpr T on_set(const T& newValue) { return newValue; }
};

// Концепт для проверки валидности политик
// template<typename P, typename T>
// concept VarPolicy = requires(T val, T current, T newValue) {
//     { P::on_get(val) }; 
//     { P::on_set(current, newValue) } -> std::convertible_to<T>;
// };

// Универсальный класс умной переменной
template<typename T, typename Policy = DefaultPolicy<T>>
// requires VarPolicy<Policy, T>
class SmartVar {
private:
    T m_value;

    // Вспомогательный переключатель вызовов (Compile-time Dispatch)
    constexpr static T invoke_on_set(const T& current, const auto& newValue) {
        if constexpr (requires { Policy::on_set(current, newValue); }) {
            // Если политика требует 2 аргумента (current, newValue)
            return Policy::on_set(current, newValue);
        } else {
            // Если политика требует только 1 операнд (newValue)
            return Policy::on_set(newValue);
        }
    }
public:
    constexpr SmartVar() 
        : m_value(invoke_on_set(T{}, T{})) {}
        
    constexpr explicit SmartVar(const T& startValue) 
        : m_value(invoke_on_set(T{}, startValue)) {}
    
    template<typename U>
    requires (!std::is_same_v<std::decay_t<U>, SmartVar>)
    constexpr explicit SmartVar(U&& startValue) 
        : m_value(invoke_on_set(T{}, std::forward<U>(startValue))) {}

    constexpr SmartVar& operator=(const auto& newValue) {
        m_value = invoke_on_set(m_value, newValue);
        return *this;
    }


    // Автоматическое приведение к типу, который возвращает политика
    constexpr operator decltype(auto)() const { return Policy::on_get(m_value); }

    // Сложение: std::string + SmartVar
    friend std::string operator+(const std::string& lhs, const SmartVar& rhs) {
        return lhs + static_cast<const std::string&>(Policy::on_get(rhs.m_value));
    }

    // Сложение: SmartVar + std::string
    friend std::string operator+(const SmartVar& lhs, const std::string& rhs) {
        return static_cast<const std::string&>(Policy::on_get(lhs.m_value)) + rhs;
    }

    // Сложение: SmartVar + const char*
    friend std::string operator+(const SmartVar& lhs, const char* rhs) {
        return static_cast<const std::string&>(Policy::on_get(lhs.m_value)) + rhs;
    }

    // Сложение: const char* + SmartVar
    friend std::string operator+(const char* lhs, const SmartVar& rhs) {
        return lhs + static_cast<const std::string&>(Policy::on_get(rhs.m_value));
    }
};



//=================================создание умной политики повторяющихся городов и улиц =================================
struct TrimAndCapitalizePolicy {

    // Шаг 1: Быстрый поиск границ без изменения памяти
    static std::string_view trim(std::string_view str) {
        auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char ch) { return std::isspace(ch); });
        auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) { return std::isspace(ch); }).base();
        return (start < end) ? std::string_view(&*start, end - start) : std::string_view{};
    }

// Шаг 2: Модификация регистра только тогда, когда это действительно нужно
    static std::string capitalize(std::string_view str) {
        if (str.empty()) return std::string{};
        
        std::string result(str);
        unsigned char leading = static_cast<unsigned char>(result[0]);
        size_t char_len = 1;
        
        if ((leading & 0x80) == 0)         char_len = 1;
        else if ((leading & 0xE0) == 0xC0) char_len = 2;
        else if ((leading & 0xF0) == 0xE0) char_len = 3;
        else if ((leading & 0xF8) == 0xF0) char_len = 4;

        if (result.length() >= char_len) {
            wchar_t wc;
            std::mbstate_t state{};
            size_t res = std::mbrtowc(&wc, result.data(), char_len, &state);
            
            if (res > 0 && res != static_cast<size_t>(-1) && res != static_cast<size_t>(-2)) {
                wc = std::towupper(wc); 
                char buf[4];
                std::mbstate_t out_state{};
                size_t out_len = std::wcrtomb(buf, wc, &out_state);
                if (out_len > 0 && out_len != static_cast<size_t>(-1)) {
                    result.replace(0, char_len, buf, out_len);
                }
            }
        }
        return result;
    }


    static std::string process(std::string_view str) {
            return capitalize(trim(str));
    }
    // static std::string process(std::string str) {
    //     // 1. Удаление пробелов слева
    //     str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
    //         return !std::isspace(ch);
    //     }));
        
    //     // 2. Удаление пробелов справа
    //     str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
    //         return !std::isspace(ch);
    //     }).base(), str.end());

    //     if (str.empty()) return str;

    //     // 3. Определяем длину UTF-8 последовательности по первому байту
    //     unsigned char leading = static_cast<unsigned char>(str[0]);
    //     size_t char_len = 1;
        
    //     if ((leading & 0x80) == 0)       char_len = 1; // ASCII
    //     else if ((leading & 0xE0) == 0xC0) char_len = 2; // Кириллица, латиница расширенная и др.
    //     else if ((leading & 0xF0) == 0xE0) char_len = 3;
    //     else if ((leading & 0xF8) == 0xF0) char_len = 4;

    //     if (str.length() >= char_len) {
    //         // Преобразуем первый UTF-8 символ в один wchar_t с помощью mbrtowc
    //         wchar_t wc;
    //         std::mbstate_t state{};
    //         size_t result = std::mbrtowc(&wc, str.data(), char_len, &state);
            
    //         // Если конвертация успешна, меняем регистр и кодируем обратно
    //         if (result > 0 && result != static_cast<size_t>(-1) && result != static_cast<size_t>(-2)) {
    //             // Применяем towupper (работает на основе setlocale из main)
    //             wc = std::towupper(wc); 
                
    //             // Кодируем wchar_t обратно в UTF-8 мультибайт
    //             char buf[4];
    //             std::mbstate_t out_state{};
    //             size_t out_len = std::wcrtomb(buf, wc, &out_state);
                
    //             if (out_len > 0 && out_len != static_cast<size_t>(-1)) {
    //                 // Заменяем старый символ новой UTF-8 последовательностью верхнего регистра
    //                 str.replace(0, char_len, buf, out_len);
    //             }
    //         }
    //     }

    //     return str;
    // }
};

// Высокопроизводительный кэш строк (Аналог FName в Unreal Engine)
class StringCache {
private:
    std::vector<std::string> m_table;
    std::unordered_map<std::string, uint32_t> m_lookup; // Мгновенный поиск O(1) вместо std::find

    StringCache() = default;
public:
    static StringCache& instance() {
        static StringCache pool;
        return pool;
    }

    uint32_t get_or_insert(const std::string& str) {
    // try_emplace вернет пару {итератор, bool_флаг_вставки}
    // Если строка уже была, вставка не произойдет, но мы мгновенно получим итератор
    auto [it, inserted] = m_lookup.try_emplace(str, 0);
    
    if (inserted) {
        m_table.push_back(str);
        uint32_t newId = static_cast<uint32_t>(m_table.size() - 1);
        it->second = newId; // Обновляем ID в мапе
        return newId;
    }
    
    return it->second;
}

    // Оптимизация: используем быстрый operator[] вместо медленного и безопасного .at()
    const std::string& get_string(uint32_t id) const {
        return m_table[id]; 
    }
};

// Политика Легковеса для SmartVar
struct FlyweightStringPolicy {
    static const std::string& on_get(uint32_t id) {
        return StringCache::instance().get_string(id);
    }

    // Принимаем string_view вместо тяжелого std::string
    static uint32_t on_set(uint32_t /*current*/, std::string_view newValue) {
        std::string_view trimmed = TrimAndCapitalizePolicy::trim(newValue);
        
        // Передаем string_view напрямую в кэш. За счет std::hash<void>
        // unordered_map выполнит поиск без создания временного std::string.
        std::string processed = TrimAndCapitalizePolicy::capitalize(trimmed);
        return StringCache::instance().get_or_insert(processed);
    }
};


// Хранит внутри только 4 байта (ID), вместо 32+ байт тяжелой std::string!
using CacheString = SmartVar<uint32_t, FlyweightStringPolicy>;

//=================================конец создания умной политики повторяющихся городов и улиц =================================




//===================================================================Тестовые реализации политик===================================================================

// struct SafeRollbackPolicy {
//     static constexpr int on_get(int value) { return value; }
    
//     static constexpr int on_set(int current, int newValue) {
//         // Если новое значение отрицательное — отменяем изменения, возвращаем старое
//         if (newValue < 0) {
//             std::cerr << "[Warning] Попытка записать " << newValue << ". Откат к " << current << "\n";
//             return current; 
//         }
//         return newValue;
//     }
// };


// template<typename T>
// struct SnapshotGetPolicy {
//     static inline T snapshot_value{}; // Внешний снимок
    
//     static constexpr T on_get(const T& /*value*/) { 
//         return snapshot_value; // Все читают только снимок
//     }
//     static constexpr T on_set(const T& /*current*/, const T& newValue) { 
//         return newValue; // Внутреннее значение меняется штатно
//     }
// };


// template<typename T>
// struct HistoryPolicy {
//     static inline std::vector<T> history; // Хранилище снимков

//     static constexpr T on_get(const T& value) { return value; }
    
//     static constexpr T on_set(const T& current, const T& newValue) {
//         if (current != T{}) { // Не сохраняем дефолтную инициализацию
//             history.push_back(current); 
//         }
//         return newValue;
//     }

//     // Метод отката (вызывается вручную через Политику)
//     static T undo(T& current_var_value) {
//         if (history.empty()) return current_var_value;
//         T previous = history.back();
//         history.pop_back();
//         return previous; // Возвращаем для перезаписи
//     }
// };