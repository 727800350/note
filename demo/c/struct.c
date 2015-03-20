#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person{
	int id;
	int age;
};

int main(){
	struct Person *person = malloc(sizeof(struct Person));
	person->id = 4;
	person->age = 24;

	printf("person with id %d and age %d\n", person->id, person->age);
	printf("person address %p\n", person);
	printf("%p\n", &(person->age));
	printf("%p\n", &person->age);

	struct Person *a = malloc(sizeof(struct Person));
	a->id = 10;
	a->age = 25;
	memcpy(&a->age, &person->age, sizeof(person->age));
// 	memcpy(a, person, sizeof(struct Person));
	printf("person with id %d and age %d\n", a->id, a->age);

	free(person);
	free(a);
	return 0;
}
