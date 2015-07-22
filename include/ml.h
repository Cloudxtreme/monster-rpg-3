#ifndef ML_H
#define ML_H

#define START_CALLBACK \
	int stage = *(int *)data; \
	*(int *)data = stage + 1; \
	int count = 0;

#define NEXT_STAGE if (stage == count++)

#endif // ML_H