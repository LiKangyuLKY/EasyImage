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
    // ���ñ�־
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

    // ��opencv��MatתΪQImage
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
    // ���ļ����е�ͼƬ
    QOpenFile = QFileDialog::getOpenFileName(this,
                                            "��ѡ��һ��ͼƬ",
                                            "",
                                            "Image File(*.jpg *.png *.bmp *.tiff)");
    if (QOpenFile != "")
    {
        String OpenFile = String(QOpenFile.toLocal8Bit().toStdString());
        OriginImage = imread(OpenFile);
        // ��ʾͼ��
        QString tip = "ԭʼͼ��";
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
    // �ҶȻ�
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
    items << tr("����ʾ") << tr("��ʾ");
    QString item = QInputDialog::getItem(this, "����", "�Ƿ���ʾ�Ҷ�ֱ��ͼ:", items, 0, false, &ok1);

    // ��opencv��MatתΪQImage
    QImage qImg = MattoQImage(GrayImage);
    // ��ʾͼ��
    if (ok1)
    {
        // ��ʾͼ��
        QString tip = "�Ҷ�ͼ��";
        ShowImage(GrayImage, 2, tip);

        ChangeBool(true);
    }
    
    if (item == "����ʾ" && ok1)
    {
        return;
    }
    else if(item == "��ʾ" && ok1)
    {
        // ͳ��ÿ�����ص�����
        vector<int> HistImage(256);
        for (int i = 0; i < GrayImage.rows; i++)
        {
            for (int j = 0; j < GrayImage.cols; j++)
            {
                HistImage[GrayImage.at<uchar>(i, j)]++;
            }
        }
        // �������Ҷ�ֵ
        int max = 0;
        int i = 0;
        while (i < 256) {
            if (HistImage[i] >= max) {
                max = HistImage[i];
                i++;
            }
            else {i++;}
        }

        // ���ƻҶ�ֱ��ͼ
        Mat BgImage = Mat::zeros(Size(256, 256), CV_8UC1) + 255;
        double scale = 256 / double(max);

        for (int i = 0; i < 256; i++)
        {
            line(BgImage, Point(i, 255), Point(i, 255 - (HistImage[i] * scale)), Scalar(0, 0, 0));
        }
        // ��ʾͼ��
        QString tip = "�Ҷ�ֱ��ͼ";
        ShowImage(BgImage, 3, tip);
    }
}

void ImageProcessUI::ImageEnhancement()
{
    bool ok2 = false;
    QStringList items;
    items << tr("ֱ��ͼ��ֵ��") << tr("Laplace�ݶ���");
    QString item = QInputDialog::getItem(this, "����", "��ǿ����:", items, 0, false, &ok2);
    if (item == "ֱ��ͼ��ֵ��" && ok2)
    {
        equalizeHist(GrayImage, EnhanceImage); 
    }
    else if (item == "Laplace�ݶ���" && ok2)
    {
        Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0,-1, 5, -1, 0, -1, 0);
        filter2D(GrayImage, EnhanceImage, GrayImage.depth(), kernel);
    }
    // ��ʾͼ��
    if (ok2)
    {
        QString tip = "��ǿ���ͼ��";
        ShowImage(EnhanceImage, 3, tip);
    }
}

