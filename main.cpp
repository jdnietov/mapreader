#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include "tmatch.h"
#define Q 77

using namespace std;

struct Location {
  double lat;
  double lon;
};

struct Location grid[Q];

const double ZOOM = 16;
const double DELTALAT = 0.0168704;
const char* IMGNAME = "screenshot.png";

int checkSystem(char* line) {
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
      << ZOOM << "z\' &> /dev/null\n";
}

void fillGrid(int init, double iniLat, double iniLon, int n) {
  grid[init].lat = iniLat;
  grid[init].lon = iniLon;

  ++init;
  for(int j = init; j < init+n-1; j++) {
    grid[j].lat = grid[j-1].lat - DELTALAT;
    grid[j].lon = iniLon;
  }
}

void initGrid() {
  fillGrid(0, 4.8196776, -74.0258801, 9);
  fillGrid(9, 4.7859368, -74.0587103, 13);
  fillGrid(22, 4.7521960, -74.0915405, 17);
  fillGrid(39, 4.7353256, -74.1243707, 16);
  fillGrid(55, 4.7353256, -74.1572009, 13);
  fillGrid(68, 4.6341032, -74.1900311, 5);
  fillGrid(73, 4.6341032, -74.2228613, 4);
}

int main () {
  ofstream bash;

  cout.precision(9);
  initGrid();

  for(int i = 0; i < Q; i++) {
    bash.open ("script.sh");
    bash << "#!/bin/bash\n";
    bash.precision(9);
    writeBash(bash, grid[i].lat, grid[i].lon);
    bash.close();

    checkSystem( (char*)"chmod +x script.sh" );
    checkSystem( (char*)"./script.sh" );

    readAndMatch( (char*) IMGNAME );

    bash.open ("script.sh");
    bash << "#!/bin/bash\n";
    bash << "rm " << IMGNAME << "\n";
    bash.close();

    checkSystem( (char*)"./script.sh" );
  }

  readAndMatch( "test_accidents.png" );

  return 0;
}
