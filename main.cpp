#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include "tmatch.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#define Q 77
#define WINW 1920
#define WINH 1080

using namespace std;
using namespace cv;

const double ZOOM = 16;
const double DELTALAT = 0.0168704;  // y-axis, increase upwards
const double DELTALNG = 0.0328303;  // x-axis, decrease left
const char* IMGNAME = "screenshot.png";

struct Location {
  int x;
  int y;
  double lat;
  double lng;
};

struct Location grid[Q];

void printPoint(float x, float y) {
  cout << "[" << x << "," << y << "]";
}

void getCoordinates(int pixelLng, int pixelLat, double lat, double lng) {
  // image's origin coordinates
  double oLat = lat + DELTALAT/2;
  double oLng = lng - DELTALNG/2;

  double resLat = oLat - DELTALAT*((double) pixelLat/WINH);
  double resLng = oLng + DELTALNG*((double) pixelLng/WINW);

  // cout << (double) pixelLat/WINH << ", " << pixelLng/WINW << endl;
  cout << "P: " << pixelLng << ", " << pixelLat << endl;
  cout << "C: "; printPoint(resLat, resLng); cout << endl << endl;
}


int chkSysCall(char* line) {
  int ret_val = system(line);
  if (ret_val == 0 && errno == 0)
  {
    return 1;
  }
  cout << line << ": error running " << line << endl;
  return 0;
}

void writeBash(ofstream &file, double lat, double lng) {
  file << "google-chrome --headless --disable-gpu --screenshot --window-size="
      << WINW << "," << WINH
      << " \'https://www.google.com.co/maps/@" << lat << "," << lng << ","
      << ZOOM << "z/data=!5m1!1e1?hl=es-419\' &> /dev/null\n";
}

void fillCol(int init, double iniLat, double iniLng, int n) {
  grid[init].lat = iniLat;
  grid[init].lng = iniLng;

  ++init;
  for(int j = init; j < init+n-1; j++) {
    grid[j].lat = grid[j-1].lat - DELTALAT;
    grid[j].lng = iniLng;
  }
}

void initGrid() {
  fillCol(0, 4.8196776, -74.0258801, 9);
  fillCol(9, 4.7859368, -74.0587103, 13);
  fillCol(22, 4.7521960, -74.0915405, 17);
  fillCol(39, 4.7353256, -74.1243707, 16);
  fillCol(55, 4.7353256, -74.1572009, 13);
  fillCol(68, 4.6341032, -74.1900311, 5);
  fillCol(73, 4.6341032, -74.2228613, 4);
}

int main () {
  ofstream bash;

  bash.precision(9);
  cout.precision(9);

  initGrid();

  for(int i = 0; i < Q; i++) {
    double lat = grid[i].lat, lng = grid[i].lng;
    cout << "---";
    printPoint(lat, lng);
    cout << "---" << endl;

    bash.open ("script.sh");
    bash << "#!/bin/bash\n";
    writeBash(bash, lat, lng);
    bash.close();

    chkSysCall( (char*)"chmod +x script.sh" );
    chkSysCall( (char*)"./script.sh" );

    Point * p_point;
    p_point = (Point*) readAndMatch( (char*) IMGNAME );
    getCoordinates(p_point->x, p_point->y, lat, lng);

    bash.open ("script.sh");
    bash << "#!/bin/bash\n";
    bash << "rm " << IMGNAME << "\n";
    bash.close();

    chkSysCall( (char*)"./script.sh" );
  }

  return 0;
}
