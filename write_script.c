#include <stdio.h>
#include <stdlib.h>
#define SCRIPTNAME "script.sh"

int main() {

  FILE *f = fopen(SCRIPTNAME, "w");
  if (f == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  // declare bash file
  fprintf(f, "#!/bin/bash\n");

  // execute browser with given url
  fprintf(f, "firefox \'https://www.google.com.co/maps/@4.7512149,-74.0711512,14z/data=!5m1!1e1?hl=es-419\' &\n");

  // save browser pid in variable
  fprintf(f, "pid=$!\n");

  // sleep process for certain time
  fprintf(f, "sleep 4\n");

  // focus window containing browser
  fprintf(f, "wmctrl -a Firefox\n");

  // take screenshot from browser window
  fprintf(f, "import -window \"$(xdotool getwindowfocus -f)\" $(date +%%F_%%H%%M%%S_%%N).png\n");

  // kill browser process
  fprintf(f, "kill $pid\n");

  fclose(f);
}
