#include "ImageProcessUI.h"
#include <iostream>

using namespace std;
using namespace cv;
#pragma execution_character_set("utf-8")

ImageProcessUI::ImageProcessUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.OpenImageAction, SIGNAL(triggered()), this, SLOT(OpenImage()));
    connect(ui.ImageGrayAction, SIGNAL(triggered()), this, SLOT(ImageGray()));
    connect(ui.ImageEnhancementAction, SIGNAL(triggered()), this, SLOT(ImageEnhancement()));
    connect(ui.EdgeDetictionAction, SIGNAL(triggered()), this, SLOT(EdgeDetiction()));
    connect(ui.AddNoisyAction, SIGNAL(triggered()), this, SLOT(AddNoisy()));
    connect(ui.ImageFilterAction, SIGNAL(triggered()), this, SLOT(ImageFilter()));
    connect(ui.MorphologyAction, SIGNAL(triggered()), this, SLOT(MorphologyFilter()));
    connect(ui.InputModelAction, SIGNAL(triggered()), this, SLOT(InputModel()));
    connect(ui.AffineAction, SIGNAL(triggered()), this, SLOT(AffineTran()));
    connect(ui.PerspectiveAction, SIGNAL(triggered()), this, SLOT(PerspectiveTran()));

    connect(ui.ThresholdAction, SIGNAL(triggered()), this, SLOT(Threshold()));
    connect(ui.OtsuAction, SIGNAL(triggered()), this, SLOT(Otsu()));

    connect(ui.MatchAction, SIGNAL(triggered()), this, SLOT(Match()));
    connect(ui.GaussinAction, SIGNAL(triggered()), this, SLOT(GaussianBg()));

    connect(ui.AboutEasyImageAction, SIGNAL(triggered()), this, SLOT(AboutEasyImage()));
    connect(ui.AboutAuthorAction, SIGNAL(triggered()), this, SLOT(AboutAuthor()));

    connect(ui.SpinBox, SIGNAL(valueChanged(int)), this, SLOT(SpinBoxChangeImageshow()));
    connect(ui.Slider, SIGNAL(valueChanged(int)), this, SLOT(SliderChangeImageshow()));
    
}
void ImageProcessUI::ChangeBool(bool flag)
{
    // 设置标志
    ui.ImageGrayAction->setEnabled(flag);
    ui.ImageEnhancementAction->setEnabled(flag);
    ui.EdgeDetictionAction->setEnabled(flag);
    ui.AddNoisyAction->setEnabled(flag);
    ui.InputModelAction->setEnabled(flag);
    ui.OtsuAction->setEnabled(flag);
    ui.ThresholdAction->setEnabled(flag);
}
QImage MattoQImage(Mat img)
{
    QImage qImg;
    Mat image = img.clone();
    if (image.channels() == 3)
    {
        cvtColor(img, image, CV_BGR2RGB);
        qImg = QImage((const uchar*)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);
    }
    else
    {
        qImg = QImage((const uchar*)(image.data), image.cols, image.rows, image.step, QImage::Format_Grayscale8);
    }
    
    qImg.bits();
    return qImg;
}

