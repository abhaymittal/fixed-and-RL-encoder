/**
 *  PROGRAM TO PERFORM RUN LENGTH ENCODING ON A PGM IMAGE
 *  @author Abhay Mittal
 *
 *  ------------------------------------------------------
 *
 *  ERROR CODES:
 *  10 - File Format invalid. Valid file formats => P2 and P5
 *  11 - Max Number of Grayscale levels > 255
 *
 **/

#include<fstream>
#include<iostream>
#include<string>
#include<iomanip>
#include<climits>
#include<cstdlib>
#include<vector>

using namespace std;

class PGM {

private:
  //Data members
  string format;   //Format specification = P5 or P2
  int width;       //Width of pixel array
  int height;      //Height of pixel array
  int maxGray;     //Maximum grayscale level
  unsigned char **pixelVal;  //Pixel array

public:
  //Member functions

  //No-arg constructor
  PGM():format(""),width(0),height(0),maxGray(0),pixelVal(NULL) {
  }

  ~PGM() {
    for(int i=0;i<height;i++)
      delete [] pixelVal[i];
    delete [] pixelVal;
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
    if(maxGray>255) {
      cout<<"Invalid gray levels. Max Supported Gray level = 255"<<endl;
      exit(11);
    }
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
    pixelVal=new unsigned char*[height];
    for(int i=0;i<height;i++) {
      pixelVal[i]=new unsigned char[width];
    }
    
    for(int i=0;i<height;i++) {
      for(int j=0;j<width;j++) {
	file>>ch;
	pixelVal[i][j]=ch;
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
	pixelVal=new unsigned char*[height];
	for(int i=0;i<height;i++) {
	  pixelVal[i]=new unsigned char[width];
	}

	int temp;
	for(int i=0;i<height;i++) {
	  for(int j=0;j<width;j++) {
	    file>>temp;
	    pixelVal[i][j]=temp;
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

  void dispPixelMap() {
    for(int i=0;i<height;i++) {
      for(int j=0;j<width;j++) {
	cout<<static_cast<int>(pixelVal[i][j])<<" ";
      }
      cout<<endl;
    }
  }

  unsigned char* serializePixelVal() {
    unsigned char* serialPixelVal = new unsigned char[height*width];

    for(int i=0;i<height;i++) {
      for(int j=0;j<width;j++){
	serialPixelVal[(i*width)+j]=pixelVal[i][j];
      }
    }

    return serialPixelVal;
  }

  int getWidth() {
    return width;
  }

  int getHeight() {
    return height;
  }
  
};

struct rlePacket {
  int frequency;
  int bit;
};

class RunLengthEncoder {
public:
  void encode(unsigned char* aray,int nElem,vector<struct rlePacket>& packets) {
    
    int wordSize=sizeof(unsigned char);
    int prevBit=-1;
    int currentBit=0;
    int counter=0;
    struct rlePacket* newPacket;

    
    for(int i=0;i<nElem;i++) {
      for(int j=wordSize-1;j>=0;j--) {
	currentBit=aray[i]&(1<<j);
	if(prevBit==currentBit)
	  counter++;
	else {
	  newPacket = new rlePacket();
	  newPacket->frequency=counter;
	  newPacket->bit=prevBit;
	  packets.push_back(*newPacket);
	  counter=1;
	  prevBit=currentBit;
	}
      }
    }

    packets.erase(packets.begin());
  }
};

int main() {
  string fileName("sample.pgm");
  PGM img;
  img.readImage(fileName);
  img.dispSpec();

  //getting the pixel array
  unsigned char *pixelMap;
  pixelMap=img.serializePixelVal();
  int nPxlMap=img.getHeight()*img.getWidth();

  RunLengthEncoder rle;
  vector<rlePacket> packets;
  rle.encode(pixelMap,nPxlMap,packets);
  cout<<"The size of the encoded vector is "<<packets.size()<<endl;
  cout<<"The following are the details of the encoded packets:"<<endl;
  for(vector<rlePacket>::iterator it=packets.begin();it!=packets.end();it++) {
    cout<<setw(5)<<(*it).frequency<<" | "<<(*it).bit<<endl;
  }
  return 0;
}




