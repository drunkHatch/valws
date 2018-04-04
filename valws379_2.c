#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define BUFFERSIZE 1

struct Node{
    long long int page_number;   // divided by 16
    struct Node *prev;
    struct Ndoe *next;
};


struct Window{
    int amount;
    struct Node *first;
    struct Node *last;
};


long long int extract_number(char * raw_data);

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
    struct Window   *window = malloc(sizeof(struct Window));
    buffer_size = sizeof(unsigned char)*BUFFERSIZE;

    //init window
    window->first = NULL;
    window->last = NULL;
    if(argc == 3){
        pagesize = atoi(argv[1]);
        wsize = atoi(argv[2]);
    }else{
        pagesize = atoi(argv[2]);
        wsize = atoi(argv[3]);
    }


    //open stdin for reading
    instream = fopen("/dev/stdin","r");
    if(instream != NULL){
        //read from stdin untill it's end
        while(fgets(buffer,13,instream)){
            int buffer_length;

            buffer_length = strlen(buffer);
            if (buffer_length > 4){
                if (buffer[0] == ' '){
                    if (buffer[1] == 'S' || buffer[1] == 'M' ||  buffer[1] == 'L'){
                        dec_num = extract_number(buffer);
                        printf("%lli\n",dec_num/pagesize);

                        page_number = dec_num/pagesize;
                        if (window->amount < wsize){
                            addBack(window, page_number);
                            window->amount = window->amount + 1;
                        }else{
                            leaveFront (window);
                            addBack(window, page_number);
                        }

                    }
                }else if(buffer[0] == 'I'){
                    if(buffer[1] == ' '){
                        dec_num = extract_number(buffer);
                        printf("%lli\n",dec_num/pagesize);

                        page_number = dec_num/pagesize;
                        if (window->amount < wsize){
                            addBack(window, page_number);
                            window->amount = window->amount + 1;
                        }else{
                            leaveFront (window);
                            addBack(window, page_number);
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



bool isEmpty (const struct Window * window){
	return window->first == NULL;
}

bool isfull (const struct Window * window, int wsize){
	return window->amount == wsize;
}


void leaveFront (struct Window * window){
	assert(!isEmpty(window));
	struct Node *old_first;

    old_first = window->first;
    window->first = old_first->next;
	//free(old_first->page_number); //把这行comment,为啥要free 一个Page number
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
