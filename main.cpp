/**
 *  PROGRAM TO PERFORM RUN LENGTH ENCODING AND FIXED LENGTH ENCODING ON A PGM IMAGE
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
#include<cmath>
#include<algorithm>


using namespace std;

/**
 * @class PGM
 * @brief Class representing a PGM image
 *
 * @author Abhay Mittal
 */
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

  //destructor
  ~PGM() {
    for(int i=0;i<height;i++)
      delete [] pixelVal[i];
    delete [] pixelVal;
  }

  //Function to read the header of PGM file
  
  void readHeader(string imgName) {
    ifstream file(imgName.c_str());

    file>>format;
    file.ignore(INT_MAX,'\n');

    //ignore the comments
    while(file.peek()==35)
      file.ignore(INT_MAX,'\n');

    file>>width;
    file>>height;
    file>>maxGray;

    file.close();
    
  }

  //function to read the complete image
  void readImage(string imgName) {
    readHeader(imgName);
    //It should be a 8 bit grayscale image
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

  //Function to read binary PGM image (Format = P5)
  void readImageP5(string imgName) {

    ifstream file(imgName.c_str());

    //ignore the header
    file.ignore(INT_MAX,'\n');
    while(file.peek()==35)  //ignore the comments
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

  //Function to read ASCII PGM image (Format = P2)
  void readImageP2(string imgName) {
    ifstream file(imgName.c_str());

    //ignore the header
    file.ignore(INT_MAX,'\n');
    while(file.peek()==35) //ignore the comments
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

  //Function to display the PGM image specifications
  void dispSpec() {
    cout<<"The image specifications are as follows:\n";

    cout<< setiosflags(ios::left)<<setw(8)<<"FORMAT"<<" : "<<setw(4)<<format<<endl;
    cout<< setiosflags(ios::left)<<setw(8)<<"WIDTH"<<" : "<<setw(4)<<width<<endl;
    cout<< setiosflags(ios::left)<<setw(8)<<"HEIGHT"<<" : "<<setw(4)<<height<<endl;
    cout<< setiosflags(ios::left)<<setw(8)<<"MAX GRAY"<<" : "<<setw(4)<<maxGray<<endl;
  }

  //Function to display the Pixel values
  void dispPixelMap() {
    for(int i=0;i<height;i++) {
      for(int j=0;j<width;j++) {
	cout<<static_cast<int>(pixelVal[i][j])<<" ";
      }
      cout<<endl;
    }
  }

  //Function to convert the 2D pixel array into a 1d array
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

//A structure representing the output packet format of the Run Length Encoder
struct rlePacket {
  int frequency; //Frequency of the bit
  int bit;
};

//Class representing the Run Length Encoder
class RunLengthEncoder {
public:
  //Function to encode a character array of n elements using Run length encoding
  void encode(unsigned char* aray,int nElem,vector<struct rlePacket>& packets) {

    int wordSize=sizeof(unsigned char)*8; //number of bits
    int prevBit=-1;
    int currentBit=0;
    int counter=0;
    struct rlePacket* newPacket;

    
    for(int i=0;i<nElem;i++) {
      for(int j=wordSize-1;j>=0;j--) {
	currentBit=((aray[i]&(1<<j))!=0)?1:0; //Determine the value of the current bit
	if(prevBit==currentBit) //if bit unchanged
	  counter++;
	else {
	  //Store information
	  newPacket = new rlePacket();
	  newPacket->frequency=counter;
	  newPacket->bit=prevBit;
	  packets.push_back(*newPacket);
	  counter=1;
	  prevBit=currentBit;
	}
      }
    }

    //Remove the first element because its dummy and has bit value = -1
    packets.erase(packets.begin());
  }
};

//Structure representing the output format of Fixed Length coder
struct huffPacket {
  int num; //Number
  int freq; //Frequency of number
};

//Class representing the coding scheme
class codeScheme {
  int nItems;  //Number of elements coded
  int nbits;   //Number of bits required
  int **codeTable; //A table consisting of the coded values and their codes

public:
  //Default constructor
  codeScheme():nItems(0),nbits(0),codeTable(NULL) {}

  //Constructor
  codeScheme(int _nItems,int _nbits):nItems(_nItems),nbits(_nbits) {
    codeTable=new int*[nItems];
    for(int i=0;i<nItems;i++) {
      codeTable[i]=new int[2];
    }
  }

  //Destructor
  ~codeScheme() {
    for(int i=0;i<nItems;i++) {
      delete [] codeTable[i];
    }
    delete [] codeTable;
  }
  
  //Function to initialize the values
  void initialize(int _nItems,int _nbits) {
    nItems=_nItems;
    nbits=_nbits;
    codeTable=new int*[nItems];
    for(int i=0;i<nItems;i++) {
      codeTable[i]=new int[2];
    }
  }

  //Function to set the number of bits
  void setNBits(int _nbits) {
    nbits=_nbits;
  }
  //Function to get number of bits
  int getNBits() {
    return nbits;
  }

  //Function to set the code at a particular index
  void setCode(int index, int value, int code) {
    codeTable[index][0]=value;
    codeTable[index][1]=code;
  }

  //Function to get code for a particular value
  int getCode(int value) {
    for(int i=0;i<nItems;i++) {
      if(codeTable[i][0]==value)
	return codeTable[i][1];
    }
    return -1;
  }

  //Function to display the coding scheme
  void dispCodeScheme() {
    int bit;
    cout<<"CODE SCHEME"<<endl;

    for(int i=0;i<nItems;i++) {

      cout<<setw(5)<<codeTable[i][0]<<" | ";
      for(int j=nbits-1;j>=0;j--) {
	bit=((codeTable[i][1]&(1<<j))!=0)?1:0;
	cout<<bit;
      }
      cout<<endl;
    }
  }
};


//Function to compare two huffPacket structures. Returns true if the frequency of h1 > frequency of h2
bool compareHuffPacket(const huffPacket h1,const huffPacket h2) {
  if(h1.freq>h2.freq)
    return true;
  return false;
}

//Class Representing the Fixed length encoder
class FixedLengthCoder {
  
public:
  //Function to determine the frequencies of frequencies determined by the Run length encoder
  void groupFrequency(vector<rlePacket>& packets,vector<huffPacket>& hfPackets) {
    vector<huffPacket>::iterator itr;
    struct huffPacket* newPacket;

    //Iterate over all the Run length packets
    for(vector<rlePacket>::iterator it=packets.begin();it!=packets.end();it++) {
      //Check if element already exists in the output vector
      findHuffPacket(hfPackets,(*it).frequency,itr);
      if(itr==hfPackets.end()) { //Not found
	newPacket=new huffPacket();
	newPacket->num=(*it).frequency;
	newPacket->freq=1;
	hfPackets.push_back(*newPacket);
      }
      else { //Found
	(*itr).freq=(*itr).freq+1;
      }
    }
  }

  //Function to find a packet Fixed length packet with a particular value. Returns the end() element of vector if element is not found
  void findHuffPacket(vector<huffPacket>& hfPackets,int item,    vector<huffPacket>::iterator& it) {

    for(it=hfPackets.begin();it!=hfPackets.end();it++) {
      if((*it).num==item)
	break;
    }
  }


  //Function to generate the fixed code
  void generateFixedCode(vector<rlePacket>& packets, codeScheme& codeSch) {

    vector<huffPacket> hfPackets;

    //Determine the frequencies of frequencies determined by RLE
    groupFrequency(packets,hfPackets);
    cout<<"The size of the grouped vector is "<<hfPackets.size()<<endl;

    //Sort packets based on their frequencies using compareHuffPacket function
    sort(hfPackets.begin(),hfPackets.end(),compareHuffPacket);
    
    cout<<"The following are the details of the sorted grouped packets:"<<endl;
    for(vector<huffPacket>::iterator it=hfPackets.begin();it!=hfPackets.end();it++) {
      cout<<setw(5)<<(*it).num<<" | "<<(*it).freq<<endl;
    }
    //determine the number of bits required to generate fixed length code
    float temp=log2(hfPackets.size());
    int nbits=0;
    if(floor(temp)==temp)
      nbits=static_cast<int>(temp);
    else
      nbits=static_cast<int>(temp)+1;

    //Save the coding scheme
    codeSch.initialize(hfPackets.size(),nbits);
    for(int i=0;i<hfPackets.size();i++) {
      codeSch.setCode(i,hfPackets[i].num,i);
    }

    codeSch.dispCodeScheme();

    //Generate the output 
    generateFile(packets,codeSch,"output.out");
    
  }

  //Function to generate the output file containing the fixed length coded version of the run length code
  void generateFile(vector<rlePacket>& packets, codeScheme& codeSch, string filename) {

    //Open the output file in binary mode
    ofstream ofile(filename.c_str(),ios::binary);

    //Data is saved byte by byte
    unsigned char ch=0;
    int nbits, code;
    int curBit=7,bit;
    nbits=codeSch.getNBits();

    //Iterate over all the run length packets
    for(vector<rlePacket>::iterator it=packets.begin();it!=packets.end();it++) {
      //find the code for a particular frequency
      code=codeSch.getCode((*it).frequency);
      
      for(int i=nbits-1;i>=0;i--) {
	bit= ((code&(1<<i))!=0)?1:0;
	ch=ch|(bit<<curBit);
	curBit--;
	if(curBit==0) {
	  ofile<<ch;
	  ch=0;
	  curBit=7;
	}
      }
    }

    //close the output file
    ofile.close();
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
  /* cout<<"The size of the encoded vector is "<<packets.size()<<endl;
  cout<<"The following are the details of the encoded packets:"<<endl;
  for(vector<rlePacket>::iterator it=packets.begin();it!=packets.end();it++) {
    cout<<setw(5)<<(*it).frequency<<" | "<<(*it).bit<<endl;
    }*/


  cout<<"----------------------------------------"<<endl;


  FixedLengthCoder fcoder;
  codeScheme codeSch;
  fcoder.generateFixedCode(packets,codeSch);

  return 0;
}
