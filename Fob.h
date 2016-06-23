#ifndef FOB_H_
#define FOB_H_

#include <iostream>

using namespace std;

class Fob {
public:
	Fob();
	virtual ~Fob();
	string  ButtonPress();
    unsigned int receiveChallenge(unsigned int challenge);
private:
    unsigned int fobSecret =  0x3d62592;
    string fobMessage = "Hello, key fob no. 0123456 is here!";
};

#endif /* FOB_H_ */
