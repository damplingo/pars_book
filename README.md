# Курсовая работа 2023/2024 

## Тема: РАЗБИЕНИЕ ТЕКСТА КНИГИ ПО РАЗДЕЛАМ
Для упрощения поиска информации в источниках для преподавателей была разработана система разбиения текста книги по разделам в соответствии с оглавлением.

## Общая информация
- класс Chapter - класс главы. Информация из .csv
- класс Contents - класс оглавления. хранит вектор из глав Chapter.
- класс Search. В нем хранится вектор пар страница-номер строки.

- Для корректного нахождения страницы функция findLIS, находящая максимальную возрастающую подпоследовательность В книге не все страницы обозначены, берем ближайшую меньшую страницу для поиска нужной главы (от start_page). На ней находим строку с меньшим расстоянием левенштейна от названия искомой главы.

## Примечания по запуску
В телеграмм преподавателю направлен .exe файл и необходимые .dll для запуска.
Подключение к базе в файле main.cpp(db_name = book_info).
В эту базу необходимо добавить таблицу и перенести информацию из файла .csv командой

```
\COPY public.in_doc_contents(id, doc_id, ordernum, index, title, start_page, end_page, parent_unit, level) FROM '7110.csv' DELIMITER ';' CSV HEADER;

```

