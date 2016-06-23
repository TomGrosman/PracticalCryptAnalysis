#include "Fob.h"
#include <string>
#include "md5.h"
#include <iostream>
#include <sstream>
#include "utils.hpp"

using namespace std;
using namespace project1;

Fob::Fob() {}

Fob::~Fob() {
	// TODO Auto-generated destructor stub
}

string Fob::ButtonPress() {
	cout << "FOB: Button Pressed, broadcasting message: \n          "    << fobMessage << "\n";
return fobMessage;
}

unsigned int Fob::receiveChallenge(unsigned int challenge) {
    //Fob only receives Challenge messages.

    unsigned int fobsCipherTextResponse = f(fobSecret,challenge);
    cout << "FOB: Received Challenge " << challenge << ".\n"
    		"   Replying using=> Fob's secret: " << fobSecret << " and Challenge: "
    		<< challenge << ",\n   to respond with generated cipher text : " << fobsCipherTextResponse << endl;
    return fobsCipherTextResponse;
}

