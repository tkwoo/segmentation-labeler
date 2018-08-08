#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>

using namespace std;
using namespace cv;

struct SImages{
    Mat imgInput;
    Mat imgShow;
    Mat imgLabel;
    SImages(Mat& in, Mat& show, Mat& label){
        imgInput = in;
        imgShow = show;
        imgLabel = label;
    }
};

Scalar Black(0,0,0);
Scalar White(255,255,255);
Scalar Red(50,50,255);
Point ptBoxStart(0,0);
Rect rectBBox(0,0,0,0);

void onMouse(int event, int x, int y, int flags, void* param){
    // cout << "event:" << event << ", flags:" << flags << endl;
    SImages* objImages = (SImages*)param;
    objImages->imgShow = objImages->imgInput.clone();
    if(x<0 || y<0 || x>objImages->imgShow.cols || y>objImages->imgShow.rows) return;
    Mat imgHSV;
    cvtColor(objImages->imgShow, imgHSV, CV_BGR2HSV);
    Vec3b vColor = objImages->imgShow.at<Vec3b>(y,x);
    Vec3b vHSV = imgHSV.at<Vec3b>(y,x);
    string strPosition = "("+to_string(x)+","+to_string(y)+")";
    string strValue = "["+to_string(vColor(2))+","+to_string(vColor(1))+","+to_string(vColor(0))+"]";
    string strHSV = "["+to_string(vHSV(0))+","+to_string(vHSV(1))+","+to_string(vHSV(2))+"]";
    Scalar textColor = Red;
    // if(vColor(2)>128 && vColor(1)>128 && vColor(0)>128) textColor = Black;
    putText(objImages->imgShow, strPosition, Point(x+3,y-7),FONT_HERSHEY_SIMPLEX, 0.4, textColor, 1);
    //*objImages->imgShow.rows/800.
    putText(objImages->imgShow, strValue, Point(x+3,y+7), FONT_HERSHEY_SIMPLEX, 0.4, textColor, 1);
    putText(objImages->imgShow, strHSV, Point(x+3,y+21), FONT_HERSHEY_SIMPLEX, 0.4, textColor, 1);
    circle(objImages->imgShow, Point(x,y), 2, Scalar(0,0,255), -1);
    rectangle(objImages->imgShow, Rect(0,0,10,10), vColor, -1);
    if(event == 1) ptBoxStart = Point(x,y);
    if(flags == 33) rectangle(objImages->imgShow, ptBoxStart, Point(x,y), Scalar(0,0,255), 1);
    if(event == 4){
        rectBBox.x = x < ptBoxStart.x ? x : ptBoxStart.x;
        rectBBox.y = y < ptBoxStart.y ? y : ptBoxStart.y;
        rectBBox.width = abs(x-ptBoxStart.x);
        rectBBox.height = abs(y-ptBoxStart.y);
        cout << rectBBox << endl;
    }
}

int main(int argc, char* argv[]){
    vector<string> vecstrArgv;
    if(argc == 1) {
        cout << "program option error" << endl;
        return 1;
    }
    for(int i = 1; i < argc; i++)
        vecstrArgv.push_back(argv[i]);
    
    Mat imgInput = imread(vecstrArgv[0],1);
    Mat imgShow = imgInput.clone();
    Size sz = imgInput.size();
    Mat imgLabel(sz.height, sz.width, CV_8UC1, Scalar(0,0,0));
    // cout << sz << imgLabel.size() << endl;
    SImages objImages(imgInput, imgShow, imgLabel);
    string strWindowName = "TKWOO IMAGE VIEWER";
    namedWindow(strWindowName, 0);
    resizeWindow(strWindowName, 800, 800);
    while(1){
        setMouseCallback(strWindowName, onMouse, &objImages);
        imshow(strWindowName, objImages.imgShow);
        if(waitKey(1)==27) break;
    }

    return 0;
}
