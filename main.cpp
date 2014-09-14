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

  int readHeader(string imgName) {
    ifstream file(imgName.c_str());

    file>>format;
    if(file.peek()==35)
      file.ignore(INT_MAX,'\n');

    file>>width;
    file>>height;
    file>>maxGray;
    
  }
  bool readImage(string imgName) {
    ifstream file;
  }

  void dispSpec() {
    cout<<"The image specifications are as follows:\n";

    cout<<setw(10)<<"FORMAT: "<<setw(4)<<format;
    cout<<setw(10)<<"WIDTH: "<<setw(4)<<width;
    cout<<setw(10)<<"HEIGHT: "<<setw(4)<<height;
    cout<<setw(10)<<"MAX GRAY: "<<setw(4)<<maxGray;
  }
  
};


int main() {
  string fileName("sample.pgm");
  PGM img;
  img.readHeader(fileName);
  img.dispSpec();
  return 0;
}
