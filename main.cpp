#include <iostream>
#include <ctime>
#include <csignal>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <errno.h>

#include "tmatch.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define Q    71
#define ICON 15
#define WINW 1920
#define WINH 1080

using namespace std;
using namespace cv;

/*
 * DELTALAT and DELTALNG values are fixed for a 1920 x 1080 screen
 * running a 100%-zoom Google Chrome window, with a 16z map zoom
 * in Ubuntu 16.04.

 TODO work on a solution to improve accuracy. Current error margins are:
 * lat error: +-0.00001319
 * lng error: +-0.0000742
*/

const int    ZOOM      = 16;
const int    PRECISION = 9;
const int    DLOAD     = 5;         // progress bar module
const double DELTALAT  = 0.022989;  // y-axis, increase upwards
const double DELTALNG  = 0.040962;  // x-axis, decrease left
const char*  IMGNAME   = "screenshot.png";

struct Location {
  double lat;
  double lng;
};

bool isGraphic;
ofstream bash, data;
struct Location grid[Q];

string currentDateTime();
void chksyscall(char* line);
void clean();
void fillCol(int init, double iniLat, double iniLng, int n);
void getCoordinates(int pixelLng, int pixelLat, double lat, double lng, void *res);
void initGrid();
void printPoint(ofstream *ofs, float x, float y);
void signalHandler( int signum );

int main (int argc, char* argv[]) {
  if(argc > 1) {
    for(int i = 1; i < argc; i++) {
      isGraphic = strcmp(argv[i], (char *)"--graphic") == 0;
      if(isGraphic)
        break;
    }
  }

  signal(SIGINT, signalHandler);

  bash.precision(PRECISION);
  data.precision(PRECISION);
  cout.precision(PRECISION);

  data.open("data.log", ios::app);
  data << "*** " << currentDateTime() << " ***" << endl;
  data.close();

  initGrid();

  int load = 0; // local variable for progress bar
  for(int i = 1; i <= Q; i++) {
    // print progress bar
    if(((i+1)*100/Q) >= load) {
      cout << "[";
      for(int j = 0; j < load; j+=DLOAD) cout << "==";
      for(int j = load; j < 100; j+=DLOAD)  cout << "  ";
      cout << "] " << load << "%" << endl;
      load+=DLOAD;
    }

    double lat = grid[i].lat, lng = grid[i].lng;
    struct Location loc;
    vector<Point> points;

    bash.open ("script.sh");
    bash << "#!/bin/bash\n";
    bash << "google-chrome --headless --disable-gpu --screenshot --window-size="
        << WINW << "," << WINH
        << " \'https://www.google.com.co/maps/@" << lat << "," << lng << ","
        << ZOOM << "z/data=!5m1!1e1?hl=es-419\' &> /dev/null\n";
    bash.close();

    chksyscall( (char*)"chmod +x script.sh" );
    chksyscall( (char*)"./script.sh" );

    readAndMatch( (char*) IMGNAME, &points, isGraphic );

    if(points.size() >= 1) {
      data.open("data.log", ios::app);
      for(vector<Point>::const_iterator pos = points.begin(); pos != points.end(); ++pos) {
        getCoordinates(pos->x+ICON/2, pos->y+ICON/2, lat, lng, &loc);
        cout << " |-- " << loc.lat << "," << loc.lng << endl;
        data << loc.lat << "," << loc.lng << endl;
        if((pos+1) == points.end())  data << endl;
      }

      data.close();
      cout << endl;
    }
  }

  return 0;
}

void getCoordinates(int pixelLng, int pixelLat, double lat, double lng, void *res) {
  // image's origin coordinates
  double oLat = lat + DELTALAT/2;
  double oLng = lng - DELTALNG/2;

  double resLat = oLat - DELTALAT*((double) pixelLat/WINH);
  double resLng = oLng + DELTALNG*((double) pixelLng/WINW);

  struct Location * locRes = (struct Location *) res;
  locRes->lat = resLat;
  locRes->lng = resLng;
}

void chksyscall(char* line) {
  int status = system(line);
  if (status < 0) {
    cout << "Error: " << strerror(errno) << '\n';
    clean();
    exit(-1);
  }
  else
  {
    if (WIFEXITED(status)) {
      // cout << "Program returned normally, exit code " << WEXITSTATUS(status) << '\n';
    } else {
      clean();
      cout << "Program exited abnormally\n";
      exit(-1);
    }
  }
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
  fillCol(0, 4.8196776, -74.0258801, 7);
  fillCol(7, 4.7859368, -74.0668421, 11);
  fillCol(18, 4.7521960, -74.1078041, 15);
  fillCol(33, 4.7353256, -74.1487661, 16);
  fillCol(49, 4.7353256, -74.1897281, 13);
  fillCol(62, 4.6341032, -74.2306901, 5);
  fillCol(67, 4.6341032, -74.2716521, 4);
}

string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n";
   clean();
   exit(signum);

}

void clean() {
  data.open("data.log", ios::app);
  data << endl;
  data.close();

  bash.close();
}

void printPoint(ofstream *ofs, float x, float y) {
  *ofs << x << y;
}
