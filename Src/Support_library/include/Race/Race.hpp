// Race.h
#pragma once
#include <Transport/Transport.hpp>
#include <vector>
#include <algorithm>

enum class RaceType { Ground, Air, Mixed };

class RACING_API Race {
private:
    double distance;
    RaceType type;
    std::vector<Transport*> registeredTransports;

public:
    Race(double distance, RaceType type);
    
    // Возвращает false, если тип ТС не подходит под тип гонки, или оно уже зарегистрировано
    bool registerTransport(Transport* transport);
    
    // Структура для возврата результатов в EXE
    struct Result {
        std::string name;
        double time;
    };

    std::vector<Result> start() const;
    size_t getParticipantsCount() const;
};