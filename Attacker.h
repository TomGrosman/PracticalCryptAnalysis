#ifndef ATTACKER_H_
#define ATTACKER_H_

#include "Fob.h"
#include "RainbowTable.h"
#include <cmath>

using namespace std;

class Attacker{
public:
    Attacker();
    virtual ~Attacker();
	void Attack(Fob theFob);

private:
    RainbowTable RT;
};

#endif /* ATTACKER_H_ */
