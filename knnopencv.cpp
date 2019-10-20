#include "knnopencv.h"

KnnOpencv::KnnOpencv():
    m_GrayTholdValue(209),
    m_result("")
{
    QString src;
    m_config[0].name = 0;
    m_config[0].rc = cv::Rect(0,0 , 380, 40);

    m_config[1].name = 1;
    m_config[1].rc = cv::Rect(0,40 , 120, 20);

    m_config[2].name = 3;
    m_config[2].rc = cv::Rect(0,60 , 320 , 20);

    m_config[3].name = 9;
    m_config[3].rc = cv::Rect(0,100 , 140 , 20);

    m_img = imread(QApplication::applicationDirPath().toStdString() + "/knn_sample/sample.bmp", cv::IMREAD_COLOR);// 3 * 10 number mat origin data;
    //m_img = imread("E:/opencv-4.1.1/samples/data/digits.png");
    cv::Mat gray ;
    cv::Mat data , labels;
    cvtColor(m_img , gray , cv::COLOR_BGR2GRAY);
    int w = 20;
    int h = 20;
    //imshow("gray", gray);
    for (int index = 0 ;index < 4 ; index ++){
        cv::Mat temp = gray(m_config[index].rc);

        //imshow(format("%d",index), temp);
        int m = temp.rows / h ;
        int n = temp.cols / w ;
        for (int i = 0 ; i < m; i ++){
            int offsetRow = i * h;
            for (int j = 0 ; j < n; j++){
                int offsetCol = j * w;
                cv::Mat tmp;
                temp(cv::Range(offsetRow , offsetRow + h), cv::Range(offsetCol , offsetCol + w)).copyTo(tmp);
                if (m_config[index].name == 1){
                    //imshow("test1",tmp);
                }
                data.push_back(tmp.reshape(0,1));
                labels.push_back(m_config[index].name);
            }
        }
    }

    data.convertTo(data, CV_32F);

    //KNN
    int K = 5;
    cv::Ptr<TrainData> tData = TrainData::create(data , ROW_SAMPLE , labels);
    m_model = KNearest::create();
    m_model->setDefaultK(K);
    m_model->setIsClassifier(true);
    m_model->train(tData);
}

KnnOpencv::~KnnOpencv(){
    m_RoiRect.clear();
}

double KnnOpencv::GetLineLength(QPixmap& pix){
    cv::Mat  image = Qimage2Mat(pix.toImage());
    if (image.data == nullptr){
        return -1 ;
    }
    //imshow("Origin", image);
    //二值化
    cv::Mat image_gray  ;
    cvtColor(image , image_gray , cv::COLOR_RGB2GRAY );
    //imshow("Gray", image_gray);
    //处理
    cv::Mat image_my ;//= getUsefulArear(image_gray, 10  , 220);
    cv::threshold(image_gray , image_my , 10 , 255 , cv::THRESH_BINARY_INV);

    //imshow("getU", image_my);
    //霍夫直线检测
    std::vector<cv::Vec4i>lines ;
    cv::HoughLinesP(image_my,lines , 1.0 ,  CV_PI/ 180 ,50 , 20 , 10);
    //LinesFliter(lines);
    if (lines.size() > 1 ){
        double Max_Length = 0;
        for (size_t i = 0 ; i < lines.size(); i ++){
           cv::Point p1(lines[i][0],lines[i][1]) ,p2(lines[i][2],lines[i][3]);
           //std::cout << "line" << i << "\t" << getDistance(p1, p2)  <<  p1  << p2 << std::endl ; // print Info
           cv::line(image , p1, p2, cv::Scalar(0,0,255));
           if (Max_Length < getDistance(p1, p2)){
               Max_Length = getDistance(p1, p2);
           }
        }
        //imshow("After3", image);
        return Max_Length;
    }else if (lines.size() == 1){
        cv::Point p1(lines[0][0],lines[0][1]) ,p2(lines[0][2],lines[0][3]);
        cv::line(image , p1, p2, cv::Scalar(0,0,255));
        //imshow("After1", image);
        return  getDistance(p1, p2);
    }else{
        //imshow("After0", image);
        return -1;
    }

}

