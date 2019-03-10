#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "timer.c"

typedef struct Input{
    int** m;
    char* x;
    char* y;
    int size_x;
    int size_y;
    int cost_i;
    int cost_d;
    int cost_r;
}Input;


int transform(Input* in, int i, int j){
    
    if( i >= 1 && j >= 1 && in->m[i][j] == 0){
        if( in->x[i] == in->y[j] ){
            in->m[i][j] = transform(in, i-1, j-1);
        }
        else{
            
            //long lines
            /*
            int a = (transform(in, i-1, j-1) + in->cost_r);
            int b = (transform(in, i, j-1) + in->cost_i);
            int c = (transform(in, i-1, j) + in->cost_d);
            int temp = ( a < b ) ? a : b;
            int min = ( c < temp) ? c : temp;
            in->m[i][j] = min;
            */
            
            //two lines
            int temp = ( (transform(in, i-1, j-1) + in->cost_r) < (transform(in, i, j-1) + in->cost_i) ) ? 
                            (transform(in, i-1, j-1) + in->cost_r) : (transform(in, i, j-1) + in->cost_i);
            
            in->m[i][j] = ( (transform(in, i-1, j) + in->cost_d) < temp) ? (transform(in, i-1, j) + in->cost_d) : temp;
            
            
            //one line
        /*    
            in->m[i][j] =         
                            //( ( c < ( a < b ) ? a : b) ? c : ( a < b ) ? a : b ); 
                            ( ( ( transform(in, i-1, j) + in->cost_d ) < ( (transform(in, i-1, j-1) + in->cost_r) < (transform(in, i, j-1) + in->cost_i) ) ? 
                                ( transform(in, i-1, j-1) + in->cost_r) : (transform(in, i, j-1) + in->cost_i)) ? 
                                ( transform(in, i-1, j) + in->cost_d ) : ( (transform(in, i-1, j-1) + in->cost_r) < (transform(in, i, j-1) + in->cost_i) ) ? 
                                    ( transform(in, i-1, j-1) + in->cost_r) : (transform(in, i, j-1) + in->cost_i) ); 
         */                           
        }
    }
    return in->m[i][j];
}
    

int memoization(struct Input* in, int i, int j){
 
    if(in->m[i][j] == 0 && i > 0 && j > 0){
        if(in->x[i] == in->y[j]){
            in->m[i][j] = in->m[i-1][j-1];
        }
        else{
            int a = in->m[i-1][j-1]+in->cost_r;
            int b = in->m[i][j-1]+in->cost_i;
            int c = in->m[i-1][j]+in->cost_d;
            
            int temp =(a < b ) ? a:b;
            int min = (c < temp)? c:temp;
            in->m[i][j] = min;
            
            }
        }
    return 0;
}

printInput(Input* in){
    
    printf("The size of x : %d\n", in->size_x);
    printf("The size of y : %d\n", in->size_y);
    printf("\nThe string of x : \n\n%s", in->x);
    printf("\nThe string of y : \n\n%s", in->y);
   
}

void printMatrix(Input* in){
    
    int i, j;
    
    // printing matrix
    printf("  ");
    for(j = 0; j < in->size_y; j++)
        printf("%c ", in->y[j]);
    printf("\n");
    for(i = 0; i < in->size_x; i++){
        printf("%c ", in->x[i]);
        for(j = 0; j < in->size_y; j++){
            printf("%d ", in->m[i][j]);
        }
        printf("\n");
    }
    
}

void init(Input* in){
    int i, j;
    
    //allocate matrix as m
    in->m = malloc(sizeof(int*) * in->size_x);
    
    for(i = 0; i < in->size_x; i++){
        in->m[i] = malloc(sizeof(int)*in->size_y);
    }
    
    //set every matrix as 0
    for(i = 0; i < in->size_x; i++){
        for(j = 0; j < in->size_y; j++){
            in->m[i][j] = 0;
        }
    }
    
    //add cost_r if one row does not much with the first column
    for(j = 0; j < in->size_y; j++){
        if(in->x[0] != in->y[j]){
            in->m[0][j] = in->cost_r;
        }
    }
    
    for(i=0; i < in->size_x; i++){
        if(in->y[0] != in->x[i]){
            in->m[i][0] = in->cost_r;
        }
    }
    
    //fill up row and column values
    for(i = 0; i < in->size_x; i++){
        in->m[i][0] += i*in->cost_d;
    }
    for(j = 0; j < in->size_y; j++){
        in->m[0][j] += j*in->cost_i;
    }

}



Input* createInput(int size_x, int size_y, char* temp_x, char* temp_y){
    Input* input = malloc(sizeof(Input));
    assert(input);
    input -> m = NULL;
    input -> x = temp_x;
    input -> y = temp_y;
    input -> size_x = size_x;
    input -> size_y = size_y;
    input -> cost_i = 3;
    input -> cost_d = 2;
    input -> cost_r = 4;
    
    return input;
}

struct Input* scanFile_fgets( char* filename ){
    
    FILE* fp = fopen(filename, "r");
    if(fp == NULL){
        perror("Error while opening the file\n");
        exit(EXIT_FAILURE);
    }
    
    char buff[1024];
    char buff2[1024];
    int size_x, size_y;
    char* temp_x;
    char* temp_y;
    
    //get the first line
    fgets(buff, 255, fp);
    size_x = atoi(buff);
    
    //get the second line
    fgets(buff2, 1024, fp);
    temp_x = buff2;
    
    //get the third line
    fgets(buff, 255, fp);
    size_y = atoi(buff);
    
    //get the fourth line
    fgets(buff, 1024, fp);
    temp_y = buff;
    
    fclose(fp);
    
    Input* in = createInput(size_x, size_y, temp_x, temp_y);
    
    printInput(in);
    
    return in;
}

struct Input* scanFile_getline( char* filename ){
    
    FILE* fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr, "cannot open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    int i, j;
    size_t buffer_size = 80;
    char* buffer = malloc(buffer_size * sizeof(char));
    
    int line_number = 0;
    int size_x, size_y;
    char* temp_x;
    char* temp_y;
    
    while(getline(&buffer, &buffer_size, fp) != -1){
        line_number++;
        switch(line_number){
            case 1:
                size_x = atoi(buffer);
                break;
            case 2:
                temp_x = strdup(buffer);
                break;
            case 3:
                size_y = atoi(buffer);
                break;
            case 4:
                temp_y = strdup(buffer);
                break;
            default:
                break;
        }
    }
    
    
    //Create file
    Input* in = createInput(size_x, size_y, temp_x, temp_y);
    
    //allocate m and set 0 
    init(in);
    
    fflush(stdout);
    fclose(fp);
    free(buffer);
    
    return in;
}


void doMemorization( Input* in ){
    
    int i, j;
    
    //do_memorization
    for(i = 0; i < in->size_x; i++){
        for(j = 0; j < in->size_y; j++){
            memoization(in, i, j);
        }
    }
    
}


int main(int argc, char* argv[]){

    char* input_file = "input.txt";
    
    if(argc == 2){
        input_file = argv[1];
    }
    else{
        fprintf(stderr, "\nusage: %s <input> \n", argv[0]);
        fprintf(stderr, "where <n> is the input file\n");
    }
    
    
    int i, j;
    Input* input = scanFile_getline(input_file);
    
    transform(input, input->size_x-1, input->size_y-1);
    
    //print information
    printf("\n");
    printf("input file : %s\n", input_file);
    printf("smallest cost : %d\n", input->m[input->size_x-1][input->size_y-1]);
    printf("\n");
    
    return 0;
}

