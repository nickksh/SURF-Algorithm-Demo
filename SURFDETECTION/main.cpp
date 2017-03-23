#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


//Include statements
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"

//Name spaces used
using namespace cv;
using namespace std;

int main()
{
	//turn performance analysis functions on if testing = true
	bool testing = false;
//	double t; //timing variable

			  //load training image
	Mat object = imread("C:/Users/Admin/Documents/telescope/SURFDETECTION/SURFDETECTION/images/training.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!object.data) {
		cout << "Can't open image";
		waitKey(0);
		return -1;
	}
	namedWindow("Window Name", CV_WINDOW_AUTOSIZE);

	//SURF Detector, and descriptor parameters
	
	vector<KeyPoint> kpObject, kpImage;
	Mat desObject, desImage;

	//SURF Detector, and descriptor parameters, match object initialization
	int minHess = 2000;
	Ptr<xfeatures2d::SURF> surf1 = xfeatures2d::SURF::create(minHess);
	surf1->detectAndCompute(object, Mat(), kpObject, desObject);

	FlannBasedMatcher matcher;

	//Initialize video and display window
	VideoCapture cap(0);  //camera 1 is webcam
	waitKey(1000);
	if (!cap.isOpened()) return -1;

	//Object corner points for plotting box
	vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(object.cols, 0);
	obj_corners[2] = cvPoint(object.cols, object.rows);
	obj_corners[3] = cvPoint(0, object.rows);

	//video loop
	char escapeKey = 'k';
	double frameCount = 0;
	float thresholdMatchingNN = 0.7;
	unsigned int thresholdGoodMatches = 3;
	unsigned int thresholdGoodMatchesV[] = { 4,5,6,7,8,9,10 };

	for (int j = 0; j<7; j++) {
		thresholdGoodMatches = thresholdGoodMatchesV[j];
	
		cout << thresholdGoodMatches << endl;

		

		while (escapeKey != 'q')
		{
			frameCount++;
			Mat frame;
			Mat image;
			cap >> frame;
			cvtColor(frame, image, CV_RGB2GRAY);

			Mat des_image, img_matches, H;
			vector<KeyPoint> kp_image;
			vector<vector<DMatch > > matches;
			vector<DMatch > good_matches;
			vector<Point2f> obj;
			vector<Point2f> scene;
			vector<Point2f> scene_corners(4);

			surf1->detectAndCompute(image, Mat(), kp_image, des_image);
	
			matcher.knnMatch(desObject, des_image, matches, 2);

			if (des_image.data != 0)
			{
				for (int i = 0; i < min(des_image.rows - 1, (int)matches.size()); i++) //THIS LOOP IS SENSITIVE TO SEGFAULTS
				{
					if ((matches[i][0].distance < thresholdMatchingNN*(matches[i][1].distance)) && ((int)matches[i].size() <= 2 && (int)matches[i].size()>0))
					{
						good_matches.push_back(matches[i][0]);
					}
				}
			}

			//Draw only "good" matches
			drawMatches(object, kpObject, image, kp_image, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


			if (good_matches.size() >= thresholdGoodMatches)
			{

				//Display that the object is found
	
				for (unsigned int i = 0; i < good_matches.size(); i++)
				{
					//Get the keypoints from the good matches
					obj.push_back(kpObject[good_matches[i].queryIdx].pt);
					scene.push_back(kp_image[good_matches[i].trainIdx].pt);
				}

				H = findHomography(obj, scene, CV_RANSAC);

				if (H.rows !=0 && H.cols !=0)
				{
					perspectiveTransform(obj_corners, scene_corners, H);
				}
				

				//Draw lines between the corners (the mapped object in the scene image 
				line(img_matches, scene_corners[0] + Point2f(object.cols, 0), scene_corners[1] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
				line(img_matches, scene_corners[1] + Point2f(object.cols, 0), scene_corners[2] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
				line(img_matches, scene_corners[2] + Point2f(object.cols, 0), scene_corners[3] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
				line(img_matches, scene_corners[3] + Point2f(object.cols, 0), scene_corners[0] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
			}
		
			//Show detected matches
			imshow("Window Name", img_matches);
			good_matches.clear();
			escapeKey = waitKey(10);
		

		/*	if (frameCount>10)
				escapeKey = 'q';*/


		}

		//average frames per second
		/*if (true)
		{
			t = ((double)getTickCount() - t) / getTickFrequency();
			cout << t << " " << frameCount / t << endl;
			cvWaitKey(0);
		}*/

		/*frameCount = 0;
		escapeKey = 'a';*/
	}

	//Release camera and exit
	//cap.release();
	return 0;
}