cv::Mat KnnOpencv::getUsefulArear(cv::Mat const& src, uchar t_min, uchar t_max){
    cv::Mat out = src.clone() ;
    for (int i = 0 ; i < out.rows ; i++){
        for(int j = 0 ; j < out.cols ; j++){
            if (out.at<uchar>(i,j) > t_min  &&  out.at<uchar>(i,j) < t_max){
                out.at<uchar>(i,j) = 0 ;
            }else{
                out.at<uchar>(i,j) = 255;
            }
        }
    }
    return out;
}

double KnnOpencv::getDistance(cv::Point const& p1 , cv::Point const& p2){
    return p2.x - p1.x ;
}

cv::Mat KnnOpencv::Qimage2Mat(QImage const& image){
    //QImage temp = image.convertToFormat(QImage::Format_RGB888);
    cv::Mat mat(image.height(), image.width(), CV_8UC4 , (void*)image.bits() , image.bytesPerLine());
    return mat ;
}


bool KnnOpencv::SetConfig(){
    return false;//TODO 暂时还用不上
}
int KnnOpencv::GetRuleType(QPixmap& pix){
    m_result = "";
    //QPixmap pix = QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId(),1841,873,17,8);
    //QPixmap pix = QPixmap(QApplication::applicationDirPath() + "/test.bmp").copy(1841,873,17,8);
    //QPixmap pix = QPixmap(QApplication::applicationDirPath() + "/Origin_screenshot_19.10.20192.bmp");
    cv::Mat  src_image = Qimage2Mat(pix.toImage());
    if (src_image.data == nullptr){
        //std::cout << "read image wrong Ptr is null" <<std::endl;
        return -1;
    }
    //imshow("Origin", src_image);
    m_RoiRect.clear();//clear old ROI data；

    //二值化
    cv::Mat image_gray , image ,dstImage = src_image.clone()  ;// image_canny;
    cv::cvtColor(src_image , image_gray , cv::COLOR_RGB2GRAY );
    //imshow("Gray",image_gray);
    //canny 处理
    //cv::Canny(image_gray ,image_canny , 70,220 );
    //cv::imshow("canny", image_canny);
    //处理
    cv::threshold(image_gray , image , m_GrayTholdValue , 255 , cv::THRESH_BINARY);
    //cv::imshow("After_deal",image);

    vector<vector<cv::Point>> contours ;
    vector<cv::Vec4i> hierarchy ;
    findContours(image, contours , hierarchy , cv::RETR_EXTERNAL , cv::CHAIN_APPROX_NONE);
    //drawContours(dstImage , contours , -1 , (255,255,255));
    //imshow("contours", dstImage);

    for(vector<vector<cv::Point>>::iterator it = contours.begin() ; it != contours.end() ; ++it){
        cv::Point2f vertex[4];
        cv::Rect rect = boundingRect(*it);
        if( rect.width > 10 || rect.height < 4 || rect.width < 1 ){
            continue;
        }
//        std::cout << rect << "\t" << rect.area() << "\t["<< rect.tl() << "]\t["<< rect.br() << "]" <<std::endl;

//        vertex[0] = rect.tl();
//        vertex[1] = cv::Point2f(rect.tl().x ,rect.br().y);
//        vertex[2] = rect.br();
//        vertex[3] = cv::Point2f(rect.br().x ,rect.tl().y);
//        for(int j = 0 ; j < 4; j++){
//            line(dstImage , vertex[j],vertex[(j+1)%4], Scalar(0,0,255), 1);
//        }
        m_RoiRect.push_back(rect);
    }
    sort(m_RoiRect.begin(), m_RoiRect.end(), sortVectorRect);

    //imshow("dstImage", dstImage);

    int j = 0;
    for(auto it = m_RoiRect.begin(); it!= m_RoiRect.end(); it++ , j++){
        //int k = con[j].order;
        cv::Rect2d rc= *it;
        cv::Mat num = image(rc).clone() , Format_num ;

        if(num.data == nullptr){
            continue;
        }
        if ( rc.width < 2  && rc.width > 0 ){
            m_result += "1";
            continue;
        }
        cv::resize(num , Format_num , cv::Size(20,20));
        //imshow(format("num %d", j+1), Format_num);
        Format_num.convertTo(Format_num, CV_32F);
        Format_num = Format_num.reshape(0,1);
        m_result += QString::number(m_model->predict(Format_num));
    }
    int result = m_result.toInt();
    if (result == 100 || result == 33 || result == 900 || result == 300){
        return result ;
    }else{
        return -1;
    }
}



































