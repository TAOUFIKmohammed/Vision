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
tm.start();//commencer le comptage
Mat imgOriginal1, imgOriginal , imgGRAY, imgBIN;
cap >> imgOriginal1;//on capture une frame depuis le flux video
imgOriginal = imgOriginal0 - imgOriginal1 ; //Soustraire les frames 

if( imgOriginal1.empty() )
{
cout << " < < < Frame Empty ! > > > ";
break;
}

cvtColor(imgOriginal, imgGRAY, cv::COLOR_RGB2GRAY);//Convert the image to gray levels
threshold( imgGRAY, imgBIN, 64, 255, 0 );//threshold the gray image as to binarize it


Moments	oMoments = moments(imgBIN);//Calcul des	moments	d'une image binaire
double dM01 = oMoments.m01;//retourne la somme des y de la forme (ou la valeur du pixel vaut 1 dans l'image binaire)
double dM10 = oMoments.m10;//retourne la somme des x de	la forme
double dArea = oMoments.m00;//retourne la nombre de pixels dans la forme

cv::Rect boundRect = boundingRect(imgBIN);//Calcul de la boîte englobante d'une image binaire
rectangle(imgOriginal, boundRect, cv::Scalar(0, 255, 0), 2);//incrustation de la boîte dans l'image de départ

//pour incruster une ligne contenant du texte et des valeurs de variables
std::ostringstream oss (std::ostringstream::ate);
std::string result;
//we create the following variable to convert	the variables values into string
oss << "var1=" << dM01/dArea << " var2=" << dM10/dArea;//ils correspondent au centre du rectangle vert
result = oss.str();
//we write the string on the output image
putText(imgOriginal, result, cvPoint(10,20),FONT_HERSHEY_SIMPLEX,0.75,CV_RGB(255,0,0),2,8,false);

//show the images
imshow("couleurs", imgOriginal); //show the original image
rectangle(imgOriginal1, boundRect, cv::Scalar(0, 255, 0), 2);//incrustation de la boîte dans l'image de départ
putText(imgOriginal1, result, cvPoint(10,20),FONT_HERSHEY_SIMPLEX,0.75,CV_RGB(255,0,0),2,8,false);
imshow("coul", imgOriginal1); //show the original image

//imshow("niveaux de gris",imgGRAY);
//imshow("binaire", imgBIN);

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


