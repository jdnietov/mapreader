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
  double lat;
  double lng;
};

ofstream bash, data;
struct Location grid[Q];

void printPoint(float x, float y);
void getCoordinates(int pixelLng, int pixelLat, double lat, double lng, void *res);
void chksyscall(char* line);
void fillCol(int init, double iniLat, double iniLng, int n);
void initGrid();
string currentDateTime();
void signalHandler( int signum );
void clean();

int main () {
  signal(SIGINT, signalHandler);

  bash.precision(9);
  data.precision(9);
  cout.precision(9);

  data.open("data.log", ios::app);
  data << "*** " << currentDateTime() << " ***" << endl << endl;
  data.close();

  initGrid();

  for(int i = 0; i < Q; i++) {
    double lat = grid[i].lat, lng = grid[i].lng;

    bash.open ("script.sh");
    bash << "#!/bin/bash\n";
    bash << "google-chrome --headless --disable-gpu --screenshot --window-size="
        << WINW << "," << WINH
        << " \'https://www.google.com.co/maps/@" << lat << "," << lng << ","
        << ZOOM << "z/data=!5m1!1e1?hl=es-419\' &> /dev/null\n";
    bash.close();

    chksyscall( (char*)"chmod +x script.sh" );
    chksyscall( (char*)"./script.sh" );

    Point * p_point;
    p_point = (Point*) readAndMatch( (char*) IMGNAME );

    struct Location loc;
    getCoordinates(p_point->x, p_point->y, lat, lng, &loc);

    data.open("data.log", ios::app);
    data << "(" << lat << ", " << lng << ")" << endl;
    data << "-- [" << loc.lat << ", " << loc.lng << "]" << endl;
    data.close();

    bash.open ("script.sh");
    bash << "#!/bin/bash\n";
    bash << "rm " << IMGNAME << "\n";
    bash.close();

    chksyscall( (char*)"./script.sh" );
  }

  return 0;
}

void printPoint(float x, float y) {
  cout << "[" << x << "," << y << "]";
}

void getCoordinates(int pixelLng, int pixelLat, double lat, double lng, void *res) {
  // image's origin coordinates
  double oLat = lat + DELTALAT/2;
  double oLng = lng - DELTALNG/2;

  double resLat = oLat - DELTALAT*((double) pixelLat/WINH);
  double resLng = oLng + DELTALNG*((double) pixelLng/WINW);

  cout << "P: " << pixelLng << ", " << pixelLat << endl;
  cout << "C: "; printPoint(resLat, resLng); cout << endl << endl;

  struct Location * locRes = (struct Location *) res;
  locRes->lat = resLat;
  locRes->lng = resLng;
}

void chksyscall(char* line) {
  int status = system(line);
  if (status < 0) {
    std::cout << "Error: " << strerror(errno) << '\n';
    clean();
    exit(-1);
  }
  else
  {
    if (WIFEXITED(status)) {
      std::cout << "Program returned normally, exit code " << WEXITSTATUS(status) << '\n';
    } else {
      clean();
      std::cout << "Program exited abnormally\n";
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
  fillCol(0, 4.8196776, -74.0258801, 9);
  fillCol(9, 4.7859368, -74.0587103, 13);
  fillCol(22, 4.7521960, -74.0915405, 17);
  fillCol(39, 4.7353256, -74.1243707, 16);
  fillCol(55, 4.7353256, -74.1572009, 13);
  fillCol(68, 4.6341032, -74.1900311, 5);
  fillCol(73, 4.6341032, -74.2228613, 4);
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
  bash.close();
  data.close();
}
