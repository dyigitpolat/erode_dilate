#include "functions.hpp"

Mat scene1( Mat e, Mat s, const String &name)
{
    Mat x, y, z, t;
    x = (e - s) | (s - e);
    cvtColor(x, x, CV_BGR2GRAY);
    mysave(name + "--SUBTRACT", x);

    x = x > 71;
    mysave(name + "--THRESHOLD", x);
    y = my_diskerode( x, 2);
    y = my_diskdilate( y, 2);

    //y = my_diskerode( y, 2);
    y = my_diskdilate( y, 7);
    y = my_diskerode( y, 5);

    y = my_diskdilate( y, 5);
    y = my_diskerode( y, 6);
    y = my_diskdilate( y, 4);
    y = my_diskerode( y, 3);
    y = my_diskdilate( y, 6);
    y = my_diskerode( y, 7);

    y = my_diskclose( y, 8);
    y = my_diskdilate( y, 5);
    y = my_diskopen( y, 2);
    y = my_diskopen( y, 2);
    y = my_diskopen( y, 3);

    y = my_diskerode( y, 7);
    y = my_diskdilate( y, 9);

    y = my_diskerode( y, 7);
    y = my_diskerode( y, 3);
    y = my_diskdilate( y, 7);
    y = my_diskerode( y, 2);
    y = my_diskdilate( y, 4);

    mysave(name + "--MORPH", y);

    z = connected_components(y);
    mysave(name + "---CC",z);
    return z;
}

Mat scene2(  Mat e, Mat s, const String &name)
{
    Mat x, y, z, t;
    x = (e - s) | (s - e);
    cvtColor(x, x, CV_BGR2GRAY);
    mysave(name + "--SUBTRACT", x);

    x = x > 64;
    mysave(name + "--THRESHOLD", x);
    y = my_diskerode( x, 2);
    y = my_diskdilate( y, 3);
    y = my_diskerode( y, 2);
    y = my_diskdilate( y, 3);
    y = my_diskerode( y, 3);
    y = my_diskdilate( y, 3);
    y = my_diskerode( y, 3);
    y = my_diskdilate( y, 3);
    y = my_diskerode( y, 3);
    y = my_diskdilate( y, 2);

    y = my_diskerode( y, 3);
    y = my_diskdilate( y, 4);

    y = my_diskerode( y, 2);
    y = my_diskdilate( y, 2);
    y = my_diskdilate( y, 2);

    y = my_diskerode( y, 3);
    y = my_diskdilate( y, 2);

    y = my_diskerode( y, 2);
    y = my_diskdilate( y, 3);


    mysave(name + "--MORPH", y);

    z = connected_components(y);
    mysave(name + "---CC", z);
    return z;
}

int main(int argc, char** argv )
{
    // Read images
    Mat empty1 = imread("data/pets2000/0000.jpg" ,CV_LOAD_IMAGE_COLOR);
    Mat scn11 = imread("data/pets2000/0150.jpg" ,CV_LOAD_IMAGE_COLOR);
    Mat scn12 = imread("data/pets2000/0800.jpg" ,CV_LOAD_IMAGE_COLOR);
    Mat scn13 = imread("data/pets2000/1300.jpg" ,CV_LOAD_IMAGE_COLOR);
    
    Mat empty2 = imread("data/pets2001/0040.jpg" ,CV_LOAD_IMAGE_COLOR);
    Mat scn21 = imread("data/pets2001/0535.jpg" ,CV_LOAD_IMAGE_COLOR);
    Mat scn22 = imread("data/pets2001/0950.jpg" ,CV_LOAD_IMAGE_COLOR);
    Mat scn23 = imread("data/pets2001/1750.jpg" ,CV_LOAD_IMAGE_COLOR);
    
    Mat ge1, ge2, gs11, gs12, gs13, gs21, gs22, gs23;
    Mat t11, t12, t13, t21, t22, t23;
    Mat temp;

    t11 = scene1(empty1, scn11,"t11");
    t12 = scene1(empty1, scn12,"t12");
    t13 = scene1(empty1, scn13,"t13");
    myshow("11n", t11);
    myshow("12n", t12);
    myshow("13n", t13);

    t21 = scene2(empty2, scn21,"t21");
    t22 = scene2(empty2, scn22,"t22");
    t23 = scene2(empty2, scn23,"t23");
    myshow("21n", t21);
    myshow("22n", t22);
    myshow("23n", t23);

    waitKey(0);
    return 0;
}