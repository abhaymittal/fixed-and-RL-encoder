#include<fstream>
#include<iostream>
#include<string>
#include<iomanip>
#include<climits>
#include<cstdlib>

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
    readHeader(imgName);
    if(format=="P5") //binary PGM
      readImageP5(imgName);
    else if(format=="P2") //ascii PGM
      readImageP2(imgName);
    else {
      cout<<"Invalid image format"<<endl;
      exit(10);
    }
  }

  void readImageP5(string imgName) {

    ifstream file(imgName.c_str());

    //ignore the header
    file.ignore(INT_MAX,'\n');
    if(file.peek()==35)  //ignore the comments
      file.ignore(INT_MAX,'\n');
    file.ignore(INT_MAX,'\n');
    file.ignore(INT_MAX,'\n');

    //Read the file

    unsigned char ch;
    pixelVal=new int*[height];
    for(int i=0;i<height;i++) {
      pixelVal[i]=new int[width];
    }
    
    for(int i=0;i<height;i++) {
      for(int j=0;j<width;j++) {
	file>>ch;
	pixelVal[i][j]=static_cast<int>(ch);
      }
    }
    file.close();
  }

  void readImageP2(string imgName) {
        ifstream file(imgName.c_str());

	//ignore the header
	file.ignore(INT_MAX,'\n');
	if(file.peek()==35) //ignore the comments
	  file.ignore(INT_MAX,'\n');
	file.ignore(INT_MAX,'\n');
	file.ignore(INT_MAX,'\n');

	//Read the file
	pixelVal=new int*[height];
	for(int i=0;i<height;i++) {
	  pixelVal[i]=new int[width];
	}

	for(int i=0;i<height;i++) {
	  for(int j=0;j<width;j++) {
	    file>>pixelVal[i][j];
	  }
	}
	file.close();
  }


  void dispSpec() {
    cout<<"The image specifications are as follows:\n";

    cout<< setiosflags(ios::left)<<setw(8)<<"FORMAT"<<" : "<<setw(4)<<format<<endl;
    cout<< setiosflags(ios::left)<<setw(8)<<"WIDTH"<<" : "<<setw(4)<<width<<endl;
    cout<< setiosflags(ios::left)<<setw(8)<<"HEIGHT"<<" : "<<setw(4)<<height<<endl;
    cout<< setiosflags(ios::left)<<setw(8)<<"MAX GRAY"<<" : "<<setw(4)<<maxGray<<endl;
  }

  void dispPixelVal() {
    for(int i=0;i<height;i++) {
      for(int j=0;j<width;j++) {
	cout<<pixelVal[i][j]<<" ";
      }
      cout<<endl;
    }
  }
  
};


int main() {
  string fileName("sample.pgm");
  PGM img;
  img.readImage(fileName);
  img.dispSpec();
  img.dispPixelVal();
  return 0;
}



