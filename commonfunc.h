#ifndef COMMONFUNC_H
#define COMMONFUNC_H

#include <opencv2/opencv.hpp>
#include <QImage>
#include <QNetworkInterface>
#include <QList>

bool sortVectorRect(const cv::Rect& r1 , const cv::Rect& r2);

QImage cvMat2QImage(const cv::Mat& mat);
QString getHostIpAddress();
#endif // COMMONFUNC_H
