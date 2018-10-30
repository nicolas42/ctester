#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS
// thing make, print, string
// things init, append, insert, remove, print, string


#define THING_SIZE 30  // size of a thing string
#define THING_FORMAT "[%d %d]"
#define THING_SEPARATOR " "  // for multiple strings

void usage(){
	fprintf(stderr, "Usage: thing [numberOfThings]\n");
	exit(1);
}

typedef struct {
	int a;
	int b;
} Thing;

void thing_make(Thing* thing, int a, int b) {
	thing->a = a;
	thing->b = b;
}

int thing_is_equal(Thing a, Thing b){
	return a.a == b.a && a.b == b.b;
}

int thing_string(char* out, Thing thing){
	int n = sprintf(out, THING_FORMAT, thing.a, thing.b);	
	return n;
}

void thing_print(Thing thing){
	char str[THING_SIZE];
	thing_string(str, thing);
	printf("%s", str);
}

typedef struct {
	Thing* values;
	int length;
	int capacity;
	int index;
} Things;

void things_init(Things* things){
	things->index = 0;
	things->length = 0;
	things->capacity = 2;
	things->values = malloc(things->capacity * sizeof(Thing));
}

void things_head(Things* things){
	things->index = 0;
}

void things_tail(Things* things){
	things->index = things->length;
}

int things_is_head(Things things){
	return things.index == 0;
}

int things_is_tail(Things things){
	return things.index == things.length;
}

void things_next(Things* things){
	if (!things_is_tail(*things)){
		things->index++;
	}
}

void things_back(Things* things){
	if (!things_is_head(*things)){
		things->index--;
	}
}

void things_append(Things* things, Thing thing){
	
	if (things->length == things->capacity){
		things->capacity = 2 * things->capacity;
		things->values = realloc(things->values, things->capacity * sizeof(Thing));
	}
	things->values[things->length] = thing;
	things->length++;
}

void things_remove(Things* things, int index){
	
	for (int i = index; i <= things->length - 2; ++i)
		things->values[index] = things->values[index + 1];
	things->length--;
}

void things_insert(Things* things, Thing thing, int index){
	
	// append one thing to ensure allocations are done
	things_append(things, thing);
	
	for (int i = index; i <= things->length - 2; ++i)
		things->values[index + 1] = things->values[index];
	
	// Actually set the thing
	things->values[index] = thing;
	things->length++;
}

int things_find(Things things, Thing thing){

	for (int i=things.index; i<things.length; i+=1){
		if (thing_is_equal(things.values[i], thing)){
			return i;
		}
	}
	return -1;
}

void things_string(char** buffer, Things things){
	// assume need THING_SIZE bytes max for serialized thing
	*buffer = realloc(*buffer, things.length * THING_SIZE); 
	int offset = 0;
	int n = -1;
	int m = -1;
	
	for (int i=0; i<things.length; i+=1){
		n = thing_string(*buffer + offset, things.values[i]);
		offset += n;
		m = sprintf(*buffer + offset, THING_SEPARATOR);
		offset += m;
	}
}

void things_print(Things things){
	char* str = malloc(8);
	things_string(&str, things);
	printf("%s", str);
	free(str);
}

void things_test_dynamic_usage(char** buffer){

	Things xs; // dynamic things
	things_init(&xs); // malloc .values
	Thing x;
	int i;
	
	for (i=0; i<10; i+=1){
		thing_make(&x, i, i);
		things_append(&xs, x);
	}

	things_print(xs);
	xs.values[0] = xs.values[1];
	xs.values[2] = xs.values[1];
	things_print(xs);
	things_remove(&xs, 3);
	things_print(xs);
	
	printf("\nFind {5,5}\n");
	thing_make(&x, 5,5);
	i=things_find(xs, x);
	thing_print(xs.values[i]);
	
	free(xs.values);
}

void things_test_static_usage(void){
	int i = 0;
	int N = 5;
	Thing s[N]; // static things, length of static things

	for (i=0; i<N; ++i)
		thing_make(&s[i], rand(), rand());

	for (i=0; i<N; ++i){
		thing_print(s[i]);
		printf(THING_SEPARATOR);
	}
	printf("\n");
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

	char* buffer = malloc(8 * sizeof(char));

	printf("\nTest Dynamic Usage\n");
	things_test_dynamic_usage(&buffer);
	
	printf("\nTest Static Usage\n");
	things_test_static_usage();
	
	free(buffer);
	


	printf("\nTest asprintf\n");
	char* test = malloc(1);
	asprintf(&test, "Hello you bastard\n"); // malloc
	puts(test);
	free(test);
	
	
	return 0;
}

/*

	// Predicted values
	int pl = 8;
	int p[pl] = {0,0,0,0,2,2,4,4};
	
	for (i = 0; i < pl; i+=2){
		thing_make(&tmp, p[i], p[i+1]);
		if (!thing_is_equal(d.values[i/2], tmp)){
			printf("Failed Test\n");
		}
	}


	// Test Code	
	// Assign one struct to another - testing
	// thing[0] = thing[1];// char strings[2][100] = {"Hello\0", "Goodbye\0"};
	
	// Make N things
	int N = 100;
	
void remove_card(Cards* cards, int cardNumber) {
    // top of cards is at cards.length
    int cardIndex = cards->length - 1 - cardNumber;
    // printf("%d\n", cardIndex);
    for (int i = cardIndex; i < cards->length; ++i) {
        cards->values[i] = cards->values[i + 1];
    }
    cards->length = cards->length - 1;
}


int data_read(Card* card, char* line) {
    // needs error checking
    sscanf(line, "%c:%d:%d,%d,%d,%d", &(card->discount), &(card->points), 
            card->tokens + PURPLE, card->tokens + BROWN, 
            card->tokens + YELLOW, card->tokens + RED);
    return 0;
}

int read_cards_file(Cards* cards, char* filename) {
    char* line;
    FILE* cardsFile = fopen(filename, "r");
    while (line = read_line(cardsFile), line[0] != '\0') {
        Card card;
        read_card(&card, line);
        append_card(cards, &card);
    }
    fclose(cardsFile);
    return 0;
}




int read_line(char* result, FILE* file) {
    int size = INIT_LINE_LENGTH;
    char* result = realloc(sizeof(char) * size);
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

