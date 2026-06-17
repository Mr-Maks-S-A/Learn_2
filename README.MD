<!--<base href="https://github.com/Mr-Maks-S-A/Learn_2/tree/">-->

<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![C++](https://img.shields.io/badge/C++-20-blue.svg)
![CMake](https://img.shields.io/badge/CMake-4.0+-brightgreen.svg)

</div>

---

<div align="left">
  <a href="https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_1_1"><img src="https://img.shields.io/badge/⏮️_Первый-v2.1.1-gray?style=flat" height="22"></a>
  <a href="https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_4_3"><img src="https://img.shields.io/badge/◀️_Назад-v2.4.3-blue?style=flat" height="22"></a> 
  <a href="https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_5_2"><img src="https://img.shields.io/badge/Вперед_▶️-v2.5.2-blue?style=flat" height="22"></a>
  <a href="https://github.com/Mr-Maks-S-A/Learn_2/tree/v_latest"><img src="https://img.shields.io/badge/⏭️_Последний-latest-brightgreen?style=flat" height="22"></a>
</div>

---

<div align="right">
  <a href="https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_5_1/Src/code/main.cpp">
    <img src="https://img.shields.io/badge/Source_Code-blue?style=social&logo=github" height="25">
  </a>
</div>

<div align="center">

#  Проект: Многофайловые проекты и библиотеки. Математические функции.
Реализован калькулятор а основе стека и шаблонный набросок методов для ввода данных<br>
дополнительно реализованы тесты<br>
 

> **Примечание:** Используется классы ,старые наброски кода.<br>
[![Ref](https://img.shields.io/badge/ref-v2.1.3-blue?style=flat-square&logo=github)](ref2_1_3) исходник функции **get_input**<br>
[![Ref](https://img.shields.io/badge/ref-v2.2.1-blue?style=flat-square&logo=github)](ref2_2_1) дополнительный исходник функции **get_input**<br>
</div>



<div align="center">

# Окружение 

| Property | Value |
| :--- | :--- |
| **OS** | CachyOS x86_64 (Linux 6.19.9) |
| **DE/WM** | KDE Plasma 6.6.3 / KWin (Wayland) |
| **Shell** | fish 4.5.0 |
| **Terminal** | alacritty 0.16.1 |
| **CPU** | AMD Ryzen 7 3700X @ 4.43 GHz |

</div>



## 🚀 Пример работы программы

### Preview
![][preview2_5_1]

### Вывод в консоль

```bash
/m/H/!/0/Learn_2►./build/bin/calculator_app
=== Программа: Многорежимный Калькулятор ===
Выберите режим работы:
1. Примитивный режим (свободные функции hpp/cpp)
2. Полноценный режим (строковый парсер выражений)
3. Выход из программы
Ваш выбор (1-3): 1

-----------------------------------------
>>> Запущен ПРИМИТИВНЫЙ режим (hpp/cpp свободные функции) <<<
Введите первое число (или '0' и затем 'q' на следующем шаге для выхода): 2
Выберите операцию (+, -, *, /, ^) [или 'q' для возврата в меню]: +
Введите второе число: 2
Результат: 4

Введите первое число (или '0' и затем 'q' на следующем шаге для выхода): 2
Выберите операцию (+, -, *, /, ^) [или 'q' для возврата в меню]: ^
Введите второе число: 3
Результат: 8

Введите первое число (или '0' и затем 'q' на следующем шаге для выхода): 0
Выберите операцию (+, -, *, /, ^) [или 'q' для возврата в меню]: q
-----------------------------------------

Выберите режим работы:
1. Примитивный режим (свободные функции hpp/cpp)
2. Полноценный режим (строковый парсер выражений)
3. Выход из программы
Ваш выбор (1-3): 2

-----------------------------------------
>>> Запущен ПОЛНОЦЕННЫЙ режим (строковый рекурсивный парсер) <<<
Введите выражение [или 'q' для возврата в меню]: 2^1
Результат: 2

Введите выражение [или 'q' для возврата в меню]: 2^0
Результат: 1

Введите выражение [или 'q' для возврата в меню]: (2+2+2+3*1000)^0
Результат: 1

Введите выражение [или 'q' для возврата в меню]: q
-----------------------------------------

Выберите режим работы:
1. Примитивный режим (свободные функции hpp/cpp)
2. Полноценный режим (строковый парсер выражений)
3. Выход из программы
Ваш выбор (1-3): 3

Завершение работы программы. До свидания!
```


#  Команды для сборки

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug 
```

```bash
cmake --build build --parallel
```

```bash
./build/bin/{projectname}  
```

для тестирования с выводом

```bash
ctest --verbose 
```

# 📊 Прогресс обучения

<details open>
<summary><b>2 Базовое программирование</b></summary>
<br>

<details>
<summary><b>📂 2.10.0 Сборка и подключение библиотек </b></summary>
<br>

| Статус | ID | Описание задачи | Preview | Ссылка |
| :---: | :---: | :--- | :---: | :---: |
|  ![][status2_10_1] | `2.10.1` | Статическая библиотека.| ![Preview][preview2_10_1] | [![Ref][github_badge]][ref2_10_1] |
|  ![][status2_10_2] | `2.10.2` | Динамическая библиотека.| ![Preview][preview2_10_2] | [![Ref][github_badge]][ref2_10_2] |
|  ![][status2_10_3] | `2.10.3` | Динамическая библиотека CMake.| ![Preview][preview2_10_3] | [![Ref][github_badge]][ref2_10_3] |
|  ![][status2_10_4] | `2.10.4` | Иерархия классов в DLL, проекты VC++.| ![Preview][preview2_10_4] | [![Ref][github_badge]][ref2_10_4] |
|  ![][status2_10_5] | `2.10.5` | Иерархия классов в DLL и CMake.| ![Preview][preview2_10_5] | [![Ref][github_badge]][ref2_10_5] |

[status2_10_5]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_10_5/ReadmeSource/Status/status.svg
[preview2_10_5]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_10_5/ReadmeSource/IMG_2_10/5_Preview.png
[ref2_10_5]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_10_5

[status2_10_4]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_10_4/ReadmeSource/Status/status.svg
[preview2_10_4]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_10_4/ReadmeSource/IMG_2_10/4_Preview.png
[ref2_10_4]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_10_4

[status2_10_3]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_10_3/ReadmeSource/Status/status.svg
[preview2_10_3]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_10_3/ReadmeSource/IMG_2_10/3_Preview.png
[ref2_10_3]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_10_3

[status2_10_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_10_2/ReadmeSource/Status/status.svg
[preview2_10_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_10_2/ReadmeSource/IMG_2_10/2_Preview.png
[ref2_10_2]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_10_2

[status2_10_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_10_1/ReadmeSource/Status/status.svg
[preview2_10_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_10_1/ReadmeSource/IMG_2_10/1_Preview.png
[ref2_10_1]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_10_1

</details>

<details>
<summary><b>📂 2.9.0 CMake </b></summary>
<br>

| Статус | ID | Описание задачи | Preview | Ссылка |
| :---: | :---: | :--- | :---: | :---: |
|  ![][status2_9_1] | `2.9.1` | Простейший проект.| ![Preview][preview2_9_1] | [![Ref][github_badge]][ref2_9_1] |
|  ![][status2_9_2] | `2.9.2` | Проект посложнее.| ![Preview][preview2_9_2] | [![Ref][github_badge]][ref2_9_2] |

[status2_9_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_9_2/ReadmeSource/Status/status.svg
[preview2_9_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_9_2/ReadmeSource/IMG_2_9/2_Preview.png
[ref2_9_2]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_9_2

[status2_9_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_9_1/ReadmeSource/Status/status.svg
[preview2_9_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_9_1/ReadmeSource/IMG_2_9/1_Preview.png
[ref2_9_1]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_9_1


</details>

<details>
<summary><b>📂 2.8.0 Перегрузка операторов </b></summary>
<br>

| Статус | ID | Описание задачи | Preview | Ссылка |
| :---: | :---: | :--- | :---: | :---: |
|  ![][status2_8_1] | `2.8.1` | Сравнения в дробях.| ![Preview][preview2_8_1] | [![Ref][github_badge]][ref2_8_1] |
|  ![][status2_8_2] | `2.8.2` | Остальные операции с дробями.| ![Preview][preview2_8_2] | [![Ref][github_badge]][ref2_8_2] |

[status2_8_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_8_2/ReadmeSource/Status/status.svg
[preview2_8_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_8_2/ReadmeSource/IMG_2_8/2_Preview.png
[ref2_8_2]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_8_2

[status2_8_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_8_1/ReadmeSource/Status/status.svg
[preview2_8_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_8_1/ReadmeSource/IMG_2_8/1_Preview.png
[ref2_8_1]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_8_1

</details>

<details>
<summary><b>📂 2.7.0 Исключения и обработка ошибок </b></summary>
<br>

| Статус | ID | Описание задачи | Preview | Ссылка |
| :---: | :---: | :--- | :---: | :---: |
|  ![][status2_7_1] | `2.7.1` | Простое исключение.| ![Preview][preview2_7_1] | [![Ref][github_badge]][ref2_7_1] |
|  ![][status2_7_2] | `2.7.2` | Исключения в конструкторах.| ![Preview][preview2_7_2] | [![Ref][github_badge]][ref2_7_2] |

[status2_7_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_7_2/ReadmeSource/Status/status.svg
[preview2_7_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_7_2/ReadmeSource/IMG_2_7/2_Preview.png
[ref2_7_2]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_7_2

[status2_7_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_7_1/ReadmeSource/Status/status.svg
[preview2_7_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_7_1/ReadmeSource/IMG_2_7/1_Preview.png
[ref2_7_1]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_7_1

</details>

<details>
<summary><b>📂 2.6.0 Препроцессор и макросы </b></summary>
<br>

| Статус | ID | Описание задачи | Preview | Ссылка |
| :---: | :---: | :--- | :---: | :---: |
|  ![][status2_6_1] | `2.6.1` | Препроцессорные директивы.| ![Preview][preview2_6_1] | [![Ref][github_badge]][ref2_6_1] |
|  ![][status2_6_2] | `2.6.2` | Макрос SUB.| ![Preview][preview2_6_2] | [![Ref][github_badge]][ref2_6_2] |

[status2_6_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_6_2/ReadmeSource/Status/status.svg
[preview2_6_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_6_2/ReadmeSource/IMG_2_6/2_Preview.png
[ref2_6_2]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_6_2

[status2_6_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_6_1/ReadmeSource/Status/status.svg
[preview2_6_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_6_1/ReadmeSource/IMG_2_6/1_Preview.png
[ref2_6_1]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_6_1

</details>

<details open>
<summary><b>📂 2.5.0 Многофайловые проекты и библиотеки </b></summary>
<br>

| Статус | ID | Описание задачи | Preview | Ссылка |
| :---: | :---: | :--- | :---: | :---: |
|  ![][status2_5_1] | `2.5.1` | Математические функции.| ![Preview][preview2_5_1] | [![Ref][github_badge]][ref2_5_1] |
|  ![][status2_5_2] | `2.5.2` | Класс.| ![Preview][preview2_5_2] | [![Ref][github_badge]][ref2_5_2] |
|  ![][status2_5_3] | `2.5.3` | Иерархия классов.| ![Preview][preview2_5_3] | [![Ref][github_badge]][ref2_5_3] |

[status2_5_3]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_5_3/ReadmeSource/Status/status.svg
[preview2_5_3]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_5_3/ReadmeSource/IMG_2_5/3_Preview.png
[ref2_5_3]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_5_3

[status2_5_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_5_2/ReadmeSource/Status/status.svg
[preview2_5_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_5_2/ReadmeSource/IMG_2_5/2_Preview.png
[ref2_5_2]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_5_2

[status2_5_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_5_1/ReadmeSource/Status/status.svg
[preview2_5_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_5_1/ReadmeSource/IMG_2_5/1_Preview.png
[ref2_5_1]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_5_1

</details>

<details >
<summary><b>📂 2.4.0 ООП: наследование и полиморфизм. </b></summary>
<br>

| Статус | ID | Описание задачи | Preview | Ссылка |
| :---: | :---: | :--- | :---: | :---: |
|  ![][status2_4_1] | `2.4.1` | Фигуры. Количество сторон.| ![Preview][preview2_4_1] | [![Ref][github_badge]][ref2_4_1] |
|  ![][status2_4_2] | `2.4.2` |Фигуры. Стороны и углы.| ![Preview][preview2_4_2] | [![Ref][github_badge]][ref2_4_2] |
|  ![][status2_4_3] | `2.4.3` |Фигуры. Методы.| ![Preview][preview2_4_3] | [![Ref][github_badge]][ref2_4_3] |


[status2_4_3]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_4_3/ReadmeSource/Status/status.svg
[preview2_4_3]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_4_3/ReadmeSource/IMG_2_4/3_Preview.png
[ref2_4_3]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_4_3

[status2_4_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_4_2/ReadmeSource/Status/status.svg
[preview2_4_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_4_2/ReadmeSource/IMG_2_4/2_Preview.png
[ref2_4_2]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_4_2

[status2_4_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_4_1/ReadmeSource/Status/status.svg
[preview2_4_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_4_1/ReadmeSource/IMG_2_4/1_Preview.png
[ref2_4_1]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_4_1


</details>

<details >
<summary><b>📂 2.3.0 ООП: абстракция и инкапсуляция. </b></summary>
<br>

| Статус | ID | Описание задачи | Preview | Ссылка |
| :---: | :---: | :--- | :---: | :---: |
|  ![][status2_3_1] | `2.3.1` | Адреса.| ![Preview][preview2_3_1] | [![Ref][github_badge]][ref2_3_1] |
|  ![][status2_3_2] | `2.3.2` | Сортируем адреса.| ![Preview][preview2_3_2] | [![Ref][github_badge]][ref2_3_2] |


[status2_3_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_3_2/ReadmeSource/Status/status.svg
[preview2_3_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_3_2/ReadmeSource/IMG_2_3/2_Preview.png
[ref2_3_2]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_3_2

[status2_3_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_3_1/ReadmeSource/Status/status.svg
[preview2_3_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_3_1/ReadmeSource/IMG_2_3/1_Preview.png
[ref2_3_1]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_3_1


</details>


<details >
<summary><b>📂 2.2.0 Классы и объекты </b></summary>
<br>

| Статус | ID | Описание задачи | Preview | Ссылка |
| :---: | :---: | :--- | :---: | :---: |
|  ![][status2_2_1] | `2.2.1` | Калькулятор двух чисел| ![Preview][preview2_2_1] | [![Ref][github_badge]][ref2_2_1] |
|  ![][status2_2_2] | `2.2.2` | Счётчик| ![Preview][preview2_2_2] | [![Ref][github_badge]][ref2_2_2] |


[status2_2_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_2_2/ReadmeSource/Status/status.svg
[preview2_2_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_2_2/ReadmeSource/IMG_2_2/2_Preview.png
[ref2_2_2]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_2_2

[status2_2_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_2_1/ReadmeSource/Status/status.svg
[preview2_2_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_2_1/ReadmeSource/IMG_2_2/1_Preview.png
[ref2_2_1]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_2_1

</details>


<details>
<summary><b>📂 2.1.0 Структуры и перечисления </b></summary>
<br>

| Статус | ID | Описание задачи | Preview | Ссылка |
| :---: | :---: | :--- | :---: | :---: |
|  ![][status2_1_1] | `2.1.1` | Месяцы. | ![Preview][preview2_1_1] | [![Ref][github_badge]][ref2_1_1] |
|  ![][status2_1_2] | `2.1.2` | Счета.  | ![Preview][preview2_1_2] | [![Ref][github_badge]][ref2_1_2] |
|  ![][status2_1_3] | `2.1.3` | Вывод структуры. | ![Preview][preview2_1_3] | [![Ref][github_badge]][ref2_1_3] |

[status2_1_3]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_1_3/ReadmeSource/Status/status.svg
[preview2_1_3]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_1_3/ReadmeSource/IMG_2_1/3_Preview.png
[ref2_1_3]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_1_3

[status2_1_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_1_2/ReadmeSource/Status/status.svg
[preview2_1_2]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_1_2/ReadmeSource/IMG_2_1/2_Preview.png
[ref2_1_2]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_1_2

[status2_1_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_1_1/ReadmeSource/Status/status.svg
[preview2_1_1]: https://github.com/Mr-Maks-S-A/Learn_2/blob/v2_1_1/ReadmeSource/IMG_2_1/1_Preview.png
[ref2_1_1]: https://github.com/Mr-Maks-S-A/Learn_2/tree/v2_1_1

</details>


</details>

[github_badge]: https://img.shields.io/badge/Ref-0075ca?style=flat&logo=github


<details>
<summary><b>1 Основы программирования на C++</b></summary>
<br>
  <details>
    <summary><b>📂 1.13.0 Игра Жизнь</b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
                <tr align="center">
                  <td>✅</td>
                  <td><code>1.13.1</code></td>
                  <td align="left">Базовая двумерная версия</td>
                  <td>
                    <a href="https://github.com/Mr-Maks-S-A/Learn/blob/v1.13.1/README_IMG/1.13.1_Game_of_Life.png">
                      <img src="https://github.com/Mr-Maks-S-A/Learn/blob/v1.13.1/README_IMG/1.13.1_Game_of_Life.png" height="40" alt="NO_IMG">
                    </a>
                  </td>
                  <td><a href="<!--https://github.com/Mr-Maks-S-A/Learn/tree/v1.13.1-->"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
                </tr>
            </tbody>
           </table>
        </td>
      </tr>
    </table>
  </details>

  <details>
    <summary><b>📂 1.12.0 Работа с файлами </b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.12.1</code></td>
                <td align="left">Вывод файла.</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.12.1_File_output.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.12.1_File_output.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.12.1"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.12.2</code></td>
                <td align="left">Чтение массива из файла</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.12.2_Reading_an_array_from_a_file.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.12.2_Reading_an_array_from_a_file.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.12.2"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.12.3</code></td>
                <td align="left">Пользовательский массив в файл</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.12.3_Custom_array_to_file.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.12.3_Custom_array_to_file.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.12.3"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.12.4</code></td>
                <td align="left">Считывание двумерного массива из файла</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.12.4_Reading_a_two-dimensional_array_from_a_file.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.12.4_Reading_a_two-dimensional_array_from_a_file.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.12.4"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>


<details>
    <summary><b>📂 1.11.0 «Строки» </b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.11.1</code></td>
                <td align="left">Приветствие.</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.11.1_Greetings.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.11.1_Greetings.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.11.1"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.11.2</code></td>
                <td align="left">Угадайка.</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/11.11.2_Guessing_game.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.11.2_Guessing_game.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.11.2"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>

 <details>
    <summary><b>📂 1.10.0 Указатели. Динамическая память</b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.10.1</code></td>
                <td align="left">Ввод массива с консоли</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.10.1_Указатели_Динамическая_память-Ввод_массива_с_консоли.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.10.1_Указатели_Динамическая_память-Ввод_массива_с_консоли.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.10.1"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.10.2</code></td>
                <td align="left">Создатель массивов.</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.10.2_Указатели_Динамическая_память-Создатель_массивов.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.10.2_Указатели_Динамическая_память-Создатель_массивов.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.10.2"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.10.3</code></td>
                <td align="left">Динамическая таблица умножения.</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.10.3_Указатели_Динамическая_память-Динамическая_таблица_умножения.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.10.3_Указатели_Динамическая_память-Динамическая_таблица_умножения.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.10.3"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>

  <details>
    <summary><b>📂 1.9.0 Указатели. Массивы и параметры функций</b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.9.1</code></td>
                <td align="left">Вывод массива</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.9.1_Указатели_Массивы_и_параметры_функций-Вывод-массива.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.9.1_Указатели_Массивы_и_параметры_функций-Вывод-массива.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.9.1"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.9.2</code></td>
                <td align="left">Снова swap</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.9.2_Указатели_Массивы_и_параметры_функций-Снова_swap.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.9.2_Указатели_Массивы_и_параметры_функций-Снова_swap.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.9.2"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.9.3</code></td>
                <td align="left">Переворот массива</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.9.3_Указатели_Массивы_и_параметры_функций-Переворот_массива.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.9.3_Указатели_Массивы_и_параметры_функций-Переворот_массива.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.9.3"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>

  <details>
    <summary><b>📂 1.8.0 Область видимости и пространства имён</b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.8.1</code></td>
                <td align="left">Считающая функция</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/8.1_Область_видимости_переменных_и_типы_памяти_Пространства_имён-Обмен_значениями.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/8.1_Область_видимости_переменных_и_типы_памяти_Пространства_имён-Обмен_значениями.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v8.1"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.8.2</code></td>
                <td align="left">Одинаковые, но разные функции</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/8.2_Область_видимости_переменных_и_типы_памяти_Пространства_имён-Одинаковые_но_разные_функции.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/8.2_Область_видимости_переменных_и_типы_памяти_Пространства_имён-Одинаковые_но_разные_функции.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v8.2"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>

  <details>
    <summary><b>📂 1.7.0 Модель памяти и хранение данных</b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.7.1</code></td>
                <td align="left">Адреса переменных</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/7.1.1_Модель_памяти_и_хранение_данных-Адреса_переменных.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/7.1.1_Модель_памяти_и_хранение_данных-Адреса_переменных.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v7.1"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.7.2</code></td>
                <td align="left">Обмен значениями</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/7.2.1_Модель_памяти_и_хранение_данных-Обмен_значениями.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/7.2.1_Модель_памяти_и_хранение_данных-Обмен_значениями.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v7.2"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>

  <details>
    <summary><b>📂 1.6.0 Функции и их параметры. Рекурсия</b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.6.1</code></td>
                <td align="left">Арифметические функции</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/6.1.1_Функции_и_их_параметры_Рекурсия-Арифметические_функции.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/6.1.1_Функции_и_их_параметры_Рекурсия-Арифметические_функции.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v6.1"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.6.2</code></td>
                <td align="left">Устранение дублирования</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/6.2.1_Функции_и_их_параметры_Рекурсия-Устранение_дублирования.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/6.2.1_Функции_и_их_параметры_Рекурсия-Устранение_дублирования.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v6.2"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.6.3</code></td>
                <td align="left">Числа Фибоначчи</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/6.3_Функции_и_их_параметры_Рекурсия-Числа_Фибоначчи.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/6.3_Функции_и_их_параметры_Рекурсия-Числа_Фибоначчи.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v6.3"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>

  <details>
    <summary><b>📂 1.5.0 Массивы</b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.5.1</code></td>
                <td align="left">Вывод массива на экран</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/5.1_Массивы-Вывод_массива_на_экран.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/5.1_Массивы-Вывод_массива_на_экран.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v5.1"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.5.2</code></td>
                <td align="left">Максимум и минимум</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/5.2_Массивы-Максимум_и_минимум.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/5.2_Массивы-Максимум_и_минимум.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v5.2"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.5.3</code></td>
                <td align="left">Двумерный массив</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/5.3_Массивы-Двумерный_массив.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/5.3_Массивы-Двумерный_массив.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v5.3"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.5.4</code></td>
                <td align="left">Обратный пузырёк</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/5.4_Массивы-Обратный_пузырёк.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/5.4_Массивы-Обратный_пузырёк.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v5.4"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>

  <details>
    <summary><b>📂 1.4.0 Циклические конструкции</b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.4.1</code></td>
                <td align="left">Сумматор</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/4.1_Циклические_конструкции-Сумматор.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/4.1_Циклические_конструкции-Сумматор.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v4.1"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.4.2</code></td>
                <td align="left">Сумма цифр числа</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/4.2_Циклические_конструкции-Сумма_цифр_числа.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/4.2_Циклические_конструкции-Сумма_цифр_числа.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v4.2"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.4.3</code></td>
                <td align="left">Таблица умножения для числа</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/4.3_Циклические_конструкции-Таблица_умножения_для_числа.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/4.3_Циклические_конструкции-Таблица_умножения_для_числа.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v4.3"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>

  <details>
    <summary><b>📂 1.3.0 Операторы ветвления. Логические операции</b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.3.1</code></td>
                <td align="left">Таблица истинности</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/3.1_Операторы_ветвления_Логические_операции-Таблица_истинности.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/3.1_Операторы_ветвления_Логические_операции-Таблица_истинности.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v3.1"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.3.2</code></td>
                <td align="left">Упорядочить числа</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/3.2_Операторы_ветвления_Логические_операции-Упорядочить_числа.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/3.2_Операторы_ветвления_Логические_операции-Упорядочить_числа.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v3.2"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.3.3</code></td>
                <td align="left">Гороскоп</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/3.3_Операторы_ветвления_Логические_операции-Гороскоп.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/3.3_Операторы_ветвления_Логические_операции-Гороскоп.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v3.3"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.3.4</code></td>
                <td align="left">Сравнить числа</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/3.4_Операторы_ветвления_Логические_операции-Сравнение_чисел.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/3.4_Операторы_ветвления_Логические_операции-Сравнение_чисел.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v3.3"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>

  <details>
    <summary><b>📂 1.2.0 Переменные и их типы</b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.2.1</code></td>
                <td align="left">Повторите число</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/2.1_Переменные_и_их_типы-Повторите_Число.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/2.1_Переменные_и_их_типы-Повторите_Число.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v2.1"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
              <tr align="center">
                <td>✅</td>
                <td><code>1.2.2</code></td>
                <td align="left">Повторите слово</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/2.2_Переменные_и_их_типы-Повторите_слово.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/2.2_Переменные_и_их_типы-Повторите_слово.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v2.2"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>

  <details>
    <summary><b>📂 1.1.0 Знакомство с C++</b></summary>
    <br>
    <table width="100%">
      <tr>
        <td width="20%" style="border: none;"></td>
        <td style="border: none;">
          <table width="100%">
            <thead>
              <tr align="center">
                <th width="10%">Статус</th>
                <th width="10%">ID</th>
                <th>Описание задачи</th>
                <th width="20%">Preview</th>
                <th width="15%">Ссылка</th>
              </tr>
            </thead>
            <tbody>
              <tr align="center">
                <td>✅</td>
                <td><code>1.1.1</code></td>
                <td align="left">Написать первую программу</td>
                <td>
                  <a href="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.1_Helloworld.png">
                    <img src="https://github.com/Mr-Maks-S-A/Learn/blob/main/README_IMG/1.1_Helloworld.png" height="40" alt="NO_IMG">
                  </a>
                </td>
                <td><a href="https://github.com/Mr-Maks-S-A/Learn/tree/v1.0"><img src="https://img.shields.io/badge/Ref-0075ca?style=for-the-badge&logo=github" height="20"></a></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </table>
  </details>

</details>