void ImageProcessUI::ShowImage(Mat img, int numLabel, QString tip)
{

    // 将opencv的Mat转为QImage
    QImage qImg = MattoQImage(img);
    switch (numLabel)
    {
    case 1:
    {
        qImg = qImg.scaled(ui.label1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui.label1->setStyleSheet("background-color:white");
        ui.label1->setAlignment(Qt::AlignCenter);
        ui.label1->setPixmap(QPixmap::fromImage(qImg));
        ui.label1->setToolTip(tip); 
        break;
    }
    case 2: 
    {
        qImg = qImg.scaled(ui.label2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui.label2->setStyleSheet("background-color:white");
        ui.label2->setAlignment(Qt::AlignCenter);
        ui.label2->setPixmap(QPixmap::fromImage(qImg));
        ui.label2->setToolTip(tip); 
        break;
    }
    case 3:
    {
        qImg = qImg.scaled(ui.label3->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui.label3->setStyleSheet("background-color:white");
        ui.label3->setAlignment(Qt::AlignCenter);
        ui.label3->setPixmap(QPixmap::fromImage(qImg));
        ui.label3->setToolTip(tip); 
        break;
    }
    case 4: 
    {
        qImg = qImg.scaled(ui.label4->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui.label4->setStyleSheet("background-color:white");
        ui.label4->setAlignment(Qt::AlignCenter);
        ui.label4->setPixmap(QPixmap::fromImage(qImg));
        ui.label4->setToolTip(tip); 
        break;
    }
    default:
        break;
    }
}

void ImageProcessUI::OpenImage()
{
    // 打开文件夹中的图片
    QOpenFile = QFileDialog::getOpenFileName(this,
                                            "请选择一张图片",
                                            "",
                                            "Image File(*.jpg *.png *.bmp *.tiff)");
    if (QOpenFile != "")
    {
        String OpenFile = String(QOpenFile.toLocal8Bit().toStdString());
        OriginImage = imread(OpenFile);
        // 显示图像
        QString tip = "原始图像";
        ShowImage(OriginImage, 1 , tip);
        ui.ImageGrayAction->setEnabled(true);
        ui.MorphologyAction->setEnabled(true);
        ui.AffineAction->setEnabled(true);
        ui.PerspectiveAction->setEnabled(true);
    }    
}

void ImageProcessUI::ImageGray()
{
    bool ok1 = false;
    // 灰度化
    GrayImage.create(OriginImage.size(), CV_8UC1);
    for (int i = 0; i < OriginImage.rows; i++)
    {
        for (int j = 0; j < OriginImage.cols; j++)
        {
            GrayImage.at<uchar>(i, j) = 0.3 * OriginImage.at<Vec3b>(i, j)[0]
                + 0.59 * OriginImage.at<Vec3b>(i, j)[1]
                + 0.11 * OriginImage.at<Vec3b>(i, j)[2];
        }
    }
    QStringList items;
    items << tr("不显示") << tr("显示");
    QString item = QInputDialog::getItem(this, "设置", "是否显示灰度直方图:", items, 0, false, &ok1);

    // 将opencv的Mat转为QImage
    QImage qImg = MattoQImage(GrayImage);
    // 显示图像
    if (ok1)
    {
        // 显示图像
        QString tip = "灰度图像";
        ShowImage(GrayImage, 2, tip);

        ChangeBool(true);
    }
    
    if (item == "不显示" && ok1)
    {
        return;
    }
    else if(item == "显示" && ok1)
    {
        // 统计每种像素的数量
        vector<int> HistImage(256);
        for (int i = 0; i < GrayImage.rows; i++)
        {
            for (int j = 0; j < GrayImage.cols; j++)
            {
                HistImage[GrayImage.at<uchar>(i, j)]++;
            }
        }
        // 计算最大灰度值
        int max = 0;
        int i = 0;
        while (i < 256) {
            if (HistImage[i] >= max) {
                max = HistImage[i];
                i++;
            }
            else {i++;}
        }

        // 绘制灰度直方图
        Mat BgImage = Mat::zeros(Size(256, 256), CV_8UC1) + 255;
        double scale = 256 / double(max);

        for (int i = 0; i < 256; i++)
        {
            line(BgImage, Point(i, 255), Point(i, 255 - (HistImage[i] * scale)), Scalar(0, 0, 0));
        }
        // 显示图像
        QString tip = "灰度直方图";
        ShowImage(BgImage, 3, tip);
    }
}

void ImageProcessUI::ImageEnhancement()
{
    bool ok2 = false;
    QStringList items;
    items << tr("直方图均值化") << tr("Laplace梯度锐化");
    QString item = QInputDialog::getItem(this, "设置", "增强策略:", items, 0, false, &ok2);
    if (item == "直方图均值化" && ok2)
    {
        equalizeHist(GrayImage, EnhanceImage); 
    }
    else if (item == "Laplace梯度锐化" && ok2)
    {
        Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0,-1, 5, -1, 0, -1, 0);
        filter2D(GrayImage, EnhanceImage, GrayImage.depth(), kernel);
    }
    // 显示图像
    if (ok2)
    {
        QString tip = "增强后的图像";
        ShowImage(EnhanceImage, 3, tip);
    }
}

void ImageProcessUI::EdgeDetiction()
{
    bool ok3 = false;
    QStringList items;
    items << tr("Roberts边缘检测算子") << tr("Sobel边缘检测算子") << tr("Laplace边缘检测算子") << tr("Prewitt边缘检测算子") << tr("Canny边缘检测算子");
    QString item = QInputDialog::getItem(this, "设置", "边缘检测:", items, 0, true, &ok3);
    if (item == "Roberts边缘检测算子" && ok3)
    {
        Mat EdgeImageX;
        Mat EdgeImageY;
        Mat kernelx  = (Mat_<char>(2, 2) << 1, 0, 0, -1);
        Mat kernely = (Mat_<char>(2, 2) << 0, 1, -1, 0);
        filter2D(GrayImage, EdgeImageX, GrayImage.depth(), kernelx);
        filter2D(GrayImage, EdgeImageY, GrayImage.depth(), kernely);
        convertScaleAbs(EdgeImageX, EdgeImageX);
        convertScaleAbs(EdgeImageY, EdgeImageY);
        add(EdgeImageX, EdgeImageY, EdgeImage);
        convertScaleAbs(EdgeImage, EdgeImage);
    }
    else if (item == "Sobel边缘检测算子" && ok3)
    {
        Mat kernel = (Mat_<char>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
        filter2D(GrayImage, EdgeImage, GrayImage.depth(), kernel);
    }
    else if (item == "Laplace边缘检测算子" && ok3)
    {
        Mat kernel = (Mat_<char>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);
        filter2D(GrayImage, EdgeImage, GrayImage.depth(), kernel);
    }
    else if (item == "Prewitt边缘检测算子" && ok3)
    {
        Mat EdgeImageX;
        Mat EdgeImageY;
        Mat kernelx = (Mat_<char>(3, 3) << 1, 1, 1, 0, 0, 0, -1, -1, -1);
        Mat kernely = (Mat_<char>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
        filter2D(GrayImage, EdgeImageX, GrayImage.depth(), kernelx);
        filter2D(GrayImage, EdgeImageY, GrayImage.depth(), kernely);
        convertScaleAbs(EdgeImageX, EdgeImageX);
        convertScaleAbs(EdgeImageY, EdgeImageY);
        max(EdgeImageX, EdgeImageY, EdgeImage);
        convertScaleAbs(EdgeImage, EdgeImage);
    }
    else if (item == "Canny边缘检测算子" && ok3)
    {
        Canny(GrayImage, EdgeImage, 80, 240);
    }
    // 显示图像
    if (ok3)
    {
        QString tip = "边缘图像";
        ShowImage(EdgeImage, 3, tip);
    } 
}

void ImageProcessUI::AddNoisy()
{
    bool ok = false;
    QStringList items;
    items << tr("椒盐噪声") << tr("高斯噪声");
    QString item = QInputDialog::getItem(this, "设置", "加入噪声:", items, 0, true, &ok);
    if (item == "椒盐噪声" && ok)
    {
        NoisyImage = GrayImage.clone();
        for (int k = 0; k < 200; k++)
        {
            //随机选取行列值
            int i = rand() % NoisyImage.cols;
            int j = rand() % NoisyImage.rows;
            NoisyImage.at<uchar>(j, i) = 255;
        }
    }
    else if (item == "高斯噪声" && ok)
    {
        Mat noisy = Mat::zeros(GrayImage.rows, GrayImage.cols, GrayImage.type());
        RNG rng;
        rng.fill(noisy, RNG::NORMAL, 15, 30); // 生成符合高斯分布的随机数
        NoisyImage = GrayImage + noisy;
    }
    // 显示图像
    if (ok)
    {
        ui.ImageFilterAction->setEnabled(true);;
        QString tip = "加入噪声后的图像";
        ShowImage(NoisyImage, 3, tip);
    }
}

void ImageProcessUI::SpinBoxChangeImageshow()
{
    ui.Slider->setValue(ui.SpinBox->value());
}

void ImageProcessUI::SliderChangeImageshow()
{
    Mat tempFilterImage = NoisyImage.clone();
    Mat dstFilterImage;
    ui.SpinBox->setValue(ui.Slider->value());
    int kernelSize = ui.Slider->value() * 2 + 3;
    switch (whichSlider)
    {
    case 1: 
        boxFilter(tempFilterImage, dstFilterImage, -1, Size(kernelSize, kernelSize));
        break;
    case 2: 
        blur(tempFilterImage, dstFilterImage, Size(kernelSize, kernelSize));
        break;
    case 3:
        GaussianBlur(tempFilterImage, dstFilterImage, Size(kernelSize, kernelSize), 0, 0);
        break;
    case 4:
        medianBlur(tempFilterImage, dstFilterImage, kernelSize);
        break;
    case 5:
        bilateralFilter(tempFilterImage, dstFilterImage, kernelSize + 7, (kernelSize + 7) * 2, (kernelSize + 7) / 2);
        break;
    default:
        break;
    }
    // 显示图像
    QString tip = "滤波后的图像";
    ShowImage(dstFilterImage, 4, tip);
}

void ImageProcessUI::ImageFilter()
{

    bool ok4;
    int i = 0;
    QStringList items;
    items << tr("方框滤波") << tr("均值滤波") << tr("高斯滤波") << tr("中值滤波") << tr("双边滤波");
    QString item = QInputDialog::getItem(this, "设置", "图像滤波:", items, 0, true, &ok4);

    if (item == "方框滤波" && ok4)
    {
        whichSlider = 1;
        boxFilter(NoisyImage, FilterImage, -1, Size(3, 3));
    }
    else if (item == "均值滤波" && ok4)
    {
        whichSlider = 2;
        blur(NoisyImage, FilterImage, Size(3, 3));
    }
    else if (item == "高斯滤波" && ok4)
    {
        whichSlider = 3;
        GaussianBlur(NoisyImage, FilterImage, Size(5, 5), 0, 0);
    }
    else if (item == "中值滤波" && ok4)
    {
        whichSlider = 4;
        medianBlur(NoisyImage, FilterImage, 3);
    }
    else if (item == "双边滤波" && ok4)
    {
        whichSlider = 5;
        bilateralFilter(NoisyImage, FilterImage, 15, 20, 50);  
    }

    if (ok4)
    {
        ui.label->setEnabled(true);
        ui.SpinBox->setEnabled(true);
        ui.Slider->setEnabled(true);
        // 显示图像
        QString tip = "滤波后的图像";
        ShowImage(FilterImage, 4, tip);
    }
}

void ImageProcessUI::MorphologyFilter()
{
    bool ok5 = false;
    QStringList items;
    items << tr("腐蚀") << tr("膨胀");
    QString item = QInputDialog::getItem(this, "设置", "形态学滤波:", items, 0, false, &ok5);
    Mat element = getStructuringElement(MORPH_RECT, Size(13, 13));
    if (item == "腐蚀" && ok5)
    {
        erode(OriginImage, FilterImage, element);
    }
    else if (item == "膨胀" && ok5)
    {
        dilate(FilterImage, FilterImage, element);
    }
    // 显示图像
    if (ok5)
    {
        QString tip = "形态学滤波后的图像";
        ShowImage(FilterImage, 3, tip);
    }
}

void ImageProcessUI::InputModel()
{
    bool ok5;
    QString QFilter = QInputDialog::getText(this, "设置", "请输入卷积核参数，例如：\n 0 1 0 1 -5 1 0 1 0", QLineEdit::Normal,"0 1 0 1 -5 1 0 1 0", &ok5);
    if (ok5)
    {
        if (QFilter.isEmpty() == true) { return; }
        string filter = QFilter.toStdString();
        int len = filter.length();
        int FilterNum[9];
        int j = 0;
        int num = 0;
        for (int i = 0; i < len; i++)
        {
            if (filter[i] == '-')
            {
                i++;
                while (filter[i] != ' ' && filter[i] != '\0')
                {
                    num = num * 10 + filter[i] - '0';
                    i++;
                }
                FilterNum[j++] = (-1) * num;
                num = 0;
            }
            else if (filter[i] == ' ')
            {
                FilterNum[j++] = num;
                num = 0;
            }
            else
            {
                num = num * 10 + filter[i] - '0';
                if (filter[i + 1] == '\0')
                {
                    FilterNum[j++] = num;//累计数的大小
                    num = 0;
                }
            }
        }

        EnhanceImage = GrayImage.clone();
        for (int i = 1; i < GrayImage.rows - 1; i++)
        {
            for (int j = 1; j < GrayImage.cols - 1; j++)
            {
                EnhanceImage.at<uchar>(i, j) = saturate_cast<uchar>(FilterNum[0] * GrayImage.at<uchar>(i - 1, j - 1) + FilterNum[1] * GrayImage.at<uchar>(i - 1, j) + FilterNum[2] * GrayImage.at<uchar>(i - 1, j + 1)
                    + FilterNum[3] * GrayImage.at<uchar>(i, j - 1) + FilterNum[4] * GrayImage.at<uchar>(i, j) + FilterNum[5] * GrayImage.at<uchar>(i, j + 1)
                    + FilterNum[6] * GrayImage.at<uchar>(i + 1, j - 1) + FilterNum[7] * GrayImage.at<uchar>(i + 1, j) + FilterNum[8] * GrayImage.at<uchar>(i + 1, j + 1));
            }
        }
        // 显示图像
        QImage qImg = MattoQImage(EnhanceImage);
        qImg = qImg.scaled(ui.label3->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui.label3->setStyleSheet("background-color:white");
        ui.label3->setAlignment(Qt::AlignCenter);
        ui.label3->setPixmap(QPixmap::fromImage(qImg));
        ui.label3->setToolTip("卷积处理后的图像");
    }
}

void ImageProcessUI::AffineTran()
{
    Point2f srcPoints[3], dstPoints[3];

    srcPoints[0] = Point2f(0, 0);
    srcPoints[1] = Point2f(0, OriginImage.rows);
    srcPoints[2] = Point2f(OriginImage.cols, 0);

    // 映射后的坐标
    dstPoints[0] = Point2f(0, OriginImage.rows * 0.3);
    dstPoints[1] = Point2f(OriginImage.cols * 0.25, OriginImage.rows * 0.75);
    dstPoints[2] = Point2f(OriginImage.cols * 0.75, OriginImage.rows * 0.25);


    Mat warpMat = getAffineTransform(srcPoints, dstPoints);

    warpAffine(OriginImage, TranImage, warpMat, OriginImage.size());

    // 显示图像
    QString tip = "仿射变换后的图像";
    ShowImage(TranImage, 3, tip);
}

void ImageProcessUI::PerspectiveTran()
{
    Point2f srcPoints[4], dstPoints[4];

    srcPoints[0] = Point2f(0, 0);
    srcPoints[1] = Point2f(OriginImage.cols, 0);
    srcPoints[2] = Point2f(0, OriginImage.rows);
    srcPoints[3] = Point2f(OriginImage.cols, OriginImage.rows);

    // 映射后的坐标
    dstPoints[0] = Point2f(OriginImage.cols * 0.05, OriginImage.rows * 0.33);
    dstPoints[1] = Point2f(OriginImage.cols * 0.9, OriginImage.rows * 0.25);
    dstPoints[2] = Point2f(OriginImage.cols * 0.2, OriginImage.rows * 0.7);
    dstPoints[3] = Point2f(OriginImage.cols * 0.8, OriginImage.rows * 0.9);

    Mat warpMat = getPerspectiveTransform(srcPoints, dstPoints);

    warpPerspective(OriginImage, TranImage, warpMat, OriginImage.size());

    // 显示图像
    QString tip = "透视变换后的图像";
    ShowImage(TranImage, 4, tip);
}

void ImageProcessUI::Threshold()
{
    threshold(GrayImage, ThreshImage, 150, 255, THRESH_TOZERO);
    // 显示图像
    QString tip = "阈值分割后的图像";
    ShowImage(ThreshImage, 4, tip);
}

void ImageProcessUI::Otsu()
{
    threshold(GrayImage, ThreshImage, 0, 255, CV_THRESH_OTSU);
    // 显示图像
    QString tip = "阈值分割后的图像";
    ShowImage(ThreshImage, 4, tip);
}

void ImageProcessUI::Match()
{
    Mat TemplateImage, dst, display;
    display = OriginImage.clone();
    // 打开文件夹中的图片
    QOpenFile = QFileDialog::getOpenFileName(this,
        "请选择模板图像",
        "",
        "Image File(*.jpg *.png *.bmp *.tiff)");
    if (QOpenFile != "")
    {
        String OpenFile = String(QOpenFile.toLocal8Bit().toStdString());
        TemplateImage = imread(OpenFile);
        // 显示图像
        QString tip = "模板图像";
        QImage qImg = MattoQImage(TemplateImage);
        ui.label3->setStyleSheet("background-color:white");
        ui.label3->setAlignment(Qt::AlignCenter);
        ui.label3->setPixmap(QPixmap::fromImage(qImg));
        ui.label3->setToolTip(tip);

    }

    matchTemplate(OriginImage, TemplateImage, dst, CV_TM_CCORR_NORMED);

    normalize(dst, dst, 0, 1, NORM_MINMAX);

    double minValue, maxValue;
    Point minLocation, maxLocation;
    Point matchLocation;
    minMaxLoc(dst, &minValue, &maxValue, &minLocation, &maxLocation, Mat());

    matchLocation = maxLocation;

    rectangle(display, matchLocation, Point(matchLocation.x + TemplateImage.cols, matchLocation.y + TemplateImage.rows), Scalar(0, 0, 0));
    QString tip = "匹配结果";
    ShowImage(display, 4, tip);
}

void ImageProcessUI::GaussianBg()
{
    Mat greyimg;
    Mat foreground, foreground2;
    Ptr<BackgroundSubtractorKNN> ptrKNN = createBackgroundSubtractorKNN(100, 400, true);
    Ptr<BackgroundSubtractorMOG2> mog2 = createBackgroundSubtractorMOG2(100, 25, true);

    VideoCapture pCapture;
    Mat pframe;
    pCapture = VideoCapture("F:/图像采集与处理课程/ImageProcessUI/image/pets2001.avi");

    while (1)
    {
        pCapture >> pframe;
        if (pframe.data == NULL)
            return;
        cvtColor(pframe, greyimg, CV_BGR2GRAY);
        long long t = getTickCount();
        ptrKNN->apply(pframe, foreground, 0.01);
        long long t1 = getTickCount();
        mog2->apply(greyimg, foreground2, -1);
        long long t2 = getTickCount();

        //imshow("Extracted Foreground", foreground);
        QString tip1 = "原始视频";
        QString tip2 = "目标检测";
        ShowImage(pframe, 3, tip1);
        ShowImage(foreground2, 4, tip2);
        waitKey(10);
    }
    waitKey();
}

void ImageProcessUI::AboutEasyImage()
{
    QMessageBox::about(this, "关于Easy Image", "Easy Image是对北京理工大学图像采集与处理课程的总结，该课程授课老师为李静老师");
}

void ImageProcessUI::AboutAuthor()
{
    QMessageBox about;
    about.setWindowTitle("关于作者");
    about.setInformativeText("欢迎扫码关注我的微信公众号：一刻AI");
    about.setIconPixmap(QPixmap("F:/图像采集与处理课程/ImageProcessUI/ico/GZH.png"));
    about.show();
    about.exec();
}