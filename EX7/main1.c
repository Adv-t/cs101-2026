#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_NUMBER 1
#define MAX_NUMBER 69
#define NUMBERS_PER_TICKET 7
#define MAX_TICKETS 5
#define OUTPUT_FILE "lotto.txt"

static int contains(const int numbers[], int count, int value) {
    for (int i = 0; i < count; ++i) {
        if (numbers[i] == value) {
            return 1;
        }
    }
    return 0;
}

static void sort_numbers(int numbers[], int count) {
    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (numbers[j] < numbers[i]) {
                int temp = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = temp;
            }
        }
    }
}

static void generate_ticket(int numbers[NUMBERS_PER_TICKET]) {
    int count = 0;
    while (count < NUMBERS_PER_TICKET) {
        int value = MIN_NUMBER + rand() % (MAX_NUMBER - MIN_NUMBER + 1);
        if (!contains(numbers, count, value)) {
            numbers[count++] = value;
        }
    }
    sort_numbers(numbers, NUMBERS_PER_TICKET - 1);
}

static void print_ticket(FILE *file, int ticket_number, const int numbers[NUMBERS_PER_TICKET]) {
    fprintf(file, "[%d]:", ticket_number);
    for (int i = 0; i < NUMBERS_PER_TICKET; ++i) {
        fprintf(file, " %02d", numbers[i]);
    }
    fputc('\n', file);
}

int main(void) {
    int ticket_count;
    int tickets[MAX_TICKETS][NUMBERS_PER_TICKET];

    
    time_t now = time(NULL) + 8 * 3600;
    struct tm *tw_time = gmtime(&now);
    if (tw_time == NULL) {
        return EXIT_FAILURE;
    }

    char time_str[32];
    strcpy(time_str, asctime(tw_time));
    time_str[strcspn(time_str, "\n")] = '\0';

    printf("歡迎光臨長庚樂透彩購買機台\n");
    printf("請問您要買幾組樂透彩 : ");
    if (scanf("%d", &ticket_count) != 1 || ticket_count < 1 || ticket_count > MAX_TICKETS) {
        return EXIT_FAILURE;
    }

    srand((unsigned int)time(NULL) ^ (unsigned int)clock());
    for (int i = 0; i < ticket_count; ++i) {
        generate_ticket(tickets[i]);
    }

    FILE *file = fopen(OUTPUT_FILE, "w");
    if (file == NULL) {
        perror("lotto.txt");
        return EXIT_FAILURE;
    }

    fprintf(file, "========= lotto649 =========\n");
    fprintf(file, "= %s =\n", time_str);

    for (int i = 0; i < ticket_count; ++i) {
        print_ticket(file, i + 1, tickets[i]);
    }
    for (int i = ticket_count; i < MAX_TICKETS; ++i) {
        fprintf(file, "[%d]: -- -- -- -- -- -- --\n", i + 1);
    }

    fprintf(file, "========= csie@CGU =========\n");

    if (fclose(file) != 0) {
        perror("lotto.txt");
        return EXIT_FAILURE;
    }

    printf("以為您購買的 %d 組樂透組合輸出至 %s\n", ticket_count, OUTPUT_FILE);

    return EXIT_SUCCESS;
}
