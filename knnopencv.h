#ifndef KNNOPENCV_H
#define KNNOPENCV_H
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>
#include <QString>
#include <QPixmap>
#include <commonfunc.h>

//using namespace cv;
using namespace std;
using namespace cv::ml;

struct Knn_Config{
    int name;
    cv::Rect rc;
};

class KnnOpencv
{
public:
    KnnOpencv();
    ~KnnOpencv();
    double GetLineLength( QPixmap& pix);
    cv::Mat getUsefulArear(cv::Mat const& src , uchar t_min = 255 , uchar t_max = 255 );
    double getDistance(cv::Point const& p1 , cv::Point const& p2);
    cv::Mat Qimage2Mat(QImage const& image);
    bool SetConfig();
    int GetRuleType(QPixmap& pix);
private:
    double m_GrayTholdValue;
    cv::Mat m_srcimage;
    cv::Mat m_trainPicture;
    QString m_filename;
    cv::Mat m_img;
    cv::Ptr<KNearest> m_model;
    vector<cv::Rect> m_RoiRect;
    Knn_Config m_config[4];
    QString m_result ;
};

#endif // KNNOPENCV_H
