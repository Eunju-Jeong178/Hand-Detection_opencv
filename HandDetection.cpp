#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define PI 3.141592

Mat getImage(const Mat& img); // ���� ���� ��������
Mat skinColorDetection(const Mat& image, int minCr = 133, int maxCr = 180, int minCb = 75, int maxCb = 130); 
// RGB -> YCbCr�� ��ȯ�� �� �Ǻλ� ����
// �����غ��� �� ���� skin detection�� �� �Ǵ� Cr, Cb �� ����
//133 <= Cr <= 180, 75 <= Cb <= 130
Point getCenter(Mat img); // �չٴ� �߽� ã��
int drawing(Mat img, Point center); // �չٴ� �߽����� �� �׸���


int main(int argc, char** argv)
{
    Mat image;
    VideoCapture cap;

    // ��ķ ����
    int deviceID = 0;
    int apiID = cv::CAP_ANY;

    cap.open(deviceID + apiID);
    if (!cap.isOpened()) return -1; 
    
    for (;;) {
        cap.read(image);

        if (image.empty()) break; 

        namedWindow("webcam");
        imshow("webcam", image); // ��ķ ����

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

// ���� ���� ��������
Mat getImage(const Mat& img) {
    Mat result;
    medianBlur(img, result, 5);
    return result;
}

// RGB -> YCbCr�� ��ȯ�� �� �Ǻλ� ����
Mat skinColorDetection(const Mat& image, int minCr, int maxCr, int minCb, int maxCb) {

    //�÷� ���� ��ȯ BGR->YCrCb
    Mat YCrCb;
    cvtColor(image, YCrCb, COLOR_BGR2YCrCb);

    //�� ä�κ��� �и�

    vector<Mat> planes;
    split(YCrCb, planes);

    Mat mask = (minCr < planes[1]) & (planes[1] < maxCr) & (minCb < planes[2]) & (planes[2] < maxCb);
    return mask;
}

// �չٴ� �߽� ����
Point getCenter(Mat img)
{
    Mat before;
    Mat after;

    before = img.clone();
    bool allBlack = true;

    // ��� ħ��(erode)��Ű�ٰ� after�� ȭ�Ұ� ���� black(0)�� �Ǹ� before�� �����ִ� ȭ���� ��� ��ġ ����
    while (allBlack)
    {
        erode(before, after, Mat());

        // after�� ���� black(0)���� �˻�
        for (int j = 0; j < img.rows; j++){
            //j���� �ּ� ��������
            uchar* data = after.ptr<uchar>(j);
            for (int i = 0; i < img.cols; i++){
                if (data[i]) allBlack = false;
            }
        }

        //  after�� ȭ�Ұ� ���� black�� �Ǹ� before�� �����ִ� ȭ���� ��� ��ġ ����
        if (allBlack){
            int col = 0;
            int row = 0;
            int count = 0;

            // before�� �ִ� ��� ȭ���� ��� ��ġ ���
            for (int j = 0; j < img.rows; j++){
                //j���� �ּ�(nc����ŭ) ��������
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

// �չٴ� �߽����� �� �׸���
int drawing(Mat img, Point center) {

    Mat dst, color, colors;
    dst = img.clone();

    color = img.clone();
    cvtColor(color, color, COLOR_GRAY2BGR);

    // �չٴ� �߽� ��ġ
    circle(color, center, 5, Scalar(0, 0, 255), -1);
    imshow("circles", color);

    // �� �׸���
    circle(color, center, 120, Scalar(50, 50, 255), 2, LINE_8, 0);
    imshow("circles", color);

    return -1;
}