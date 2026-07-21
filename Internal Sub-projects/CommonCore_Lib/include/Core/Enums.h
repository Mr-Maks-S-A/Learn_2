#pragma once

namespace Racing {

    // Тип проводимого заезда
    enum class RaceType {
        GroundOnly = 1, // Только наземные ТС
        AirOnly,        // Только воздушные ТС
        Mixed           // Наземные и воздушные ТС
    };

    // Категория самого транспортного средства
    enum class VehicleType {
        Ground,
        Air
    };

} // namespace Racing