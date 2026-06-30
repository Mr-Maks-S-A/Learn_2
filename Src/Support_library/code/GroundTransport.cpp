#include <Transport/Ground.hpp>

double GroundTransport::calculateTime(double distance) const {
    double pureMoveTime = distance / speed;
    
    // Рассчитываем количество остановок
    // Используем небольшой эпсилон, чтобы избежать остановки прямо на финише
    int restCount = static_cast<int>(pureMoveTime / moveTimeBeforeRest);
    if (pureMoveTime > 0 && static_cast<int>(pureMoveTime) % static_cast<int>(moveTimeBeforeRest) == 0) {
        // Если приехали ровно в момент отдыха, последняя остановка не нужна
        restCount--; 
    }

    double totalRestTime = 0;
    for (int i = 1; i <= restCount; ++i) {
        totalRestTime += getRestDuration(i);
    }

    return pureMoveTime + totalRestTime;
}