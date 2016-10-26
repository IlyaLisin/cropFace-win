#include "FaceImageProcessing.h"

void FaceImageProcessing::setImagePath(char* _path) {
	path = _path;
}

void FaceImageProcessing::setHaarcascadePath(char* _hcPath) {
	hcPath = _hcPath;
}

void FaceImageProcessing::foundFacePoints(float* landmarks) {
	// read image and convert
	cv::Mat_<unsigned char> img(cv::imread(path, CV_LOAD_IMAGE_GRAYSCALE));

	if (!img.data)
	{
		printf("Cannot load %s\n", path);
		exit(1);
	}

	int foundface;
	
	if (!stasm_search_single(&foundface, landmarks,
		(char*)img.data, img.cols, img.rows, path, hcPath))
	{
		printf("Error in stasm_search_single: %s\n", stasm_lasterr());
		exit(1);
	}
	if (!foundface) {
		printf("No face found in %s\n", path);
		exit(1);
	}

	for (int i = 0; i < 2 * stasm_NLANDMARKS; i++) {
		if (landmarks[i] < 0) landmarks[i] = 0;
	}
	
	// draw the landmarks on the image as white dots	
	//stasm_force_points_into_image(landmarks, img.cols, img.rows);
	//for (int i = 0; i < N_BOUND_FACE_POINTS; i++)
	//	img(cvRound(landmarks[i * 2 + 1]), cvRound(landmarks[i * 2])) = 255;
	
}

void FaceImageProcessing::cropFace(float* landmarks) {
	// Read input image and convert to float
	cv::Mat imgIn = cv::imread(path);
	imgIn.convertTo(imgIn, CV_32FC3, 1 / 255.0);

	// Output image is set to white
	cv::Mat imgOut = cv::Mat::ones(imgIn.size(), imgIn.type());
	imgOut = cv::Scalar(1.0, 1.0, 1.0);

	std::vector <cv::Point2f> outPoints;
	for (int i = 0; i < N_BOUND_FACE_POINTS; i++) {
		outPoints.push_back(cv::Point2f(landmarks[i * 2], landmarks[i * 2 + 1]));
	}

	cv::Rect r = cv::boundingRect(outPoints);

	// Offset points by left top corner of the respective rectangles
	std::vector<cv::Point> croppedIntPoints;
	for (int i = 0; i < N_BOUND_FACE_POINTS; i++)
	{
		// fillConvexPoly needs a vector of Point and not Point2f
		croppedIntPoints.push_back(cv::Point((int)(outPoints[i].x - r.x), (int)(outPoints[i].y - r.y)));
	}

	// Copy image rect
	cv::Mat imgCropped;
	imgIn(r).copyTo(imgCropped);

	// Get mask by filling triangle
	cv::Mat mask = cv::Mat::zeros(r.height, r.width, CV_32FC3);
	fillConvexPoly(mask, croppedIntPoints, cv::Scalar(1.0, 1.0, 1.0), 16, 0);

	// Copy region of the rectangular patch to the output image
	multiply(imgCropped, mask, imgCropped);
	multiply(imgOut(r), cv::Scalar(1.0, 1.0, 1.0) - mask, imgOut(r));
	imgOut(r) = imgOut(r) + imgCropped;

	// Convert back to uint because OpenCV antialiasing
	// does not work on image of type CV_32FC3
	imgIn.convertTo(imgIn, CV_8UC3, 255.0);
	imgOut.convertTo(imgOut, CV_8UC3, 255.0);

	imshow("Input", imgIn);
	imshow("Output", imgOut);
	cv::waitKey(0);
}