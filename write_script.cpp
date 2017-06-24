#include <iostream>
#include <fstream>
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <string>

using namespace std;

int main () {
  const string mapZoom = "16";
  double lat = 4.7512149;
  double lon = -74.0711512;
  ofstream bash;

  bash.open ("script.sh");
  bash.precision(9);
  bash << "#!/bin/bash\n";
  bash << "google-chrome \'https://www.google.com.co/maps/@"
    << lat << ","
    << lon << ","
    << mapZoom << "z/data=!5m1!1e1?hl=es-419\' &\n";
  bash << "pid=$!\n";
  bash << "sleep 4\n";
  bash << "wmctrl -a chrome\n";
  bash << "xdotool key Escape\nsleep 0.5\n";
  bash << "import -window \"$(xdotool getwindowfocus -f)\" $(date +%F_%H%M%S_%N).png\n";
  bash << "xdotool key Ctrl+w";
  bash.close();

  system("chmod +x script.sh");
  system("./script.sh");
  return 0;
}
