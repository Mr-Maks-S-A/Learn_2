#include <iostream>
#include <string>
#include <cassert>
#include <Fraction/Fraction.hpp> // Подключаем твою библиотеку

int main() {
    std::cout << "=== Running Manual Verification Scenario ===\n";

    // 1. Проверка неравенства. f1=4/3, f2=6/11:
    Fraction manual_f1(4, 3);
    Fraction manual_f2(6, 11);
    
    std::cout << "f1" << ((manual_f1 == manual_f2) ? " == " : " not == ") << "f2" << '\n';
    std::cout << "f1" << ((manual_f1 != manual_f2) ? " != " : " not != ") << "f2" << '\n';
    std::cout << "f1" << ((manual_f1 < manual_f2) ? " < " : " not < ") << "f2" << '\n';
    std::cout << "f1" << ((manual_f1 > manual_f2) ? " > " : " not > ") << "f2" << '\n';
    std::cout << "f1" << ((manual_f1 <= manual_f2) ? " <= " : " not <= ") << "f2" << '\n';
    std::cout << "f1" << ((manual_f1 >= manual_f2) ? " >= " : " not >= ") << "f2" << '\n';

    std::cout << "\n=== Running Automated Assert Tests ===\n";


    // Тест 1: Проверка конструкторов и dump
    {
        Fraction f1(3, 4);
        Fraction f2(4, 5);
        assert(f1.dump() == "3/4");
        assert(f2.dump() == "4/5");
    }

    // Тест 2: Проверка неравенства    
    {        
        Fraction f1(4, 3);        
        Fraction f2(6, 11);        
        assert(!(f1 == f2));        
        assert(f1 != f2);        
        assert(!(f1 < f2));        
        assert(f1 > f2);        
        assert(!(f1 <= f2));        
        assert(f1 >= f2);   
    }    

    // Тест 3: Проверка равенства    
    {        
        Fraction f1(4, 3);        
        Fraction f2(8, 6);        
        assert(f1 == f2);        
        assert(!(f1 != f2));        
        assert(!(f1 < f2));        
        assert(!(f1 > f2));        
        assert(f1 <= f2);        
        assert(f1 >= f2);    
    }    

    // Тест 4: Проверка арифметического сценария    
    {        
        Fraction f1(3, 4);        
        Fraction f2(4, 5);        
        assert((f1 + f2).dump() == "31/20"); // 3/4 + 4/5 = 31/20        
        assert((f1 - f2).dump() == "-1/20"); // 3/4 - 4/5 = -1/20        
        assert((f1 * f2).dump() == "3/5");   // 3/4 * 4/5 = 3/5        
        assert((f1 / f2).dump() == "15/16"); // 3/4 / 4/5 = 15/16        
        assert((++f1 * f2).dump() == "7/5"); // ++3/4 * 4/5 = 7/5        
        assert(f1.dump() == "7/4");          // Значение дроби 1 = 7/4        
        assert((f1-- * f2).dump() == "7/5"); // 7/4-- * 4/5 = 7/5        
        assert(f1.dump() == "3/4");          // Значение дроби 1 = 3/4    
    }    

    // Тест 5: Дополнительные проверки с унарными операторами
    {        
        Fraction f1(2, 3);       
        Fraction f2(-2, 3);        
        assert((-f1).dump() == "-2/3");        
        assert((-f2).dump() == "2/3");        
        assert((-f1) == f2);    

        assert((+f1).dump() == "2/3");
    }    

    // Тест 6: Проверка сокращения дробей    
    {        
        Fraction f1(4, 8);        
        Fraction f2(2, 4);        
        Fraction f3(1, 2);        
        assert(f1.dump() == "1/2");        
        assert(f2.dump() == "1/2");        
        assert(f3.dump() == "1/2");        
        assert(f1 == f2);        
        assert(f2 == f3);    
    }    

    // Тест 7: Проверка с отрицательными дробями    
    {        
        Fraction f1(-3, 4);        
        Fraction f2(3, -4);        
        Fraction f3(-3, -4);        
        assert(f1.dump() == "-3/4");        
        assert(f2.dump() == "-3/4");        
        assert(f3.dump() == "3/4");        
        assert(f1 == f2);        
        assert(f1 != f3);    
    }    

    std::cout << "All tests passed successfully!" << std::endl;    
    return 0;
}