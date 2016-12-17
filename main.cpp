#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <cstdlib>
#include "dimage.h"
#include "dwordfeatures.h"
#include "ddynamicprogramming.h"
#include "dthresholder.h"
#include "scanTestingImages.h"

using namespace std;

void trimImg(DImage &img, int noiseRemovalSize=2);
void findMatchingPatches(string similar, DImage img1, string word1, DImage img2, string word2, DImage img3, string word3);
void findMatchingPatches_grow(string similar, DImage img1, string word1, DImage img2, string word2, DImage img3, string word3);
double findMatching_exemplar_grow(string similar, DImage img1, string word1, DImage examplar);

int main(int argc, char** argv)
{
    /*if (argc < 3){
        //cout << "USAGE: .exe, ngram_transcription, ngram_glyph, word_glyph"
        //<< ", word_transcription, data_size" << endl;
        cout << "USAGE: .exe, data_size" << endl;
        exit (EXIT_FAILURE);
    }*/



     /*
    string similar = argv[1]; // actual ngram string
    DImage exemplar(argv[2]); // ngram picture
    
    DImage img1(argv[3]); // entire image picture
    string word1 = argv[4]; // entire string of word

    // ngram_trans, word_glyph, word_trans, ngram_glpyh
    double finalScore = findMatching_exemplar_grow(similar,img1,word1, exemplar);
    cout << finalScore << endl;
     */


    ///*
    ofstream resultsFile;
    resultsFile.open("test_output/smithSet_ngrams.txt");

    for (int a = 1; a < 10; a++){
    int data_size;
    // Smith Data set
    // 1-grams - 1165
    // bigrams - 908
    // trigrams - 671
    // 4grams - 466
    // 5grams - 312
    // 6grams - 193
    // 7grams - 99
    // 8grams - 40
    // 9grams - 16
    
    switch (a){
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



    int train_size = data_size *.5; // 3 fold testing
    double pred_accuracy = 0;

    int desired_ngram = a;


    for (int i = train_size; i < data_size; i ++){ // single gram accuracy 

    string bestword;

    stringstream word_filename;
    if (i < 10){
        word_filename << "smith_fullgrams/w_" << desired_ngram << "000000" << i << ".pgm";
    } else if (i >= 10 && i < 100){
        word_filename << "smith_fullgrams/w_" << desired_ngram << "00000" << i << ".pgm";
    } else if (i >= 100 && i < 1000){
        word_filename << "smith_fullgrams/w_" << desired_ngram << "0000" << i << ".pgm";
    } else if (i >= 1000){
        word_filename << "smith_fullgrams/w_" << desired_ngram << "000" << i << ".pgm";
    }

    ifstream fs(word_filename.str());
    //fs.open(argv[2]);
    //stringstream ss;
    string line;
    istringstream iss;
    string word_label;

    int linecount = 0;
    while(getline(fs,line)){
         istringstream iss(line);
         if (linecount == 2){
            iss >> word_label;
            linecount = 0;
            break;
         }
        linecount++;
    }
    fs.close();
    
    double bestMatchScore = 99999;
    string ngram_label;

    for (int j = 0; j < train_size; j++){
        stringstream ngram_filename;
        if (j < 10){
            ngram_filename << "smith_fullgrams/w_" << desired_ngram << "000000" << j << ".pgm";
        } else if (j >= 10 && j < 100){
            ngram_filename << "smith_fullgrams/w_" << desired_ngram << "00000" << j << ".pgm";
        } else if (j >= 100 && j < 1000){
            ngram_filename << "smith_fullgrams/w_" << desired_ngram << "0000" << j << ".pgm";
        } else if (j >= 1000){
            ngram_filename << "smith_fullgrams/w_" << desired_ngram << "000" << j << ".pgm";
        }
    
        ifstream fs(ngram_filename.str());
        while(getline(fs,line)){
            istringstream iss(line);
            if (linecount == 2){
                iss >> ngram_label;
                linecount = 0;
                break;
            }
        linecount++;
        } 
    fs.close();

    // findMatching_exemplar_grow needs these objects
    string temp_ngram_file = ngram_filename.str();
    string temp_word_file = word_filename.str();
    const char* cstr1 = temp_ngram_file.c_str();
    const char* cstr2 = temp_word_file.c_str();
    DImage ngram_file(cstr1); 
    DImage word_file(cstr2); 

    // ngram_trans, word_glyph, word_trans, ngram_glpyh


    double score = findMatching_exemplar_grow(ngram_label,word_file, word_label,ngram_file);

    //cout << "current score: " << score << endl;

        if (score < bestMatchScore){
            bestMatchScore = score;
            bestword = ngram_label;


        }
    }

    if (bestword == word_label){ 
        pred_accuracy++;
        }


    
    }

    cout << "total accuracy for " << a << "-grams: " << pred_accuracy/ (data_size-train_size) << endl;
    resultsFile << "total accuracy for " << a << "-grams: " << pred_accuracy/ (data_size-train_size) << endl;
    }   
    resultsFile.close();
    //*/
    return 0;
}

struct point
{
    int x, y;
    point(int x, int y)
    {
        this->x=x;
        this->y=y;
    }
};

void trimImg(DImage &img, int noiseRemovalSize)
{
    DImage threshed;
    DThresholder::threshImage_(threshed, img, DThresholder::getOtsuThreshVal(img));
    
    //first we perform a morphological open to get rid of noise which might interfere with trimming
    vector<struct point> pointsToDilate;
    DImage open_tmp(threshed);
    
    //errode
    for (int x=0; x<open_tmp.width(); x++)
        for (int y=0; y<open_tmp.height(); y++)
    {
        bool allBlack = true;
        for (int ri=-noiseRemovalSize; ri<=noiseRemovalSize && allBlack; ri++)
        {
            for (int rj=-noiseRemovalSize; rj<=noiseRemovalSize; rj++)
            {
                if (ri*ri+rj*rj<(noiseRemovalSize*noiseRemovalSize)) //somewhat circular
                {
                    if (ri+x>=0 && ri+x<open_tmp.width() &&
                            rj+y>=0 && rj+y<open_tmp.height())
                    {
                        if (open_tmp.getPixelGray(x+ri,y+rj)==255)//if off
                        {
                                allBlack=false;
                                break;
                        }
                    }
                }
            }
        }
        if (allBlack)
        {
            threshed.setPixel(x,y,0);//on
            pointsToDilate.push_back(point(x,y));
        }
        else
        {
            threshed.setPixel(x,y,255);//off
        }
        
    }
    
    //dialate
    for (struct point p : pointsToDilate)
    {
        
        for (int ri=-noiseRemovalSize; ri<=noiseRemovalSize; ri++)
        {
            for (int rj=-noiseRemovalSize; rj<=noiseRemovalSize; rj++)
            {
                if (ri*ri+rj*rj<(noiseRemovalSize*noiseRemovalSize))
                {
                    if (ri+p.x>=0 && ri+p.x<open_tmp.width() &&
                            rj+p.y>=0 && rj+p.y<open_tmp.height())
                    {
                        threshed.setPixel(p.x,p.y,0);//on
                    }
                }
            }
        }
    }
    
    //Trim the borders of whitespace
    int leftX, rightX, topY, botY;
    
    for (int x=0; x<threshed.width(); x++)
        for (int y=0; y<threshed.height(); y++)
        {
            if (threshed.getPixelGray(x,y)==0)//if on
            {
                leftX=x;
                x=threshed.width();
                break;
            }
        }
    for (int x=threshed.width()-1; x>=0; x--)
        for (int y=0; y<threshed.height(); y++)
        {
            if (threshed.getPixelGray(x,y)==0)//if on
            {
                rightX=x;
                x=-1;
                break;
            }
        }
    
    for (int y=0; y<threshed.height(); y++)
        for (int x=0; x<threshed.width(); x++)
        {
            if (threshed.getPixelGray(x,y)==0)//if on
            {
                topY=y;
                y=threshed.height();
                break;
            }
        }
    for (int y=threshed.height()-1; y>=0; y--)
        for (int x=0; x<threshed.width(); x++)
        {
            if (threshed.getPixelGray(x,y)==0)//if on
            {
                botY=y;
                y=-1;
                break;
            }
        }
    
    img = img.copy(leftX,topY,1+rightX-leftX,1+botY-topY);
}

//This returns a subimage centered around the desired n-gram
//testPortionScale refers to how many times larger the portion should be to the expected size of the n-gram
DImage getTestPortion(const string& similar, const DImage& img, const string& word, double testPortionScale, int* start=NULL, int* end=NULL)
{
    double startLoc = word.find(similar) / (0.5 + word.length());
    double portion = similar.length() / (0.5 + word.length());
    
    int testPortionStart=max(0.0,(startLoc-(portion*(testPortionScale-1)/2))*img.width());
    
    int testPortionEnd=min((double)img.width(), ((startLoc-(portion*(testPortionScale-1)/2)) + (testPortionScale*portion))*img.width());
    
    int testPortionWidth=testPortionEnd-testPortionStart;
    if (start != NULL && end != NULL)
    {
        *start = testPortionStart;
        *end = testPortionEnd;
    }
    return img.copy(testPortionStart,0,testPortionWidth,img.height());
}

DImage getTestPortion(const string& similar, const DImage& img, int start, int end)
{
    return img.copy(start,0,end-start,img.height());
}

DImage getTestPortion(const string& similar, const DImage& img, const string& word, double testPortionScale, double portionOffset, int* start=NULL, int* end=NULL)
{
    //cout << "in this getTestPortion" << endl;


    double startLoc;
    // if ngram not in subword results in extremely large number
    if (word.find(similar) != string::npos){ // then it was more than likely found
            startLoc = word.find(similar) / (0.5 + word.length()); 
    } else {
            startLoc = .1;
    }
    //cout << "word.find(similar) results in " << word.find(similar) << endl;;
    //cout << "startLoc " << startLoc << endl;
    //cout << "A" << endl;
    double portion = similar.length() / (0.5 + word.length());
    //cout << "B" << endl;

    int testPortionStart=max(0.0,(startLoc-(portion*(testPortionScale-1)/2) + portion*portionOffset)*img.width());
    //cout << "C" << endl;

    int testPortionEnd=min((double)img.width(), ((startLoc-(portion*(testPortionScale-1)/2)) + (testPortionScale*portion) + portion*portionOffset)*img.width());
    //cout << "D" << endl;

    int testPortionWidth=testPortionEnd-testPortionStart;
    //cout << "E" << endl;

    if (start != NULL && end != NULL)
    {
        *start = testPortionStart;
        *end = testPortionEnd;
    }
    //cout << "F" << endl;
    //cout << "testPortionStart: " << testPortionStart << endl;
    //cout << "testPortionWidth: " << testPortionWidth << endl;
    //cout << "img.height: " << img.height() << endl;

    return img.copy(testPortionStart,0,testPortionWidth,img.height());
}

void makeWindows(vector<DImage>& windows, int& portionWindowSize, int& portionWindowStide, const DImage& testPortion, double portionWindowScale, double portionWindowStrideScale)
{
    portionWindowSize = testPortion.width()*portionWindowScale;
    portionWindowStide = portionWindowSize*portionWindowStrideScale;
    
    ;
    for (int i=0; i< (testPortion.width()-portionWindowSize)/portionWindowStide; i++)
    {
        windows.push_back(testPortion.copy(i*portionWindowStide,
                                       0,
                                       portionWindowSize,
                                       testPortion.height()));
    }
}

void findMatchingPatches(string similar, DImage img1, string word1, DImage img2, string word2, DImage img3, string word3)
{
    
    trimImg(img1);
    trimImg(img2);
    trimImg(img3);
    
    DImage testPortion1 = getTestPortion(similar,img1,word1,3);
    DImage testPortion2 = getTestPortion(similar,img2,word2,3);
    DImage testPortion3 = getTestPortion(similar,img3,word3,3);
    
    
    double portionWindowScale=.6;
    double portionWindowStrideScale=.1;
    
    int portionWindowSize1, portionWindowSize2, portionWindowSize3;
    int portionWindowStide1, portionWindowStide2, portionWindowStide3;
    
    vector<DImage> windows1;
    makeWindows(windows1,portionWindowSize1,portionWindowStide1,testPortion1,portionWindowScale,portionWindowStrideScale);
    
    vector<DImage> windows2;
    makeWindows(windows2,portionWindowSize2,portionWindowStide2,testPortion2,portionWindowScale,portionWindowStrideScale);
    
    vector<DImage> windows3;
    makeWindows(windows3,portionWindowSize3,portionWindowStide3,testPortion3,portionWindowScale,portionWindowStrideScale);
    
    tuple<int,int, int> bestMatch;
    double bestCost=99999;
    
    
    for (int i1=0; i1< (testPortion1.width()-portionWindowSize1)/portionWindowStide1; i1++)
    {
        for (int i2=0; i2< (testPortion2.width()-portionWindowSize2)/portionWindowStide2; i2++)
        {
            for (int i3=0; i3< (testPortion3.width()-portionWindowSize3)/portionWindowStide3; i3++)
            {
                DFeatureVector fv1 = DWordFeatures::extractWordFeatures(windows1[i1]);
                DFeatureVector fv2 = DWordFeatures::extractWordFeatures(windows2[i2]);
                DFeatureVector fv3 = DWordFeatures::extractWordFeatures(windows3[i3]);
                
                double cost = DDynamicProgramming::findDPAlignment(fv2,fv1,100,100,0);
                cost += DDynamicProgramming::findDPAlignment(fv3,fv1,100,100,0);
                cost += DDynamicProgramming::findDPAlignment(fv3,fv3,100,100,0);
                if (cost < bestCost)
                {
                    bestCost = cost;
                    bestMatch = make_tuple(i1,i2,i3);
                }
            }
        }
    }
    
    DFeatureVector fv1 = DWordFeatures::extractWordFeatures(windows1[get<0>(bestMatch)]);
    DFeatureVector fv2 = DWordFeatures::extractWordFeatures(windows2[get<1>(bestMatch)]);
    
    int path12len;
    int path12[(1+fv1.vectLen)*(1+fv2.vectLen)];
    double cost12 = DDynamicProgramming::findDPAlignment(fv2,fv1,100,100,0,&path12len,path12);
    
    
    DImage warped12 = DDynamicProgramming::piecewiseLinearWarpDImage(windows2[get<1>(bestMatch)],windows1[get<0>(bestMatch)].width(),path12len,path12,false);
    
    img1.save("img1Trimmed.png",DImage::DFileFormat_png);
    img2.save("img2Trimmed.png",DImage::DFileFormat_png);
    testPortion1.save("testPortion1.png",DImage::DFileFormat_png);
    testPortion2.save("testPortion2.png",DImage::DFileFormat_png);
    windows1[get<0>(bestMatch)].save("window1.png",DImage::DFileFormat_png);
    windows2[get<1>(bestMatch)].save("window2.png",DImage::DFileFormat_png);
    windows3[get<2>(bestMatch)].save("window3.png",DImage::DFileFormat_png);
    warped12.save("warped12.png",DImage::DFileFormat_png);
    
}


//void findMatchingPatches_grow(string similar, DImage img1, string word1, DImage img2, string word2, DImage img3, string word3)
//{
    
//    trimImg(img1);
//    trimImg(img2);
//    trimImg(img3);
    
//    double scan = 0;
//    for (;;)
//    {
    
//        DImage testPortion1 = getTestPortion(similar,img1,word1,1);
//        DImage testPortion2 = getTestPortion(similar,img2,word2,1);
//        DImage testPortion3 = getTestPortion(similar,img3,word3,1);
        
        
        
//        DFeatureVector fv1 = DWordFeatures::extractWordFeatures(testPortion1);
//        DFeatureVector fv2 = DWordFeatures::extractWordFeatures(testPortion2);
        
//        int path12len;
//        int path12[(1+fv1.vectLen)*(1+fv2.vectLen)];
//        double cost12 = DDynamicProgramming::findDPAlignment(fv2,fv1,100,100,0,&path12len,path12);
        
//    }
    
    
//    DImage warped12 = DDynamicProgramming::piecewiseLinearWarpDImage(testPortion2,testPortion1.width(),path12len,path12,false);
    
//    img1.save("img1Trimmed.png",DImage::DFileFormat_png);
//    img2.save("img2Trimmed.png",DImage::DFileFormat_png);
//    testPortion1.save("testPortion1.png",DImage::DFileFormat_png);
//    testPortion2.save("testPortion2.png",DImage::DFileFormat_png);
//    warped12.save("warped12.png",DImage::DFileFormat_png);
//}

double findMatching_exemplar_grow(string similar, DImage img1, string word1, DImage examplar)
{
    // I'm getting memory allocation errors from dImage, it aborts if images are badly off
    /*if (word1.find(similar) == string::npos){
        return 99999;
    }*/
    trimImg(img1);
    trimImg(examplar);
    
    double bestScore=999999;
    double bestScan;
    
    int bandRadius = 100;
    double bandCost = 100;
	double nonDiagonalCost = 100;

    double testPortionScale=.7;
    int growRate=1; // anything larger than 1 will exapnd it faster than low score thresholds would keep 
    // it from increasing 
    //cout << "in here" << endl;
    for (double scan = 0; scan<0.7; scan +=0.1)
    {
        
        //cout << "before A" << endl;

        DImage testPortion1 = getTestPortion(similar,img1,word1,testPortionScale,scan);
        //testPortion1.save(("tmp/testPortion_"+to_string(scan)+".png").c_str(),DImage::DFileFormat_png);
        
        //cout << "J" << endl;
        DFeatureVector fv1 = DWordFeatures::extractWordFeatures(testPortion1);
        DFeatureVector fvE = DWordFeatures::extractWordFeatures(examplar);
        fvE.blank[0]=true;
        fvE.blank[fvE.vectLen-1]=true;
        //cout << "A" << endl;

        int path12len;
        int path12[(1+fv1.vectLen)*(1+fvE.vectLen)];
        double cost12 = DDynamicProgramming::findDPAlignment(fv1,fvE,bandRadius,bandCost,nonDiagonalCost,&path12len,path12);
        if (cost12<bestScore)
        {
            bestScore=cost12;
            bestScan=scan;
        }
        
        //cout << "B" << endl;
        if (scan != 0.0)
        {
            DImage testPortion1 = getTestPortion(similar,img1,word1,testPortionScale,-1*scan);
            //testPortion1.save(("tmp/testPortion_-"+to_string(scan)+".png").c_str(),DImage::DFileFormat_png);
            
            //cout << "C" << endl;
            
            DFeatureVector fv1 = DWordFeatures::extractWordFeatures(testPortion1);
            DFeatureVector fvE = DWordFeatures::extractWordFeatures(examplar);
            fvE.blank[0]=true;
            fvE.blank[fvE.vectLen-1]=true;
            
            int path12len;
            int path12[(1+fv1.vectLen)*(1+fvE.vectLen)];
            double cost12 = DDynamicProgramming::findDPAlignment(fv1,fvE,bandRadius,bandCost,nonDiagonalCost,&path12len,path12);
            
            //cout << "D" << endl;
            if (cost12<bestScore)
            {
                bestScore=cost12;
                bestScan=-1*scan;
            }
        }
        
    }
    
    //cout << "best scan: " << bestScan << ", score: " << bestScore << endl;
    
    int start, end;
    DImage testPortion1 = getTestPortion(similar,img1,word1,testPortionScale,bestScan,&start,&end);
    DFeatureVector fv1 = DWordFeatures::extractWordFeatures(testPortion1);
    DFeatureVector fvE = DWordFeatures::extractWordFeatures(examplar);
    fvE.blank[0]=true;
    fvE.blank[fvE.vectLen-1]=true;
    int path12len;
    int path12[(1+fv1.vectLen)*(1+fvE.vectLen)];
    double cost12 = DDynamicProgramming::findDPAlignment(fv1,fvE,bandRadius,bandCost,nonDiagonalCost,&path12len,path12);
    //assert(fabs(cost12-bestScore)<.001);
    DImage warped12 = DDynamicProgramming::piecewiseLinearWarpDImage(testPortion1,examplar.width(),path12len,path12,false);
    
    testPortion1.save("test_output/testPortion1.png",DImage::DFileFormat_png);
    warped12.save("test_output/warped12.png",DImage::DFileFormat_png);
    
    //cout << "in here 2" << endl;
    //Grow
    //TODO
    double scorePrevS=9999; // this will allow both sides to grow independently
    double scorePrevB=9999;
    while(1)
    {
        DImage testPortionS = getTestPortion(similar,img1,start-growRate,end);
        DImage testPortionB = getTestPortion(similar,img1,start,end+growRate);
        
        DFeatureVector fvS = DWordFeatures::extractWordFeatures(testPortionS);
        double scoreS = DDynamicProgramming::findDPAlignment(fvS,fvE,bandRadius,bandCost,nonDiagonalCost);
        
        DFeatureVector fvB = DWordFeatures::extractWordFeatures(testPortionB);
        double scoreB = DDynamicProgramming::findDPAlignment(fvB,fvS,bandRadius,bandCost,nonDiagonalCost);
        
        //do that again
        
        if (scoreS < scoreB)
        {
            if (scoreS <= scorePrevS)
            {
                start-=growRate;
                scorePrevS=scoreS;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (scoreB <= scorePrevB)
            {
                end+=growRate;
                scorePrevB=scoreB;
            }
            else
            {
                break;
            }
        }
    }
    DImage testPortionGrown = getTestPortion(similar,img1,start,end);
    testPortionGrown.save("test_output/testPortionGrown.png",DImage::DFileFormat_png);

    double returnVal = min(scorePrevS,scorePrevB);
    // try and incorporate bestScore her
    returnVal = min(returnVal,bestScore);

    return returnVal;
}
