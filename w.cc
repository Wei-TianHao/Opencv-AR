#include <opencv2/opencv.hpp>
using namespace std;

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

IplImage *g_pGrayImage = NULL;
IplImage *g_pBinaryImage = NULL;
const char *pstrWindowsBinaryTitle = "二值图(http://blog.csdn.net/MoreWindows)";

void on_trackbar(int pos)
{
	cvThreshold(g_pGrayImage, g_pBinaryImage, pos, 255, CV_THRESH_BINARY);
	cvShowImage(pstrWindowsBinaryTitle, g_pBinaryImage);
}

int main( int argc, char** argv )
{	
	const char *pstrWindowsSrcTitle = "pic";
	const char *pstrWindowsToolBarName = "二值图阈值";

	// 从文件中加载原图
	IplImage *pSrcImage = cvLoadImage("aruco.jpg", CV_LOAD_IMAGE_UNCHANGED);

	// 转为灰度图
	g_pGrayImage =  cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImage, g_pGrayImage, CV_BGR2GRAY);

	// 创建二值图
	g_pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);

	// 显示原图
	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsSrcTitle, pSrcImage);
	// 创建二值图窗口
	cvNamedWindow(pstrWindowsBinaryTitle, CV_WINDOW_AUTOSIZE);

	// 滑动条	
	int nThreshold = 0;
	cvCreateTrackbar(pstrWindowsToolBarName, pstrWindowsBinaryTitle, &nThreshold, 254, on_trackbar);

	on_trackbar(1);

	cvWaitKey(0);

	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(pstrWindowsBinaryTitle);
	cvReleaseImage(&pSrcImage);
	cvReleaseImage(&g_pGrayImage);
	cvReleaseImage(&g_pBinaryImage);
	return 0;
}