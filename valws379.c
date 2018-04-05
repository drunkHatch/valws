#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define BUFFERSIZE 1
#define HASHCONSTANT 4

// a linked list, add to tail and remove from head
    //next to tail
    //prev to head
    //need a funtion to add and a function to remove
    // also remember to update table
// a table, always place head in it, DONE

// node for linked list
struct ListNode{
    int bucket;   // divided by bucket number
    //struct ListNode *prev;
    struct ListNode *next;
};
struct HashTable{
    int             unique_count;
    struct Node     **table;
    struct Node     **tails;
};

// node for table
struct Node{
    long long int page_number;   // divided by 16(page size)
    //struct Node *prev;
    struct Node *next;
};

// is it the linked list I want?
struct Window{
    int amount;
    struct ListNode *first;
    struct ListNode *last;
};


long long int extract_number(char * raw_data);

// init hash table here and they are GLOBAL
struct Node         *table, *tails;
struct Window       *window;
struct HashTable    *real_table;

void init_hash_table_and_window(int hash_factor){
    window = malloc(sizeof(struct Window));
    window->amount = 0;
    real_table = malloc(sizeof(struct HashTable));
    real_table->table = (struct Node **)malloc(sizeof(struct Node*) * hash_factor); // init hash table
    real_table->tails = (struct Node **)malloc(sizeof(struct Node*) * hash_factor); // init tails of hash table
    for (size_t i = 0; i < hash_factor; i++) {
        real_table->table[i] = NULL;
        real_table->tails[i] = NULL;
    }
}

int page_number_is_unique(long long int page_number_to_be_removed, int bucket){
    struct Node *current_node;

    current_node = real_table->table[bucket];
    while(current_node != NULL){
        //printf("%p %p\n", current_node,  current_node->next);
        if (current_node->page_number == page_number_to_be_removed) {
            return 0;
        }
        current_node = current_node->next;
    }
    return 1;
}

void remove_node_on_table_and_window(int bucket){
    int page_number_to_be_removed;
    struct ListNode *old_head;

    window->first = window->first->next;

    old_head = real_table->table[bucket];
    page_number_to_be_removed = real_table->table[bucket]->page_number;
    real_table->table[bucket] = real_table->table[bucket]->next;
    free(old_head);

    if (real_table->table[bucket] == NULL){
        real_table->unique_count--;
    }else if(page_number_is_unique(page_number_to_be_removed, bucket)){
        real_table->unique_count--;
    }
}



void add_new_node_on_table(long long int new_page_number, int bucket){
    struct Node *current_node;
    struct Node *new_node;

    new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->page_number = new_page_number;

    current_node = real_table->table[bucket]; // bug here
    if (current_node == NULL) {
        real_table->tails[bucket] = new_node;
        real_table->table[bucket] = new_node;
        real_table->unique_count++;
    }
    else{
        while(1){
            if (current_node == NULL) {
                real_table->unique_count++;
                break;
            }else if (current_node->page_number == new_page_number) {
                break;
            }
            current_node = current_node->next;
        }
        //add after check for dupliacte
        real_table->tails[bucket]->next = new_node;
        real_table->tails[bucket] = new_node;
    }
}

void remove_old_window_node(){
    window->first = window->first->next;
    //window->first->prev = NULL;
}

void add_new_window_node(int bucket){
    struct ListNode *new_node;

    new_node = malloc(sizeof(struct ListNode));
    new_node->bucket = bucket;
    if (window->last != NULL){
        window->last->next = new_node;
        new_node->next = NULL;
        window->last = new_node;
    }else{
        window->last = new_node;
        window->first = new_node;
    }
}

