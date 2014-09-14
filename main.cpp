#include<fstream>
#include<iostream>
#include<string>

using namespace std;

class PGM {
  string format;
  int width;
  int height;
  int maxGray;
  int **pixelVal;
};
