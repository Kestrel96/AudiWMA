#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Mat output;
    Mat canny_output;
    Mat drawing1;
    Mat Roi;
    Mat Roi2;

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    int thresh = 100;
    int max_thresh = 255;
    RNG rng(12345);


//        Mat img= imread("4.jpg",IMREAD_GRAYSCALE);
//        Mat source= imread("4.jpg",IMREAD_COLOR);

    Mat img= imread("VW.jpg",IMREAD_GRAYSCALE);
    Mat source= imread("VW.jpg",IMREAD_COLOR);

//    Mat img= imread("SUB.jpg",IMREAD_GRAYSCALE);
//    Mat source= imread("SUB.jpg",IMREAD_COLOR);


      blur( img, img, Size(3,3) );
     // adaptiveThreshold(output,output,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,201,-4);

    dilate(img,output,Mat(),Point(-1,-1),2);
    erode(output,output,Mat(),Point(-1,-1),2);

    imshow("OUT",output);
    waitKey(0);

    Canny( output, output, thresh, thresh*2, 3 );
    findContours( output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    Mat drawing = Mat::zeros(output.size(), CV_8UC3 );
    drawing1=Mat::zeros(output.size(), CV_8UC3 );

    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        if(contourArea(contours[i])>100){
            drawContours( drawing, contours, (int)i, color, 1, 8, hierarchy, 0, Point() );
        }

    }

    vector<RotatedRect> minRect( contours.size() );

    for( size_t i = 0; i < contours.size(); i++ )
    {
        minRect[i] = minAreaRect( contours[i] );
    }

    bool circles =0;

    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

        if(contourArea(contours[i])>100 ){

            Point2f rect_points[4]; minRect[i].points( rect_points );

            double Ratio=(rect_points[2].x-rect_points[0].x)/(rect_points[0].y-rect_points[2].y);
            Rect ROI(rect_points[1].x,rect_points[1].y,rect_points[2].x-rect_points[1].x,rect_points[3].y-rect_points[2].y);


            if(Ratio<=3 && Ratio>1 && ROI.area()<100000){
                for( int j = 0; j < 4; j++ ){
                    line( drawing1, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
                }

                circles=1;
                cout<<circles<<endl;

                drawContours( drawing1, contours, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
                cout<<"Area:"<<i<<" -> "<<contourArea(contours[i])<<endl;
                cout<<"1: "<<rect_points[0]<<" 2: "<<rect_points[1]<<" 3: "<<rect_points[2]<<" 4: "<<rect_points[3]<<endl;
                cout<<"W/H ratio: "<<Ratio<<endl;
                cout<<"ROI AREA: "<<ROI.area()<<endl;
                Roi=img(ROI);
                adaptiveThreshold(Roi,Roi,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,201,-4);
                imshow("ROI",Roi);

            }

        }


    }



    if(circles==1){

        vector<Vec3f> circles;

        HoughCircles( Roi, circles, HOUGH_GRADIENT, 15, 50, 200, 100, 10, 0);
        cout<<"Circles: "<<circles.size()<<endl;
        Roi2=Mat::zeros(Roi.size(),CV_8UC3);

        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            circle(Roi2, center, 3, Scalar(125,23,155), -1, 8, 0 );
            // circle outline
            circle(Roi2, center, radius, Scalar(255,15,55), 3, 8, 0 );
        }
        imshow("5",Roi2);

    }


    imshow("1",img);
    imshow("2",output);
    imshow("3",drawing);
    imshow("4",drawing1);
    imshow("6",source);


    waitKey(0);

    return 0;
}
