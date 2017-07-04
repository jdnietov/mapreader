#include <iostream>
#include <fstream>
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <string>
#include <errno.h>
#include "tmatch.h"
#define LATTILES 1
#define LONTILES 2

using namespace std;

const double ZOOM = 16;
const double LATDELTA = 0.016892;
const double LONDELTA = -0.032916;

const char* IMGNAME = "img.png";

int checkSystem(char* line) {
  int ret_val = system(line);
  if (ret_val == 0 && errno == 0)
  {
    cout << line << ": sucess" << endl;
    return 1;
  }
  cout << line << ": error" << endl;
  return 0;
}

void writeBash(ofstream &file, double lat, double lon) {
  // TODO change google-chrome bash to headless
  file << "google-chrome \'https://www.google.com.co/maps/@"
    << lat << ","
    << lon << ","
    << ZOOM << "z/data=!5m1!1e1?hl=es-419\' &\n";
  file << "pid=$!\n";
  file << "sleep 4\n";
  file << "wmctrl -a chrome\n";
  file << "xdotool key Escape\nsleep 0.5\n";
  // file << "import -window \"$(xdotool getwindowfocus -f)\" $(date +%F_%H%M%S)_" << lat << "_" << lon << ".png\n";
  file << "import -window \"$(xdotool getwindowfocus -f)\" " << IMGNAME << "\n";
  file << "xdotool key Ctrl+w\n";
}

int main () {
  cout << "executing"<< endl;
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

  const char *filename = "test_accidents.png";
  readAndMatch( (char*) filename );

  return 0;
}
