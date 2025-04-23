#include "../../utils/uthash.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1024 * 1024
#define MAX_WORDS 4096
#define DEBUG 1

typedef struct {
  char word[100];    // clave: la palabra
  int count;         // valor: cantidad de ocurrencias
  UT_hash_handle hh; // manejador para uthash
} WordCount;

void print_table(WordCount **word_counts, int limit) {

  WordCount *s;

  for (s = *word_counts; s != NULL && limit > 0;
       s = (WordCount *)(s->hh.next), limit--) {
    printf("%s: %d\n", s->word, s->count);
  }
}

int by_count(const WordCount *a, const WordCount *b) {
  return b->count - a->count;
}

void sort_by_count(WordCount **word_counts) {
  HASH_SORT(*word_counts, by_count);
}

void add_word(WordCount **word_counts, char *word) {

  WordCount *entry = NULL;

  HASH_FIND_STR(*word_counts, word, entry);

  if (entry == NULL) {
    entry = (WordCount *)malloc(sizeof *entry);
    strcpy(entry->word, word);
    entry->count = 1;
    HASH_ADD_STR(*word_counts, word, entry);
  } else {
    entry->count++;
  }
}

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);
  int id, comm_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  char *words[MAX_WORDS];
  int total_words = 0;

  if (id == 0) {

    char buffer[MAX_LEN];
    size_t bytesRead = fread(buffer, 1, MAX_LEN, stdin);

    buffer[bytesRead] = '\0';

    char *delimiter = " \t\n'<>/,.;:-";
    char *token = strtok(buffer, delimiter);

    while (token != NULL) {
      words[total_words++] = token;
      token = strtok(NULL, delimiter);
    }

#ifdef DEBUG
    for (int i = 0; i < total_words; i++) {
      printf("%s\n", words[i]);
    }
#endif
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
