#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

#define ERR_ARG_LEN 1
#define ERR_INVALID_NUM_PLAYERS 2
#define ERR_INVALID_PLAYER_ID 3
#define ERR_COMMUNICATIONS 6

#define ARG_LEN 3
#define LEN_TOKENS 5
#define LEN_NOT_WILD 4
#define CARDS_CAPACITY 8
#define CARDS_TO_TAKE 3
#define UNLIMITED -1
#define INIT_LINE_LENGTH 80

/* Card struct with its cost (tokens), 
 *	its discount 'P', 'B', 'Y', 'R', 
 * and its associated points 
 */
typedef struct {
    char discount;
    int points;
    int tokens[LEN_TOKENS];
} Card;

/* Cards collection for dynamic allocation of Card structs */
typedef struct {
    int capacity;
    int length; // top card is located at the position length
    Card* values;
} Cards;

/* Player Data - Vast majority of allocation */
typedef struct {
    Cards gameCards;
    int gameTokens[LEN_TOKENS];
    Cards myCards;
    int myTokens[LEN_TOKENS];
    int discount[LEN_TOKENS];
    Cards sortedGameCards;
} Player;    

/* Indexes for token array indexing */
enum Colors {
    PURPLE,
    BROWN,
    YELLOW,
    RED,
    WILD
};

/* Exit program with error code and error message sent to stderr
* Returns nothing
*/
void exit_and_perror(int errorCode, char* errorMessage);
/* Read line from file with dynamic allocation 
Returns char* line data */
char* read_line(FILE* file);
/* Load cards from a properly formatted cards file 
* Returns integer indicating success/failure */
int read_cards_file(Cards* ret, char* filename);
/* Read card from line in cards file 
* Returns integer indicating success/failure */
int read_card(Card* card, char* line);
/* Initialize token memory to zero
* Returns nothing */
void init_tokens(int tokens[LEN_TOKENS], int numTokens);
/* Add the first four token values from difference to the tokens array
* Returns integer indicating success/failure */
int add_nonwild_tokens(int tokens[LEN_TOKENS], int difference[LEN_TOKENS]);
/* Subtract the first four token values from difference to the tokens array
* Returns integer indicating success/failure */
int subtract_nonwild_tokens(int tokens[LEN_TOKENS], 
        int difference[LEN_TOKENS]);
/* Initialize cards collection with capacity 8 and length zero
Returns nothing */
void init_cards(Cards* cards);
/* Append card to cards collection and increase length
* Returns integer indicating success/failure */
int append_card(Cards* cards, Card* card);
/* Remove card at particular cardNumber (distinct from index)
* Returns nothing */
void remove_card(Cards* cards, int cardNumber);
/* Print formatted card
Return nothing */
void print_card(Card card);
/* Print cards formatted
Return nothing */
void print_cards(Cards cards);
/* Print a token datatype
* Return nothing */
void print_tokens(int tokens[LEN_TOKENS]);
/* Respond to message provided by stdin
* Returns void */
void respond(Player* player, char* message);
/* Initialize the data in the player struct.  
Initialize token arrays, and card collections
Return void */
void init_game(Player* player);
/* Read newcard message in the format from the spec 
Return integer indicating success/failure */
int read_newcard(Card* card, char* line);
/* Read purchased message in the format from the spec 
Return integer indicating success/failure */
int read_purchased(char* playerCharacter, int* cardNumber, 
        int tokens[LEN_TOKENS], char* line);
/* Update the discount array in the player struct 
return nothing */
void update_discount(Player* player);
/* Return whether the player can afford the given card
Return integer where 1 indicates yes and 0 indicates no */
int can_afford(Player* player, Card* card);
/* Subtract all of the tokens in difference from the 'tokens' array 
Return integer indicating success or failure */
int subtract_tokens(int tokens[LEN_TOKENS], int difference[LEN_TOKENS]);
/* Return the tokens that should be bought according to the 
preferences outlined in the spec
Return nothing */
void find_three_tokens(int tokens[LEN_TOKENS], Player* player);
/* Remove appropriate player tokens and add appropriate card
Return void */
void purchase_card(Player* player, Card* card, int price[LEN_TOKENS]);
/* Copy a tokens array from source to destination 
Return nothing */
void copy_tokens(int destination[LEN_TOKENS], int source[LEN_TOKENS]);
/* Return integer indicating whether the player can afford the given card
* Returns integer */
int can_afford(Player* player, Card* card);
/* Return an integer or -1 indicating the index of the best card 
for the player to purchase currently
Returns an integer */
int find_best_card(Player* player);
/* Return 1 is card A is superior to card B according to the spec 
Returns integer */ 
int is_card_better(Card* cardA, Card* cardB);
/* Add the first four token integers and return it
Returns integer */
int sum_nonwild_tokens(int tokens[LEN_TOKENS]);
/* Return 1 if all the integers in the token array are positive 
Returns an integer */
int is_all_positive(int tokens[LEN_TOKENS]);
/* Take appropriate action given the dowhat message
Return nothing */
void dowhat(Player* player);

