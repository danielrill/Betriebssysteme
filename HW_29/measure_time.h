#ifndef MEASURE_TIME_H
#define MEASURE_TIME_H

int get_diff(struct timespec*, struct timespec*, struct timespec*);

int measure(void (*exe)(), unsigned int loops, struct timespec *results);

#endif
