
#include "Attacker.h"
#include "md5.h"
#include "utils.hpp"


using namespace project1;

Attacker::Attacker() {}


Attacker::~Attacker() {
	// TODO Auto-generated destructor stub
}

void Attacker::Attack(Fob theFob) {
	unsigned int  reply = 0;
	unsigned int theKey = 0;
	std::vector<std::pair<unsigned int,unsigned int>> secrets;
    cout <<"ATTACKER: Generating Rainbow Table with " << numRows
        << " start points, using challenge: " << challenge << endl;;
    
    //AFTER the rainbow table is generated for the first time, we can
    // Comment the following line out . This will remove the need to generate a whole table everytime running.

    //RT.computeRainbowTable();
    
            cout << "\nATTACKER: Pressing Key Fob Button\n" ;
    
	string fobMessage = theFob.ButtonPress() ;
        cout << "ATTACKER: Fob message received. \n   The Fob replied with \'" <<
        		fobMessage << "\'\n\nATTACKER: Starting attack..." << endl;
    
        cout << "  Pretending to be car. Sending challenge " << challenge << endl;
    
    reply = theFob.receiveChallenge(challenge);
        cout << "\nATTACKER: Looking up response " << reply << " in Rainbow table" <<endl;
    
    secrets = RT.RainbowTableLookup(reply);
    theKey = std::get<0>(secrets.at(0));
    cout << "Checking for false positives" << endl;
    for(int i = 0; i < secrets.size(); i++) {	
    	if(f(std::get<0>(secrets.at(i)),challenge,(std::get<1>(secrets.at(i)))) == reply+std::get<1>(secrets.at(i))) { // Supposed to match against the correct result from Fob.
    		theKey = std::get<0>(secrets.at(i));
    	}
    }

	// TODO Check if key was found
    if (theKey == 0)
        cout << "ATTACKER: Key NOT found, attack was unsuccessful. Exiting... \n\n";
    else   //Key found
    {
        cout << "ATTACKER: Key found: " << theKey << endl;
        cout << "ATTACKER: Validating key by testing Fob's responses\n\n";
        
        unsigned int newChallenge;
        string fobMessage;
        int successRate = 0;
        
        for (int i = 1; i <= ValidationAttempts;i++){
            cout << "\n  ATTACKER: Verification attempt # " << i;
            cout << "\n      ATTACKER: Pressing Button to Activate Fob\n      " ;
            fobMessage = theFob.ButtonPress() ;
            
            // use an arbitrary challenge
            newChallenge = rand() % KEYSPACE;
            reply = f(theKey, newChallenge);
            cout << "      ATTACKER: Fob is activated. Sending challenge " << newChallenge;
            
            cout << "\n      ATTACKER: Expecting response " << reply << "\n      ";
        
            if (reply == theFob.receiveChallenge(newChallenge)){
                cout << "  ATTACKER: SUCCESS! expected result " << reply <<  " obtained \n";
                successRate ++;
            }
            else
                cout << "  ATTACKER: FAILURE expected result not obtained \n";
        }
        cout << "\n\nFob's key verified " << successRate << " out of " << ValidationAttempts << " attempts \n\n";
    }

}