int main(int argc, char** argv) {

    if(argc != ARG_LEN) {
        fprintf(stderr, "Usage: {player} pcount myid\n");
        exit(ERR_ARG_LEN);
    }
    
    Player player;
    init_game(&player);
    char* message;
    
    for (; ; ) {
        message = read_line(stdin);
        respond(&player, message);
/*         print_cards(player.gameCards);
        print_cards(player.myCards);
        print_tokens(player.myTokens);
        print_tokens(player.gameTokens);
 */    }

    
    // only hub should do this
/*     Cards deck;
    init_cards(&deck);
    read_cards_file(&deck, "cards");
    print_cards(deck); */

    
    free(player.gameCards.values);
    free(player.myCards.values);
    return 0;
}

int is_all_positive(int tokens[LEN_TOKENS]) {
    int result = 1;
    for (int i = 0; i < LEN_TOKENS; ++i) {
        if (tokens[i] < 0) {
            return result = 0;
        }
    }
    return result;
}

int sum_nonwild_tokens(int tokens[LEN_TOKENS]) {
    int sum = 0;
    for (int i = 0; i < LEN_NOT_WILD; ++i) {
        sum = sum + tokens[i];
    }
    return sum;
}

int is_card_better(Card* cardA, Card* cardB) {
    if (cardA->points > cardB->points) {
        return 1;
    } else if (cardA->points == cardB->points &&
            sum_nonwild_tokens(cardA->tokens) <= 
            sum_nonwild_tokens(cardB->tokens)) {
        
        return 1;
    }
    return 0;
}

int find_best_card(Player* player) {
    Cards* cards = &(player->gameCards);
    int bestCardIndex = -1; // not found
    for (int i = 0; i < cards->length; ++i) {
        if (-1 == bestCardIndex) {
            if (can_afford(player, cards->values + i)) {
                bestCardIndex = i;
            }
        } else if (is_card_better(cards->values + i, 
                cards->values + bestCardIndex)) {
                    
            bestCardIndex = i;
        }
    }
    return bestCardIndex;
}


int can_afford(Player* player, Card* card) {
    
    int result[LEN_TOKENS];
    for (int i = 0; i < LEN_TOKENS; ++i) {
        result[i] = player->myTokens[i] - card->tokens[i];
    }
    if (is_all_positive(result)) {
        return 1;
    } else if (result[WILD] >= sum_nonwild_tokens(result)) {
        return 1;
    }
    return 0;
    
}

void copy_tokens(int destination[LEN_TOKENS], int source[LEN_TOKENS]) {
    for (int i = 0; i < LEN_TOKENS; ++i) {
        destination[i] = source[i];
    }
}

void purchase_card(Player* player, Card* card, int price[LEN_TOKENS]) {

    // append card, subtract tokens, calculate price    
    copy_tokens(price, player->myTokens);
    append_card(&(player->myCards), card);
    subtract_nonwild_tokens(player->myTokens, card->tokens);
    for (int i = 0; i < LEN_NOT_WILD; ++i) {
        if (player->myTokens[i] < 0) { 
            // subtract negative
            player->myTokens[WILD] += player->myTokens[i]; 
            player->myTokens[i] = 0;
        }
    }
    
    subtract_tokens(price, player->myTokens);
    
    if (!is_all_positive(player->myTokens)) {
        if (DEBUG) {
            printf("Purchase card error\n");
        }
        exit(-1);
    }
    update_discount(player);
}

