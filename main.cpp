#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using std::cout;
using std::cin;
using std::endl;
using namespace cv;
void imhist(Mat image, int histogram[])
{

    for(int i = 0; i < 256; i++)
    {
        histogram[i] = 0;

    }
    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)

            histogram[(int)image.at<uchar>(y,x)]++;
}
void cumhist(int histogram[], int cumhistogram[])
{
    cumhistogram[0] = histogram[0];

    for(int i = 1; i < 256; i++)
    {
        cumhistogram[i] = histogram[i] + cumhistogram[i-1];
    }
}
void histDisplay(int histogram[], const char* name)
{
    int hist[256];
    for(int i = 0; i < 256; i++)
    {
        hist[i]=histogram[i];
    }
    int hist_w = 600; int hist_h = 355;
    int bin_w = cvRound((double) hist_w/256);

    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));

    int max = hist[0];
    for(int i = 1; i < 256; i++){
        if(max < hist[i]){
            max = hist[i];
        }
    }
    for(int i = 0; i < 256; i++){
        hist[i] = ((double)hist[i]/max)*histImage.rows;
    }
    for(int i = 0; i < 256; i++)
    {
        line(histImage, Point(bin_w*(i), hist_h),
             Point(bin_w*(i), hist_h - hist[i]),
             Scalar(0,0,0), 1, 8, 0);
    }
    namedWindow(name, CV_WINDOW_AUTOSIZE);
    imshow(name, histImage);
}
int main()
{
    Mat image = imread("/home/azamat/ClionProjects/Aizobraj/a/apple.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    int histograma[257] = {0};

       for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++) {
            Vec3b intensity = image.at<Vec3b>(Point(j, i));

        //    histogram[(int)image.at<uchar>(i,j)]++;
            int Green = intensity.val[0];
            histograma[Green] = histograma[Green] + 1;
        }

    Mat HistPlotG (355, 600, CV_8UC3, Scalar(255, 255, 255));
        for (int i = 0; i < 256; i=i+2)
    {
            line(HistPlotG, Point(i, 355), Point(i, 600-histograma[i]), Scalar(0, 255, 0),1,8,0);
        }
   int histogram[256];
    imhist(image, histogram);
    int size = image.rows * image.cols;
    float alpha = 255.0/size;
    float PrRk[256];
    for(int i = 0; i < 256; i++)
    {
        PrRk[i] = (double)histogram[i] / size;
    }
    int cumhistogram[256];
    cumhist(histogram,cumhistogram );
    int Sk[256];
    for(int i = 0; i < 256; i++)
    {
        Sk[i] = cvRound((double)cumhistogram[i] * alpha);
    }
    float PsSk[256];
    for(int i = 0; i < 256; i++)
    {
        PsSk[i] = 0;
    }

    for(int i = 0; i < 256; i++)
    {
        PsSk[Sk[i]] += PrRk[i];
    }

    int final[256];
    for(int i = 0; i < 256; i++)
        final[i] = cvRound(PsSk[i] * 255.0 * 255.0);
       // final[i] = cvRound(PsSk[i]*255);
    Mat new_image = image.clone();
    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
            new_image.at<uchar>(y,x) = saturate_cast<uchar>(Sk[image.at<uchar>(y,x)]);
    namedWindow("Original Image");
    imshow("Original Image", image);
    histDisplay(histogram, "Original Histogram");
    namedWindow("Equilized Image");
    imshow("Equilized Image",new_image);
    histDisplay(final, "Equilized Histogram");
    namedWindow("Green Histogram");
    imshow("Green Histogram", HistPlotG);

    waitKey();
    return 0;
}