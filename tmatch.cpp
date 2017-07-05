#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "tmatch.h"

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result;
char* image_window = "Source Image";
// char* result_window = "Result window";

string template_name = "accident.png";

int match_method = 4;
float thres = 0.9;

/// Function Headers
void MatchingMethod( int, void* );
int readAndMatch( char* );

/** @function main */
int readAndMatch( char* imgname )
{
  /// Load image and template
  img = imread( imgname, 1 );
  templ = imread( template_name, 1 );

  /// Create windows
  namedWindow( image_window, CV_WINDOW_AUTOSIZE );
  // namedWindow( result_window, CV_WINDOW_AUTOSIZE );

  MatchingMethod( 1, 0 );

  waitKey(0);
  return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
  Mat img_display;
  img.copyTo( img_display );

  Mat img_gray;
  cvtColor(img, img_gray, COLOR_BGR2GRAY);

  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );

  matchTemplate( img, templ, result, match_method );
  // normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  for(int k=1;k<=5;k++)
  {
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    result.at<float>(minLoc.x,minLoc.y)=1.0;
    result.at<float>(maxLoc.x,maxLoc.y)=0.0;

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

  /// Show me what you got
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  // rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  }

  imshow( image_window, img_display );
  // imshow( result_window, result );

  return;
}
