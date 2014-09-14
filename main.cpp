#include<fstream>
#include<iostream>
#include<string>
#include<iomanip>
#include<climits>

using namespace std;

class PGM {

private:
  //Data members
  string format;   //Format specification = P5 or P2
  int width;       //Width of pixel array
  int height;      //Height of pixel array
  int maxGray;     //Maximum grayscale level
  int **pixelVal;  //Pixel array

public:
  //Member functions

  //No-arg constructor
  PGM():format(""),width(0),height(0),maxGray(0),pixelVal(NULL) {
  }

  void readHeader(string imgName) {
    ifstream file(imgName.c_str());

    file>>format;
    file.ignore(INT_MAX,'\n');
    if(file.peek()==35)
      file.ignore(INT_MAX,'\n');

    file>>width;
    file>>height;
    file>>maxGray;

    file.close();
    
  }
  void readImage(string imgName) {
    
  }

  void dispSpec() {
    cout<<"The image specifications are as follows:\n";

    cout<< setiosflags(ios::left)<<setw(8)<<"FORMAT"<<" : "<<setw(4)<<format<<endl;
    cout<< setiosflags(ios::left)<<setw(8)<<"WIDTH"<<" : "<<setw(4)<<width<<endl;
    cout<< setiosflags(ios::left)<<setw(8)<<"HEIGHT"<<" : "<<setw(4)<<height<<endl;
    cout<< setiosflags(ios::left)<<setw(8)<<"MAX GRAY"<<" : "<<setw(4)<<maxGray<<endl;
  }
  
};


int main() {
  string fileName("sample.pgm");
  PGM img;
  img.readHeader(fileName);
  img.dispSpec();
  return 0;
}
