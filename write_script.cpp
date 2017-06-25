#include <iostream>
#include <fstream>
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <string>
#define LATTILES 4
#define LONTILES 4

using namespace std;

const double DELTALAT = 0.016892;
const double DELTALON = -0.032916;

int main () {
  const string mapZoom = "16";
  double ini_lat = 4.8166031;
  double ini_lon = -74.0345367;
  ofstream bash;

  bash.open ("script.sh");
  bash.precision(9);
  bash << "#!/bin/bash\n";

  cout.precision(9);
  double lat = ini_lat;
  double lon = ini_lon;
  for(int i = 0; i < LONTILES; i++) {
    for(int j = 0; j < LATTILES; j++) {
      cout << lat << ", " << lon << endl;

      bash << "google-chrome \'https://www.google.com.co/maps/@"
        << lat << ","
        << lon << ","
        << mapZoom << "z/data=!5m1!1e1?hl=es-419\' &\n";
      bash << "pid=$!\n";
      bash << "sleep 4\n";
      bash << "wmctrl -a chrome\n";
      bash << "xdotool key Escape\nsleep 0.5\n";
      bash << "import -window \"$(xdotool getwindowfocus -f)\" $(date +%F_%H%M%S)_" << lat << "_" << lon << ".png\n";
      bash << "xdotool key Ctrl+w\n";

      lat -= DELTALAT;
    }
    lon += DELTALON;
  }
  bash.close();

  system("chmod +x script.sh");
  system("./script.sh");
  return 0;
}
