#include <iostream>
#include <fstream>
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <string>
#define LATTILES 4
#define LONTILES 4

using namespace std;

const double ZOOM = 16;
const double LATDELTA = 0.016892;
const double LONDELTA = -0.032916;

void writeBash(ofstream &file, double lat, double lon) {
  file << "google-chrome \'https://www.google.com.co/maps/@"
    << lat << ","
    << lon << ","
    << ZOOM << "z/data=!5m1!1e1?hl=es-419\' &\n";
  file << "pid=$!\n";
  file << "sleep 4\n";
  file << "wmctrl -a chrome\n";
  file << "xdotool key Escape\nsleep 0.5\n";
  file << "import -window \"$(xdotool getwindowfocus -f)\" $(date +%F_%H%M%S)_" << lat << "_" << lon << ".png\n";
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

      system("chmod +x script.sh");
      system("./script.sh");
      ini_lat -= LATDELTA;
    }
    ini_lon += LONDELTA;
  }

  return 0;
}