void ImageProcessUI::EdgeDetiction()
{
    bool ok3 = false;
    QStringList items;
    items << tr("Roberts��Ե�������") << tr("Sobel��Ե�������") << tr("Laplace��Ե�������") << tr("Prewitt��Ե�������") << tr("Canny��Ե�������");
    QString item = QInputDialog::getItem(this, "����", "��Ե���:", items, 0, true, &ok3);
    if (item == "Roberts��Ե�������" && ok3)
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
    else if (item == "Sobel��Ե�������" && ok3)
    {
        Mat kernel = (Mat_<char>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
        filter2D(GrayImage, EdgeImage, GrayImage.depth(), kernel);
    }
    else if (item == "Laplace��Ե�������" && ok3)
    {
        Mat kernel = (Mat_<char>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);
        filter2D(GrayImage, EdgeImage, GrayImage.depth(), kernel);
    }
    else if (item == "Prewitt��Ե�������" && ok3)
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
    else if (item == "Canny��Ե�������" && ok3)
    {
        Canny(GrayImage, EdgeImage, 80, 240);
    }
    // ��ʾͼ��
    if (ok3)
    {
        QString tip = "��Եͼ��";
        ShowImage(EdgeImage, 3, tip);
    } 
}

void ImageProcessUI::AddNoisy()
{
    bool ok = false;
    QStringList items;
    items << tr("��������") << tr("��˹����");
    QString item = QInputDialog::getItem(this, "����", "��������:", items, 0, true, &ok);
    if (item == "��������" && ok)
    {
        NoisyImage = GrayImage.clone();
        for (int k = 0; k < 200; k++)
        {
            //���ѡȡ����ֵ
            int i = rand() % NoisyImage.cols;
            int j = rand() % NoisyImage.rows;
            NoisyImage.at<uchar>(j, i) = 255;
        }
    }
    else if (item == "��˹����" && ok)
    {
        Mat noisy = Mat::zeros(GrayImage.rows, GrayImage.cols, GrayImage.type());
        RNG rng;
        rng.fill(noisy, RNG::NORMAL, 15, 30); // ���ɷ��ϸ�˹�ֲ��������
        NoisyImage = GrayImage + noisy;
    }
    // ��ʾͼ��
    if (ok)
    {
        ui.ImageFilterAction->setEnabled(true);;
        QString tip = "�����������ͼ��";
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
    // ��ʾͼ��
    QString tip = "�˲����ͼ��";
    ShowImage(dstFilterImage, 4, tip);
}

void ImageProcessUI::ImageFilter()
{

    bool ok4;
    int i = 0;
    QStringList items;
    items << tr("�����˲�") << tr("��ֵ�˲�") << tr("��˹�˲�") << tr("��ֵ�˲�") << tr("˫���˲�");
    QString item = QInputDialog::getItem(this, "����", "ͼ���˲�:", items, 0, true, &ok4);

    if (item == "�����˲�" && ok4)
    {
        whichSlider = 1;
        boxFilter(NoisyImage, FilterImage, -1, Size(3, 3));
    }
    else if (item == "��ֵ�˲�" && ok4)
    {
        whichSlider = 2;
        blur(NoisyImage, FilterImage, Size(3, 3));
    }
    else if (item == "��˹�˲�" && ok4)
    {
        whichSlider = 3;
        GaussianBlur(NoisyImage, FilterImage, Size(5, 5), 0, 0);
    }
    else if (item == "��ֵ�˲�" && ok4)
    {
        whichSlider = 4;
        medianBlur(NoisyImage, FilterImage, 3);
    }
    else if (item == "˫���˲�" && ok4)
    {
        whichSlider = 5;
        bilateralFilter(NoisyImage, FilterImage, 15, 20, 50);  
    }

    if (ok4)
    {
        ui.label->setEnabled(true);
        ui.SpinBox->setEnabled(true);
        ui.Slider->setEnabled(true);
        // ��ʾͼ��
        QString tip = "�˲����ͼ��";
        ShowImage(FilterImage, 4, tip);
    }
}

void ImageProcessUI::MorphologyFilter()
{
    bool ok5 = false;
    QStringList items;
    items << tr("��ʴ") << tr("����");
    QString item = QInputDialog::getItem(this, "����", "��̬ѧ�˲�:", items, 0, false, &ok5);
    Mat element = getStructuringElement(MORPH_RECT, Size(13, 13));
    if (item == "��ʴ" && ok5)
    {
        erode(OriginImage, FilterImage, element);
    }
    else if (item == "����" && ok5)
    {
        dilate(FilterImage, FilterImage, element);
    }
    // ��ʾͼ��
    if (ok5)
    {
        QString tip = "��̬ѧ�˲����ͼ��";
        ShowImage(FilterImage, 3, tip);
    }
}

void ImageProcessUI::InputModel()
{
    bool ok5;
    QString QFilter = QInputDialog::getText(this, "����", "���������˲��������磺\n 0 1 0 1 -5 1 0 1 0", QLineEdit::Normal,"0 1 0 1 -5 1 0 1 0", &ok5);
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
                    FilterNum[j++] = num;//�ۼ����Ĵ�С
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
        // ��ʾͼ��
        QImage qImg = MattoQImage(EnhanceImage);
        qImg = qImg.scaled(ui.label3->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui.label3->setStyleSheet("background-color:white");
        ui.label3->setAlignment(Qt::AlignCenter);
        ui.label3->setPixmap(QPixmap::fromImage(qImg));
        ui.label3->setToolTip("���������ͼ��");
    }
}

void ImageProcessUI::AffineTran()
{
    Point2f srcPoints[3], dstPoints[3];

    srcPoints[0] = Point2f(0, 0);
    srcPoints[1] = Point2f(0, OriginImage.rows);
    srcPoints[2] = Point2f(OriginImage.cols, 0);

    // ӳ��������
    dstPoints[0] = Point2f(0, OriginImage.rows * 0.3);
    dstPoints[1] = Point2f(OriginImage.cols * 0.25, OriginImage.rows * 0.75);
    dstPoints[2] = Point2f(OriginImage.cols * 0.75, OriginImage.rows * 0.25);


    Mat warpMat = getAffineTransform(srcPoints, dstPoints);

    warpAffine(OriginImage, TranImage, warpMat, OriginImage.size());

    // ��ʾͼ��
    QString tip = "����任���ͼ��";
    ShowImage(TranImage, 3, tip);
}

void ImageProcessUI::PerspectiveTran()
{
    Point2f srcPoints[4], dstPoints[4];

    srcPoints[0] = Point2f(0, 0);
    srcPoints[1] = Point2f(OriginImage.cols, 0);
    srcPoints[2] = Point2f(0, OriginImage.rows);
    srcPoints[3] = Point2f(OriginImage.cols, OriginImage.rows);

    // ӳ��������
    dstPoints[0] = Point2f(OriginImage.cols * 0.05, OriginImage.rows * 0.33);
    dstPoints[1] = Point2f(OriginImage.cols * 0.9, OriginImage.rows * 0.25);
    dstPoints[2] = Point2f(OriginImage.cols * 0.2, OriginImage.rows * 0.7);
    dstPoints[3] = Point2f(OriginImage.cols * 0.8, OriginImage.rows * 0.9);

    Mat warpMat = getPerspectiveTransform(srcPoints, dstPoints);

    warpPerspective(OriginImage, TranImage, warpMat, OriginImage.size());

    // ��ʾͼ��
    QString tip = "͸�ӱ任���ͼ��";
    ShowImage(TranImage, 4, tip);
}

void ImageProcessUI::Threshold()
{
    threshold(GrayImage, ThreshImage, 150, 255, THRESH_TOZERO);
    // ��ʾͼ��
    QString tip = "��ֵ�ָ���ͼ��";
    ShowImage(ThreshImage, 4, tip);
}

void ImageProcessUI::Otsu()
{
    threshold(GrayImage, ThreshImage, 0, 255, CV_THRESH_OTSU);
    // ��ʾͼ��
    QString tip = "��ֵ�ָ���ͼ��";
    ShowImage(ThreshImage, 4, tip);
}

void ImageProcessUI::Match()
{
    Mat TemplateImage, dst, display;
    display = OriginImage.clone();
    // ���ļ����е�ͼƬ
    QOpenFile = QFileDialog::getOpenFileName(this,
        "��ѡ��ģ��ͼ��",
        "",
        "Image File(*.jpg *.png *.bmp *.tiff)");
    if (QOpenFile != "")
    {
        String OpenFile = String(QOpenFile.toLocal8Bit().toStdString());
        TemplateImage = imread(OpenFile);
        // ��ʾͼ��
        QString tip = "ģ��ͼ��";
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
    QString tip = "ƥ����";
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
    pCapture = VideoCapture("F:/ͼ��ɼ��봦��γ�/ImageProcessUI/image/pets2001.avi");

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
        QString tip1 = "ԭʼ��Ƶ";
        QString tip2 = "Ŀ����";
        ShowImage(pframe, 3, tip1);
        ShowImage(foreground2, 4, tip2);
        waitKey(10);
    }
    waitKey();
}

void ImageProcessUI::AboutEasyImage()
{
    QMessageBox::about(this, "����Easy Image", "Easy Image�ǶԱ�������ѧͼ��ɼ��봦��γ̵��ܽᣬ�ÿγ��ڿ���ʦΪ���ʦ");
}

void ImageProcessUI::AboutAuthor()
{
    QMessageBox about;
    about.setWindowTitle("��������");
    about.setInformativeText("��ӭɨ���ע�ҵ�΢�Ź��ںţ�һ��AI");
    about.setIconPixmap(QPixmap("F:/ͼ��ɼ��봦��γ�/ImageProcessUI/ico/GZH.png"));
    about.show();
    about.exec();
}