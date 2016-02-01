#ifndef ML_H
#define ML_H

#define START_CALLBACK \
	Generic_Callback_Data *gcbd = static_cast<Generic_Callback_Data *>(generic_data); \
	void *data = gcbd->userdata; \
	int stage = *(int *)data; \
	*(int *)data = stage + 1; \
	int count = 0;

#define NEXT_STAGE if (stage == count++)

#endif // ML_H
