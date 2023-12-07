#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Console.h"

#define ABS(x)      ((x) >  0  ? (x) : -(x))
#define MAX(x, y)   ((x) > (y) ? (x) : (y))
#define MIN(x, y)   ((x) < (y) ? (x) : (y))

#define TRUE 1
#define FALSE 0

#define MAX_LEN 256
#define ESC 27

typedef struct {//글자의 궤적을 표현
    char c;
    int sx, sy;     // starting coordinate
    int ex, ey;     // ending coordinate

    int vlen;       // vertical distance to move
} MovingCharacter;

typedef struct {
    MovingCharacter *chars;         // pointer for the dynamically allocated array
    int *index;                     // pointer for the dynamically allocated array
    int len;                        // the # of characters
} CharBuffer;

// Constructor that allocates arrays of MovingCharacters and indices
void CreateCharBuffer(CharBuffer *cb, char text[], int width, int height);

// Destructor to deallocate chars and index
void DeleteCharBuffer(CharBuffer *cb);

// suffle index
void ShuffleIndex(CharBuffer *cb);

// set x and y according to t
void GetCharacterCoordinate(MovingCharacter *p, int t, int *px, int *py);

int main()
{
    int no_message = 10;
    static char message[][MAX_LEN] = {
        "Merry Christmas and Happy New Year!",
        "God loves you and has a wonderful plan for your life!",
        "Everything is gonna be okay!",
        "May you always find happiness and peace in your heart.",
        "You are the masterpiece of God!",
        "Live long and prosper!",
        "May your life be filled with joy and blessing.",
        "Wishing you peace, love, and happiness in all your endeavors.",
        "May love and joy surround you every day.",
        "May you always find strength and courage to face any challenge."
    };

    srand(time(NULL));
    int screen_width = getWindowWidth();
    int screen_height = getWindowHeight() - 1;

    clrscr();

    gotoxy(1, screen_height + 1);
    printf("Press ESC to quit.");

    CharBuffer cb = { NULL, NULL, 0 };          // initialization is essential

    int cont = TRUE;
    while(cont){
        // create a character buffer with a text randomly chosen from message
        CreateCharBuffer(&cb, message[rand()%10], screen_width, screen_height);

        // repeat for i from 0 to cb.len
        for(int i = 0; i < cb.len; i++){
            MovingCharacter *p = NULL;

            // set p to the address of the index[i]-th character
            p = cb.chars[cb.index[i]];
            

            // if p->c is space, skip the remaining part of the loop body
            if(isspace(p->c)){
                continue;
            }
            
            
            int x = 0, y = 0;
            // initialize (x, y) by the starting coordinate of p, print p->c at (x, y), and call "fflush(stdout)"
            fflush(stdout);

            // move the character from (p->sx, p->sy) to (p->ex, p->ey)
            for(int t = 1; t <= p->vlen; t++){
                int prevx = 0, prevy = 0;
                // save (x, y) in (prevx, prevy)
                prevx = x;
                prevy = y;

                // get the coordinate of p at time t by calling GetCharacterCoordinate().
                GetCharacterCoordinate(p,t,&x,&y);

                // erase the previous coordinate
                gotoxy(prevx,prevy);
                printf(" ");

                // display p->c at the new coordinate
                gotoxy(x,y);
                printf("%c", p->c);

                // move the cursor at (1, screen_height), call "fflush(stdout)", and wait for 10 msec.
                gotoxy(1,screen_height);
                fflush(stdout);
                Mysleep(10);


                if(kbhit() && getch() == ESC){  // the user pressed ESC
                    cont = FALSE;           // breaks the outter loop
                    i = cb.len;             // breaks the middle loop
                    break;                  // breaks the inner loop
                }
            }
        }

        // if the user did not press ESC (cont is TRUE)
        //      move the cursor at (1, screen_height), call "fflush(stdout)", and wait for 3 sec
        //      erase the (screen_height / 2)-th row and call "fflush(stdout)"
        if(cont == TRUE){
            gotoxy(1,screen_height);
            fflush(stdout);
            MySleep(3);
            for(int i =0; i<screen_width; i++){
                gotoxy(i,screen_height /2);
                printf(" ");
            }
            fflush(stdout);
        }
        




        // deallocate character buffer in cb (call DeleteCharBuffer())
        DeleteCharBuffer(*cb);

    }
                                                                                                                        
    clrscr();
    printf("Bye!\n");

    return 0;
}

void CreateCharBuffer(CharBuffer *cb, char text[], int width, int height)
// TO DO: create a character buffer in cb
{
    // if cb is not empty (cb->chars is not NULL), delete it by calling DeleteCharBuffer
    if(cb -> chars != NULL){
        DeleteCharBuffer(cb);
    }



    // set cb->len to the length of text
    cb -> len = strlen(text);


    // allocate cb->chars and cb->index (the array length should be cb->len)
    (cb -> chars) = (struct CharBuffer*)malloc(sizeof(CharBuffer)*(cb -> len));
    (cb -> index) = (struct CharBuffer*)malloc(sizeof(CharBuffer)*(cb -> len));
    // on failure, display an error message and quit
    if(cb -> chars  == NULL|| cb -> index==NULL){
        printf("Failed to allocate memory in Line %d of %s\n", __LINE__, __FILE__);
        exit(-1);
    }



    // initialize cb->chars
    for(int i = 0; i < cb->len; i++){
        MovingCharacter *p = &cb->chars[i];

        // set p->c to text[i]        
        p -> c = text[i];

        // set p->sx to (width - 1) * i / (cb->len - 1) + 1
        // if i is even, set p->sy to 1
        // otherwise, set p->sy to height
        p ->sx = (width - 1) * i / (cb->len - 1) + 1;
        if(i%2 == 0)
            p ->sy = 1;
        else
            p ->sy = height;


        // set the ending coordinate of p by ((width - cb->len) / 2 + i, height / 2)
        p -> ex = (width - cb->len) / 2 + i;
        p -> ey = height /2;


        // set p->vlen to the absolute distance between p->sy and p->ey
        p -> vlen = abs((p ->sy) - (p->ey));
    }

    // set cb->index[i] to i for all i in range [0, cb->len
    for(int i = 0; i <= (cb -> len);i++){
        cb -> index[i] = i;
    }
    
    

    // shuffle index by calling ShuffleIndex()
    ShuffleIndex(cb);
    
    
}

void DeleteCharBuffer(CharBuffer *cb)
{
    // if cb was allocated (cb->chars is not NULL)
    //      deallocate cb->chars and cb->index
    //      for safety, set the two pointers to NULL
    //      reset cb->len to zero
    if(cb -> chars != NULL){
        free(cb ->chars);
        free(cb->index);

        cb ->chars = NULL;
        cb->index = NULL;

        cb -> len = 0;
    }


}

void ShuffleIndex(CharBuffer *cb)
{
    // TO DO: shuffle cb->index using the algorith used in the previous homework
     for (int i = 0; i < cb -> len - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < cb -> len; j++) {
            if (cb->index[j]> cb->index[max_idx]) {
                max_idx = j;
            }
        }
        if (max_idx != i) {
            struct CharBuffer temp = cb -> index[i];
            cb->index[i] = cb->index[max_idx];
            cb -> chars[max_idx] = temp;
        }
    }



}

void GetCharacterCoordinate(MovingCharacter *p, int t, int *px, int *py)
{
    *px = ((p->vlen - t) * p->sx + t * p->ex) / p->vlen;
    *py = ((p->vlen - t) * p->sy + t * p->ey) / p->vlen;
}