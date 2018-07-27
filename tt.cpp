
#include <time.h>
#include <signal.h>
#include <iostream>
#include "./motor.cpp"


bool sim = false;

tiptilt *tt;


void intHandler(int signum)
{
}

//--------------------------------------------------------------------------------------


int main(int argc, char **argv)
{
    signal(SIGINT, intHandler);
    
    
    tt = new tiptilt();
    
    int i = 0;
    
    if (argc == 2) {
        
        if (strcmp(argv[1], "reset") == 0) {
            tt->reset_pos();
            goto exit;
        }
        
        float focus_move = atof(argv[1]);
        if (fabs(focus_move > 12500.0)) {
            printf("max move %f\n", focus_move);
        }
        else {
            tt->MoveFocus(focus_move);
        }
    }
    
    if (argc == 3) {
        float dx = atof(argv[1]);
        float dy = atof(argv[2]);
        
        if (fabs(dx > 12500.0) || fabs(dy > 12500)) {
            printf("max tilt %f %f\n", dx, dy);
        }
        else {
            tt->Move(dx, dy);
        }
    }
    if (argc == 4) {
        float a = atof(argv[1]);
        float b = atof(argv[2]);
        float c = atof(argv[3]);
        tt->setxyz(a,b,c);
    }
    
exit:;
    
    delete tt;
    
}
