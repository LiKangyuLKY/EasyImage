#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ImageProcessUI.h"
#include <QFileDialog>
#include <QImage>
#include <QAction>
#include <opencv2/opencv.hpp>
#include <QInputDialog>
#include <QMessageBox>

using namespace std;
using namespace cv;

class ImageProcessUI : public QMainWindow
{
    Q_OBJECT


public:
    explicit ImageProcessUI(QWidget* parent = Q_NULLPTR);

private slots:
    void ChangeBool(bool flag);  // 标志位
    void OpenImage();    // 打开图像
    void ImageGray();    // 图像灰度化
    void ShowImage(Mat img , int numLabel , QString tip);   // 显示图像
    void ImageEnhancement();  // 图像增强
    void EdgeDetiction();   // 边缘提取
    void MorphologyFilter(); // 形态学滤波
    void ImageFilter();   // 图像滤波
    void AddNoisy();  // 加入噪声
    void AffineTran(); // 仿射变换
    void PerspectiveTran(); // 透射变换
    void InputModel();      // 输入模式
    void SpinBoxChangeImageshow();
    void SliderChangeImageshow();
    void Threshold();
    void Otsu();
    void Match(); // 模板匹配
    void GaussianBg(); // 高斯背景建模

    void AboutEasyImage();    // 关于Easy Image
    void AboutAuthor();      // 关于作者

private:
    Ui::ImageProcessUIClass ui;
    Mat OriginImage;    // 原始图像
    Mat GrayImage;      // 灰度图像
    Mat EnhanceImage;    // 锐化
    Mat EdgeImage;  // 边缘
    Mat NoisyImage;  // 噪声图像
    Mat FilterImage; // 滤波
    Mat TranImage;  // 几何变换
    Mat ThreshImage; // 分割
    int whichSlider = 0;
    QString QOpenFile;   // 图像路径
};
