#include <stdlib.h>
#include <stdio.h>

#define DEF_CAPACITY 4

typedef struct vector {
	void **data;
	int size;
	int capacity;
} vector;

void vector_init(vector *v)
{
	v->capacity = DEF_CAPACITY;
	v->size = 0;
	v->data = malloc(sizeof(void *) * v->capacity);
}

static void resize(vector *v, int newCap)
{
	printf("\t ---> resize: %d  to  %d\n", v->capacity, newCap);

	void **data = realloc(v->data, sizeof(void *) * newCap);
	if (data) {
		v->data = data;
		v->capacity = newCap;
	}
}

int getSize(vector *v)
{
	return v->size;
}

void vector_add(vector *v, void * data )
{
	if (v->capacity == v->size)
		resize(v, v->capacity * 2);

	v->data[v->size++] = data;
}

void *get(vector *v, int idx)
{
	if (idx >= 0 && idx < v->size)
		return v->data[idx];
	return NULL;
}

void set(vector *v, int idx, void *data)
{
	if(idx >= 0 && idx < v->size)
		v->data[idx] = data;
}

void vector_delete(vector *v, int idx)
{
	if (idx < 0 || idx >= v->size)
		return;

	v->data[idx] = NULL;

	for ( int i = idx; i < v->size -1; i++)
	{
		v->data[i] = v->data[i+1];
		v->data[i+1] = NULL;
	}

	v->size--;

	if (v->size > 0 && v->size == v->capacity / 4)
		resize(v, v->capacity / 2);
}

void vector_free(vector *v)
{
	free(v->data);
}

// Test Main

int main()
{
	vector v;

	vector_init(&v);

	vector_add(&v, "Halli");
	vector_add(&v, "Hallo");
	vector_add(&v, "Howdy");

	for (int i = 0; i < getSize(&v); i++)
		printf("%s ", (char *) get(&v, i));
	printf("\n");

	vector_delete(&v, 3);
	vector_delete(&v, 2);
	vector_delete(&v, 1);

	set(&v, 0, "Tschüss");
	vector_add(&v, "auf Wiedersehen");

	for (int i = 0; i < getSize(&v); i++)
		printf("%s ", (char *) get(&v, i));
	printf("\n");

	vector_free(&v);

	return 0;
}
