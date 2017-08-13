#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "tmatch.h"

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat img_gray; Mat templ; Mat result;
Point maxPoint;
char* image_window = "Source Image";
// char* result_window = "Result window";

string template_name = "accident.png";

int match_method = CV_TM_CCOEFF_NORMED;
float threshold_min = 0.05; float threshold_max = 0.95;

/// Function Headers
void MatchingMethod( int, void* );
void * readAndMatch( char*, void * );

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

bool doesMatch(float f) {
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED ) {
    if(f <= threshold_min)
      return true;
  } else {
    if(f >= threshold_max)
      return true;
  }
  return false;
}

void * readAndMatch( char* imgname )
{
  /// Load image and template
  img = imread( imgname );
  templ = imread( template_name, 1 );
  cvtColor(img, img_gray, COLOR_BGR2GRAY);

  /// Create windows
  namedWindow( image_window, CV_WINDOW_AUTOSIZE );
  // namedWindow( result_window, CV_WINDOW_AUTOSIZE );

  MatchingMethod( 0, 0 );

  waitKey(0);

  return &maxPoint;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* ) {
  /// Source image to display
  Mat img_display;
  img.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  /// Do the Matching and Normalize
  matchTemplate( img, templ, result, match_method );
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  float max = 0;
  for(int i = 0; i < result_rows; i++) {
    for(int j = 0; j < result_cols; j++) {
      float res = result.at<float>(i,j);
        if(doesMatch(res)) {
          cout << "(" << j << ", " << i << "): " << res << endl;
          rectangle( img_display,
            Point(j, i),
            Point(j + templ.cols , i + templ.rows ),
            Scalar::all(0), 2, 8, 0
         );
       }
     }
   }

  imshow( image_window, img_display );

  return;
}
