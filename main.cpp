#include <vector>
#include "FaceImageProcessing.h"

int main(int argc, char* argv[])
{
	FaceImageProcessing* faceImageProcessing = new FaceImageProcessing();

	faceImageProcessing->setHaarcascadePath("F:/Google Drive/Verification_system/crop-face/Debug/");
	faceImageProcessing->setImagePath("F:/Google Drive/Verification_system/crop-face/Debug/testface.jpg");

	// TO DO readme in redmine ( как все поднять )
	// TO DO config class ( получает настройки, путь)
	
	float landmarks[2 * stasm_NLANDMARKS];

	faceImageProcessing->foundFacePoints(landmarks);
	faceImageProcessing->cropFace(landmarks);
	
	cv::waitKey(0);

	return 0;
}