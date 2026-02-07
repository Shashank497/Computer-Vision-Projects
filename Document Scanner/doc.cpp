#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

/////////////////////////// Document Scanner //////////////////////////////////

Mat imgOriginal,imgThr,imgGray,imgBlur,imgCanny,imgDil,imgErode,imgWarp,imgCrop;
vector<Point>initialPoints,docPoints;
float w=420,h=596;  //Targeted width & height for warped document

Mat preprocessing(Mat img)
{
    //Grayscale Conversion
    cvtColor(imgOriginal,imgGray,COLOR_BGR2GRAY);
    //Blur 
    GaussianBlur(imgGray,imgBlur,Size(3,3),3,0);
    //Edge Detection
    Canny(imgBlur,imgCanny,25,75);

    //More clear edges
    Mat kernel=getStructuringElement(MORPH_RECT,Size(3,3));
    dilate(imgCanny,imgDil,kernel);
    //erode(imgDil,imgErode,kernel);
    return imgDil;
}

vector<Point> getContours(Mat img)
{
    vector<vector<Point>>contours;
    vector<Vec4i>hierarchy;

    findContours(imgDil,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    vector<vector<Point>>conPoly(contours.size());

    vector<Point>biggest;
    int maxArea=0;
    for(int i=0;i<contours.size();i++)
    {
        int area=contourArea(contours[i]);
        //cout<<area<<endl;

        if(area>1000)
        {
            float peri=arcLength(contours[i],true);
            approxPolyDP(contours[i],conPoly[i],0.02*peri,true);
            if(area>maxArea && conPoly[i].size()==4)
            {   
                //drawContours(img,conPoly,i,Scalar(255,0,255),2);
                biggest={conPoly[i][0],conPoly[i][1],conPoly[i][2],conPoly[i][3]};
                maxArea=area;
            }
        }
    }
    return biggest;
}

vector<Point>reorder(vector<Point>points)
{
    vector<Point>newPoints;
    vector<int>sumPoints,subPoints;
    for(int i=0;i<4;i++)
    {
        sumPoints.push_back(points[i].x+points[i].y);
        subPoints.push_back(points[i].x-points[i].y);
    }
    newPoints.push_back(points[min_element(sumPoints.begin(),sumPoints.end())-sumPoints.begin()]);  //point:0
    newPoints.push_back(points[max_element(subPoints.begin(),subPoints.end())-subPoints.begin()]);  //point:1
    newPoints.push_back(points[min_element(subPoints.begin(),subPoints.end())-subPoints.begin()]);  //point:2
    newPoints.push_back(points[max_element(sumPoints.begin(),sumPoints.end())-sumPoints.begin()]);  //point:3

    return newPoints;
}

Mat getWarp(Mat img,vector<Point>points,float w,float h)
{   
    //source points:the recorded corners fron the image
    Point2f src[4]={points[0],points[1],points[2],points[3]};
    //destination points:a perfect rectangle
    Point2f dest[4]={{0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h}};

    Mat matrix=getPerspectiveTransform(src,dest);
    warpPerspective(img,imgWarp,matrix,Point(w,h));
    return imgWarp;
}

int main()
{
    VideoCapture cap(0);
    if(!cap.isOpened()) return -1;

    while(true)
    {
        //1.Reading frames from Camera
        cap.read(imgOriginal);
    
        //2.Pre-processing
        imgThr=preprocessing(imgOriginal);

        //3.Get Biggest Contours
        initialPoints=getContours(imgThr);

        //If 4 sided contour(document) is found then only go for Warp
        if(initialPoints.size()==4)
        {
            docPoints = reorder(initialPoints);
            
            circle(imgOriginal, docPoints[0], 10, Scalar(255, 0, 0), FILLED); 
            circle(imgOriginal, docPoints[1], 10, Scalar(0, 255, 0), FILLED);
            circle(imgOriginal, docPoints[2], 10, Scalar(0, 0, 255), FILLED);
            circle(imgOriginal, docPoints[3], 10, Scalar(0, 255, 255), FILLED);

            //4. Warp
            imgWarp = getWarp(imgOriginal, docPoints, w, h);

            //5. Crop
            Rect roi(5, 5, w - (2 * 5), h - (2 * 5));
            imgCrop = imgWarp(roi);
            
            //6. Display the scanned document
            imshow("Scanned Document", imgCrop);
        }
        imshow("Image",imgOriginal);

        if (waitKey(1) == 27) 
        {
            break; 
        }

    }
    return 0;
}