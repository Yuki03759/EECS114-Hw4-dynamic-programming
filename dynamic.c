#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

const int cost_i = 3;
const int cost_d = 2;
const int cost_r = 4;

typedef struct Input{
    int** m;
    char* x;
    char* y;
    int size_x;
    int size_y;
}Input;


int transform(Input* in, int i, int j){
    
    if( i >= 1 && j >= 1 && in->m[i][j] == 0){
        if( in->x[i] == in->y[j] ){
            in->m[i][j] = transform(in, i-1, j-1);
        }
        else{
            
            int a = (transform(in, i-1, j-1) + cost_r);
            int b = (transform(in, i, j-1) + cost_i);
            int c = (transform(in, i-1, j) + cost_d);
            int temp = ( a < b ) ? a : b;
            int min = ( c < temp) ? c : temp;
            in->m[i][j] = min;
            
            //one line
        /*    
            in->m[i][j] =         
                            //( ( c < ( a < b ) ? a : b) ? c : ( a < b ) ? a : b ); 
                            ( ( ( transform(in, i-1, j) + cost_d ) < ( (transform(in, i-1, j-1) + cost_r) < (transform(in, i, j-1) + cost_i) ) ? 
                                ( transform(in, i-1, j-1) + cost_r) : (transform(in, i, j-1) + cost_i)) ? 
                                ( transform(in, i-1, j) + cost_d ) : ( (transform(in, i-1, j-1) + cost_r) < (transform(in, i, j-1) + cost_i) ) ? 
                                    ( transform(in, i-1, j-1) + cost_r) : (transform(in, i, j-1) + cost_i) ); 
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
            int a = in->m[i-1][j-1]+cost_r;
            int b = in->m[i][j-1]+cost_i;
            int c = in->m[i-1][j]+cost_d;
            
            int temp =(a < b ) ? a:b;
            int min = (c < temp)? c:temp;
            in->m[i][j] = min;
            
            }
        }
    return 0;
}

void init(Input* in){
    int i, j;
    
    //allocate matrix as m
    in->m = malloc(sizeof(int*)*in->size_y);
    for(i = 0; i < in->size_x; i++){
        in->m[i] = malloc(sizeof(int*)*in->size_x);
    }
    
    printf("in->size_x : %d\n", in->size_x);
    printf("in->size_y : %d\n", in->size_y);
    
    
    
    for(i = 0; i < in->size_x; i++){
        for(j = 0; j < in->size_y; j++){
           // printf("%d : %d\n", i, j);
            in->m[i][j] = 0;
        }
    }
    
    //if x[0] and y[0] are the same, add 4
    if(in->x[0] != in->y[0])
        in->m[0][0] = cost_r;
    
    //fill up row and column values
    for(i = 0; i < in->size_x; i++){
        in->m[i][0] = i*cost_d + in->m[0][0];
    }
    for(j = 0; j < in->size_y; j++){
        in->m[0][j] = j*cost_i + in->m[0][0];
    }

}

void printMatrix(Input* in){
    int i, j;
    
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

Input* createInput(void){
    Input* input = malloc(sizeof(Input));
    assert(input);
    input -> m = NULL;
    input -> x = NULL;
    input -> y = NULL;
    input -> size_x = 0;
    input -> size_y = 0;
    return input;
}

printInput(Input* in){
    printf("in->size_x : %d\n", in->size_x);
    printf("in->size_y : %d\n", in->size_y);
    printf("in->x : %s\n", in->x);
    printf("in->y : %s\n", in->y);
}


struct Input* readFile ( char * filename ){
    
    int i, j;
    FILE* fp = fopen(filename, "r");
    char buff[1024];
    char buff2[1024];
    if(fp == NULL){
        perror("Error while opening the file\n");
        exit(EXIT_FAILURE);
    }
    
    //Create file
    Input* in = createInput();
    
    
    //get the first line
    fgets(buff, 255, fp);
    in->size_x = atoi(buff);
    
    //get the second line
    fgets(buff2, 1024, fp);
    in->x = buff2;
    
    //get the third line
    fgets(buff, 255, fp);
    in->size_y = atoi(buff);
    
    //get the fourth line
    fgets(buff, 1024, fp);
    in->y = buff;
    
    printInput(in);
    
    //allocate m and set 0 
    init(in);
    
    fclose(fp);
    
    return in;
}

void doMemorization(Input* in){
    
    int i, j;
    
    //do_memorization
    for(i=0; i < in->size_x; i++){
        for(j=0; j < in->size_y; j++){
            memoization(in, i, j);
        }
    }
    
}


int main(){

    int i, j;
    
    Input* input = readFile("input.txt");

    //transform(input, input->size_x-1, input->size_y-1);
    //doMemorization(input);
    //printMatrix(input);
    
    return 0;
}

