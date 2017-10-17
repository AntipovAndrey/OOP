# OOP - Лабораторные работы по ООП за третий семестр, ИС ИТиП ИТМО

## Основные лабораторные
1. Создание класса для работы с INI файлами - парсинг, получение данных.
2. Создание класса для хранения чисел с возможностью узнавать среднее арифмитическое, 
количество чисел больше/меньше заданной границы.
3. Создание класса-обертки для работы с WAV файлами - сохранение в другой файл, 
реверберация по выбранным каналам, обрезка некоторого количество секунд из аудио-файла.
## Дополнительные лабораторные <br />
1. Программа для преобразования файла в формате <br />
<Название регистра><tab><адресс 1><адресс 2><команда> <br />
в .ini и .h файлы. Для linux программа компилируется с gcc с main_linux.cpp, <br />
В windows - main_windows.cpp с компилятором VS. <br />
**Использование** <br /> 
 Windows - <исполняемый файл> <имя входного файла> <br />
 Linux - <исполняемый файл> [параметры] <br />
<исполняемый файл> <имя входного файла> <br />
  **Параметры:** </h2><br />
-f имя_файла - название файла для обработки <br />
-i имя_файла - название выходного INI файла <br />
-р имя_файла - название выходного .h файла <br />
Если -i, -p не указан, то имя выходного файла будет сформировано из 
имени входного.
