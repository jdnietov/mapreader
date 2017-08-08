#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include "tmatch.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#define Q 77

using namespace std;
using namespace cv;

const double ZOOM = 16;
const double DELTALAT = 0.0168704;
const double DELTALON = 0.0328303;
const char* IMGNAME = "test_accidents.png";

struct Location {
  double lat;
  double lon;
};

struct Location grid[Q];

int chkSysCall(char* line) {
  int ret_val = system(line);
  if (ret_val == 0 && errno == 0)
  {
    return 1;
  }
  cout << line << ": error running " << line << endl;
  return 0;
}

void writeBash(ofstream &file, double lat, double lon) {
  file << "google-chrome --headless --disable-gpu --screenshot --window-size=1920,1080 "
    << "\'https://www.google.com.co/maps/@"
      << lat << ","
      << lon << ","
      << ZOOM << "z/data=!5m1!1e1?hl=es-419\' &> /dev/null\n";
}

void fillCol(int init, double iniLat, double iniLon, int n) {
  grid[init].lat = iniLat;
  grid[init].lon = iniLon;

  ++init;
  for(int j = init; j < init+n-1; j++) {
    grid[j].lat = grid[j-1].lat - DELTALAT;
    grid[j].lon = iniLon;
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

  initGrid();

  // for(int i = 0; i < Q; i++) {
  //   bash.open ("script.sh");
  //   bash << "#!/bin/bash\n";
  //   bash.precision(9);
  //   writeBash(bash, grid[i].lat, grid[i].lon);
  //   bash.close();

    // chkSysCall( (char*)"chmod +x script.sh" );
    // chkSysCall( (char*)"./script.sh" );

    Point * p_point;
    p_point = (Point*) readAndMatch( (char*) IMGNAME );
    cout << "[" << p_point->x << ", " << p_point->y << "]" << endl;

    // TODO get coordinates from point in image

    // bash.open ("script.sh");
    // bash << "#!/bin/bash\n";
    // bash << "rm " << IMGNAME << "\n";
    // bash.close();
    //
    // chkSysCall( (char*)"./script.sh" );
  // }

  return 0;
}
