#include <iostream>
#include <opencv/cv.h>
#include <opencv\highgui.h>
#include <vector>
#include <math.h>
#include <Windows.h>

using namespace cv;
using namespace std;
float x_old,y_old,contour_size_old;
float pi=3.14;

Mat threshold_hsv(Mat A)
{
	int h,s,v;
	int h_low=130;
	int h_high=256;
	int s_low=10;
	int s_high=256;
	int v_low=250;
	int v_high=256;

	for(int i=0; i<A.rows; i++)
		{
		 for(int j=0; j<A.cols; j++)
		 {
           h=A.data[A.channels()*(A.cols*i + j) + 0];
           s=A.data[A.channels()*(A.cols*i + j) + 1];
           v=A.data[A.channels()*(A.cols*i + j) + 2];
		   //std::cout<<h<<" ";
		   if((h<10 ||(h>=h_low && h<=h_high)) && s>s_low && s<s_high && v>v_low && v<v_high)
		   {
			    A.data[A.channels()*(A.cols*i + j) + 0]=255;
				A.data[A.channels()*(A.cols*i + j) + 1]=255;
				A.data[A.channels()*(A.cols*i + j) + 2]=255;
		   }
		   else
		   {
			    A.data[A.channels()*(A.cols*i + j) + 0]=0;
				A.data[A.channels()*(A.cols*i + j) + 1]=0;
				A.data[A.channels()*(A.cols*i + j) + 2]=0;
				
		   }
		 }
		}
	return A;
	
}


void cursor(int x, int y)
{
	float summation=sqrt(pow((x-x_old),2)+pow((y-y_old),2));
	//std::cout<<" <<"<<summation<<">> ";
	if(summation>=3)
	SetCursorPos((1366)-(1366/410*(x-100)),(768*(y-80)/270));
	x_old=x;
	y_old=y;
}

int main()
{

	Point center;
	Mat A,A_tmp,A_hsv,A_thresh, A_bw;

	vector<vector<Point> > contours;
	Mat canny;
	Scalar color(255,255,255);
	//IplImage* Iplimg, *Iplimg_hsv;
	//cvCaptureFromCAM(2);
	//CvCapture* cap=cvCaptureFromCAM(0);
	VideoCapture cap;
	
	
	cap.open(0);

	namedWindow("camwin",1);
	//namedWindow("hsvwin",1);
	//namedWindow("threshwin",1);
	//namedWindow("bwwin",1);

	while(1)
	{
		cap>>A;
		
		//blur(A, A_hsv, Size(60,60) );
		cvtColor(A, A_hsv, CV_BGR2HSV);
		
		threshold_hsv(A_hsv);
		A_bw=A_hsv >128;
				
		Canny(A_bw,canny,50,150,3);
		blur(canny, canny, Size(90,90) );
		
		findContours(canny,contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
		
		//std::cout<<contours.size()<<"  ";
		while(contours.size()>1)
		{
			if(contours[0].size()<=contours[1].size())
			{
				contours.erase(contours.begin());
			}
			else contours.erase(contours.begin()+1);
		}
		if(contours.size()!=0)if(contours[0].size()<=150)contours.erase(contours.begin());

		if(contours.size()!=0)
		{
			
		Rect rect = boundingRect(contours[0]);
		Point pt1, pt2;
		pt1.x = rect.x;
		pt1.y = rect.y;
		pt2.x = rect.x + rect.width;
		pt2.y = rect.y + rect.height;
		center.x=(pt1.x+pt2.x)/2;
		center.y=(pt1.y+pt2.y)/2;
		rectangle(A, pt1, pt2, CV_RGB(0,255,0), 2);
		
		std::cout<<"<< "<<6/tan((320-center.x)*27*pi/(320*180)) <<" >>"<< endl;
		//drawContours(A,contours,-1,color, 3, 8);
		float contour_size_sum=contours[0].size()-contour_size_old;
		contour_size_old=contours[0].size();
		//std::cout<<" <<"<<contours[0].size()<<">> ";
		//if(contour_size_sum >=-10 && contour_size_sum<=10)
		//cursor(center.x,center.y);
		}
		/*std::cout<<"hh"<<(int)A_tmp.data[A_tmp.channels()*(A_tmp.cols*A_tmp.rows/2 + A_tmp.cols/2) + 1]<<"hh";
		std::cout<<"ss"<<(int)A_tmp.data[A_tmp.channels()*(A_tmp.cols*A_tmp.rows/2 + A_tmp.cols/2) + 2]<<"ss";
		std::cout<<"vv"<<(int)A_tmp.data[A_tmp.channels()*(A_tmp.cols*A_tmp.rows/2 + A_tmp.cols/2) + 3]<<"vv";
		A.data[A.channels()*(A.cols*A.rows/2 + A.cols/2) + 1]=255;
		A.data[A.channels()*(A.cols*A.rows/2 + A.cols/2) + 2]=0;
		A.data[A.channels()*(A.cols*A.rows/2 + A.cols/2) + 3]=0;*/
		imshow("camwin",A);
		//imshow("hsvwin",A_hsv);
		//imshow("threshwin",A_thresh);
		//imshow("bwwin",A_bw);
		waitKey(30);
		
	
	}

	return 0;
}