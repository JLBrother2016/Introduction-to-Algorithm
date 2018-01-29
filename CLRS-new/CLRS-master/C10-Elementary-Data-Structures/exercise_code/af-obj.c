#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10

typedef int list_t;
typedef int obj_t;

int empty_list = -1;
int cells[MAX_SIZE * 3];
int free_list;

#define NEXT(i) cells[(i) + 1]
#define PREV(i) cells[(i) + 2]
#define KEY(i)  cells[i]

void init_storage() {
    int i;
    for (i = 0; i < (MAX_SIZE - 1) * 3; i += 3)
        NEXT(i) = i + 3;
    
    NEXT(i) = -1;
    free_list = 0;
}

list_t allocate_object() {
    if (free_list == -1) {
        fprintf(stderr, "Storage depleted\n");
        exit(1);
    }
    
    list_t new1 = free_list;
    free_list = NEXT(free_list);
    return new1;
}

void free_object(list_t list) {
    NEXT(list) = free_list;
    free_list = list;
}

list_t cons(obj_t key, list_t list) {
    list_t new1 = allocate_object();
    
    NEXT(new1) = list;
    PREV(new1) = empty_list;
    KEY(new1) = key;
    
    if (list != empty_list) {
        PREV(list) = new1;
    }
    
    return new1;
}

void delete1(list_t list) {
    if (PREV(list) != empty_list) {
        NEXT(PREV(list)) = NEXT(list);
    }
    
    if (NEXT(list) != empty_list) {
        PREV(NEXT(list)) = PREV(list);
    }
    
    free_object(list);
}

obj_t get(list_t list) {
    if (list == empty_list) return -1;
    return KEY(list);
}

list_t next(list_t list) {
    if (list == empty_list) return -1;
    return NEXT(list);
}

int main()
{
    init_storage();
    list_t new1_list = cons(5, empty_list);
    new1_list = cons(4, new1_list);
    printf("%d\n", get(new1_list));
    printf("%d\n", get(next(new1_list)));
    delete1(new1_list);
    
    return 0;
}
