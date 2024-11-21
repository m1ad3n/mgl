#ifndef UTILS_HEADER
#define UTILS_HEADER

/* linked lists */
typedef struct llist {
	void* value;
	struct llist* next;
} llist;

llist* llist_new(void* value_a);
void llist_destroy(llist* root);

/* basic utils */
void time_it(const char* msg);
char* getfile_content(const char* path);

#endif // UTILS_HEADER
