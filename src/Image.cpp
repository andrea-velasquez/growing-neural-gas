#ifndef IMAGE_CPP
#define IMAGE_CPP

#include "Image.h"
#include <cmath>

const int Image::sobelGx[3][3] ={{1, 0, -1},
                                {2, 0, -2},
                                {1, 0, -1}};
const int Image::sobelGy[3][3] ={{1, 2, 1},
                                {0, 0, 0},
                                {-1, -2, -1}};

int Image::sumProduct(const int G[3][3], cv::Mat A)
{
  int result = 0;
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
    result += G[i][j]*A.at<uchar>(i, j);
  return result;
}

void Image::grayscale(cv::Mat &img, cv::Mat &imgGray)
{
  imgGray = cv::Mat::zeros(img.size(), CV_8UC1);
  for (int i=0; i<img.rows; ++i)
    for (int j=0; j<img.cols; ++j)
    {
      cv::Vec3b & color = img.at<cv::Vec3b>(i,j);
      imgGray.at<uchar>(i, j) = (color[0]+color[1]+color[2])/3;
    }
}

void Image::sobel(cv::Mat &img, cv::Mat &imgSobel)
{
  imgSobel = cv::Mat::zeros(img.size(), CV_8UC1);

  // Get grayscale image
  cv::Mat imgGray;
  Image::grayscale(img, imgGray);

  // Iterate through matrix
  for (int i=0; i<img.rows-2; ++i)
    for (int j=0; j<img.cols-2; ++j)
    {
      int gx = sumProduct(sobelGx, imgGray(cv::Rect(j, i, 3, 3)));
      int gy = sumProduct(sobelGy, imgGray(cv::Rect(j, i, 3, 3)));
      
      // Euclidean distance
      imgSobel.at<uchar>(i, j) = Image::normalize(std::sqrt(gx*gx + gy*gy));
    }
}

void Image::getAllPixelsOn(cv::Mat &imgSobel, std::vector<cv::Point>& pixels)
{
  for (int i=0; i<imgSobel.rows; ++i) // y
    for (int j=0; j<imgSobel.cols; ++j) // x
    {
      int px = imgSobel.at<cv::Vec3b>(i, j)[0];
      if (Image::isPixelOn(px))
        pixels.push_back(cv::Point(i, j));
    }
}

#endif