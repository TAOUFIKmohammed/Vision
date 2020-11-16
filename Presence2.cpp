#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv )
{
TickMeter tm;//la classe calcul le temps de passage en comptant le nombre de ticks par seconde
//VideoCapture cap(0);//pour capturer depuis la cam
VideoCapture cap("Meet_Crowd2.avi"); //pour traiter la video

if ( !cap.isOpened())
{
	cout << "Error retrieving frame \n";
	cout << "Could not open reference " << endl;
return -1;
}

Mat imgOriginal0;
cap >> imgOriginal0;//on capture une frame depuis le flux video

while(true) //Boucle principale
{
int i=0;
tm.start();//commencer le comptage
Mat imgOriginal1, imgOriginal , imgGRAY, imgBIN;
cap >> imgOriginal1;//on capture une frame depuis le flux video
imgOriginal = imgOriginal0 - imgOriginal1 ;
 

if( imgOriginal1.empty() )
{
cout << " < < < Frame Empty ! > > > ";
break;
}

cvtColor(imgOriginal, imgGRAY, cv::COLOR_RGB2GRAY);//Convert the image to gray levels
threshold( imgGRAY, imgBIN, 60, 255, 0 );//threshold the gray image as to binarize it


Moments	oMoments = moments(imgBIN);//Calcul des	moments	d'une image binaire
double dM01 = oMoments.m01;//retourne la somme des y de la forme (ou la valeur du pixel vaut 1 dans l'image binaire)
double dM10 = oMoments.m10;//retourne la somme des x de	la forme
double dArea = oMoments.m00;//retourne la nombre de pixels dans la forme

//pour incruster une ligne contenant du texte et des valeurs de variables
std::ostringstream oss (std::ostringstream::ate);
std::string result;
//we create the following variable to convert	the variables values into string
oss << "var1=" << dM01/dArea << " var2=" << dM10/dArea;//ils correspondent au centre du rectangle vert
result = oss.str();

//Pour récupérer les contours de l'image seuillée afin de créer des boîtes englobantes 
std::vector<std::vector<cv::Point> > contours;
std::vector<cv::Vec4i> hierarchy;
cv::findContours( imgBIN, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
std::vector<cv::Rect> boundRect( contours.size() );

for( int i = 0; i < contours.size(); i++ )
    { 
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = cv::boundingRect( cv::Mat(contours_poly[i]) );
    }   
std::vector<cv::Point> rect_points; 
for( int i = 0; i < contours.size(); i++ )
{
    rect_points.push_back(boundRect[i].tl());
    rect_points.push_back(boundRect[i].br());
}

// On dessine le rectangle quand 2 points sont distants de 25x25 de pixels 
for( int i = 0; i < rect_points.size(); i++ )
{
    for( int j = 0; j < rect_points.size(); j++ )
    {
        if (i == j) 
            continue;

        int x_distance = (rect_points[i].x - rect_points[j].x);
        if (x_distance < 0) 
            x_distance *= -1;

        int y_distance = (rect_points[i].y - rect_points[j].y);
        if (y_distance < 0) 
            y_distance *= -1;

        if ( (x_distance < 25) && (y_distance < 25) )
        {
            std::cout << "Drawing rectangle " << i << " from " 
                      << rect_points[i] << " to " << rect_points[j] 
                      << " distance: " << x_distance << "x" << y_distance << std::endl;
            cv::rectangle( imgOriginal1, rect_points[i], rect_points[j], cv::Scalar(0, 255, 0), 2 );
            break;
        }
    }

}

cv::imshow("white rectangles", imgOriginal1);    

if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	{
		cout << "ESC key pressed by user - video thread terminates" << endl;
		break;
	}
	tm.stop();// finir le comptage
	cout << "Total time:" << tm.getTimeSec() << endl;//affichage de temps d'execution
	}
	return 0;
	tm.reset();// pour réinicialiser le temps de comptage
}




