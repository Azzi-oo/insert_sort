#include <stdio.h>
#include <stdlib.h>

struct Record {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int status;
    int code;
};

// Функция для сравнения двух записей по дате и времени
int compareRecords(const void *a, const void *b) {
    struct Record *recordA = (struct Record *)a;
    struct Record *recordB = (struct Record *)b;

    // Сравниваем годы
    if (recordA->year != recordB->year)
        return recordA->year - recordB->year;

    // Сравниваем месяцы
    if (recordA->month != recordB->month)
        return recordA->month - recordB->month;

    // Сравниваем дни
    if (recordA->day != recordB->day)
        return recordA->day - recordB->day;

    // Сравниваем часы
    if (recordA->hour != recordB->hour)
        return recordA->hour - recordB->hour;

    // Сравниваем минуты
    if (recordA->minute != recordB->minute)
        return recordA->minute - recordB->minute;

    // Сравниваем секунды
    if (recordA->second != recordB->second)
        return recordA->second - recordB->second;

    return 0; // Записи равны
}

void printRecord(struct Record record) {
    printf("Дата и время: %d-%02d-%02d %02d:%02d:%02d\n", record.year, record.month, record.day,
           record.hour, record.minute, record.second);
    printf("Статус: %d\n", record.status);
    printf("Код: %d\n", record.code);
    printf("\n");
}

void printRecords(struct Record *records, int numRecords) {
    printf("Всего записей: %d\n", numRecords);
    for (int i = 0; i < numRecords; i++) {
        printRecord(records[i]);
    }
}

int main() {
    FILE *file = fopen("datasets/door_state_1", "rb");
    if (file == NULL) {
        printf("n/a\n");
        return 1;
    }

    // Определяем размер файла
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Проверяем, что размер файла кратен размеру записи
    if (fileSize % sizeof(struct Record) != 0) {
        fclose(file);
        printf("n/a\n");
        return 1;
    }

    // Вычисляем количество записей
    int numRecords = fileSize / sizeof(struct Record);

    // Выделяем память под массив записей
    struct Record *records = (struct Record *)malloc(numRecords * sizeof(struct Record));
    if (records == NULL) {
        fclose(file);
        printf("n/a\n");
        return 1;
    }

    // Читаем записи из файла
    size_t bytesRead = fread(records, sizeof(struct Record), numRecords, file);
    fclose(file);

    if (bytesRead != numRecords) {
        free(records);
        printf("n/a\n");
        return 1;
    }

    int choice;
    do {
        printf("Меню:\n");
        printf("0 - Вывести содержимое файла\n");
        printf("1 - Отсортировать содержимое файла и вывести\n");
        printf("2 - Добавить запись, отсортировать содержимое файла и вывести\n");
        printf("Выберите пункт меню (0-2): ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                printRecords(records, numRecords);
                break;
            case 1:
                qsort(records, numRecords, sizeof(struct Record), compareRecords);
                printRecords(records, numRecords);
                break;
            case 2: {
                struct Record newRecord;

                printf("Введите дату и время (гггг мм дд чч мм сс): ");
                scanf("%d %d %d %d %d %d", &newRecord.year, &newRecord.month,
                      &newRecord.day, &newRecord.hour, &newRecord.minute, &newRecord.second);

                printf("Введите статус (0/1): ");
                scanf("%d", &newRecord.status);

                printf("Введите код: ");
                scanf("%d", &newRecord.code);

                numRecords++;
                records = (struct Record *)realloc(records, numRecords * sizeof(struct Record));
                records[numRecords - 1] = newRecord;

                qsort(records, numRecords, sizeof(struct Record), compareRecords);
                printRecords(records, numRecords);
                break;
            }
            default:
                printf("Некорректный выбор. Попробуйте снова.\n");
                break;
        }
    } while (choice != 0);

    file = fopen("datasets/door_state_1", "wb");
    if (file == NULL) {
        free(records);
        printf("n/a\n");
        return 1;
    }

    size_t bytesWritten = fwrite(records, sizeof(struct Record), numRecords, file);
    fclose(file);

    if (bytesWritten != numRecords) {
        free(records);
        printf("n/a\n");
        return 1;
    }

    free(records);

    return 0;
}
