#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(){
	fprintf(stderr, "Usage: thing [numberOfThings]\n");
	exit(1);
}

typedef struct Thing {
	int a;
	int b;
} Thing;

void thing_make(Thing* thing, int a, int b) {
	thing->a = a;
	thing->b = b;
}

void thing_rand(Thing* thing) {
	thing->a = rand();
	thing->b = rand();
}

void thing_print(Thing thing){
	printf("thing {a: %d b: %d}\n", thing.a, thing.b);
}

typedef struct Things {
	Thing* things;
	int length;
	int capacity;
} Things;

void things_init(Things* things){
	things->length = 0;
	things->capacity = 2;
	things->things = malloc(things->capacity * sizeof(Thing));
}

void things_append(Things* things, Thing thing){
	
	if (things->length == things->capacity){
		things->capacity = 2 * things->capacity;
		things->things = realloc(things->things, things->capacity * sizeof(Thing));
	}
	things->things[things->length] = thing;
	things->length++;
}

void things_remove(Things* things, int index){
	
	for (int i = index; i <= things->length - 2; ++i)
		things->things[index] = things->things[index + 1];
	things->length--;
}

void things_print(Things things){
	for (int i=0; i<things.length; ++i)
		thing_print(things.things[i]);
}

int main(int argc, char** argv){
	
	int THINGS_LENGTH = 0;

	// Argument Validation
	switch (argc){
	case 2:
		THINGS_LENGTH = atoi(argv[1]);
		break;
	case 1:
		THINGS_LENGTH = 5; // default
		break;
	default:
	    	usage();
		break;
	}


	printf("\nAllocate Static and Dynamic Things\n");
	Thing sthings[THINGS_LENGTH];
	for (int i=0; i<THINGS_LENGTH; ++i)
		thing_make(&sthings[i], rand(), rand());

	Things dthings;
	things_init(&dthings);
	for (int i=0; i<THINGS_LENGTH; ++i)
		things_append(&dthings, sthings[i]);

	printf("\nIn dynamic things - Remove 3 and move 0 to 1\n");
	things_remove(&dthings, 3);
	dthings.things[1] = dthings.things[0];




	printf("\nPrint Static Things\n");
	printf("length: %d\n", THINGS_LENGTH);
	for (int i=0; i<THINGS_LENGTH; ++i)
		thing_print(sthings[i]);
	
	printf("\nPrint Dynamic Things\n");
	printf("length: %d\n", dthings.length);
	things_print(dthings);



	printf("\nFree Stuff\n");
	free(dthings.things);
	


	return 0;
}









/*
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

