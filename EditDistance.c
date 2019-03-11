#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "timer.c"

typedef struct Input {
    int** m;
    char* x;
    char* y;
    int size_x;
    int size_y;
}Input;

typedef struct QNode {
    struct QNode* next;
    char* Operation;
    char z;
    int totalcost;
    int size;
}QNode;

typedef struct Queue{
    struct QNode* head;
    struct QNode* tail;
}Queue;

typedef struct Cost{
    int cost_insert;
    int cost_delete;
    int cost_replace;
}Cost;
    
Cost* createCost(){
    
    Cost* cost = malloc(sizeof(Cost));
    cost -> cost_insert = 3;
    cost -> cost_delete = 2;
    cost -> cost_replace = 4;
    return cost;
    
}

Input* createInput(int size_x, int size_y, char* x, char* y){
    
    Input* input = malloc(sizeof(Input));
    input -> m = NULL;
    input -> x = x;
    input -> y = y;
    input -> size_x = size_x;
    input -> size_y = size_y;
    
    return input;
}

void printInput(Input* in){
    
    printf("\nprintInput\n");
    printf("The string of x : %s", in->x);
    printf("The string of y : %s", in->y);
    printf("  The size of x : %d\n", in->size_x);
    printf("  The size of y : %d\n", in->size_y);
   
}

void printNode(Queue* q){
    
    QNode* qnode = q->head;
    printf("\nprintNode\n");
    while(qnode != NULL){
        
        //printf("total cost : %d\n", qnode -> totalcost);
        printf("     z     : %c\n", qnode -> z);
        qnode = qnode -> next;
    }
}

void printMatrix(Input* in){
    
    int i, j;
    
    // printing matrix
    printf("  ");
    for(j = 0; j < in->size_y; j++)
        printf("%3c ", in->y[j]);
    printf("\n");
    for(i = 0; i < in->size_x; i++){
        printf("%3c ", in->x[i]);
        for(j = 0; j < in->size_y; j++){
            printf("%3d ", in->m[i][j]);
        }
        printf("\n");
    }
    
}

void init(Input* in){
    
    int i, j;
    Cost* cost = createCost();
    
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
    if(in -> x[0] != in -> y[0]){
        for(j = 0; j < in->size_y; j++){
            if(in->x[0] != in->y[j]){
                in->m[0][j] = cost->cost_replace;
            }
        }
    
        for(i=0; i < in->size_x; i++){
            if(in->y[0] != in->x[i]){
                in->m[i][0] = cost->cost_replace;
            }
        }
    }
    
    
    //fill up row and column values
    for(i = 0; i < in->size_x; i++){
        in->m[i][0] += i*cost->cost_delete;
    }
    for(j = 0; j < in->size_y; j++){
        in->m[0][j] += j*cost->cost_insert;
    }
    
}

struct Input* scanFile(char* filename){
    
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

void Enqueue(Queue* q, Input* in, int totalcost, int qsize){
    
    QNode* newNode = malloc(sizeof(QNode));
    newNode -> size = qsize;
    newNode -> z = in -> x[newNode -> size];
    newNode -> next = NULL;
    newNode -> totalcost = totalcost;
    
    if( q -> head == NULL && q -> tail == NULL ){
        
        q -> head = q -> tail = newNode;
    
    }
    else{
        
        q -> tail -> next = newNode;
        q -> tail = newNode;
    }
}

QNode* delete(QNode* qnode, Input* in, int i, int j){
   
}

QNode* insert(QNode* qnode, Input* in, int i, int j){
    
    printf("want to insert %c\n", in->y[j]);
    qnode -> z = in -> y[j];
    qnode = qnode -> next;
    qnode -> size++;
    
    return qnode;

}

QNode* replace(QNode* qnode, Input* in, int i, int j){
    
    printf("want to change %c to %c\n", in->x[i], in->y[j]);
    qnode->z = in->y[j];
    qnode = qnode->next;
    qnode ->size++;
    
    return qnode;
}

int memoization(Input* in, Cost* cost, int i, int j){
 
    if(in->m[i][j] == 0 && i > 0 && j > 0){
        if(in->x[i] == in->y[j]){
            in->m[i][j] = in->m[i-1][j-1];
        }
        else{
            int a = in->m[i-1][j-1]+cost->cost_replace;
            int b = in->m[i][j-1]+cost->cost_insert;
            int c = in->m[i-1][j]+cost->cost_delete;
            
            int temp =(a < b ) ? a:b;
            int min = (c < temp)? c:temp;
            in->m[i][j] = min;
            
            }
        }
    return 0;
}

int transform(Queue* q, Input* in, Cost* cost, int i, int j){

    if( i >= 1 && j >= 1 && in -> m[i][j] == 0 ){
        
        if( in -> x[i] == in -> y[j] ){
            
            in -> m[i][j] = transform(q, in, cost, i-1, j-1);
        }
        else{
            
            int diagonal = (transform(q, in, cost, i-1, j-1) + cost->cost_replace);
            int left = (transform(q, in, cost, i, j-1) + cost->cost_insert);
            int bottom = (transform(q, in, cost, i-1, j) + cost->cost_delete);
            
            int temp = ( diagonal < left ) ? diagonal : left;
            int min = ( bottom < temp) ? bottom : temp;
            in->m[i][j] = min; 
            
        }
    }
    
    return in -> m[i][j];
    
}    

/*
void trace(Input* in, int i, int j, int totalcost){
    
    int max = 0;
    int right, diagonal, bottom;
    while( max != totalcost ){
        diagonal = m[i+1][m+
    }
    
    
}
*/

void doMemorization(Input* in){
    
    int i, j;
    Cost* cost = createCost();
    //do_memorization
    for(i = 0; i < in->size_x; i++){
        for(j = 0; j < in->size_y; j++){
            memoization(in, cost, i, j);
        }
    }
    
}

void printFinalInfo(Input* in, char* filename){
  
     //print information
    printInput(in);
    printf("\n");
    printf("input file : %s\n", filename);
    printf("smallest cost : %d\n", in->m[in->size_x-1][in->size_y-1]);
    printf("\n");
}

void writeFile(Input* in, char* filename){
    
    FILE* fp = fopen(filename, "w");
    
    int i, j;
    
    // printing matrix
    fprintf(fp, "  ");
    for(j = 0; j < in->size_y; j++)
        fprintf(fp, "%3c ", in->y[j]);
    fprintf(fp, "\n");
    for(i = 0; i < in->size_x; i++){
        fprintf(fp, "%3c ", in->x[i]);
        for(j = 0; j < in->size_y; j++){
            fprintf(fp, "%3d ", in->m[i][j]);
        }
        fprintf(fp, "\n");
    }
    
    fclose(fp);
}

int main(){
    
    int i, j;
    int totalcost = 0;
    int qsize=0;
    char* input_file = "./input/test.txt";
    char* output_file = "./output/test.txt";
   
    Queue *q = malloc(sizeof(Queue));
    
    Input* input = scanFile(input_file);
    Cost* cost = createCost();
    
    int size_x = input -> size_x;
    int size_y = input -> size_y;
    
    for(i=0; i < input->size_x; i++){
        
        Enqueue(q, input, 0, qsize++);
    }
    
    QNode* firstNode = q->head;
    
    //transform(q, input, cost, size_x-1, size_y-1);
    
    doMemorization(input);
    
    //trace(input, cost, 0, 0, input->m[size_x-1][size_y-1]);
    
    printMatrix(input);
    
    printNode(q);
    
    printFinalInfo(input, input_file);
    
    return 0;
}