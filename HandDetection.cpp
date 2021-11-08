#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define PI 3.141592

Mat getImage(const Mat& img); // 블러된 영상 가져오기
Mat skinColorDetection(const Mat& image, int minCr = 133, int maxCr = 180, int minCb = 75, int maxCb = 130); 
// RGB -> YCbCr로 변환한 후 피부색 검출
// 실험해봤을 때 가장 skin detection이 잘 되는 Cr, Cb 로 설정
//133 <= Cr <= 180, 75 <= Cb <= 130
Point getCenter(Mat img); // 손바닥 중심 찾기
int drawing(Mat img, Point center); // 손바닥 중심으로 원 그리기


int main(int argc, char** argv)
{
    Mat image;
    VideoCapture cap;

    // 웹캠 실행
    int deviceID = 0;
    int apiID = cv::CAP_ANY;

    cap.open(deviceID + apiID);
    if (!cap.isOpened()) return -1; 
    
    for (;;) {
        cap.read(image);

        if (image.empty()) break; 

        namedWindow("webcam");
        imshow("webcam", image); // 웹캠 실행

        image = getImage(image); 
        
        image = skinColorDetection(image); 
       
        Point center; 
        center = getCenter(image);  

        int drawcircle; 
        drawcircle = drawing(image, center);

        if (waitKey(30) >= 0) break;
    }

    waitKey(1000);
    return 0;
}

// 블러된 영상 가져오기
Mat getImage(const Mat& img) {
    Mat result;
    medianBlur(img, result, 5);
    return result;
}

// RGB -> YCbCr로 변환한 후 피부색 검출
Mat skinColorDetection(const Mat& image, int minCr, int maxCr, int minCb, int maxCb) {

    //컬러 공간 변환 BGR->YCrCb
    Mat YCrCb;
    cvtColor(image, YCrCb, COLOR_BGR2YCrCb);

    //각 채널별로 분리

    vector<Mat> planes;
    split(YCrCb, planes);

    Mat mask = (minCr < planes[1]) & (planes[1] < maxCr) & (minCb < planes[2]) & (planes[2] < maxCb);
    return mask;
}

// 손바닥 중심 검출
Point getCenter(Mat img)
{
    Mat before;
    Mat after;

    before = img.clone();
    bool allBlack = true;

    // 계속 침식(erode)시키다가 after의 화소가 전부 black(0)이 되면 before에 남아있는 화소의 평균 위치 리턴
    while (allBlack)
    {
        erode(before, after, Mat());

        // after가 전부 black(0)인지 검사
        for (int j = 0; j < img.rows; j++){
            //j열의 주소 가져오기
            uchar* data = after.ptr<uchar>(j);
            for (int i = 0; i < img.cols; i++){
                if (data[i]) allBlack = false;
            }
        }

        //  after의 화소가 전부 black이 되면 before에 남아있는 화소의 평균 위치 리턴
        if (allBlack){
            int col = 0;
            int row = 0;
            int count = 0;

            // before에 있는 모든 화소의 평균 위치 계산
            for (int j = 0; j < img.rows; j++){
                //j열의 주소(nc개만큼) 가져오기
                uchar* data = before.ptr<uchar>(j);
                for (int i = 0; i < img.cols; i++){
                    if (data[i]){
                        count++;
                        row += j;
                        col += i;
                    }
                }
            }

            if (count == 0) count++;
            row = (int)(row / count);
            col = (int)(col / count);

            Point center(col, row);
            return center;
        }
        allBlack = true;
        before = after.clone();
    }
}

// 손바닥 중심으로 원 그리기
int drawing(Mat img, Point center) {

    Mat dst, color, colors;
    dst = img.clone();

    color = img.clone();
    cvtColor(color, color, COLOR_GRAY2BGR);

    // 손바닥 중심 위치
    circle(color, center, 5, Scalar(0, 0, 255), -1);
    imshow("circles", color);

    // 원 그리기
    circle(color, center, 120, Scalar(50, 50, 255), 2, LINE_8, 0);
    imshow("circles", color);

    return -1;
}