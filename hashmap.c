#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STARTING_BUCKETS 8
#define MAX_KEY_SIZE 100 

/* "The shape of a problem doesn't start anywhere" - Jon Blow 
 * Aviv's Succint Hashmap
 * A simple hashmap in C
 * Store your values in a dictionary
 * Made by a cool dude
* */

/* pseudo struct HashmapObject:
 * 	const char* key;	
 * 	void* value;
 * 	HashmapObject* next;
 * */
typedef struct HashmapObject {
	char* key;
	void* value;
	void* next;
} HashmapObject;

/* pseudo struct Hashmap: 
 * 	**HashmapObject items
 * 	capacity (tbd, won't do anything for now)
 *
 * */

typedef struct Hashmap {
  HashmapObject** items;
  int capacity;
} Hashmap; 

Hashmap* Hashmap_new() {
	Hashmap* h = malloc(sizeof(Hashmap));
 	h->capacity = STARTING_BUCKETS;
	h->items = calloc(1, h->capacity * sizeof(HashmapObject));	
	return h;
}

void Hashmap_free(Hashmap* h) {
	free(h->items);
	free(h);	
}

int hash(const char* key) {
	int sum = 0;
	// key[i] works because a valid string is null terminated
	for (int i = 0; key[i]; i++) {
		sum += key[i];	
	}
	return sum;
}

void Hashmap_set(Hashmap* h, char* key, void* value) {
	HashmapObject* addition = malloc(sizeof(HashmapObject));
	addition->key = malloc(sizeof(MAX_KEY_SIZE));
	strcpy(addition->key, key);	
	addition->value = value;
	addition->next = NULL;	
	int bucket = hash(key) % h->capacity;
	if (!h->items[bucket]) { h->items[bucket] = addition; }
	else { 
		HashmapObject* nextObject = h->items[bucket];
		while(1) {	
			if (strcmp(nextObject->key, addition->key) == 0) {	
				nextObject->value = addition->value;
				return;
			} 
			if (nextObject->next == NULL) {
				nextObject->next = addition;
				return;	
			}
			nextObject = nextObject->next;
		}
		nextObject = addition;
	}	
}



void* Hashmap_get(Hashmap* h, const char* key) {
 	int bucket = hash(key) % h->capacity;
	HashmapObject* currentHashObject = h->items[bucket];
	if (currentHashObject == NULL) return NULL; 	
	while (1) {
		if(strcmp(key, currentHashObject->key) == 0) {
			return currentHashObject->value;
		}
		currentHashObject = currentHashObject->next;
	}
}

void Hashmap_delete(Hashmap* h, const char* key) {
	int bucket = hash(key) % h->capacity;
	HashmapObject* currentObject = h->items[bucket];
	HashmapObject* prevObject = NULL;
	while (1) {
		if (strcmp(currentObject->key, key) == 0) {
			HashmapObject* nextObject = currentObject->next;
			if (prevObject != NULL) {
				prevObject->next = currentObject->next;	
			} else {
				h->items[bucket] = nextObject;
			}
			return;
		}
		prevObject = currentObject;
		currentObject = (HashmapObject*)(currentObject->next);
	}
}
int main() {
  Hashmap *h = Hashmap_new();
  // basic get/set functionality
  int a = 5;
  float b = 7.2;
  Hashmap_set(h, "item a", &a);
  Hashmap_set(h, "item b", &b);
  assert(Hashmap_get(h, "item a") == &a);
  assert(Hashmap_get(h, "item b") == &b);

  // using the same key should override the previous value
  int c = 20;
  Hashmap_set(h, "item a", &c);
  assert(Hashmap_get(h, "item a") == &c);

  // basic delete functionality
  Hashmap_delete(h, "item a");
  assert(Hashmap_get(h, "item a") == NULL);

  // handle collisions correctly
  // note: this doesn't necessarily test expansion
  int i, n = STARTING_BUCKETS * 10, ns[n];
  char key[MAX_KEY_SIZE];
  for (i = 0; i < n; i++) {
    ns[i] = i;
    sprintf(key, "item %d", i);
    Hashmap_set(h, key, &ns[i]);
  }
  for (i = 0; i < n; i++) {
    sprintf(key, "item %d", i);
    assert(Hashmap_get(h, key) == &ns[i]);
  }

  Hashmap_free(h);
  /*
     stretch goals:
     - expand the underlying array if we start to get a lot of collisions
     - support non-string keys
     - try different hash functions
     - switch from chaining to open addressing
     - use a sophisticated rehashing scheme to avoid clustered collisions
     - implement some features from Python dicts, such as reducing space use,
     maintaing key ordering etc. see https://www.youtube.com/watch?v=npw4s1QTmPg
     for ideas
     */
  printf("ok\n");
}
