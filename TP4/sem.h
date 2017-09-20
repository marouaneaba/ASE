#ifndef _sem_h
#define _sem_h

#include "context.h"

struct sem_s {
	int count;
	struct ctx_s * blocked;
};

void sem_init(struct sem_s *sem, unsigned int val);
void sem_up(struct sem_s *sem);
void sem_down(struct sem_s *sem);

#endif
