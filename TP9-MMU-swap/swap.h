#ifndef _swap_h
#define _swap_h

char init_swap(const char *path);
char store_to_swap(int vpage, int ppage);
char fetch_from_swap(int vpage, int ppage);

#endif
