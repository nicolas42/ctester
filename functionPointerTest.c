#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Thing {
	int a;
	int b;
} Thing;

typedef struct Things {
	Thing* values;
	int length;
	int capacity;

//	void (*remove)(struct Things* things, int index);
} Things;


void remove2(Things* things, int index);

void usage(){
	fprintf(stderr, "Usage: thing [numberOfThings]\n");
	exit(1);
}

void make(Thing* thing, int a, int b) {
	thing->a = a;
	thing->b = b;
}

void print(Thing thing){
	printf("thing {a: %d b: %d}\n", thing.a, thing.b);
}

void init(Things* things){
	things->length = 0;
	things->capacity = 2;
	things->values = malloc(things->capacity * sizeof(Thing));
	// things->remove = remove2;
}

void append(Things* things, Thing thing){
	
	if (things->length == things->capacity){
		things->capacity = 2 * things->capacity;
		things->values = realloc(things->values, things->capacity * sizeof(Thing));
	}
	things->values[things->length] = thing;
	things->length++;
}

void remove2(Things* things, int index){
	
	for (int i = index; i <= things->length - 2; ++i)
		things->values[index] = things->values[index + 1];
	things->length--;
}

void printall(Things things){
	for (int i=0; i<things.length; ++i)
		print(things.values[i]);
}

int main(int argc, char** argv){
	
	int N = 0;

	// Argument Validation
	switch (argc){
	case 2:
		N = atoi(argv[1]);
		break;
	case 1:
		N = 5; // default
		break;
	default:
	    	usage();
		break;
	}


	Things things;
	init(&things);
	for (int i=0; i<N; ++i){
		Thing tmp;
		make(&tmp, rand(), rand());
		append(&things, tmp);
	}

	
	remove2(&things, 3); // remove is already defined in stdio :(
	// things.values[1] = things.values[0];
	printall(things);
	


	free(things.values);
	return 0;
}







/*

char* read_line(FILE* file) {
    int size = INIT_LINE_LENGTH;
    char* result = malloc(sizeof(char) * size);
    int position = 0;
    int next = 0;

    while (1) {

        if (position == size) {
            size = size * 2;
            // printf("\nREALLOC\n");
            result = realloc(result, sizeof(char) * size);
        }
        next = fgetc(file);
        if (next == EOF || next == '\n') {
            result[position] = '\0';
            return result;
        } else {
            result[position++] = (char)next;
        }
    }
}

*/

