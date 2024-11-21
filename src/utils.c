#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

llist* llist_new(void* value_a) {
	llist* tmp = (llist*)malloc(sizeof(llist));
	tmp->value = value_a;
	tmp->next = NULL;
	return tmp;
}

void llist_destroy(llist* node) {
	if (!node) return;
	if (node->next) llist_destroy(node->next);
	free(node);
}

/*
 * This function measures the time between calls,
 * saving the start time the first time it's called,
 * printing the elapsed time the next time,
 * and then resetting the start time.
 */
void time_it(const char* msg) {
	static clock_t start, end;
	if (!start) {
		start = clock();
		return;
	}

	end = clock();
	fprintf(stdout, "[TIME_IT]: %s took %.5f secs\n", msg, (double)(end - start) / CLOCKS_PER_SEC);
	start = 0;
}

/**
 * returns pointer to the heap allocated memory
 * so must be freed
 */
char* getfile_content(const char* path) {
	FILE* fptr = fopen(path, "r");
	fseek(fptr, 0, SEEK_END);

	size_t size = ftell(fptr);
	char* content = (char*)malloc(size + 1);
	content[size] = '\0';

	fseek(fptr, 0, SEEK_SET);
	for (size_t i = 0; i < size; i++)
		content[i] = fgetc(fptr);

	return content;
}