int main(int argc, char *argv[]){
    char            buffer[14];
    int             pagesize;
    int             wsize;
    FILE            *instream;
    int             bytes_read = 0;
    int             buffer_size = 0;
    char            address[9];
    long long int   dec_num;
    long long int   page_number;
    int             hash_factor;

    // what?
    buffer_size = sizeof(unsigned char)*BUFFERSIZE;
    //init window
    //window->first = NULL;
    //window->last = NULL;

    if(argc == 3){                   //without [-i]
        pagesize = atoi(argv[1]);
        wsize = atoi(argv[2]);

        hash_factor = wsize / HASHCONSTANT; // init for hash function
        init_hash_table_and_window(hash_factor);

        //open stdin for reading
        instream = fopen("/dev/stdin","r");
        //read from stdin untill it's end
        while(fgets(buffer,13,instream)){
            int buffer_length;
            buffer_length = strlen(buffer);
            if (buffer_length > 4){
                if (buffer[0] == ' '){
                    if (buffer[1] == 'S' || buffer[1] == 'M' ||  buffer[1] == 'L'){
                        int bucket;
                        dec_num = extract_number(buffer);
                        //printf("%lli\n",dec_num/pagesize);

                        page_number = dec_num/pagesize;
                        bucket = page_number % hash_factor;
                        if (window->amount < wsize){
                            //add to window

                            add_new_window_node(bucket);


                            //increase window amount

                            window->amount++; // let it out to control easily between if else statement

                            //add to table
                            //increase table unique_count

                            add_new_node_on_table(page_number, bucket); // bug here

                            //addBack(window, page_number);
                            //window->amount = window->amount + 1;
                        }else{

                            add_new_window_node(bucket);
                            add_new_node_on_table(page_number, bucket);
                            remove_node_on_table_and_window(window->first->bucket);
                            //remove_old_window_node();

                            printf("%d\n", real_table->unique_count);

                            //leaveFront (window);
                            //addBack(window, page_number);
                            //TODO:  compare in the window and get unique
                        }
                    }
                }else if(buffer[0] == 'I'){
                    if(buffer[1] == ' '){
                        int bucket;
                        dec_num = extract_number(buffer);
                        //printf("%lli\n",dec_num/pagesize);

                        page_number = dec_num/pagesize;
                        bucket = page_number % hash_factor;
                        if (window->amount < wsize){
                            add_new_window_node(bucket);
                            window->amount++;
                            add_new_node_on_table(page_number, bucket);

                            //addBack(window, page_number);
                            //window->amount = window->amount + 1;
                        }else{
                            add_new_window_node(bucket);
                            add_new_node_on_table(page_number, bucket);

                            remove_node_on_table_and_window(window->first->bucket);

                            //remove_old_window_node();

                            printf("%d\n", real_table->unique_count);

                            //leaveFront (window);
                            //addBack(window, page_number);
                            //TODO:  compare in the window and get unique
                        }

                    }
                }
            }
            //fprintf(stdout,"%s\n",buffer);
        }


    }else{      // with [-i]
        pagesize = atoi(argv[2]);
        wsize = atoi(argv[3]);

        hash_factor = wsize / HASHCONSTANT; // init for hash function
        init_hash_table_and_window(hash_factor);

        //open stdin for reading
        instream = fopen("/dev/stdin","r");
        //read from stdin untill it's end
        while(fgets(buffer,13,instream)){
            int buffer_length;
            buffer_length = strlen(buffer);
            if (buffer_length > 4){
                if (buffer[0] == ' '){
                    if (buffer[1] == 'S' || buffer[1] == 'M' ||  buffer[1] == 'L'){
                        int bucket;
                        dec_num = extract_number(buffer);
                        //printf("%lli\n",dec_num/pagesize);

                        page_number = dec_num/pagesize;
                        bucket = page_number % hash_factor;
                        if (window->amount < wsize){
                            add_new_window_node(bucket);

                            window->amount++;

                            add_new_node_on_table(page_number, bucket);

                            //addBack(window, page_number);
                            //window->amount = window->amount + 1;
                        }else{
                            add_new_window_node(bucket);

                            add_new_node_on_table(page_number, bucket);

                            remove_node_on_table_and_window(window->first->bucket);

                            //remove_old_window_node();
                            printf("%d\n", real_table->unique_count);
                            //leaveFront (window);
                            //addBack(window, page_number);
                            //TODO:  compare in the window and get unique
                        }

                    }
                }
            }
            //fprintf(stdout,"%s\n",buffer);
        }

    }
}


long long int extract_number(char * raw_data){
    char extracted_hex[12];
    long long int extracted_dec;
    int length;

    length = strlen(raw_data);
    for(int i=3; i<length; i++){
        if(raw_data[i] != ','){
            extracted_hex[i-3] = raw_data[i];
        }else{
            extracted_hex[i-3] = '\0';
        }
    }
    sscanf(extracted_hex, "%X", &extracted_dec);

    return extracted_dec;
}

/*
// would they work properly?
bool isEmpty (const struct Window * window){
	return window->first == NULL;
}

// would they work properly?
bool isfull (const struct Window * window, int wsize){
	return window->amount == wsize;
}
*/
/*
void leaveFront (struct Window * window){
	assert(!isEmpty(window));
	struct Node *old_first;
    old_first = window->first;
    window->first = old_first->next;
    free(old_first);
	if(window->first != NULL){
		window->first-> prev = NULL;}
	else{
		window->last = NULL;}

}



void addBack (struct Window * window, long long int page_number){
	struct Node *new_node = malloc(sizeof(struct Node));
	new_node->page_number = malloc(sizeof(page_number)+1);
	new_node->page_number = page_number;
	if(!isEmpty(window)){
    	new_node->prev = window->last;
    	window->last->next = new_node;
    	window->last = new_node;
		//free(new_node);
	}else{
		new_node->prev = NULL;
		new_node->next = NULL;
		window->first = new_node;
		window->last = new_node;
		//free(new_node);
	}
}
*/
