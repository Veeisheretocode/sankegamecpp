//non oop version

//#include<bits/stdc++.h>
#include<iostream>
//#include<conio.h> //console io- we'll be using two of its funcs in Input func
#include <ncurses.h> //replacement for conio
#include <unistd.h>  //for sleep
#include<termios.h>
#include<fcntl.h>

bool gameOver; //global var
const int width = 20; //for map
const int height = 20; //also for map
int x,y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection {STOP=0, LEFT, RIGHT, UP, DOWN};
eDirection dir;




//unfortunately linux doesnt have conio.h so we'll have to write the kbhit function
int kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // disable buffering

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// same goes for getch

char mygetch() {
    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // raw mode

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}



void  Setup()
{
    gameOver = false;
    dir = STOP; //direction should be stopped before we start the game
    x = width/2;
    y = height/2; //head will be in middle of map
    fruitX = rand() % width;
    fruitY = rand() % height; //place fruit at a random place in map
    score = 0;
}

void Draw()
{
    system("clear"); //clears the screen - terminal
    //top wall
    for(int i=0; i<width+2 ; i++){
        std::cout<<"#";
    }
    std::cout<<std::endl;

    //map
    for(int i=0; i<height; i++){
        for(int j=0;j<width;j++){

            if(j==0) std::cout<<"#";

            //prints snake head
            if(i==y && j==x) std::cout<<"O";
            //prints fruit
            else if(i==fruitY && j==fruitX) std::cout<< "F";



            else 
            {
                //draw the tail

                bool printTail = false;
                for(int k=0; k<nTail; k++){
                    if(tailX[k] == j && tailY[k] == i) 
                    {
                    std::cout<<"o";
                    printTail = true;
                    }
                }
               if(!printTail) std::cout << " "; 
            }
        

            if(j==width-1) std::cout<<"#";
        }
        std::cout<<std::endl;
    }


    //bottom wall
    for(int i=0; i<width+2 ; i++){
        std::cout<<"#";
    }
    std::cout<<std::endl;

    std::cout<<"Score : "<< score << std::endl;
}

void Input(){

    if(kbhit()){  
        switch(mygetch()){
            case 'a':
                dir =LEFT;
                break;
            case 's':
                dir =DOWN;
                break;
            case 'd':
                dir =RIGHT;
                break;
            case 'w':
                dir =UP;
                break;
            case 'x':
                gameOver =true;
                break;
        }
    }
}

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x; //tail needs to follow the head
    tailY[0] = y;
    
    for(int i = 1; i< nTail ; i++){

        prev2X = tailX[i]; //hold current position
        prev2Y = tailY[i];
        tailX[i] = prevX; //hold current value into previous
        tailY[i] = prevY;
        prevX = prev2X;//hold the previous value
        prevY = prev2Y;
    }

     switch(dir){
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
            case UP:
                y--;
                break;
            case DOWN:
                y++;
                break;
            default:
                break;
        }

        // if we hit wall game over
        if(x> width || x < 0 || y>height || y<0) gameOver = true;

        //eat fruit and inc score
        if(x==fruitX && y== fruitY){

            score++;
            fruitX = rand() % width;
            fruitY = rand() % height;
            nTail++;
        }
}

int main(){


    Setup();
    while(!gameOver){
        Draw();
        Input();
        Logic();
        usleep(100000);
    }

    return 0;
}