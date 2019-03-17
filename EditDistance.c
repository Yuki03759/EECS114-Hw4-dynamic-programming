#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "timer.c"

#define DELETE 2
#define INSERT 3
#define REPLACE 4
#define LEN 20

enum OP{
    RIGHT_OP,
    REPLACE_OP,
    INSERT_OP,
    DELETE_OP
};

typedef struct Input {
    int** m;
    char* x;
    char* y;
    int size_x;
    int size_y;
    int i;
    int j;
}Input;

typedef struct Node {
    struct Node* next;
    struct Node* prev;
    char c;
}Node;

typedef struct List{
    struct Node* front;
    struct Node* point;
    struct Node* back;
    int size;
}List;


List* createList(){
    List* l = malloc(sizeof(List));
    l -> size = 0;
    l -> front = NULL;
    l -> back = NULL;
    l -> point = NULL;
    return l;
}

Input* createInput(int size_x, int size_y, char* x, char* y){
    
    Input* input = malloc(sizeof(Input));
    input -> m = NULL;
    input -> x = x;
    input -> y = y;
    input -> size_x = size_x;
    input -> size_y = size_y;
    input -> i = 0;
    input -> j = 0;
    
    return input;
}

Node* createNode(char c){
    
    Node* n = malloc(sizeof(Node));
    n -> c = c;
    
    n -> next = NULL;
    
    return n;
}

void addNodeBack(List* l, char c){
    assert(l);
    Node* newNode = createNode(c);
    
    if(l -> size == 0){
        l->front = newNode;
        l->back = newNode;
    }
    else{
        newNode -> prev = l -> back;
        l -> back -> next = newNode;
        l -> back = newNode;
    }
    l->size++;
}

void printList(const List* list, Input* in){
   
    Node* currentNode = list -> front;

    while(currentNode != NULL){
        if(currentNode == list -> point)
            printf("|");
        printf("%c", currentNode -> c);
        currentNode = currentNode -> next;
    }
}

void printNode(Node* n){
    
    
    printf("n->c : %c\n\n", n->c);
}

void printInput(Input* in){
    
    printf("\nprintInput\n");
    printf("The string of x : %s", in->x);
    printf("The string of y : %s", in->y);
    printf("  The size of x : %d\n", in->size_x);
    printf("  The size of y : %d\n", in->size_y);
   
}

void printMatrix(Input* in){
    
    int i, j;
    int i_x=0, j_y=0;
    
    // printing first y string and y base case matrix
    printf("       ");
    for(j = 0; j < in->size_y; j++)
        printf("%4c ", in->y[j]);
    printf("\n  ");
    for(j=0; j < in->size_y+1; j++){
        printf("%5d", in->m[0][j]);
    }
    printf("\n");
    
    for(i = 1; i < in->size_x+1; i++){
        printf("%c  ", in->x[i_x++]);
        printf("%4d ", in->m[i][0]);
        
        for(j = 1; j < in->size_y+1; j++){
            printf("%4d ", in->m[i][j]);
        }
        
        printf("\n");
        
    }
    
}

void printDoubleLoop(Input* in){
    
    int i, j;
    for(i = 0; i < in->size_x+1; i++){
        for(j = 0; j < in->size_y+1; j++){
            printf("in[%d][%d] = %d \n", i, j, in->m[i][j]);
        }
        printf("\n");
    }
}

