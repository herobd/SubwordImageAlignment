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
#include "src/dimage.h"
#include "word_morphing.cpp"
//#include <boost/algorithm/string.hpp>


using namespace std;
//using namespace boost::algorithm;


    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
    }

    std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
    }

int main(int argc, char** argv)
{


	string word_filename = "test_images/img6.pgm";
    DImage exemplar("test_images/img6.pgm");
    // trim whitespace 
    clipWordImageToInk(exemplar);

	cout << word_filename << endl;
	ifstream fs(word_filename);
    string line;
    istringstream iss;
    string word_label;
    string x_segs;
    string y_segs;
    vector<string> new_x_segs;
    vector<string> new_y_segs;



    // make new PGM file
    ofstream resultsFile;
    resultsFile.open("test_output/test_file.pgm");
    resultsFile << "P5" << endl;

    int linecount = 0;
    while(getline(fs,line)){
         istringstream iss(line);
         if (linecount == 1){ // don't remember what number this is
            resultsFile << iss.str() << endl;
         }
         if (linecount == 2){
         	iss >> word_label;
            resultsFile << iss.str() << endl;
         }
         if (linecount == 4){  // x-segs 
            iss >> x_segs;
            x_segs.erase(0,1);
            new_x_segs = split(x_segs,',');
            resultsFile << iss.str() << endl;
         }
         if (linecount == 5){
         	iss >> y_segs;
            y_segs.erase(0,1);
            new_y_segs = split(y_segs,',');
            resultsFile << iss.str() << endl;
         }
         if (linecount >= 7){
            
            cout << "in the >= 7 line count" << endl;
            cout << "first y seg " << stoi(new_y_segs[0]) << endl;
            cout << "second y seg " << stoi(new_y_segs[1]) << endl;

            if (linecount >= stoi(new_y_segs[0]) && 
            linecount < stoi(new_y_segs[1])){
             // just to get the y bounds
            cout << "writing to the file " << endl;
            resultsFile << iss.str() << endl << endl;
            }
        }
        cout << "linecount  " << linecount << endl;
        linecount++;
    }

    fs.close();
    
/*
    // erase hash tags
    x_segs.erase(0,1);
    y_segs.erase(0,1);


    vector<string> new_x_segs = split(x_segs,',');
    vector<string> new_y_segs = split(y_segs,',');
    
    
    for (int i = 0; i < new_x_segs.size(); i++){
        cout << new_x_segs[i] << endl; 
    }

    cout << "y segs " << endl << endl;
    for (int i = 0; i < new_y_segs.size(); i++){
        cout << new_y_segs[i] << endl;
    }
*/

    resultsFile.close();



	return 0;

}