#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
using namespace std;
using namespace cv;

void detect_draw(cv::Mat image, cv::Mat &cameraMatrix, cv::Mat &distCoeffs) {
//    cv::Mat imageUndistorted;
//    undistort(image, imageUndistorted, cameraMatrix, distCoeffs);
//    imshow("win1", image);
//    imshow("win2", imageUndistorted);
//    waitKey(100000);
//    return;
    
    
    cv::Mat imageCopy;
    image.copyTo(imageCopy);
    
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f> > corners;
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::detectMarkers(image, dictionary, corners, ids);
    
    cout << cameraMatrix << endl;
    cout << distCoeffs << endl;
    
    // if at least one marker detected
    if (ids.size() > 0) {
        cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
        std::vector<cv::Vec3d> rvecs, tvecs;
        
        
        
        cv::aruco::estimatePoseSingleMarkers(corners, 0.1, cameraMatrix, distCoeffs, rvecs, tvecs);
        
        for(int i=0; i<ids.size(); i++) {
//            cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
            float a = 0.05;
            vector< Point3f > box;
            box.push_back(Point3f( a, a, 2*a));
            box.push_back(Point3f( a,-a, 2*a));
            box.push_back(Point3f(-a,-a, 2*a));
            box.push_back(Point3f(-a, a, 2*a));
            
            box.push_back(Point3f( a, a, 0));
            box.push_back(Point3f( a,-a, 0));
            box.push_back(Point3f(-a,-a, 0));
            box.push_back(Point3f(-a, a, 0));
            
            
            
            box.push_back(Point3f( 0,0,0));
            
            
            vector< Point2f > imagePoints;
            projectPoints(box, rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints);
            
            // draw axis lines
            Scalar color = Scalar(207, 249, 102);
            for(int j = 0; j < 4; j++) {
                line(imageCopy, imagePoints[j],     imagePoints[(j+1)%4],   color, 3);
                line(imageCopy, imagePoints[j+4],   imagePoints[(j+5)%4+4], color, 3);
                line(imageCopy, imagePoints[j],     imagePoints[j+4],       color, 3);
                
            }
        }
    }
    
    cv::imshow("out", imageCopy);

}
int main( int argc, char** argv )
{
    
    Mat intrinsic = Mat(3, 3, CV_32FC1);
    Mat distCoeffs;
    FileStorage fsRead("./cameraParams.xml", FileStorage::READ);
    fsRead["intrinsic"]>>intrinsic;
    fsRead["distCoeffs"]>>distCoeffs;
//    
//    IplImage* img = cvLoadImage("fuck.png", 1);
//    
//    cv::Mat inputImage = cv::cvarrToMat(img);
//    
//    cv::waitKey(100000);
    
    cv::VideoCapture inputVideo;
    inputVideo.open(0);
    
    while (inputVideo.grab()) {
        cv::Mat image, imageCopy;
        inputVideo.retrieve(image);
                char key = (char) cv::waitKey(20);
        
        detect_draw(image, intrinsic, distCoeffs);
        
        if (key == 27)
            break;
    }
    
    
    
    
//return
    return 0;
}
