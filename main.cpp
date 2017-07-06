#include <iostream>
#include <fstream>
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <string>
#include <errno.h>
#include "tmatch.h"
#define LATTILES 1
#define LONTILES 1

using namespace std;

const double ZOOM = 16;
const double LATDELTA = 0.016892;
const double LONDELTA = -0.032916;

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
  // TODO check --headless warnings
  file << "google-chrome --headless --disable-gpu --screenshot --window-size=1920,1080 "
    << "\'https://www.google.com.co/maps/@"
      << lat << ","
      << lon << ","
      << ZOOM << "z\' &> /dev/null\n";
}

int main () {
  double ini_lat = 4.8166031;
  double ini_lon = -74.0345367;
  ofstream bash;

  cout.precision(9);
  for(int i = 0; i < LONTILES; i++) {
    for(int j = 0; j < LATTILES; j++) {
      bash.open ("script.sh");
      bash << "#!/bin/bash\n";
      bash.precision(9);
      writeBash(bash, ini_lat, ini_lon);
      bash.close();

      // run template matching

      checkSystem( (char*)"chmod +x script.sh" );
      checkSystem( (char*)"./script.sh" );

      readAndMatch( (char*) IMGNAME );

      // delete img
      bash.open ("script.sh");
      bash << "#!/bin/bash\n";
      bash << "rm " << IMGNAME << "\n";
      bash.close();

      checkSystem( (char*)"./script.sh" );

      ini_lat -= LATDELTA;
    }
    ini_lon += LONDELTA;
  }

  readAndMatch( "test_accidents.png" );

  return 0;
}
