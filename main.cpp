#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <sys/time.h>

#define SHIFT_VALUE 5 // 移動量[pixel]
#define SHIFT_CYCLE 0.3  // 移動周期[s]
#define SCALE_RESOLUTION 0.01 // 拡大解像度
// メイン
int main(int argc, char **argv) {
	cv::Mat src = cv::imread("input_jpg/00000001.jpg", 1);	
	cv::resize(src, src, cv::Size(1280,960));
	char code = 0;

	double ratio = 1.0;
	int width = src.cols;
	int height = src.rows;

	while( 1 )
	{
		ratio += SCALE_RESOLUTION;
		if( (ratio-1.0)*width > SHIFT_VALUE*2 )
		{
			break;
		}
		if( ratio > 1.5 )
		{
			break;
		}
	}
	cv::Mat shift_img;
	
	cv::resize( src, shift_img, cv::Size(), ratio, ratio );

	bool direction = true; // true: 右、false:左
	double startPosX = 0; //
	std::cout << "org img w:" << width << " h:" << height << std::endl;
	std::cout << "scaling img w:" << shift_img.cols << " h:" << shift_img.rows << std::endl;
	
	double shift_val = SHIFT_VALUE*4.0*0.03/SHIFT_CYCLE;
	std::cout << "shift_val:" << shift_val << std::endl;
	//std::cout << "cycle:" << SHIFT_VALUE/shift_val*4.0*0.02 << "[s]" << std::endl;

	cv::namedWindow("shake", CV_WINDOW_AUTOSIZE);
	//cv::setWindowProperty("Capture", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	struct timeval s,e;
	struct timeval time_management_start, time_management_end;
	gettimeofday(&s, NULL);
	gettimeofday(&time_management_start, NULL);
	double d_buf_time = 0.0;
	double spend_time = 0.0; // [s]
	while( code != 'q' ) 
	{
		//std::cout << startPosX << std::endl;
		cv::Rect roi(int(startPosX)+(ratio-1.0)*width/2, 0, width, height);
		cv::Mat show_img = shift_img(roi);
		cv::imshow("shake", show_img);
		gettimeofday(&time_management_end, NULL);
		d_buf_time += ((time_management_end.tv_sec-time_management_start.tv_sec)+(time_management_end.tv_usec-time_management_start.tv_usec)/(1000.0*1000.0))*1000.0; 
		int wait_time = 30 - int(d_buf_time);	
		d_buf_time -= int(d_buf_time);
		//std::cout << "wait_time:" << wait_time << " buf_time:" << d_buf_time << std::endl;	
		if( wait_time < 0 )
		{
			wait_time = 0;
		}
		code = cv::waitKey(wait_time);
		gettimeofday(&time_management_start, NULL);
		gettimeofday(&e, NULL);
		if( int(fabs(startPosX)+shift_val) > SHIFT_VALUE )
		{
			direction = !direction;
			if(direction)
			{	
				std::cout << "spend time:" << spend_time << " time interval:" << (e.tv_sec-s.tv_sec)+(e.tv_usec-s.tv_usec)/(1000.0*1000.0) << std::endl;
			}
		}			
		spend_time += (e.tv_sec-s.tv_sec)+(e.tv_usec-s.tv_usec)/(1000.0*1000.0);
		gettimeofday(&s, NULL);
		if(spend_time > 5.0) 
		{
			if( direction == true)
			{
				//std::cout << direction << std::endl;
				startPosX -= shift_val;
			}
			else
			{
				//std::cout << direction << std::endl;
				startPosX += shift_val;
			}
		}
		if( spend_time > 60.0 )
		{
			break;
		}		
	}
	return 0;
}
