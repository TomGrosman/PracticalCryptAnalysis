
#include "md5.h"
#include "RainbowTable.h"
#include <sys/time.h>
#include <vector>

using namespace std;
using namespace project1;

RainbowTable::RainbowTable(){}
RainbowTable::~RainbowTable(){}

void RainbowTable::computeRainbowTable()
{

    double elapsedTime;
    struct timeval t1, t2;
    
    gettimeofday(&t1, NULL);
    
    while( hashMap.size() <  numRows) {
//        cout << startingPointCounter << endl;
        
        unsigned int hexKey = rand() % (unsigned int) KEYSPACE ;
        unsigned int startingPoint = f(hexKey, challenge);
 
        // We have a start point, Is it already in the table?
        if (hashMap.count(startingPoint)>0)  //not a unique starting point, no need to generate chain
            continue;
        else
            startingPointCounter++;
        
        unsigned int FobsChain [1024] = {};
        bool SecretInserted = false;
        
                    //debug trace to see if secret was put in table
        if (startingPoint == 64365970){
                   cout << " \nFOB KEY GENERATED as starting point" << startingPoint << "\n";

        }
        
        totalPoints++;
        unsigned int seed= startingPoint;

        FobsChain[0] = seed;
        
        for ( unsigned int i = 1; i < chainLength; i++){
            seed = f(seed,challenge,i);
            FobsChain[i] = seed;
            
            //debug trace to see if secret was put in table
            if (seed == 64365970){
                       cout << " \nFOB KEY GENERATED in row with starting point " << startingPoint <<
                       " at position " << i << "\n";
//                SecretInserted = true;
            }
            
            totalPoints++;
        }
        hashMap.insert ( std::pair<unsigned int,unsigned int>(startingPoint,seed) );
        if (SecretInserted){
            for (int i = 0;i <= chainLength; i++)
                cout << "[" << i << "," << FobsChain[i] << "]  ";
                }

        if (hashMap.size() % 8192 == 0){
            gettimeofday(&t2, NULL);
            elapsedTime = (t2.tv_sec - t1.tv_sec);
            elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to sec
            cout << "\nProgress...  Inserted " << hashMap.size()<< " rows in " << elapsedTime <<" seconds";
        }
    }

    
    cout << "\n\nTable generated in " <<elapsedTime<< " sec.\n  Chain length is =>         " << chainLength << endl;
    cout << "  Table size is  =>          " << hashMap.size() << endl;
    cout << "  Total points generated =>  " << totalPoints << " = " << totalPoints/elapsedTime << " points/sec" << endl;
    
    chainFile.open(outputFileName);
    for (std::unordered_map<unsigned int,unsigned int>::iterator it=hashMap.begin(); it!=hashMap.end(); ++it){
//        chainFile <<   it->first << it->second << "    " <<"\n";
        chainFile <<   it->first <<   " " << it->second << "    " <<"\n";
    }
    
    chainFile.close();
}



// Return a vector of unsigned int instead of singles, since false positives or other 
// false indicators may take place before a real result.
std::vector<std::pair<unsigned int, unsigned int>> RainbowTable::RainbowTableLookup(unsigned int key)
{
    
    /*cout << "EXPLICIT TEST FOR STARTING POINT 87897457" << endl;
    unsigned int testing = 87897457;
    for(int i = 1; i <= 1024; i++) {
        cout << "testing: " << testing << " at i: " << i << endl;
    testing = f(testing,challenge,i);
}*/

    
    // Define a local struct, since extractSecret might be used in different places
    cout << "initial Starting Point: " << key << endl;
    struct whenMatchFound
    {
        static unsigned int extractSecret(unsigned int startingPoint, int chainindex)
        {
            unsigned int secrettemp = startingPoint;
            for(int i = 1; i < chainindex; i++) {
                secrettemp = project1::f(secrettemp,challenge,i);
            }
            return secrettemp;
        }
    };

    // Local variables to be used.
    string entrypoint, endpoint;
    unsigned int entrypointint, endpointint, index, secret;
    secret = 0;
    std::vector<pair<unsigned int, unsigned int>> secrets;
    std::ifstream tableFile(outputFileName); // Open the Rainbow Table file.

    // Load the stored file into a hash map and while at it, check if the hash matches any end points or starting points.
    unordered_map <unsigned int,unsigned int> hashMap;

    index = chainLength-1; // In case hashes match any of the loaded points immediately, take note of the index.

//    char comma;
    // Load from file to map
    while(tableFile >> entrypoint >> endpoint) {
        entrypointint = std::stoul(entrypoint,NULL,10);
        endpointint = std::stoul(endpoint,NULL,10);
        if(key == endpointint) {
            cout << "Hash match at endpoint: " << entrypointint << endl; 
            //secrets.push_back(whenMatchFound::extractSecret(entrypointint,index));
        }
        // No need to compare to starting points from this point on. Implement map as a quick lookup structure.
        //hashMap.insert({endpointint,entrypointint});
        hashMap.insert(std::make_pair(endpointint,entrypointint));

        index--;
    }
    
    // Iterate the reduction functions, apply offset and start over untill hash compares to end-point.
    // At this point, we know the row and the number of functions needed to be applied to get to the before it.
    for(int i = project1::chainLength-1; i >= 0; i--) {
        secret = key+i;
        for(int j = i; j <= project1::chainLength-1; j++) {
            secret = f(secret, challenge, j+1);
            std::unordered_map<unsigned int,unsigned int>::const_iterator got = hashMap.find (secret);
            if ( got == hashMap.end() ) {} // No match
            else {                  
                    // Match
                secrets.push_back(std::make_pair(whenMatchFound::extractSecret(got->second,i),(unsigned int) i-1)); // Feed extractSecret() the starting ooint and the row number for which to work up against.
            }
        }
    }
    return secrets;
}
