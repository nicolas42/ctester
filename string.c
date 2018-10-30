typedef struct {
	char* values;
	int length;
	int capacity;
} String;

void string_init(String* string){
	string->length = 0;
	string->capacity = 2;
	string->values = malloc(string->capacity * sizeof(char));
}

void string_append_char(String* string, char chr){
	
	if (string->length == string->capacity){
		string->capacity = 2 * string->capacity;
		string->values = realloc(string->values, string->capacity * sizeof(char));
	}
	string->values[string->length] = chr;
	string->length++;
}

void string_append(String* a, String* b){	
	for (int i=0; i<b->length; i+=1){
		string_append_char(a, b->values[i]);
	}
}

void string_remove(String* string, int index){
	for (int i = index; i <= string->length - 2; ++i)
		string->values[index] = string->values[index + 1];
	string->length--;
}

void string_to_cstring(String* string){
	if (string->values[string->length] != '\0'){
		string_append_char(string, '\0');
	}
}

int string_is_equal(String a, String b){
	if (a.length != b.length){
		return 0;
	}
	for (int i = 0; i < a.length; ++i){
		if (a.values[i] != b.values[i])
			return 0;
	}
	return 1;
}

