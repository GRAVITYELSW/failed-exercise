#include <iostream>
#include <opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void getCon(Mat imgDil, Mat imgRe)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());
    string objetType, color;
    int i, k;
    for(i = 0; i < contours.size(); i++)
    {  
        k = 1;
        float peri = arcLength(contours[i], true);
        approxPolyDP(contours[i], conPoly[i], 0.02*peri, true);
        boundRect[i] = boundingRect(conPoly[i]);

        //判断颜色，绘制轮廓
        Point m((boundRect[i].x+80), (boundRect[i].y+100));
        Vec3b bgr = imgRe.at<Vec3b>(m);
        if(bgr[0] == 0 && bgr[1] == 0 && bgr[2] == 0)
        {
            color = "黑色";
            drawContours(imgRe, contours, i, Scalar(255, 0, 255), 5);
        }
        else if(bgr[0] == 255 && bgr[1] == 0 && bgr[2] == 0)
        {
            color = "蓝色";
            drawContours(imgRe, contours, i, Scalar(0, 0, 255), 5);
        }
        else if(bgr[0] == 0 && bgr[1] == 255 && bgr[2] == 0)
        {
            color = "绿色";
            drawContours(imgRe, contours, i, Scalar(0, 0, 255), 5);
        }
        else if(bgr[0] == 0 && bgr[1] == 0 && bgr[2] == 255)
        {
            color = "红色";
            drawContours(imgRe, contours, i, Scalar(255, 0, 255), 5);
        }
        else
        {
            k = 0;
            drawContours(imgRe, contours, i, Scalar(255, 0, 255), 5);
        }
            
        //判断形状
        int obj = (int)conPoly[i].size();
        if(obj == 3)                
            objetType = "三角形";
        if(obj == 4)
        {
            float aspRation = (float)boundRect[i].width / (float)boundRect[i].height;                
            if(aspRation > 0.95 && aspRation < 1.05)
                objetType = "正方形";
            else 
                objetType = "矩形";
        }
        if(obj > 4)
            objetType = "圆形";
        //写序号
        putText(imgRe, to_string(i + 1), {boundRect[i].x+5, boundRect[i].y-5}, FONT_HERSHEY_DUPLEX, 1, Scalar(0, 69, 255), 2);
        if(k)
        {
            cout << '<' << i+1 << ">\t" << '<' << objetType << ">\t" << '<' << color << '>' << endl;
        }
        else
        {
            cout << '<' << i+1 << ">\t" << '<' << objetType << ">\t";
            printf("<B:%d G:%d R:%d>\n", bgr[0], bgr[1], bgr[2]);
        }
    }
}

int main()
{
    string path = "task1_2.png";
    Mat img = imread(path);
    Mat imgRe, imgGray, imgBlur, imgCanny, imgDil;
    resize(img, imgRe, Size(), 0.3, 0.3);
    cvtColor(imgRe, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(5,5), 5, 0);
    Canny(imgBlur, imgCanny, 25, 50);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(4,4));
    dilate(imgCanny, imgDil, kernel);

    getCon(imgDil, imgRe);

    imshow("Image", imgRe);
    waitKey(0);
    return 0;
}