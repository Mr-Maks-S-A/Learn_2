#include <iostream> // Для потоков ввода/вывода
#include <clocale>  // Библиотека для работы с локалью
#include <cstdlib>  // Для макросов EXIT_SUCCESS


int main(){

    // Устанавливаем локаль для корректного вывода кириллицы в консоль
    std::setlocale(LC_ALL, "Russian");

    std::cout<<"Привет мир!\n";
    return EXIT_SUCCESS;
};
