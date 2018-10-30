#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS
// v make, print, string
// s init, append, insert, remove, print, string


#define THING_SIZE 30  // size of a v string
#define THING_FORMAT "[%d %d]"
#define THING_SEPARATOR " "  // for multiple strings

void usage(){
	fprintf(stderr, "Usage: v [numberOfS]\n");
	exit(1);
}

typedef struct {
	int a;
	int b;
} V;

void make(V* v, int a, int b) {
	v->a = a;
	v->b = b;
}

void copy(V* a, V b){
	a->a = b.a;
	a->b = b.b;
}

int is_equal(V a, V b){
	return a.a == b.a && a.b == b.b;
}

int vstring(char* out, V v){
	int n = sprintf(out, THING_FORMAT, v.a, v.b);	
	return n;
}

void vprint(V v){
	char str[THING_SIZE];
	vstring(str, v);
	printf("%s", str);
}

typedef struct {
	V* values;
	int length;
	int capacity;
	int index;
} S;

void init(S* s){
	s->index = 0;
	s->length = 0;
	s->capacity = 2;
	s->values = malloc(s->capacity * sizeof(V));
}

void head(S* s){
	s->index = 0;
}

void tail(S* s){
	s->index = s->length;
}

int is_head(S s){
	return s.index == 0;
}

int is_tail(S s){
	return s.index == s.length;
}

void next(S* s){
	if (!is_tail(*s)){
		s->index++;
	}
}

void back(S* s){
	if (!is_head(*s)){
		s->index--;
	}
}

void append(S* s, V v){
	
	if (s->length == s->capacity){
		s->capacity = 2 * s->capacity;
		s->values = realloc(s->values, s->capacity * sizeof(V));
	}
	s->values[s->length] = v;
	s->length++;
}

void remove2(S* s, int index){
	
	for (int i = index; i <= s->length - 2; ++i)
		s->values[index] = s->values[index + 1];
	s->length--;
}

void insert(S* s, V v, int index){
	
	// append one v to ensure allocations are done
	append(s, v);
	
	for (int i = index; i <= s->length - 2; ++i)
		s->values[index + 1] = s->values[index];
	
	// Actually set the v
	s->values[index] = v;
	s->length++;
}

int find(S s, V v){

	for (int i=s.index; i<s.length; i+=1){
		if (is_equal(s.values[i], v)){
			return i;
		}
	}
	return -1;
}

void string(char** buffer, S s){
	// assume need THING_SIZE bytes max for serialized v
	*buffer = realloc(*buffer, (s.length - s.index) * THING_SIZE); 
	int offset = 0;
	int n = -1;
	int m = -1;
	
	for (int i=s.index; i<s.length; i+=1){
		n = vstring(*buffer + offset, s.values[i]);
		offset += n;
		m = sprintf(*buffer + offset, THING_SEPARATOR);
		offset += m;
	}
}

void print(S s){
	char* str = malloc(8);
	string(&str, s);
	printf("%s", str);
	free(str);
}

int at(S* s, int a){
	if (a >= s->length){
		return -1;
	}
	s->index = a;
	return 0;
}

int skip(S* s, int a){
	if (s->index + a >= s->length){
		return -1;
	}
	s->index += a;
	return 0;
}

void poke(S* s, int a, V v){
	s->values[a] = v;
}

int swap(S* s, int a, int b){
	if (a >= s->length || b >= s->length){
		return -1;
	}

	V c;
	copy(&c, s->values[a]);
	
	poke(s, a, s->values[b]);
	poke(s, b, c);
	return 0;
}

void pick(V* v, S s, int a){
	copy(v, s.values[a]);
}

void test_dynamic_usage(char** buffer){

	S s; // dynamic series
	V v;
	int i;

	init(&s); // malloc .values
	
	for (i=0; i<10; i+=1){
		make(&v, i, i);
		append(&s, v);
	}
	print(s);
	
	pick(&v,s,1);
	poke(&s,0,v);
	poke(&s,2,v);
	print(s);
	
	remove2(&s, 3);
	print(s);
	
	printf("\nFind [5,5]\n");
	make(&v,5,5);
	i=find(s,v);
	at(&s,i);
	print(s);

	swap(&s,1,2);
	print(s);
	
	free(s.values);
}

void test_static_usage(void){
	int i = 0;
	int N = 5;
	V s[N]; // static s, length of static s

	for (i=0; i<N; ++i)
		make(&s[i], rand(), rand());

	for (i=0; i<N; ++i){
		vprint(s[i]);
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
	test_dynamic_usage(&buffer);
	
	printf("\nTest Static Usage\n");
	test_static_usage();
	
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
		make(&tmp, p[i], p[i+1]);
		if (!is_equal(d.values[i/2], tmp)){
			printf("Failed Test\n");
		}
	}


	// Test Code	
	// Assign one struct to another - testing
	// v[0] = v[1];// char strings[2][100] = {"Hello\0", "Goodbye\0"};
	
	// Make N s
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

