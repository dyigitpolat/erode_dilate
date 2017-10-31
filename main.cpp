#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void mysave(const String &name, Mat m)
{
    vector<int> params;
    params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    params.push_back(5);
    try
    {
        imwrite("./results/"+name+".png", m, params);
        cout << "./results/"+name+".png" << endl;
    }
    catch(runtime_error& ex)
    {
        printf("Exception converting image to PNG format: %s\n", ex.what());
    }

}

void myshow(const String &name, Mat m)
{
    cv::imshow(name, m);
    moveWindow(name, 150,150);
}

Mat my_erode(Mat img, Mat se)
{
    Mat out = Mat(img.rows, img.cols, 0);
    out = out * 0;
    int H = out.rows;
    int W = out.cols;
    int Hs = se.rows;
    int Ws = se.cols;
    
    //assuming img is binary: 255 is 1.
    for(int i = 0; i < H-Hs; i++)
    {
        for(int j = 0; j < W-Ws; j++)
        {
            int val = 255;
            for(int x = 0; x < Hs; x++)
            {
                for(int y = 0; y < Ws; y++)
                {
                    if(se.data[x*Ws + y] && !img.data[(i+x)*W + (j+y)])
                    {
                        val = 0;
                    }
                }
            }
            out.data[(i+Hs)*W + (j+Ws)] = val;
        }
    }

    return out;
}

Mat my_dilate(Mat img, Mat se)
{
    Mat out = Mat(img.rows, img.cols, 0);
    out = out * 0;
    int H = out.rows;
    int W = out.cols;
    int Hs = se.rows;
    int Ws = se.cols;

    //assuming img is binary: 255 is 1.
    for(int i = 0; i < H-Hs; i++)
    {
        for(int j = 0; j < W-Ws; j++)
        {
            int val = 0;
            for(int x = 0; x < Hs; x++)
            {
                for(int y = 0; y < Ws; y++)
                {
                    if(se.data[x*Ws + y] && img.data[(i+x)*W + (j+y)])
                    {
                        val = 255;
                    }
                }
            }
            out.data[(i+Hs)*W + (j+Ws)] = val;
        }
    }

    return out;
}

Mat my_diskerode(Mat img, int r)
{
    Mat extended, cropped;
    Mat se = getStructuringElement( MORPH_ELLIPSE, Size( r, r+1), Point( 0, 0) );
    //extend                           
    copyMakeBorder( img, extended, 0, se.cols, 0, se.cols, BORDER_REPLICATE, 0);
    extended = my_erode(extended, se);
    extended(cv::Rect(se.cols/2,se.rows/2,img.cols,img.rows)).copyTo(cropped);
    return cropped;
}

Mat my_diskdilate(Mat img, int r)
{
    Mat extended, cropped;
    Mat se = getStructuringElement( MORPH_ELLIPSE, Size( r, r+1), Point( 0, 0) );
    //extend                           
    copyMakeBorder( img, extended, 0, se.cols, 0, se.cols, BORDER_REPLICATE, 0);
    extended = my_dilate(extended, se);
    extended(cv::Rect(se.cols/2,se.rows/2,img.cols,img.rows)).copyTo(cropped);
    return cropped;
}

Mat my_diskopen(Mat img, int r)
{
    Mat result;
    result = my_diskerode(img,r);
    result = my_diskdilate(result,r);
    return result;
}

Mat my_diskclose(Mat img, int r)
{
    Mat result;
    result = my_diskdilate(img,r);
    result = my_diskerode(result,r);
    return result;
}


Mat connected_components(Mat m)
{
    Mat labels;
    Mat out = Mat(m.rows, m.cols, 0);
    out *= 0;

    int n;
    n = cv::connectedComponents(m, labels, 8, CV_32S);

    
    int H = m.rows;
    int W = m.cols;

    for(int i = 0; i < H; i++)
    {
        for(int j = 0; j < W; j++)
        {
            out.data[i*W + j] = labels.at<int>(i, j)*255/n;
        }
    }

    
    return out;

}


int main(int argc, char** argv )
{
    // Read image
    Mat img = imread(argv[1] ,CV_LOAD_IMAGE_COLOR);
    Mat gray_img, thresholded, eroded, morph, masked, cc;

    cvtColor(img, gray_img, CV_BGR2GRAY);
    thresholded = (gray_img < 52);
    mysave(string(argv[1]) + "--THRESHOLD", thresholded);
    //moveWindow("threshold", 50,50);

    //erode
    eroded = my_diskerode(thresholded, 4);
    morph = my_diskdilate(eroded, 4);
    morph = my_diskerode(morph, 5);
    morph = my_diskdilate(morph, 5);
    morph = my_diskerode(morph, 6);
    morph = my_diskdilate(morph, 7);
    morph = my_diskerode(morph, 8);
    morph = my_diskdilate(morph, 10);

    //mask with first erosion
    masked = eroded & morph;
    morph = my_diskdilate(masked, 4);
    masked = thresholded & morph;
    morph = my_diskerode(masked, 3);
    masked = thresholded & morph;
    morph = my_diskdilate(masked, 5);
    masked = masked & morph;
    masked = my_diskdilate(masked, 5);
    masked = masked & thresholded;
    masked = my_diskerode(masked, 3);
    masked = my_diskdilate(masked, 10);
    masked = masked & thresholded;

    masked = my_diskdilate(masked, 10);
    masked = my_diskerode(masked, 6);
    masked = masked & thresholded;

    masked = my_diskdilate(masked, 3);

    mysave(string(argv[1]) + "-MASKED", masked);
    cc = connected_components(masked);
    mysave(string(argv[1]) + "---CC", cc);

    //waitKey(0);
    return 0;
}

void reference_()
{
    // Read image
    Mat img = imread("data/license_plates/est_095yhf_close.jpg",CV_LOAD_IMAGE_COLOR);
    Mat imgOut = img.clone();
    

    int H = imgOut.rows;
    int W = imgOut.cols;
    int C = 3;
    for(int i = 0; i < H; i++)
    {
        for(int j = 0; j < W; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                imgOut.data[i*W*C + j*C + k] = (i%3 && j%3 ? imgOut.data[i*W*C + j*C + k] : 0 );
            }
        }
    }

    for(int i = 0; i < H; i++)
    {
        for(int j = 0; j < W; j++)
        {
            //imgOut.data[i*W + j] = (i%3 && j%3 ? imgOut.data[i*W + j] : 0 );
        
        }
    }

    imshow("Test", imgOut);
    waitKey(0);
}