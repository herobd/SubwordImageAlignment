#include "wstitcher.h"




wstitcher::wstitcher(){}

wstitcher::~wstitcher(){};
/**
create 5 instances of the the greatest ngram stitched length permutations

*/
void wstitcher::maximizedNgrams(string word){

    // 3 biggest substrings
    if (word.length() > 4){ // 3 substrings
        string substA = word.substr(0,word.length()-1);
        string substB = word.substr(0,word.length()-2);
        string substC = word.substr(0,word.length()-3);

        // counter part
        string cpA = word.substr(word.length()-1,word.length());
        string cpB = word.substr(word.length()-2,word.length());
        string cpC = word.substr(word.length()-3,word.length());

        map<string,string> map1 = getMap(substA.length());
        map<string,string> map2 = getMap(substB.length());
        map<string,string> map3 = getMap(substC.length());
        map<string,string> map4 = getMap(cpA.length());
        map<string,string> map5 = getMap(cpB.length());
        map<string,string> map6 = getMap(cpC.length());
      	
      	// make five random constructs of these 3 combos
        map<string,string>::iterator it;
        map1.find(substA)
        
        


    } else if (word.length() == 4){ // 2 substrings 

        string substA = word.substr(0,word.length()-1);
        string substB = word.substr(0,word.length()-2);

        // counter part
        string cpA = word.substr(word.length()-1,word.length());
        string cpB = word.substr(word.length()-2,word.length());
    } else if (word.length() == 3){ // 1 substring

        string substA = word.substr(0,word.length()-1);

        // counter part
        string cpA = word.substr(word.length()-1,word.length());

    } else { // bigram

    }
if (ngrams.find("ce") != ngrams.end()){
            cout << "found" << endl;
        }


}


map<string,string> wstitcher::getMap(int length){
	
		map<string,string> currMap;
		switch (length){
        case 1:
            currMap = oneGrams;
            break;
        case 2:
            currMap = twoGrams;
            break;
        case 3:
            currMap = threeGrams;
            break;
        case 4:
            currMap = fourGrams;
            break;
        case 5:
            currMap = fiveGrams;
            break;
        case 6:
            currMap = sixGrams;
            break;
        case 7:
            currMap = sevenGrams;
            break;
        case 8:
            currMap = eightGrams;
            break;
        case 9:
            currMap = nineGrams;
            break;
        default:
            break;
    }

    return currMap;

}

void wstitcher::loadData(){


		 // construct dictionary
	    for (int j = 0; j < train_size; j++){
            stringstream ngram_filename;
            if (j < 10){
                ngram_filename << "../smith_fullgrams/w_" << desired_ngram << "000000" << j << ".pgm";
            } else if (j >= 10 && j < 100){
                ngram_filename << "../smith_fullgrams/w_" << desired_ngram << "00000" << j << ".pgm";
            } else if (j >= 100 && j < 1000){
                ngram_filename << "../smith_fullgrams/w_" << desired_ngram << "0000" << j << ".pgm";
            } else if (j >= 1000){
                ngram_filename << "../smith_fullgrams/w_" << desired_ngram << "000" << j << ".pgm";
            }

            string temp_str = ngram_filename.str();
            DImage ngram_glyph(temp_str.c_str());
            string transcription = ngram_glyph.getCommentByIndex(1);
        
        	allWords.insert(transcription);

        }


        // load in all data

        for (int i = 1; i < segmented_length+1; i++){

        int data_size;
        map<string,int> current_data;

        cout << "current n gram length being loaded " << i << endl;

        switch (i){
        case 1:
            data_size = 1165;
            break;
        case 2:
            data_size = 908;
            break;
        case 3:
            data_size = 671;
            break;
        case 4:
            data_size = 466;
            break;
        case 5:
            data_size = 312;
            break;
        case 6:
            data_size = 193;
            break;
        case 7:
            data_size = 99;
            break;
        case 8:
            data_size = 40;
            break;
        case 9:
            data_size = 16;
            break;
        default:
            break;
        }      

        for (int j = 0; j < data_size; j++){
            //cout << j << endl;
            stringstream ngram_filename;
            if (j < 10){
                ngram_filename << "../smith_fullgrams/w_" << i << "000000" << j << ".pgm";
            } else if (j >= 10 && j < 100){
                ngram_filename << "../smith_fullgrams/w_" << i << "00000" << j << ".pgm";
            } else if (j >= 100 && j < 1000){
                ngram_filename << "../smith_fullgrams/w_" << i << "0000" << j << ".pgm";
            } else if (j >= 1000){
                ngram_filename << "../smith_fullgrams/w_" << i << "000" << j << ".pgm";
            }


            string theFile = ngram_filename.str();
            DImage ngram_glyph_o(theFile.c_str());
            string label = ngram_glyph_o.getCommentByIndex(1);
            
            if (i == 1){
                all_1_grams.insert(label);
                oneGrams.push_back(theFile);
            } else if (i ==2){
                all_2_grams.insert(label);
                twoGrams.push_back(theFile);
            } else if (i ==3){
                all_3_grams.insert(label);
                threeGrams.push_back(theFile);
            } else if (i ==4){
                all_4_grams.insert(label);
                fourGrams.push_back(theFile);
            } else if (i ==5){
                all_5_grams.insert(label);
                fiveGrams.push_back(theFile);
            } else if (i ==6){
                all_6_grams.insert(label);
                sixGrams.push_back(theFile);
            } else if (i ==7){
                all_7_grams.insert(label);
                sevenGrams.push_back(theFile);
            } else if (i ==8){
                all_8_grams.insert(label);
                eightGrams.push_back(theFile);
            } else if (i ==9){
                all_9_grams.insert(label);
                nineGrams.push_back(theFile);
            }

            big_ngram_set.insert(label);
      
        }   

    }





}


/*void wstitcher::updateMap(int length, map<string,string> map){
		
		switch (length){
        case 1:
            oneGrams = map;
            break;
        case 2:
            twoGrams = map;
            break;
        case 3:
            threeGrams = map;
            break;
        case 4:
            fourGrams = map;
            break;
        case 5:
            fiveGrams = map;
            break;
        case 6:
            sixGrams = map;
            break;
        case 7:
            sevenGrams = map;
            break;
        case 8:
            eightGrams = map;
            break;
        case 9:
            nineGrams;
            break;
        default:
            break;
    }
}

*/