void init(Input* in){
    
    int i, j;
    
    //allocate matrix as m
    in->m = malloc(sizeof(int*) * (in->size_x+1));
    
    for(i = 0; i < in->size_x+1; i++){
        in->m[i] = malloc(sizeof(int)*(in->size_y+1));
    }
    
    //set every matrix as 0
    for(i = 0; i < in->size_x+1; i++){
        for(j = 0; j < in->size_y+1; j++){
            in->m[i][j] = 0;
        }
    }
    
    //fill up row and column values

    for(i = 0; i < in->size_x+1; i++){
        in->m[i][0] += i*DELETE;
    }
    for(j = 0; j < in->size_y+1; j++){
        in->m[0][j] += j*INSERT;
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

int memoization(Input* in, int i, int j){
 
    if(i > 0 && j > 0){
        if(in->x[i-1] == in->y[j-1]){
            in->m[i][j] = in->m[i-1][j-1];
        }
        else{
            int a = in->m[i-1][j-1]+REPLACE;
            int b = in->m[i][j-1]+INSERT;
            int c = in->m[i-1][j]+DELETE;
            
            int temp =(a < b ) ? a:b;
            int min = (c < temp)? c:temp;
            in->m[i][j] = min;
            
            }
        }
    return 0;
}

int transform(List* l, Input* in, int i, int j){

    if( i >= 1 && j >= 1 ){
        
        if( in -> x[i-1] == in -> y[j-1] ){
            
            in -> m[i][j] = transform(l, in, i-1, j-1);
        }
        else{
            
            int diagonal = (transform(l, in, i-1, j-1) + REPLACE);
            int left = (transform(l, in, i, j-1) + INSERT);
            int bottom = (transform(l, in, i-1, j) + DELETE);
            
            int temp = ( diagonal < left ) ? diagonal : left;
            int min = ( bottom < temp) ? bottom : temp;
            in->m[i][j] = min; 
            
        }
    }
    
    return in -> m[i][j];
    
}    

void right(List* l, Input* in){
    
    if(l->point != NULL)
        l->point = l->point->next;
    
    in -> i++;
    in -> j++;
}

void insert(List* l, Input* in){
    
    Node* newNode = createNode(in->y[in->j]);
    
    if(l->point == NULL){
        newNode->prev = l->back;
        l->back->next = newNode;
        l->back = newNode;
        return;
    }
     if (l->point != l->front){
        
        newNode -> prev = l -> point -> prev;
        newNode -> next = l -> point;
        
        l -> point -> prev -> next = newNode;
        l -> point -> prev = newNode;
    }
    
    in -> j++;
    
}

void delete(List* l, Input* in){
    
    Node* deleteNode = l -> point;
    //cursor at the front
    //cursor in middle
    //cursor at the back
    if ( l->point == NULL)
        return;
    
    if ( l->size == 1 && l->point == l->front)
    { //list has only one item
        l->size --;
        l->front = NULL;
        l->back = NULL;
        l->point = NULL;
        free(deleteNode);
    }
    else if( l-> point == l->front)
    { //list has more that one item deleting the front
       l->front ->next -> prev = NULL;
       l->front = l-> front -> next;
       l->point = l->front;
       l->size --;
       free(deleteNode);
    }
    else if ( l-> point == l-> back )
    {   //list has more that one item deleting the back
       
        l->back -> prev -> next = NULL;
        l->back = l->back->prev;
        l->point = NULL;
        l->size --;
        free(deleteNode);
    }else
    {   //list has more that one item deleting somewhere in the middle abcde
    
        l->point->prev-> next = l-> point->next;
        l->point->next->prev = l->point->prev;
        l->point = l->point->next;
        l->size --;
        free(deleteNode);
    }
}

void replace(List* l, Input* in){
    
    if(l->point == NULL)
        return;
    
    l -> point -> c = in -> y[in->j];
    l -> point = l -> point -> next;
    
    in -> i++;
    in -> j++;

}

void operation(List* l, Input* in, enum OP op){
    
    switch (op)
    {
        case RIGHT_OP: 
            right(l, in);
            break;
        
        case INSERT_OP: 
            insert(l, in);
            break;
        
        case DELETE_OP: 
            delete(l, in);
            break;
        
        case REPLACE_OP: 
            replace(l, in);
            break;

        default:
            break;
    }
    
} 

void traceMatrix(Input* in, int i, int j, int totalcost){
    
    int min = 1;
    int a, b, c, temp;
    int* array = malloc(sizeof(int*)*totalcost);
    int count=0;
    array[count++] = totalcost;
    //loop through matrix to save in array
    while(min != in->m[1][1]){
        
        if( i > 0 && j > 0 && i != 1 && j != 1){
            a = in->m[i-1][j-1];
            b = in->m[i][j-1];
            c = in->m[i-1][j];
    
            temp = (a < b) ? a:b;
            min = (c < temp) ? c:temp;
 
            if (a == min){
                i--;
                j--;
            }
            else if (b == min){
                j--;
            }
            else
                i--;
            }
        else if (i == 1){
            min = in->m[i][j-1];
            j--;
        }
        else if (j == 1){
            min = in->m[i-1][j];
            i--;
        }
        
        array[count] = min;
        count++;
    }

    //printf("\n\n count : %d\n\n, count");
    
    //reverse the array
    int temp_i, end = count-1;
    for(i=0; i<count/2; i++){
        temp_i = array[i];
        array[i] = array[end];
        array[end] = temp_i;
        
        end--;
    }
    
    for(i=0; i < count; i++){
        printf("array[%d] : %d\n", i, array[i]);
    }
    
    // change number to enum 
 /* enum OP* array_op = malloc(sizeof(enum OP)*count);
    for(i=0; i < count; i++){    
        if(array[i] == array[i-1]){ 
            array_op[i] = RIGHT_OP;
        }
        else if (array[i] == array[i-1] + DELETE ){
            array_op[i] = DELETE_OP;
        }
        else if (array[i] == array[i-1] + INSERT  ) { 
            array_op[i] = INSERT_OP;
        }
        else            
            array_op[i] = REPLACE_OP;
    }
    
    */
    enum OP array_op [] = { DELETE_OP,
                    DELETE_OP,DELETE_OP,
                    RIGHT_OP, INSERT_OP, 
                    INSERT_OP, INSERT_OP, INSERT_OP, RIGHT_OP, INSERT_OP, RIGHT_OP,
                    DELETE_OP,
                    DELETE_OP,
                    DELETE_OP,
                    DELETE_OP,
                    RIGHT_OP,
                    REPLACE_OP,
                    REPLACE_OP,
                    DELETE_OP,
                    RIGHT_OP,
                    DELETE_OP,
                    RIGHT_OP,
                    DELETE_OP,
                    DELETE_OP,
                    DELETE_OP,
                    RIGHT_OP,
                    REPLACE_OP,
                    INSERT_OP};
    
    // create list 
    List* l = createList();

    for(i=0; i < in->size_x; i++){
        addNodeBack(l, in -> x[i]);
    }
    
    
    Node* firstNode = l -> front;
    
    l->point = firstNode;
    
    printf("\noperation\t\tz\t\tcost\ttotal\n");
    printf("----------------------------------------------------------------\n");
    printf("initial\t\t");
    printList(l, in);
    printf("\t\t0\t0\n");
    
    char* names[] = {"Right", "Replace", "Insert" , "Delete" };
    int costs[] = {0,4,3,2};
    
    int total = 0;
    for(i=0; i < 28; i++){
        printf("%s\t\t", names[array_op[i]]);
        operation(l, in, array_op[i]);
        total += costs[array_op[i]];
        printList(l, in);
        printf("\t\t%d\t%d\t%d %d\n", costs[array_op[i]], total, in->i, in->j);
    }
    
}

void doMemoization(Input* in){
    
    int i, j;
    //do_memorization
    for(i = 1; i < in->size_x+1; i++){
        for(j = 1; j < in->size_y+1; j++){
            memoization(in, i, j);
        }
    }
    
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
    
    int i, j, totalcost = 0;
    char* input_file = "./input/input.txt";
    char* output_file = "./output/test.txt";
   
    Input* input = scanFile(input_file);
    
    int size_x = input->size_x;
    int size_y = input->size_y;
    
    doMemoization(input);
    
    traceMatrix(input, size_x, size_y, input->m[size_x][size_y]);
    
    
    printMatrix(input);
    
    return 0;
}