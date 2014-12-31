#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <cxcore.h>
#include <stdio.h>
#define h 280
#define w 320
#include <math.h>
#include "SerialPort.h"
using namespace std;
CSerialPort serial;



int main()
{

   serial.OpenSerial();
    CvCapture* capture = cvCaptureFromCAM(2 );
   if ( !capture ) {
     fprintf( stderr, "ERROR: capture is NULL \n" );
     getchar();
     //return -1;
   }
   int x;

   while(x!=27){
   //IplImage*f=0;

   //f=setup();
 IplImage* rimg=cvCreateImage(cvSize(w,h),8,3);
	IplImage* hsvimg =cvCreateImage(cvSize(w,h),8,3);
  cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
   IplImage*img=0;
   img= cvQueryFrame( capture );
   img= cvQueryFrame( capture );
   img= cvQueryFrame( capture );
   cvResize(img,rimg,CV_INTER_LINEAR);
  cvShowImage("original",rimg);
    cvCvtColor( rimg,hsvimg, CV_BGR2HSV );
  //cvReleaseImage(&rimg);


  // cent(f);
   IplImage* thresh_r =cvCreateImage(cvSize(w,h),8,1);
  IplImage* thresh_b =cvCreateImage(cvSize(w,h),8,1);
  IplImage* thresh_g =cvCreateImage(cvSize(w,h),8,1);
  IplImage* hough =cvCreateImage(cvSize(w,h),8,1);
  cvInRangeS(hsvimg,cvScalar(0,121,78),cvScalar(15,250,255),thresh_r);
  cvInRangeS(hsvimg,cvScalar(101,121,64),cvScalar(170,250,255),thresh_b);
  cvInRangeS(hsvimg,cvScalar(64,121,64),cvScalar(106,255,255),thresh_g);

  cvNamedWindow("threshed",CV_WINDOW_AUTOSIZE);
  // centroid of red
   // Calculate the moments to estimate the position of the ball
        CvMoments *momentsr = (CvMoments*)malloc(sizeof(CvMoments));
        cvMoments(thresh_r, momentsr, 1);

        // The actual moment values
        double moment10r = cvGetSpatialMoment(momentsr, 1, 0);
        double moment01r = cvGetSpatialMoment(momentsr, 0, 1);
        double arear = cvGetCentralMoment(momentsr, 0, 0);

        int posX_r;
        int posY_r;

        posX_r = moment10r/arear;
        posY_r = moment01r/arear;

       cout<< "x="<<posX_r;
       cout<< "y="<<posY_r;

       // centroid of blue
   // Calculate the moments to estimate the position of the ball
        CvMoments *momentsb = (CvMoments*)malloc(sizeof(CvMoments));
        cvMoments(thresh_b, momentsb, 1);

        // The actual moment values
        double moment10b = cvGetSpatialMoment(momentsb, 1, 0);
        double moment01b = cvGetSpatialMoment(momentsb, 0, 1);
        double areab = cvGetCentralMoment(momentsb, 0, 0);

        int posX_b;
        int posY_b;

        posX_b = moment10b/areab;
        posY_b = moment01b/areab;

       cout<< "x="<<posX_b;
       cout<< "y="<<posY_b;

        // centroid of green
   // Calculate the moments to estimate the position of the ball

    cvCopy(thresh_g,hough,NULL);
    cvSmooth(hough,hough,CV_GAUSSIAN,5,5,0,0);

    //cvCanny(grayimg,thresh,h1,h2,3);
       CvMemStorage * storage=cvCreateMemStorage(0);
     CvSeq* circles= cvHoughCircles( hough, storage,CV_HOUGH_GRADIENT,4, hough->height/2 ,100 ,40,0 ,0);

                  cvReleaseMemStorage(&storage);

                    // draw circles
                    int i;
                    int u;
                    int y;
        for (i=0;i<circles->total;i++){
                         float *p = (float*)cvGetSeqElem(circles, i);
                                                                                                             //cvCircle( img,center,radius,color,thickness,line type, shift)
                   CvPoint center = cvPoint(cvRound(p[0]),cvRound(p[1]));
	    // CvScalar val = cvGet2D(hough, center.y, center.x);
	     //if (val.val[0] < 1) continue;
	        //printf("x=%d, y=%d",cvRound(p[0]),cvRound(p[1]));
	        if(i==0){
	            u=cvRound(p[0]);
	            y=cvRound(p[1]);

             cout<< u;
             cout<< y;
	        }

             cvCircle(hsvimg,  center, 3,             CV_RGB(0,255,0), -1, CV_AA, 0);
             cvCircle(hsvimg,  center, cvRound(p[2]), CV_RGB(255,0,0),  3, CV_AA, 0);
             cvCircle(hough, center, 3,             CV_RGB(0,255,0), -1, CV_AA, 0);
             cvCircle(hough, center, cvRound(p[2]), CV_RGB(255,0,0),  3, CV_AA, 0);
        }
        cvAdd(thresh_r,thresh_b,thresh_b);
        cvAdd(thresh_b,thresh_g,thresh_g);
       cvShowImage("threshed",thresh_g);



        cvReleaseImage(&thresh_r);
        cvReleaseImage(&thresh_b);
       cvReleaseImage(&thresh_g);
        cvReleaseImage(&hsvimg);
        cvReleaseImage(&hough);


        int dist0;
        int dist;
    dist0= (((posX_b-u)*(posX_b-u)) + ((posY_b-y)*(posY_b-y)));
    dist=sqrt(dist0);
    cout<<"dist="<<dist;

     int dist2;
        int dist3;
    dist2= (((posX_b-10)*(posX_b-10)) + ((posY_b-10)*(posY_b-10)));
    dist3=sqrt(dist2);
    cout<<"dist3="<<dist3;

    float slope1_f;
    float slope2_f;

    slope1_f=((posY_b-y)/(posX_b-u));
    slope2_f=((posY_r-y)/(posX_r-u));

    float slope1_b;
    float slope2_b;

    slope1_b=((posY_b-10)/(posX_b-10));
    slope2_b=((posY_r-10)/(posX_r-10));

   if(posX_b > posX_r){

   if(dist < 35){
  serial.Send("s");
  Sleep(1000);
  serial.Send("c");
  Sleep(50);
  serial.Send("e");
  Sleep(1000);
  serial.Send("s");
  }
  // blue is front part


  else if (slope1_f>slope2_f){
  serial.Send("r");

  }
  else if (slope1_f<slope2_f){
  serial.Send("l");

  }
  else if (slope1_f==slope2_f){
  serial.Send("f");
  }
   }

   else {

        if(dist3 < 70){
  serial.Send("s");
  Sleep(1000);
  serial.Send("d");
  Sleep(500);
  serial.Send("s");
  }


   if (slope1_b>slope2_b){
  serial.Send("l");

  }
  else if (slope1_b<slope2_b){
  serial.Send("r");

  }
  else if (slope1_b==slope2_b){
  serial.Send("f");
  }


   }

   cvLine(rimg, cvPoint(posX_r, posY_r), cvPoint(u, y), cvScalar(255,255,255), 2);
    cvLine(rimg, cvPoint(posX_b, posY_b), cvPoint(u, y), cvScalar(255,255,255), 2);
    cvShowImage("original",rimg);

    cvReleaseImage(&rimg);

   x=cvWaitKey(10);
   }



serial.CloseSerial();

    return 0;
}
