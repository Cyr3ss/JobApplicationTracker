# Документация по проекту "Трекер Заявок на Работу"

## 1. Обзор

Данный документ описывает основные технические аспекты проекта "Трекер Заявок на Работу", включая использованные алгоритмы, структуры данных, функции/модули и возникшие при разработке сложности.

## 2. Структуры Данных

*   **`struct Application`**:
    *   Описание: Основная структура данных, представляющая одну заявку на работу.
    *   Поля:
        *   `int id`: Уникальный идентификатор заявки.
        *   `string companyName`: Название компании.
        *   `string jobTitle`: Название вакансии.
        *   `string applicationDate`: Дата подачи заявки (в виде строки).
        *   `string status`: Текущий статус заявки.
        *   `string notes`: Дополнительные заметки.
*   **`vector<Application> applications`**:
    *   Описание: Динамический массив (вектор) из стандартной библиотеки C++, используемый для хранения всех объектов `Application` в оперативной памяти программы.
*   **`vector<string> JOB_STATUSES`**:
    *   Описание: Вектор строк, хранящий предопределенный список возможных статусов для заявок, из которого пользователь делает выбор.
*   **`string`**:
    *   Описание: Стандартный класс C++ для работы со строками, используется для всех текстовых полей заявок и для взаимодействия с пользователем.

## 3. Алгоритмы

*   **Работа с файлами (сохранение и загрузка):**
    *   **Сохранение:** Данные из вектора `applications` записываются в текстовый файл (`FILENAME`). Каждая заявка пишется в новую строку. Поля заявки разделяются символом-разделителем `|`. Файл перезаписывается при каждом сохранении.
    *   **Загрузка:** При запуске программы файл `FILENAME` читается построчно. Каждая строка парсится: с помощью `stringstream` и `getline` строка разбивается на сегменты по символу-разделителю `|`. Проверяется корректность количества сегментов. ID преобразуется в число с помощью `stoi`. Создается объект `Application`, который добавляется в вектор `applications`.
*   **Генерация ID:**
    *   При загрузке данных из файла определяется максимальный существующий `id`. Переменная `nextId` устанавливается в `maxId + 1`.
    *   Если файл пуст или не существует, `nextId` инициализируется значением `1`.
    *   При создании новой заявки ей присваивается текущее значение `nextId`, после чего `nextId` инкрементируется.
*   **Поиск Заявки:**
    *   Для операций обновления и удаления используется алгоритм `find_if` из `<algorithm>`. Он ищет в векторе `applications` заявку, у которой поле `id` совпадает с введенным пользователем ID.
*   **Удаление Заявки:**
    *   Используется комбинация `remove_if` и `vector::erase`. `remove_if` перемещает все элементы, не соответствующие условию удаления (т.е. все, кроме удаляемого), в начало вектора и возвращает итератор на "новую" позицию конца полезных данных. Затем `vector::erase` удаляет элементы от этого итератора до фактического конца вектора.
*   **Валидация Ввода Пользователя:**
    *   **Числовой ввод (`getIntInput`)**: В цикле `while(true)` считывается ввод. `cin.good()` проверяет успешность операции. В случае ошибки флаги ошибок `cin` сбрасываются (`cin.clear()`), а некорректный ввод из буфера удаляется с помощью `clearInputBuffer()` (который использует `cin.ignore()`).
    *   **Строковый ввод (`getStringInput`)**: Используется `getline` для чтения всей строки. Проверяется, не пустая ли строка и не состоит ли она только из пробелов (если пустой ввод не разрешен параметром `allowNull`). Ввод запрашивается в цикле до получения корректных данных.
    *   `clearInputBuffer()`: Используется для очистки буфера ввода (особенно символа новой строки `\n`) после операций `cin >> var`, чтобы предотвратить проблемы с последующими вызовами `getline`.

## 4. Функции/Модули

Проект разделен на следующие основные функции:

*   **Вспомогательные функции:**
    *   `clearInputBuffer()`: Очищает буфер стандартного ввода.
    *   `getStringInput()`: Обеспечивает безопасный ввод строки от пользователя с проверками.
    *   `getIntInput()`: Обеспечивает безопасный ввод целого числа от пользователя с проверками.
*   **Функции для работы с файлами:**
    *   `loadApplications(vector<Application>& apps)`: Загружает данные о заявках из файла `FILENAME` в вектор `apps`.
    *   `saveApplications(const vector<Application>& apps)`: Сохраняет текущие данные о заявках из вектора `apps` в файл `FILENAME`.
*   **CRUD-операции и связанные функции:**
    *   `selectStatus(const string& currentStatus, bool allowNoChange)`: Отображает список статусов и позволяет пользователю выбрать один из них.
    *   `createApplication(vector<Application>& apps)`: Управляет процессом создания новой заявки.
    *   `readApplications(const vector<Application>& apps)`: Отображает все сохраненные заявки.
    *   `updateApplication(vector<Application>& apps)`: Управляет процессом обновления существующей заявки.
    *   `deleteApplication(vector<Application>& apps)`: Управляет процессом удаления заявки.
*   **Пользовательский интерфейс:**
    *   `displayMenu()`: Отображает главное меню программы.
*   **Главная функция:**
    *   `main()`: Точка входа в программу. Инициализирует данные, управляет главным циклом программы (отображение меню, обработка выбора пользователя) и завершает работу.

## 5. Возникшие Сложности и Их Решения

*   **Взаимодействие `cin >> var` и `getline(cin, str)`:**
    *   **Сложность:** После ввода числа с помощью `cin >> var`, символ новой строки (`\n`) оставался в буфере ввода. Последующий вызов `getline` считывал этот символ как пустую строку, пропуская ожидаемый ввод пользователя.
    *   **Решение:** Реализована функция `clearInputBuffer()`, использующая `cin.ignore(numeric_limits<streamsize>::max(), '\n');` для очистки буфера ввода после каждого успешного `cin >> var`.
*   **Парсинг строк из файла:**
    *   **Сложность:** Необходимо было надежно разделять строку на поля по заданному разделителю и обрабатывать возможные ошибки (например, неверное количество полей, некорректный формат ID).
    *   **Решение:** Использован `stringstream` для удобного парсинга строки. Добавлены проверки на количество полученных сегментов и `try-catch` блок для `stoi` при преобразовании ID.
*   **Обеспечение корректной работы с уникальными ID:**
    *   **Сложность:** Требовалось, чтобы ID не повторялись и корректно генерировались даже после удаления записей или при пустом файле.
    *   **Решение:** При загрузке данных определяется максимальный ID, и `nextId` устанавливается соответствующим образом. Для пустого файла `nextId` устанавливается в 1.
*   **Валидация пользовательского ввода:**
    *   **Сложность:** Предотвращение ввода пустых строк для обязательных полей или нечисловых данных там, где ожидаются числа.
    *   **Решение:** Функции `getStringInput` и `getIntInput` реализованы с циклами `while(true)` и проверками, запрашивающими повторный ввод до тех пор, пока данные не будут корректными.
*   **Обновление полей с возможностью "оставить как есть":**
    *   **Сложность:** Реализовать интуитивно понятное поведение, когда пользователь при обновлении поля может нажать Enter, чтобы оставить старое значение.
    *   **Решение:** В функции `getStringInput` добавлен параметр `allowNull`. В `updateApplication` логика обработки пустого ввода от `getStringInput(..., true)` позволяет либо обновить поле (если введены новые данные или старое поле было пустым), либо оставить старое значение (если новый ввод пуст, а старое значение было непустым).
