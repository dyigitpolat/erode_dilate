#include "functions.hpp"

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
