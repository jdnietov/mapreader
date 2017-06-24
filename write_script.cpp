#include <iostream>
#include <fstream>

using namespace std;

int main () {
  ofstream myfile;
  myfile.open ("script.sh");
  myfile << "#!/bin/bash\n";
  myfile << "firefox \'https://www.google.com.co/maps/@4.7512149,-74.0711512,14z/data=!5m1!1e1?hl=es-419\' &\n";
  myfile << "pid=$!\n";
  myfile << "sleep 4\n";
  myfile << "import -window \"$(xdotool getwindowfocus -f)\" $(date +%%F_%%H%%M%%S_%%N).png\n";
  myfile << "kill $pid\n";
  myfile.close();
  return 0;
}
