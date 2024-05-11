# Computer_club
Тестовое задание по C++ для компании YADRO

Полное условие задачи прикепеленно в файле.

В данной задаче был реализован класс Computer_club. Который анализирует входной файл, в случае ошибки входных данных выдает содержимое строки на которой была ошибка и завершает програму с кодом 1.
После проверки условий указать в файле перводится на начало и идет изучение каждого события. Так же в момент начала анализа создается вектор из Computer_table. Данный класс считает выручку каждого конкретного стола, так же считает время которое был занят данный стол.(В данном ТЗ не было сказано информации о том, что если стол уже оплачен на час и клиент уходит раньше, новый клиент, который садится за этот стол, снова оплачивает этот час или нет, однако исходя из примера в ТЗ видно что если клиент оплатил 12 час, то если кто-то сел за этот же стол в 12:33 он за 12 час не платит. В моей программе реализован именно такой подсчет выручки. Так же из проверок которых не было в условиях была добавлена проверка на индекс стола при 2 событии. Если индекс стола больше чем количество столов, тогда выдается ошибка(проеврка на отрицательный индекс учтена при проверки входных данных) Так же реализована проверка в 4 событии, если клиент который стоял в очереди на стол уже ушел из клуба, то за стол садиться следующий в очереди.

## Запуск кода
Для компиляции данного файла использовать:
g++ program.cpp -std=c++11 -o program

Для запуска 
./program filename

## Примеры тестов
Все файлы приложены в  папке test

### Тест №1 (тест из ТЗ)
| Входные данные | Выходные данные|
| ------------- | ------------- |
| 3  | 09:00  |
| 09:00 19:00  | 08:48 1 client1  |
|10|08:48 13 NotOpenYet|
|08:48 1 client1| 09:41 1 client1|
|09:41 1 client1| 09:48 1 client2|
|09:48 1 client2| 09:52 3 client1|
|09:52 3 client1| 09:52 13 ICanWaitNoLonger|
|09:54 2 client1 1| 09:54 2 client1 1|
|10:25 2 client2 2| 10:25 2 client2 2|
|10:58 1 client3| 10:58 1 client3|
|10:59 2 client3 3| 10:59 2 client3 3|
|11:30 1 client4|11:30 1 client4|
|11:35 2 client4 2| 11:35 2 client4 2|
|11:45 3 client4| 11:35 13 PlaceIsBusy|
|12:33 4 client1| 11:45 3 client4|
|12:43 4 client2|12:33 4 client1|
|15:52 4 client4| 12:33 12 client4 1|
||12:43 4 client2|
||15:52 4 client4|
||19:00 11 client3|
||19:00|
||1 70 05:58|
||2 30 02:18|
||3 90 08:01|

В данном тесте видно, что выручка на первом столе считается, по тому принципу о котором писалось выше. То есть в 9:54 сел client1 за первый стол и ушел с него в 12:33 в таком случае он должен по условию заплатить за 9,10,11,12 часы. После чего когда client1 ушел сразу же на его место сел client4(из очереди), то есть в 12:33 он сел за стол, а ушел в 15:52 тогда он должен был заплатить за 12,13,14,15 часы. Тогда сумарная выручка должна была быть 80, но так как в примере она была 70=> что если час оплачен одним клиентом то другие могут пользовать в этот час данным компьютером бесплатно(если клиент уйдет с данного стола)

### Тест №2(проверка запуска программы)
**Запуск:**
./program 

**Вывод:**
Неверный запуск программы, необходим один файл на вход
(анлогично если будет больше одного файла)

### Тест №3(на предварительную проверку файла)
Проверка на отсутсвование времени закрытия клуба
| Входные данные | Выходные данные|
| ------------- | ------------- |
|3|Error in lines: 09:00 |
|09:00||
|10||
|08:48 1 client1||
|09:41 1 client1||
|09:48 1 client2||
|09:52 3 client1||

### Тест №4(на предварительную проверку файла)
Проверка на отсутстивие цены за час
| Входные данные | Выходные данные|
| ------------- | ------------- |
|3|Error in lines: 08:48 1 client1|
|09:00 19:00||
|08:48 1 client1||

### Тест №5(на предварительную проверку файла)
Проверка на неверный индефикатор(имя клиента)
| Входные данные | Выходные данные|
| ------------- | ------------- |
|3|Error in lines: 08:48 1 client1!|
|09:00 19:00||
|10||
|08:48 1 client1!||

### Тест №6(на предварительную проверку файла)
Проверка на неверный номер стола(например на то отрицательный ли он)
| Входные данные | Выходные данные|
| ------------- | ------------- |
|3|Error in lines: 09:54 2 client1 -1|
|09:00 19:00||
|10||
|09:54 2 client1 -1||

### Тест №7(на предварительную проверку файла)
Проверка на неверный формат времени
| Входные данные | Выходные данные|
| ------------- | ------------- |
|3|Error in lines: 8:48 1 client1|
|09:00 19:00||
|10||
|8:48 1 client1||

### Тест №8(на воспроизведение всех возодных ошибок(состояний 13))

По мимо воспроизвдения всех ошибок, так же проверка переполения очереди ожидания
| Входные данные | Выходные данные|
| ------------- | ------------- |
|2| 09:00|
|09:00 19:00|08:48 1 client1|
|10| 08:48 13 NotOpenYet|
|08:48 1 client1|09:00 1 client1|
|09:00 1 client1|09:05 1 client1|
|09:05 1 client1|09:05 13 YouShallNotPass|
|09:10 1 client2|09:10 1 client2|
|09:20 2 client2 1|09:20 2 client2 1|
|09:30 2 client3 1|09:30 2 client3 1|
|10:00 2 client1 1|09:30 13 ClientUnknown|
|10:10 1 client3|10:00 2 client1 1|
|10:15 3 client3|10:00 13 PlaceIsBusy|
|10:21 2 client3 2|10:10 1 client3|
|10:25 3 client1|10:15 3 client3|
|10:30 4 client2|10:15 13 ICanWaitNoLonger|
|15:00 1 client4|10:21 2 client3 2|
|15:00 4 cl2|10:25 3 client1|
|15:01 1 client5|10:30 4 client2|
|15:02 1 client6|10:30 12 client1 1|
|15:10 3 client4|15:00 1 client4|
|15:11 3 client5|15:00 4 cl2|
|15:12 3 client6|15:00 13 ClientUnknown|
|16:10 4 client4|15:01 1 client5|
||15:02 1 client6|
||15:10 3 client4|
||15:11 3 client5|
||15:12 3 client6|
||15:12 11 client6|
||16:10 4 client4|
||19:00 11 client1|
||19:00 11 client3|
||19:00 11 client4|
||19:00 11 client5|
||19:00|
||1 100 09:40|
||2 90 08:39|

### Тест №9(на уход клиента из клуба, пока он стоит в очереди)
| Входные данные | Выходные данные|
| ------------- | ------------- |
|2|09:00|
|09:00 19:00|09:00 1 client1|
|10|09:10 1 client2|09:15 2 client1 1|
|09:00 1 client1|09:16 2 client2 2|
|09:10 1 client2|09:20 1 client3|
|09:15 2 client1 1|09:30 1 client4|
|09:16 2 client2 2|09:35 3 client3|
|09:20 1 client3|09:40 3 client4|
|09:30 1 client4|09:45 4 client3|
|09:35 3 client3|09:50 4 client1|
|09:40 3 client4|**09:50 12 client4 1**|
|09:45 4 client3|19:00 11 client2|
|09:50 4 client1|19:00 11 client3|
||19:00 11 client4|
||19:00|
||1 100 09:45|
||2 100 09:44|

### Тест №10(на отсутствие событий)
| Входные данные | Выходные данные|
| ------------- | ------------- |
|2|09:00|
|09:00 19:00|19:00|
|10|1 0 00:00|
||2 0 00:00|