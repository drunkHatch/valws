#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define BUFFERSIZE 1
#define HASHCONSTANT 4
#define NUM_COMMANDS 1

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
    FILE            *fp;
    char            *file_name;
    int             file_line_count = 0;

    //delcare for gnuplot
    //FILE * temp = fopen("data.temp", "w");
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    char * commandsForGnuplot[] = {"bin_width = 1"};

    // what?
    buffer_size = sizeof(unsigned char)*BUFFERSIZE;
    file_name = "data.csv";
    fp=fopen(file_name,"w+");
    //init window
    //window->first = NULL;
    //window->last = NULL;

    //fprintf(gnuplotPipe, "e");
    for (int i=0; i < NUM_COMMANDS; i++){
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    }
    //fprintf(gnuplotPipe, "plot '-' smooth acsplines\n");
    fprintf(gnuplotPipe, "plot '-' smooth frequency with boxes\n");
    


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


                        }else{

                            add_new_window_node(bucket);
                            add_new_node_on_table(page_number, bucket);
                            remove_node_on_table_and_window(window->first->bucket);
                            //remove_old_window_node();
                            if (file_line_count == 0) {
                                fprintf(gnuplotPipe, "%d\n", real_table->unique_count);
                                file_line_count++;
                            }else if (file_line_count == 4) {
                                fprintf(gnuplotPipe, "%d\n", real_table->unique_count);
                                file_line_count = 0;
                            }else{
                                fprintf(gnuplotPipe, "%d\n", real_table->unique_count);
                                file_line_count++;
                            }
                            printf("%d\n", real_table->unique_count);

                           
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

                        }else{
                            add_new_window_node(bucket);
                            add_new_node_on_table(page_number, bucket);

                            remove_node_on_table_and_window(window->first->bucket);

                            if (file_line_count == 0) {
                                fprintf(gnuplotPipe, "%d\n", real_table->unique_count);
                                file_line_count++;
                            }else if (file_line_count == 4) {
                                fprintf(gnuplotPipe, "%d\n", real_table->unique_count);
                                file_line_count = 0;
                            }else{
                                fprintf(gnuplotPipe, "%d\n", real_table->unique_count);
                                file_line_count++;
                            }

                            printf("%d\n", real_table->unique_count);

                    
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
                        page_number = dec_num/pagesize;
                        bucket = page_number % hash_factor;
                        if (window->amount < wsize){
                            add_new_window_node(bucket);

                            window->amount++;

                            add_new_node_on_table(page_number, bucket);

                        }else{
                            add_new_window_node(bucket);

                            add_new_node_on_table(page_number, bucket);

                            remove_node_on_table_and_window(window->first->bucket);

                            if (file_line_count == 0) {
                                fprintf(gnuplotPipe, "%d\n", real_table->unique_count);
                                file_line_count++;
                            }else if (file_line_count == 4) {
                                fprintf(gnuplotPipe, "%d\n", real_table->unique_count);
                                file_line_count = 0;
                            }else{
                                fprintf(gnuplotPipe, "%d\n", real_table->unique_count);
                                file_line_count++;
                            }

                            printf("%d\n", real_table->unique_count);
                           
                        }

                    }
                }
            }
            //fprintf(stdout,"%s\n",buffer);
        }

    }

    return 0;

    
}



