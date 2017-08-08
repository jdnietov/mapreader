#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "tmatch.h"

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result;
Point maxPoint;
char* image_window = "Source Image";
char* result_window = "Result window";

string template_name = "accident.png";

int match_method = 1;

/// Function Headers
void MatchingMethod( int, void* );
void * readAndMatch( char* );

/** @function main */
void * readAndMatch( char* imgname )
{
  /// Load image and template
  img = imread( imgname, 1 );
  templ = imread( template_name, 1 );

  /// Create windows
  namedWindow( image_window, CV_WINDOW_AUTOSIZE );
  namedWindow( result_window, CV_WINDOW_AUTOSIZE );

  MatchingMethod( 0, 0 );

  waitKey(0);
  return &maxPoint;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
 {
   /// Source image to display
   Mat img_display;
   img.copyTo( img_display );

   /// Create the result matrix
   int result_cols =  img.cols - templ.cols + 1;
   int result_rows = img.rows - templ.rows + 1;

   result.create( result_cols, result_rows, CV_32FC1 );

   /// Do the Matching and Normalize
   matchTemplate( img, templ, result, match_method );
   normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

   /// Localizing the best match with minMaxLoc
   Point minLoc; Point maxLoc;
   Point matchLoc;
   double minVal; double maxVal;

   minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
   cout << "minVal: " << minVal << ", maxVal: " << maxVal << endl;

   /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
   if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
    else
    { matchLoc = maxLoc; }

  /// Show me what you got
  maxPoint.x = matchLoc.x + templ.cols;
  maxPoint.y = matchLoc.y + templ.rows;

  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
   rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

  imshow( image_window, img_display );
   imshow( result_window, result );

   return;
 }
