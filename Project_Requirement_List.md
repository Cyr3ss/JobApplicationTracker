# Список Требований к Проекту "Трекер Заявок на Работу"

Ниже перечислены ключевые требования, реализованные в проекте для достижения его целей:

1.  **Создание Заявки:** Пользователь должен иметь возможность создать новую запись о заявке, указав:
    *   Название компании
    *   Название вакансии
    *   Дату подачи
    *   Текущий статус заявки
    *   Дополнительные заметки (опционально)
2.  **Просмотр Всех Заявок:** Пользователь должен иметь возможность отобразить список всех сохраненных заявок со всеми их деталями.
3.  **Обновление Заявки:** Пользователь должен иметь возможность выбрать существующую заявку по ее ID и обновить любое из ее полей.
4.  **Удаление Заявки:** Пользователь должен иметь возможность выбрать существующую заявку по ее ID и удалить ее из системы.
5.  **Сохранение Данных в Файл:** Все данные о заявках должны автоматически сохраняться в текстовый файл (например, `applications.txt`) после каждого изменения (создания, обновления, удаления) и при выходе из программы.
6.  **Загрузка Данных из Файла:** При запуске программы данные о ранее сохраненных заявках должны автоматически загружаться из текстового файла.
7.  **Генерация Уникального ID:** Каждая новая заявка должна автоматически получать уникальный числовой идентификатор (ID), который не должен повторяться. ID должен генерироваться последовательно (например, если максимальный ID в системе 5, следующий будет 6; если система пуста, первый ID будет 1).
8.  **Консольный Интерфейс Пользователя:** Взаимодействие с пользователем должно осуществляться через текстовое меню в консоли с понятными опциями.
9.  **Валидация Ввода Пользователя:**
    *   Система не должна допускать ввод пустых значений для обязательных полей (например, название компании, вакансии).
    *   При вводе числовых значений (например, выбор пункта меню, ID заявки) система должна проверять, что введено именно число, и запрашивать повторный ввод в случае ошибки.
10. **Выбор Статуса из Списка:** При создании или обновлении заявки пользователь должен иметь возможность выбрать статус из предопределенного списка вариантов (например, "Подано", "Собеседование HR", "Оффер" и т.д.), а не вводить его вручную произвольным текстом.

**Возможные простые расширения (не реализованы, но легко добавляемы):**

*   Поиск заявок по названию компании.
*   Фильтрация заявок по статусу.
*   Подсчет общего количества заявок или заявок в определенном статусе.
*   Добавление нового поля, например, "Ссылка на вакансию".
