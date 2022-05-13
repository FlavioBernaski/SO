#include <pthread.h>

/* --- monitor operations --- */
void initMonitor();
void destroyMonitor();
int put(int thread_id);
int take();