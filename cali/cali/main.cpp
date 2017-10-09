#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
//    vector<Mat> images;
//    for(int i = 64; i < 75; i++) {
//        //        string s = "pic0.JPG";
//        //        s[3] = i + '0';
//        
//        string s = "IMG_1164.JPG";
//        s[6] = i / 10 + '0';
//        s[7] = i % 10 + '0';
//        IplImage* img = cvLoadImage(s.c_str(), 1);
//        
//        images.push_back(cv::cvarrToMat(img));
//    }
//    
    Mat intrinsic = Mat(3, 3, CV_32FC1);
    Mat distCoeffs;
    
    int numBoards = 20;
    int numCornersHor = 9;
    int numCornersVer = 6;
    int numSquares = numCornersHor * numCornersVer;
    Size board_sz = Size(numCornersHor, numCornersVer);
    
    VideoCapture capture = VideoCapture(0);
    
    vector<vector<Point3f>> object_points;
    vector<vector<Point2f>> image_points;
    vector<Point2f> corners;
    int successes=0;
    Mat image;
    Mat gray_image;
    capture >> image;
    
    vector<Point3f> obj;
    for(int j=0;j<numSquares;j++)
        obj.push_back(Point3f(j/numCornersHor, j%numCornersHor, 0.0f));
    
    while(successes<numBoards) {
        
        cvtColor(image, gray_image, CV_BGR2GRAY);
        bool found = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        
        if(found)
        {
            cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
            drawChessboardCorners(gray_image, board_sz, corners, found);
        }
        imshow("win1", image);
        imshow("win2", gray_image);
        waitKey(50);
        capture >> image;
        
        
        if(found!=0)
        {
            image_points.push_back(corners);
            object_points.push_back(obj);
            successes++;
            if(successes >= numBoards)
                break;
            printf("%d snaps stored!", successes);
            
        }
    }
    cout << successes << " images used." << endl;
    vector<Mat> rvecs;
    vector<Mat> tvecs;
    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;
    
    
    calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);
    
    cout << intrinsic << endl;
    cout << distCoeffs << endl;
    
    FileStorage fsFeature("./cameraParams.xml", FileStorage::WRITE);
    fsFeature<<"intrinsic"<<intrinsic;
    fsFeature<<"distCoeffs"<<distCoeffs;
    fsFeature.release();
    
    
    FileStorage fsRead("./cameraParams.xml", FileStorage::READ);
    fsRead["intrinsic"]>>intrinsic;
    fsRead["distCoeffs"]>>distCoeffs;
    
    
    Mat imageUndistorted;
    while(1)
    {
        capture >> image;
        undistort(image, imageUndistorted, intrinsic, distCoeffs);
        
        imshow("win1", image);
        imshow("win2", imageUndistorted);
        waitKey(1);
    }
    capture.release();
    
    return 0;
}
