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
    void ChangeBool(bool flag);  // ��־λ
    void OpenImage();    // ��ͼ��
    void ImageGray();    // ͼ��ҶȻ�
    void ShowImage(Mat img , int numLabel , QString tip);   // ��ʾͼ��
    void ImageEnhancement();  // ͼ����ǿ
    void EdgeDetiction();   // ��Ե��ȡ
    void MorphologyFilter(); // ��̬ѧ�˲�
    void ImageFilter();   // ͼ���˲�
    void AddNoisy();  // ��������
    void AffineTran(); // ����任
    void PerspectiveTran(); // ͸��任
    void InputModel();      // ����ģʽ
    void SpinBoxChangeImageshow();
    void SliderChangeImageshow();
    void Threshold();
    void Otsu();
    void Match(); // ģ��ƥ��
    void GaussianBg(); // ��˹������ģ

    void AboutEasyImage();    // ����Easy Image
    void AboutAuthor();      // ��������

private:
    Ui::ImageProcessUIClass ui;
    Mat OriginImage;    // ԭʼͼ��
    Mat GrayImage;      // �Ҷ�ͼ��
    Mat EnhanceImage;    // ��
    Mat EdgeImage;  // ��Ե
    Mat NoisyImage;  // ����ͼ��
    Mat FilterImage; // �˲�
    Mat TranImage;  // ���α任
    Mat ThreshImage; // �ָ�
    int whichSlider = 0;
    QString QOpenFile;   // ͼ��·��
};
