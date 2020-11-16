#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv )
{
TickMeter tm; //la classe calcul le temps de passage en comptant le nombre de ticks par seconde
//VideoCapture cap(0);//pour capturer depuis la cam
VideoCapture cap("Meet_Crowd2.avi"); //pour traiter la video

if ( !cap.isOpened())
{
	cout << "Error retrieving frame \n";
	cout << "Could not open reference " << endl;
return -1;
}

while(true) //Boucle principale
{
tm.start();//commencer le comptage 
Mat imgOriginal, imgGRAY, imgBIN;

cap >> imgOriginal;//on capture une frame depuis le flux video

if( imgOriginal.empty() )
{

cout << " < < < Frame Empty ! > > > ";
break;
}

cvtColor(imgOriginal, imgGRAY, cv::COLOR_RGB2GRAY);//Convert the image to gray levels
threshold( imgGRAY, imgBIN, 128, 255, 0 );//threshold the gray image as to binarize it

//show the images
imshow("couleurs", imgOriginal); //show the original image
imshow("niveaux de gris",imgGRAY);
imshow("binaire", imgBIN);

if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	{
		cout << "ESC key pressed by user - video thread terminates" << endl;
		break;
	}
	tm.stop(); // finir le comptage 
	cout << "Total time:" << tm.getTimeSec() << endl; //affichage de temps d'execution 
	}
	return 0;
	tm.reset();// pour réinicialiser le temps de comptage 
}

