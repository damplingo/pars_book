# pars_book

РАЗБИЕНИЕ ТЕКСТА КНИГИ ПО РАЗДЕЛАМ
класс Chapter - класс главы. Информация из .csv
класс Contents - класс оглавления. хранит вектор из глав Chapter.
класс Search. В нем хранится вектор пар страница-положение курсора на странице.
Для корректного нахождения страницы функция findLIS, находящая максимальную возрастающую подпоследовательность В книге не все страницы обозначены, берем ближайшую меньшую страницу для поиска нужной главы (от start_page). На ней находим строку с меньшим расстоянием левенштейна от названия искомой главы. Нужно понять как из слова "ОБЪЕКТ" сделать "объект" т.к. названия глав пишутся капсом