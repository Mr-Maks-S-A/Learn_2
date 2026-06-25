#include <iostream>
#include <string>
#include <cassert>
#include <random>
#include <cmath>
#include <stdexcept>
#include <iomanip>
#include <Fraction/Fraction.hpp>

// Перевод дроби в double для эталонной проверки
double to_double(const Fraction& f) {
    std::string s = f.dump();
    size_t slash = s.find('/');
    double num = std::stod(s.substr(0, slash));
    double denom = std::stod(s.substr(slash + 1));
    return num / denom;
}

// Функция для красивого вывода лога ошибки
void log_error(const std::string& op, const Fraction& f1, const Fraction& f2, 
               double expected, double got, int n1, int d1, int n2, int d2) {
    std::cerr << "\n❌ STRESS TEST FAILED ERROR REPORT:\n"
              << "----------------------------------------\n"
              << "Operation:       " << op << "\n"
              << "Raw Inputs:      f1(" << n1 << ", " << d1 << "), f2(" << n2 << ", " << d2 << ")\n"
              << "Fractions:       f1 = " << f1.dump() << ", f2 = " << f2.dump() << "\n"
              << "Expected value:  " << std::setprecision(10) << expected << "\n"
              << "Got value:       " << std::setprecision(10) << got << " (from dump: " << (op == "+" || op == "-" || op == "*" || op == "/" ? "computed fraction" : "N/A") << ")\n"
              << "----------------------------------------\n";
}

int main() {
    std::cout << "=== STARTING MAX STRESS TEST (100,000 iterations) ===\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> num_dist(-500, 500);
    std::uniform_int_distribution<int> denom_dist(-500, 500);

    const int ITERATIONS = 100000;
    const double EPSILON = 1e-7;

    int logged_errors = 0;

    for (int i = 1; i <= ITERATIONS; ++i) {
        // Логирование прогресса каждые 10 000 итераций, чтобы видеть, что тест жив
        if (i % 10000 == 0) {
            std::cout << "-> Progress: " << std::setw(6) << i << " / " << ITERATIONS << " iterations checked...\n";
        }

        int n1 = num_dist(gen);
        int d1 = denom_dist(gen);
        int n2 = num_dist(gen);
        int d2 = denom_dist(gen);

        if (d1 == 0) d1 = 1;
        if (d2 == 0) d2 = 1;

        Fraction f1(n1, d1);
        Fraction f2(n2, d2);

        double val1 = static_cast<double>(n1) / d1;
        double val2 = static_cast<double>(n2) / d2;

        
        // Сложение
        Fraction res_add = f1 + f2;
        if (std::abs(to_double(res_add) - (val1 + val2)) >= EPSILON) {
            log_error("+", f1, f2, val1 + val2, to_double(res_add), n1, d1, n2, d2);
            exit(1);
        }

        // Вычитание
        Fraction res_sub = f1 - f2;
        if (std::abs(to_double(res_sub) - (val1 - val2)) >= EPSILON) {
            log_error("-", f1, f2, val1 - val2, to_double(res_sub), n1, d1, n2, d2);
            exit(1);
        }

        // Умножение
        Fraction res_mul = f1 * f2;
        if (std::abs(to_double(res_mul) - (val1 * val2)) >= EPSILON) {
            log_error("*", f1, f2, val1 * val2, to_double(res_mul), n1, d1, n2, d2);
            exit(1);
        }

        // Деление
        if (n2 != 0) {
            Fraction res_div = f1 / f2;
            if (std::abs(to_double(res_div) - (val1 / val2)) >= EPSILON) {
                log_error("/", f1, f2, val1 / val2, to_double(res_div), n1, d1, n2, d2);
                exit(1);
            }
        } else {
            try {
                Fraction res_div = f1 / f2;
                std::cerr << "❌ ERROR: Division by zero fraction (" << f2.dump() << ") did not throw an exception!\n";
                exit(1);
            } catch (const std::invalid_argument&) {
                // Исключение поймано успешно
            }
        }

        
        if ((f1 == f2) != (std::abs(val1 - val2) < EPSILON)) {
            log_error("==", f1, f2, std::abs(val1 - val2) < EPSILON, f1 == f2, n1, d1, n2, d2);
            exit(1);
        }
        if ((f1 != f2) != (std::abs(val1 - val2) >= EPSILON)) {
            log_error("!=", f1, f2, std::abs(val1 - val2) >= EPSILON, f1 != f2, n1, d1, n2, d2);
            exit(1);
        }
        if ((f1 < f2) != (val1 < val2 - EPSILON)) {
            log_error("<", f1, f2, val1 < val2 - EPSILON, f1 < f2, n1, d1, n2, d2);
            exit(1);
        }
        if ((f1 > f2) != (val1 > val2 + EPSILON)) {
            log_error(">", f1, f2, val1 > val2 + EPSILON, f1 > f2, n1, d1, n2, d2);
            exit(1);
        }

        Fraction f1_copy = f1;
        Fraction target_pref(n1 + d1, d1);
        if (to_double(++f1_copy) != to_double(target_pref)) {
            log_error("++f (prefix)", f1, Fraction(), to_double(target_pref), to_double(f1_copy), n1, d1, 0, 0);
            exit(1);
        }
        
        f1_copy = f1;
        Fraction res_post = f1_copy++;
        if (to_double(res_post) != val1 || std::abs(to_double(f1_copy) - (val1 + 1.0)) >= EPSILON) {
            log_error("f++ (postfix)", f1, Fraction(), val1 + 1.0, to_double(f1_copy), n1, d1, 0, 0);
            exit(1);
        }
    }

    std::cout << "🚀 STRESS TEST PASSED COMPLETELY! 100,000 states verified without any crashes!\n";
    return EXIT_SUCCESS;
};