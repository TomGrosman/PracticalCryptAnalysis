#include <iostream>
#include "Fob.h"
#include "Attacker.h"
#include <time.h>
using namespace std;

int main(int argc, char *argv[])
{
    srand(time(NULL));
	//Initialize scenario
    Fob theFob ;
    Attacker theAttacker;

    //launch the attack
    theAttacker.Attack(theFob);

    return 0;
}
