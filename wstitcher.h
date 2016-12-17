#ifndef WSTITCHER_H
#define WSTITCHER_H

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <cstring>
#include <map>
#include <set>
#include <typeinfo>
#include "dimage.h"
#include "dthresholder.h"

class wstitcher{


private:
	int train_size = 583;
	int segmented_length = 9;
	set<string> allWords;
	set<string> big_ngram_set;
	map<string,vector<string>> oneGrams;
    map<string,vector<string>> twoGrams;
    map<string,vector<string>> threeGrams;
    map<string,vector<string>> fourGrams;
    map<string,vector<string>> fiveGrams;
    map<string,vector<string>> sixGrams;
    map<string,vector<string>> sevenGrams;
    map<string,vector<string>> eightGrams;
    map<string,vector<string>> nineGrams;

public:
	maximizedNgrams(string word){};
	getMap(int length);


}