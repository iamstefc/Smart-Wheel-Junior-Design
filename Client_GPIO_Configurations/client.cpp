// Code developed and compiled by Stefano & Irina
// NOTE: Irina used similar code that she coded in past labs
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h> 
#include <errno.h> 
#include <string> 
#include <netdb.h> 
#include <fstream>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include "GPIO.cpp"

#define PORT 8865 // port identified by server to communicate with client

using namespace cv;
using namespace std;

void detectAndDisplay( Mat frame );

/** Global variables */
int faceDetected = 0;
String face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;

String window_name = "Capture - Face detection";

int main(int argc, char *argv[]){
	
	// socket programming client code initialization
	int packet;
	char buf[100];
	struct hostent *host;
	struct sockaddr_in client2server; // client's information for server
	//-- end initialization
	
	GPIO button_ford("0","in","23"); // start "forward"
    GPIO button_stop("0","in","24"); // stop
	VideoCapture capture;
    Mat frame;
    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
    //-- 2. Read the video stream
    capture.open( -1 );
	
	// Error checking: opening video capture, getting host name, socket enabled
    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }
	if (argc != 2) {
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}
	if ((host=gethostbyname(argv[1])) == NULL) {
		herror("gethostbyname");
		exit(1);
	}
	if ((packet = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	//-- end of error checking
	
	client2server.sin_family = AF_INET;
	client2server.sin_port = htons(PORT);
	client2server.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(client2server.sin_zero), 8);
	
	// 
	if (connect(packet, (struct sockaddr *)&client2server, sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
	cout << "Connected to Server";
	/**
		Within the following loop, the video capture is opened to capture face while
		enabling or disabling buttons (depending on different flag on face detection).
		Scenerios:  If face detected & button pressed -> car able to move
					If face not detected, but button pressed -> car stops
	*/
	while(capture.read(frame)){	
		detectAndDisplay( frame );
		if(button_ford.getvalue() == "1" && faceDetected == 1){ // go, enabled buttons
			if(send(packet, "ford", 4, 0) == -1){
				exit (1); 
			}
			cout << "Motor ON";
			usleep(3000000);
		}
		if(button_stop.getvalue() == "1" || faceDetected == 0 ){ // stop, disabled buttons
			if(send(packet, "stop", 4, 0) == -1){
                exit (1);
            }
			cout<< "Motor OFF";
			usleep(100000);
		}
		int c = waitKey(10);
        if( (char)c == 27 ) { break; } // escape, close prog
	  }
    close(packet); // must close socket after communication terminated
	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay(Mat frame){
    std::vector<Rect> faces;
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(20, 200) );
    
	// flag detects whether there is a face in webcam frame
	if(faces.size() != 0)
		faceDetected = 1;
	else faceDetected = 0;
	
	for( size_t i = 0; i < faces.size(); i++ ){
		rectangle(frame,Point(faces[i].x,faces[i].y), Point(faces[i].x+faces[i].width,faces[i].y+faces[i].height), Scalar(255,0,255),2,4,0);
		Mat faceROI = frame_gray( faces[i] );	
    }
    //-- Show what you got
    imshow( window_name, frame );
}
