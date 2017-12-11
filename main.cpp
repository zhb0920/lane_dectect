#include "process.h"
#define endLine 320
int main()
{
VideoCapture cap("./2.mp4");
if(!cap.isOpened())
{
cout<< "open failed "<< endl;
}
int  thresh=160;
Mat threshold_img,gray;
Mat dstImg;
Mat frame;
Mat edges;
Mat dilateImg;

cap>>frame;
gray=Mat(frame.size(),CV_8UC1);
cout<< "row: "<< frame.rows << "\n"<<"cols:"<<frame.cols<<endl;
int i,j;
namedWindow("result");
//RIO区域
Mat mask = Mat(gray.size(),CV_8UC1);
vector<vector<Point> > contour;
vector<Point> pts;
pts.push_back(Point(460,300));
pts.push_back(Point(60,540));
pts.push_back(Point(900,540));
pts.push_back(Point(500,300));
contour.push_back(pts);
drawContours(mask,contour,0,Scalar::all(255),-1);
VideoWriter videoWrite("video.mp4",CV_FOURCC('M','J','P','G'),30,frame.size());

    while(1)
    {
        cap>>frame;

        //G=0.5*(B+R)
        for(i=0;i<gray.rows;i++)
        {
                uchar*  p1=gray.ptr<uchar>(i);
                uchar*  p2=frame.ptr<uchar>(i);
               for(j=0;j<gray.cols;j++)
                {
                p1[j]=0.5*(p2[3*j]+p2[3*j+2]);
               }
        }
         //滤波
        blur(gray,gray ,Size(3,3));
        imshow("gray",gray);

        //二值化
        threshold(gray,threshold_img,thresh,255, CV_THRESH_OTSU  );
        //imshow("thresh",threshold_img);
        //截取RIO
        threshold_img.copyTo(dstImg,mask);
        //imshow("dstImg",dstImg);
        //膨胀
        Mat element = getStructuringElement(MORPH_RECT,Size(5,5));
        dilate(dstImg,dilateImg,element);
        //imshow("erodeImg",dilateImg);
         //边缘检测
        Canny(dilateImg,edges,120,200,3);
        vector<Vec4i> lines;
        Vec4i leftLine,rightLine;
        vector<Vec2f>::iterator it;
        //霍夫直线变换
        HoughLinesP(edges, lines, 1, CV_PI/180, 50, 0,10);
        cout<<"num of line is "<< lines.size()<<endl;
        int m=0,n=0;

        for(int j=0;j<4;j++)
        {
           leftLine[j]=0;
           rightLine[j]=0;

        }
        for( size_t i = 0; i < lines.size(); i++ )
        {

              Vec4i  l;
              l=lines[i];
              float angle;
              angle=atan2(l[2]-l[0],l[3]-l[1])*180/CV_PI;

              if(angle>90)
              {
                  m++;
                  for(int j=0;j<4;j++)
                  {
                     leftLine[j]+=l[j];

                  }

              }
              else if(angle< 90)
              {
                   n++;
                  for(int j=0;j<4;j++)
                  {

                     rightLine[j]+=l[j];

                  }

              }
         }
        //计算直线平均值
        for(int j=0;j<4;j++)
        {

           rightLine[j]=rightLine[j]/n;
           leftLine[j]=leftLine[j]/m;
        }


        float angle_left,angle_right;
        //计算角度
        angle_left=atan2(leftLine[2]-leftLine[0],leftLine[3]-leftLine[1]);
        angle_right=atan2(rightLine[2]-rightLine[0],rightLine[3]-rightLine[1]);
        cout << "left:"<< angle_left<<endl;
        cout << "right"<< angle_right<<endl;

        float leftK,rightK;
        leftK=static_cast<float>(leftLine[1]-leftLine[3])/(leftLine[2]-leftLine[0]);
        cout <<"leftLine 0 1 2 3 "<<leftLine[0]<<" "<<leftLine[1]<<" "<<leftLine[2]<<"  "<<leftLine[3]<<" "<<endl;
        cout << "leftK= "<< leftK << endl;

        leftLine[0]=leftLine[0]-(frame.cols-leftLine[1])/leftK;
        leftLine[1]=frame.cols;
        if(leftLine[3]>endLine)
        {
        leftLine[2]=leftLine[2]+(leftLine[3]-endLine)/leftK;
        leftLine[3]=endLine;
        }

        rightK=static_cast<float>(rightLine[3]-rightLine[1])/(rightLine[2]-rightLine[0]);
        rightLine[2]=rightLine[2]+(frame.cols-rightLine[3])/rightK;
        rightLine[3]=frame.cols;

        if(rightLine[1]>endLine)
        {
        rightLine[0]=rightLine[0]-(rightLine[1]-endLine)/rightK;
        rightLine[1]=endLine;
        }
        Mat GreenImg=Mat(frame.size(),CV_8UC3,Scalar(0,255,0));
        Mat maskGreen=Mat(frame.size(),CV_8UC3,Scalar(0,0,0));

        vector<vector<Point> > maskContour;
        vector<Point> maskPts;
        maskPts.push_back(Point(leftLine[2],leftLine[3]));
        maskPts.push_back(Point(leftLine[0],leftLine[1]));
        maskPts.push_back(Point(rightLine[2],rightLine[3]));
        maskPts.push_back(Point(rightLine[0],rightLine[1]));
        maskContour.push_back(maskPts);
        drawContours(maskGreen,maskContour,0,Scalar::all(255),-1);
        GreenImg.copyTo(maskGreen,maskGreen);
        //imshow("maskGreen",maskGreen);
        addWeighted(frame,0.8,maskGreen,0.2,0,frame);
        line( frame, Point(rightLine[0],rightLine[1]),Point(rightLine[2],rightLine[3]), Scalar(0,0,255), 3, CV_AA);
        line( frame, Point(leftLine[0],leftLine[1]),Point(leftLine[2],leftLine[3]), Scalar(0,0,255), 3, CV_AA);

        circle(frame,Point(rightLine[0],rightLine[1]),10,Scalar(0,0,255));
        circle(frame,Point(leftLine[2],leftLine[3]),10,Scalar(0,0,255));
        videoWrite<<frame;
        imshow("result",frame);
        waitKey(30);
    }
    return 0;
}
