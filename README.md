# OOP - OOP course projects, ITMO University

## Main
1. Small utility to work with INI files - parsing and extracting data.
2. Wrapper class for ```std::multiset``` to keep track of average, min and max values.
3. Utility to work with ```WAV``` files - saving to another file, trimming, applying reverberation for particular channels, converting to mono.
4. A small programm to factorize a ```uint64_t``` number.
5. Implementation of threadpool using ```<thread>``` wich takes implementations of ```Reader``` and ```Writer``` interfaces, number of threads and a worker function. The threadpool uses a shared queue, one producer thread and ```n``` consumer threads.  It supports terminating, pausing and resuming of calculations. As an example, this project includes a program which factorizes many numbers using this threadpool.
6. ```std::vector```-style ```Vector``` implementation with allocator, move-semantics and placement-new.
## Extra <br />
1. Programm to transform a file in the following format: <br />
```<register name><tab><addr 1><addr 2><command> ```<br />
to a ```.ini``` and a ```.h``` files. To compile it on linux use gcc with ```main_linux.cpp``` file, <br />
or ```main_windows.cpp``` using VS compiler on Windos <br />
**Usage** <br /> 
 Windows - ```<program> <input file name>``` <br />
 Linux - ```<program> [params]``` <br />
```<program> <input file name>``` <br />
  **Params:** </h2><br />
-f file_name - source file <br />
-i file_name - outpit INI file <br />
-h file_name - output .h file <br />


# OOP - Лабораторные работы по ООП за третий семестр, ИС ИТиП ИТМО

## Основные лабораторные
1. Создание класса для работы с INI файлами - парсинг, получение данных.
2. Создание класса для хранения чисел с возможностью узнавать среднее арифмитическое, 
количество чисел больше/меньше заданной границы.
3. Создание класса-обертки для работы с ```WAV``` файлами - сохранение в другой файл, 
реверберация по выбранным каналам, обрезка некоторого количество секунд из аудио-файла.
4. Создание класса для факторизации целого числа типа ```uint64_t```
5. Обретка над 4 лабораторной для параллельного вычисления используюя ```<thread>```. Возможность останавливать вычисления, ставить на паузу и продолжать, включив режим диалога с пользователем. Можно выбрать количество потоков, производящих расчеты.
6. Реализация ```Vector``` в стиле ```std::vector```.
## Дополнительные лабораторные <br />
1. Программа для преобразования файла в формате <br />
```<Название регистра><tab><адресс 1><адресс 2><команда> ```<br />
в ```.ini``` и ```.h``` файлы. Для linux программа компилируется с gcc с ```main_linux.cpp```, <br />
В windows - ```main_windows.cpp с компилятором VS.``` <br />
**Использование** <br /> 
 Windows - ```<исполняемый файл> <имя входного файла>``` <br />
 Linux - ```<исполняемый файл> [параметры]``` <br />
```<исполняемый файл> <имя входного файла>``` <br />
  **Параметры:** </h2><br />
-f имя_файла - название файла для обработки <br />
-i имя_файла - название выходного INI файла <br />
-h имя_файла - название выходного .h файла <br />
Если -i, -h не указан, то имя выходного файла будет сформировано из 
имени входного.
