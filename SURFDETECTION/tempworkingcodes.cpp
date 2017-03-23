

/*
--------------------------------------------------------------------------------------------------------
Working feature detection code working 1

--------------------------------------------------------------------------------------------------------


#include <stdio.h>
#include <iostream>
#include <opencv2\core.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\xfeatures2d.hpp>
#include <opencv2\highgui.hpp>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;


int main(int argc, char ** argv)
{
Mat image1, image2;

image1 = imread("img1.jpg");
int minhessian = 400;

VideoCapture cap(0);
vector<KeyPoint> Keypoints_1, Keypoints_2;
Ptr<SURF> detector = SURF::create(minhessian);
detector->detect(image1, Keypoints_1);

Mat keypointimage1;

drawKeypoints(image1, Keypoints_1, keypointimage1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);



if (!cap.isOpened())
{
cout << "error reading from video" << endl;
waitKey(0);
}

while (1)
{
cap >> image2;

if (!image1.data || !image2.data)
{
cout << "error reading both images" << endl;
waitKey(0);
}


detector->detect(image2, Keypoints_2);

Mat keypointimage2;


drawKeypoints(image2, Keypoints_2, keypointimage2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

imshow("first keypoint", keypointimage1);
imshow("second keypoint", keypointimage2);

waitKey(50);

}

}



--------------------------------------------------------------------------------------------------------
Working feature detection code working 2

--------------------------------------------------------------------------------------------------------


#include <stdio.h>
#include <iostream>
#include <opencv2\core.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\xfeatures2d.hpp>
#include <opencv2\highgui.hpp>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;


int main(int argc, char ** argv)
{
Mat image1, image2,descriptor_1,descriptor_2;

image1 = imread("img1.jpg");
int minhessian = 40;


Ptr<SURF> detector = SURF::create();
detector->setHessianThreshold(minhessian);

std::vector<KeyPoint> Keypoints_1, Keypoints_2;

detector->detectAndCompute(image1, Mat(), Keypoints_1, descriptor_1);


VideoCapture cap(0);
if (!cap.isOpened())
{
cout << "error reading from video" << endl;
waitKey(0);
}

while (1)
{
cap >> image2;

if (!image1.data || !image2.data)
{
cout << "error reading both images" << endl;
waitKey(0);
}


detector->setHessianThreshold(minhessian);

detector->detectAndCompute(image2, Mat(), Keypoints_2, descriptor_2);



BFMatcher matcher(NORM_L1);
vector<DMatch> matches;
matcher.match(descriptor_1, descriptor_2, matches);
Mat image_matches;

drawMatches(image1, Keypoints_1, image2, Keypoints_2, matches, image_matches);



imshow("first matches", image_matches);


waitKey(2);

}

}



--------------------------------------------------------------------------------------------------------
Working feature detection code working 2

--------------------------------------------------------------------------------------------------------




















*/