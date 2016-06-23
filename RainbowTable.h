#ifndef _RAINBOWTABLE_H_
#define _RAINBOWTABLE_H_

#include <unordered_map>
#include <map>
#include <sstream>
#include <fstream>
#include "utils.hpp"
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;
using namespace project1;
 
class RainbowTable {
public:
    RainbowTable();
    virtual ~RainbowTable();
    void computeRainbowTable();
	std::vector<std::pair<unsigned int, unsigned int>> RainbowTableLookup(unsigned int key);    
private:
    ofstream chainFile;
    int totalPoints =0;
    unordered_map <unsigned int,unsigned int> hashMap;
    unsigned int startingPointCounter=0;
};
#endif
