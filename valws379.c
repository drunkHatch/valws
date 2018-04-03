#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE 1

typedef struct {
  long long int memory_address;
  long long int page_number;   // divided by 16
  void *prev;
  void *next;

} M_REFERENCE;

long long int extract_number(char * raw_data);

int main(int argc, char *argv[]){
    char buffer[14];
    int pagesize = atoi(argv[1]);
    int wsize = atoi(argv[2]);
    FILE          *instream;
    int           bytes_read = 0;
    int           buffer_size = 0;
    char          address[9];
    long long int           dec_num;
    M_REFERENCE   node;

    buffer_size = sizeof(unsigned char)*BUFFERSIZE;

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
                        //node.prev = node;
                        node.memory_address = dec_num;
                        node.page_number = dec_num/pagesize;

                        
                        
                    }
                }else if(buffer[0] == 'I'){
                    dec_num = extract_number(buffer);
                    printf("%lli\n",dec_num/pagesize);
                    node.memory_address = dec_num;
                    node.page_number = dec_num/pagesize;
                    
                }
            }
            //fprintf(stdout,"%s\n",buffer);
        }
    }

}


long long int extract_number(char * raw_data){
    char extracted_hex[9];
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
