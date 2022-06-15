#include "commonfunc.h"

bool sortVectorRect(const cv::Rect& r1 , const cv::Rect& r2){
    cv::Point2d p1(r1.x + (r1.width) / 2.0, r1.y + (r1.height) / 2.0) ;
    cv::Point2d p2(r2.x + (r2.width) / 2.0, r2.y + (r2.height) / 2.0);
    if ( p1.y  < 2 + p2.y ){
        if(p1.x  < p2.x ){
            return true;
        }else{
            return false;
        }
    }else{
        return false ;
    }
}

QImage cvMat2QImage(const cv::Mat& mat){
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1){
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++){
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++){
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3){
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4){
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else{
        return QImage();
    }
}


QString getHostIpAddress(){
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }
     // 如果没有找到，则以本地IP地址为IP
     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
     return strIpAddress;
}

QPoint getMinAndMax(QStringList list){
    QPoint out;
    out.setX(99999);
    out.setY(0);
    foreach (QString i , list){
        if (i.toInt() > out.y()){
           out.setY(i.toInt());
        }
        if (i.toDouble() < out.x()){
            out.setX(i.toInt());
        }
    }
    return out;
}