void find_three_tokens(int tokens[LEN_TOKENS], Player* player) {
    int numToTake = CARDS_TO_TAKE;
    for (int i = 0; i < LEN_NOT_WILD && numToTake > 0; ++i) {
        if (numToTake <= player->gameTokens[i]) {
            tokens[i] += numToTake;
            break;
        } else {
            tokens[i] += player->gameTokens[i];
            numToTake -= player->gameTokens[i];
        }
    }
}

void dowhat(Player* player) {

    int tokens[LEN_TOKENS];
    init_tokens(tokens, 0);    

    int bestCardIndex = find_best_card(player);
    if (bestCardIndex >= 0) {
        purchase_card(player, player->gameCards.values + 
                bestCardIndex, tokens);
                
        // send message
        printf("purchase%d:%d,%d,%d,%d,%d\n", bestCardIndex, tokens[PURPLE], 
                tokens[BROWN], tokens[YELLOW], tokens[RED], tokens[WILD]);
        fflush(stdout);
    } else if (sum_nonwild_tokens(player->gameTokens) >= CARDS_TO_TAKE) {
        find_three_tokens(tokens, player);
        subtract_nonwild_tokens(player->myTokens, tokens);
        printf("take%d,%d,%d,%d\n", 
                tokens[PURPLE], tokens[BROWN], tokens[YELLOW], tokens[RED]);
        fflush(stdout);
    } else {
        // take_wild_token(player);
        player->myTokens[WILD] += 1;
        printf("wild\n");
        fflush(stdout);
    }
}

void update_discount(Player* player) {
    
    init_tokens(player->discount, 0);
    for (int i = 0; i < player->myCards.length; ++i) {
        switch (player->myCards.values[i].discount) {
            case 'P':
                player->discount[PURPLE] += 1;
                break;
            case 'B':
                player->discount[BROWN] += 1;
                break;
            case 'Y':
                player->discount[YELLOW] += 1;
                break;
            case 'R':
                player->discount[RED] += 1;
                break;
            default:
                printf("Invalid discount char");
        }
    }
}

void init_game(Player* player) {
    init_cards(&(player->gameCards));
    init_cards(&(player->myCards));
    init_cards(&(player->sortedGameCards));
    // gameTokens = {0, 0, 0, 0, UNLIMITED};
    // myTokens = {0, 0, 0, 0, 0};
    // discount = {0, 0, 0, 0, 0};
    
    for (int i = 0; i < LEN_TOKENS; ++i) {
        player->gameTokens[i] = 0;
        player->myTokens[i] = 0;
        player->discount[i] = 0;
    }
    player->gameTokens[WILD] = UNLIMITED;
}

