#include <stdio.h>
#include <math.h>

#include <opencv/cv.hpp>
//#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>


double FindShiTomasiScoreAtPoint(cv::Mat image, int nHalfBoxSize, cv::Point2i irCenter) {

  double dXX = 0;
  double dYY = 0;
  double dXY = 0;

  unsigned char imbuf1,imbuf2,imbuf3,imbuf4;

  cv::Point2i irStart = irCenter - cv::Point2i(nHalfBoxSize, nHalfBoxSize);
  cv::Point2i irEnd = irCenter + cv::Point2i(nHalfBoxSize, nHalfBoxSize);

  int r, c;
  for(r = irStart.y; r <= irEnd.y; r++){
    for(c = irStart.x; c <= irEnd.x; c++) {

      imbuf1 = image.at<unsigned char>(r,c+1);
      imbuf2 = image.at<unsigned char>(r,c-1);
      imbuf3 = image.at<unsigned char>(r+1,c);
      imbuf4 = image.at<unsigned char>(r-1,c);

      double dx = imbuf1 - imbuf2;
      double dy = imbuf3 - imbuf4;
      dXX += dx*dx;
      dYY += dy*dy;
      dXY += dx*dy;
    }
  }

  int nPixels = (irEnd.x - irStart.x + 1) * (irEnd.y - irStart.y + 1);
  dXX = dXX / (2.0 * nPixels);
  dYY = dYY / (2.0 * nPixels);
  dXY = dXY / (2.0 * nPixels);

  // Find and return smaller eigenvalue:
  double result;
  result = 0.5 * (dXX + dYY - sqrt( (dXX + dYY) * (dXX + dYY) - 4 * (dXX * dYY - dXY * dXY) ));
  return result;
}




int main()
{
  double result;

  printf("shi-tomashi start\n");

  cv::Mat image(512, 512, CV_8UC1);
  //cv::Mat image;
  image = cv::imread("image/src.bmp",0);
  if(image.empty())
  {
    printf("imread error\n");
    return -1;
  }

  int nHalfBoxSize = 10;
  cv::Point2i irCenter(100,100);

  result = FindShiTomasiScoreAtPoint(image, nHalfBoxSize, irCenter);

  printf("shi-tomasi end\n");
  printf("result = %lf\n",result);

  return 0;
}
