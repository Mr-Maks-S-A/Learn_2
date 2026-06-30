// Transport.h
#pragma once
#include <string>

#ifdef RACINGENGINE_EXPORTS
#define RACING_API __declspec(dllexport)
#else
#define RACING_API __declspec(dllimport)
#endif

enum class TransportType { Ground, Air };

class RACING_API Transport {
protected:
    std::string name;
    double speed;
    TransportType type;

public:
    Transport(std::string name, double speed, TransportType type);
    virtual ~Transport() = default;

    std::string getName() const;
    TransportType getType() const;
    
    // Главный метод: принимает дистанцию, возвращает итоговое время
    virtual double calculateTime(double distance) const = 0;
};