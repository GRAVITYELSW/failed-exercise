#include <iostream>
#include <opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat background;

int kp = 0.0 , ki = 0.0  , kd = 0.0;
double Kp ,Ki ,Kd;
//目标坐标
double X = 0;
double Y = 0;
//实际坐标
double ax = 0;
double ay = 0;
//目标与实际的差值
double errx = 0;
double erry = 0;
//上一次的差值
double last_errx = 0;
double last_erry = 0;
//误差累计
double wuchax = 0;
double wuchay = 0;
//pid的实现
double pidx = 0;
double pidy = 0;
//单位时间
double dt = 0.05;
//质量
double m = 1;
//阻力系数
double u = 0.9;
//力
double fx = 0;
double fy = 0;
//加速度
double aax = 0;
double aay = 0;
//速度
double vx = 0;
double vy = 0;

//pid
double pidResolvex(double kp, double ki, double kd, double X, double x)
{
    errx = X - x;
    wuchax = wuchax + errx;
    pidx = kp * errx + ki * wuchax + kd * (errx - last_errx);
    last_errx = errx;
    return pidx;
}
double pidResolvey(double kp, double ki, double kd, double Y, double y)
{
    erry = Y - y;
    wuchay = wuchay + erry;
    pidy = kp * erry + ki * wuchay + kd * (erry - last_erry);
    last_erry = erry;
    return pidy;
}

//回调函数
void on_trackbar_p(int ,void*)
{
    Kp=kp/100.0;
}
void on_trackbar_i(int ,void*)
{
    Ki=ki/1000.0;
    wuchax = 0;
    wuchay = 0;
}
void on_trackbar_d(int ,void*)
{
    Kd=kd/10.0;
}

//鼠标的坐标
void mouseEvent(int event, int x, int y, int flags, void *)
{
	if (event == CV_EVENT_MOUSEMOVE)
	{
		X = (double)x;
        Y = (double)y;
	}
}

int main()
{
    //滑动条
    namedWindow("img");
    createTrackbar( "Kp", "img", &kp, 100, on_trackbar_p );
    createTrackbar( "Ki", "img", &ki, 20, on_trackbar_i );
    createTrackbar( "Kd", "img", &kd, 1000, on_trackbar_d );
    on_trackbar_p(0,0);
    on_trackbar_i(0,0);
    on_trackbar_d(0,0);

    setMouseCallback("img", mouseEvent);

    while(true)
    {
        background = Mat(1280, 1024, CV_8UC3, Scalar(255, 255, 255));

        //鼠标的十字
        line(background, Point(X-25, Y), Point(X-4, Y), Scalar(255, 0, 0), 1);
        line(background, Point(X+4, Y), Point(X+25, Y), Scalar(255, 0, 0), 1);
        line(background, Point(X, Y-25), Point(X, Y-4), Scalar(255, 0, 0), 1);
        line(background, Point(X, Y+4), Point(X, Y+25), Scalar(255, 0, 0), 1);

        string P = to_string(Kp);
        string I = to_string(Ki);
        string D = to_string(Kd);
        putText(background, "Kp="+P, Point(20,50), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 70, 255), 2, 5, false);
        putText(background, "Ki="+I, Point(20,100), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 70, 255), 2, 8, false);
        putText(background, "Kd="+D, Point(20,150), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 70, 255), 2, 8, false);

        pidx = pidResolvex(Kp, Ki, Kd, X, ax);
        pidy = pidResolvey(Kp, Ki, Kd, Y, ay);
        fx = pidx - u * vx;
        fy = pidy - m * 10 - u * vy;
        aax = fx / m;
        aay = fy / m;
        vx += aax * dt;
        vy += aay * dt;
        ax += vx * dt;
        ay += vy * dt;

        //跟随的圆圈
        circle(background, Point(ax, ay), 8, Scalar(255, 255, 0),5);

        cout << X << " " << Y << endl;
        cout << ax << " " << ay << endl;
        cout << endl;
        imshow("img", background);
        waitKey(10);
    }
}