#pragma once

#include <string>

namespace Racing {

    struct RaceResult {
        std::string vehicleName; // Название ТС
        double totalTime;        // Итоговое время прохождения дистанции (в часах)

        RaceResult(std::string name, double time)
            : vehicleName(std::move(name)), totalTime(time) {}
    };

} // namespace Racing