void respond(Player* player, char* line) {
    int numTokens;
    char playerCharacter;
    int cardNumber;
    int messageTokens[LEN_TOKENS] = {0, 0, 0, 0, 0};
    Card card;
    if (DEBUG) {
        printf("Message: %s\n", line);
    }
    // eog
    if (0 == strncmp(line, "eog", strlen("eog"))) {
        fprintf(stderr, "Game over. Winners are L\n");
        exit(0);
    // # inform player of tokens in non-wild piles - tokensT
    } else if (0 == strncmp(line, "tokens", strlen("tokens"))) {
        sscanf(line, "tokens%d", &numTokens);
        init_tokens(player->gameTokens, numTokens);
    // a card has been drawn from the deck - newcardD:V:TP,TB,TY,TR
    } else if (0 == strncmp(line, "newcard", strlen("newcard"))) {
        read_newcard(&card, line);
        append_card(&(player->gameCards), &card);    
    // someone purchased a card - purchasedP:C:TP,TB,TY,TR,TW
    } else if (0 == strncmp(line, "purchased", strlen("purchased"))) {
        read_purchased(&playerCharacter, &cardNumber, messageTokens, line);
        remove_card(&(player->gameCards), cardNumber); // card number
        add_nonwild_tokens(player->gameTokens, messageTokens);
    // dowhat
    } else if (0 == strncmp(line, "dowhat", strlen("dowhat"))) {
        fprintf(stderr, "Received dowhat\n");
        dowhat(player);
    // someone took tokens - tookP:Tp,Tb,Ty,Tr    
    } else if (0 == strncmp(line, "took", strlen("took"))) {
        sscanf(line, "purchased%c:%d,%d,%d,%d", &playerCharacter, 
                messageTokens + 0, messageTokens + 1, 
                messageTokens + 2, messageTokens + 3);
        subtract_nonwild_tokens(player->gameTokens, messageTokens);
    // someone took a wild token
    } else if (0 == strncmp(line, "wild", strlen("wild"))) {
    // malformed message
    } else {
        exit_and_perror(ERR_COMMUNICATIONS, "Communications Error\n");
    }
    if (DEBUG) {
        // printf("%s\n", line);
        printf("Game Cards: %d/%d\n", 
                player->gameCards.length, player->gameCards.capacity);
        printf("Game Tokens: "); 
        print_tokens(player->gameTokens);
        printf("\n");
    }
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

void exit_and_perror(int errorCode, char* errorMessage) {
    fprintf(stderr, errorMessage);
    exit(errorCode);
}

void init_tokens(int tokens[LEN_TOKENS], int numTokens) {
    for (int i = 0; i < LEN_TOKENS - 1; ++i) {
        tokens[i] = numTokens;
    }
}

int add_nonwild_tokens(int tokens[LEN_TOKENS], int difference[LEN_TOKENS]) {
    for (int i = 0; i < LEN_NOT_WILD; ++i) {
        tokens[i] = tokens[i] + difference[i];
    }
    return 0;
}

int subtract_nonwild_tokens(int tokens[LEN_TOKENS], 
        int difference[LEN_TOKENS]) {
    
    for (int i = 0; i < LEN_NOT_WILD; ++i) {
        tokens[i] = tokens[i] - difference[i];
    }
    return 0;
}

int subtract_tokens(int tokens[LEN_TOKENS], int difference[LEN_TOKENS]) {
    for (int i = 0; i < LEN_TOKENS; ++i) {
        tokens[i] = tokens[i] - difference[i];
    }
    return 0;
}

void init_cards(Cards* cards) {
    cards->length = 0;
    cards->capacity = CARDS_CAPACITY;
    cards->values = malloc(cards->capacity * sizeof(Card));
}

int read_card(Card* card, char* line) {
    // needs error checking
    sscanf(line, "%c:%d:%d,%d,%d,%d", &(card->discount), &(card->points), 
            card->tokens + PURPLE, card->tokens + BROWN, 
            card->tokens + YELLOW, card->tokens + RED);
    return 0;
}

int append_card(Cards* cards, Card* card) {
    if (cards->length == cards->capacity) {
        cards->capacity = 2 * cards->capacity;
        cards->values = realloc(cards->values, 
                cards->capacity * sizeof(Card));
    }
    cards->values[cards->length] = *card;
    cards->length = cards->length + 1;
    return 0;
}

int read_newcard(Card* card, char* line) {
    // needs error checking
    sscanf(line, "newcard%c:%d:%d,%d,%d,%d", 
            &(card->discount), &(card->points), 
            card->tokens + PURPLE, card->tokens + BROWN, 
            card->tokens + YELLOW, card->tokens + RED);
    
    return 0;
}

void remove_card(Cards* cards, int cardNumber) {
    // top of cards is at cards.length
    int cardIndex = cards->length - 1 - cardNumber;
    // printf("%d\n", cardIndex);
    for (int i = cardIndex; i < cards->length; ++i) {
        cards->values[i] = cards->values[i + 1];
    }
    cards->length = cards->length - 1;
}

void print_tokens(int tokens[LEN_TOKENS]) {
    for (int i = 0; i < LEN_TOKENS; ++i) {
        printf("%d ", tokens[i]);
    }
    printf("\n");
}

void print_card(Card card) {
    printf("%c:%d:%d,%d,%d,%d\n", card.discount, card.points, 
            card.tokens[PURPLE], card.tokens[BROWN], 
            card.tokens[YELLOW], card.tokens[RED]);
}

void print_cards(Cards cards) {
    printf("Discount:Points:TokenCost(Purple, Brown, Yellow, Red)\n");
    printf("D:V:P,B,Y,R\n");
    for (int i = 0; i < cards.length; ++i) {
        print_card(cards.values[i]);
    }
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

int read_purchased(char* playerCharacter, int* cardNumber, 
        int tokens[LEN_TOKENS], char* line) {
    
    int nargs;
    nargs = sscanf(line, "purchased%c:%d:%d,%d,%d,%d,%d", 
            playerCharacter, cardNumber, tokens + 0, tokens + 1, 
            tokens + 2, tokens + 3, tokens + 4);
    
    if (nargs != 7) {
        return 1;
    }
    return 0;
}