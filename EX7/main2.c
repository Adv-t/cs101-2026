#include <stdio.h>
#include <stdlib.h>

#define BOOK_FILE "Book.bin"
#define TITLE_SIZE 128

typedef struct Books {
    int id;
    int price;
    char title[TITLE_SIZE];
} book_t;

int total_price(const book_t books[], size_t count)
{
    int total = 0;

    for (size_t i = 0; i < count; ++i) {
        total += books[i].price;
    }
    return total;
}

void list_books(const book_t books[], size_t count)
{
    printf("%-4s %-8s %s\n", "ID", "Price", "Title");
    printf("---------------------------------------------\n");
    for (size_t i = 0; i < count; ++i) {
        printf("%-4d %-8d %s\n", books[i].id, books[i].price,
               books[i].title);
    }
}

static int save_books(const char *filename, const book_t books[], size_t count)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }

    int success = fwrite(books, sizeof(book_t), count, file) == count;
    if (fclose(file) != 0) {
        success = 0;
    }
    return success;
}

static book_t *load_books(const char *filename, size_t *count)
{
    FILE *file = fopen(filename, "rb");
    book_t *books = NULL;
    long file_size;

    if (file == NULL || count == NULL) {
        if (file != NULL) {
            fclose(file);
        }
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }
    file_size = ftell(file);
    if (file_size < 0 || file_size % (long)sizeof(book_t) != 0) {
        fclose(file);
        return NULL;
    }
    *count = (size_t)file_size / sizeof(book_t);
    rewind(file);

    if (*count > 0) {
        books = malloc(*count * sizeof(book_t));
        if (books == NULL || fread(books, sizeof(book_t), *count, file) != *count) {
            free(books);
            books = NULL;
        }
    }
    fclose(file);
    return books;
}

int main(void)
{
    const book_t initial_books[] = {
        {1, 1000, "All the Light We Cannot See"},
        {2,  300, "The 38 Letters from J.D. Rockefeller to his son"},
        {3, 1000, "The Ballad of Songbirds and Snakes"},
        {4,  550, "Killers of the Flower Moon"},
        {5,  870, "Elon Musk"},
        {6, 1344, "Milk and Honey 12-Month 2024"}
    };
    const size_t initial_count = sizeof(initial_books) / sizeof(initial_books[0]);
    size_t count = 0;
    book_t *books;

    if (!save_books(BOOK_FILE, initial_books, initial_count)) {
        fprintf(stderr, "Unable to write %s.\n", BOOK_FILE);
        return EXIT_FAILURE;
    }

    books = load_books(BOOK_FILE, &count);
    if (books == NULL && count != 0) {
        fprintf(stderr, "Unable to read %s.\n", BOOK_FILE);
        return EXIT_FAILURE;
    }

    list_books(books, count);
    printf("---------------------------------------------\n");
    printf("Total price: %d\n", total_price(books, count));

    free(books);
    return EXIT_SUCCESS;
}
