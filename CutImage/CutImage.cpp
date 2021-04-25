
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <tuple>

using namespace cv;
using namespace std;

class ConsoleArgumentsHandler
{
public:
	bool HandleConsoleArguments(int argc, char** argv)
	{
		try {
			if (argc > 1)
				pathForInputImage = argv[1];
			else 
				std::cin >> pathForInputImage;
			if (argc > 2)
				pathForOutputImage = argv[2];
			else
				pathForOutputImage = pathForInputImage;
			if (argc > 3)
				width = stoi(argv[3]);
			if (argc > 4)
				height = stoi(argv[4]);
		}
		catch (...)
		{			
			return false;
		}
		return true;
	}

	tuple<int, int> GetCropedSize()
	{
		return make_tuple(width, height);
	}

	string GetPathForInputImage()
	{
		return pathForInputImage;
	}

	string GetPathForOutputImage()
	{
		return pathForOutputImage;
	}

private:
	int width = 520;
	int height = 520;
	string pathForInputImage;
	string pathForOutputImage;
};

static class ImageCroper
{
public:
	static Mat GetCropedImageFromCenterOfOtherImage(const Mat& inputImg, int widthOfResultImg, int heightOfResultImg)
	{
		auto cropingWidth = inputImg.cols < widthOfResultImg ? inputImg.cols : widthOfResultImg;
		auto cropingHeight = inputImg.rows < heightOfResultImg ? inputImg.rows : heightOfResultImg;
		const auto regionForCroping = Rect((inputImg.cols - cropingWidth) / 2, (inputImg.rows - cropingHeight) / 2, cropingWidth, cropingHeight);
		auto cropingImg = Mat(inputImg, regionForCroping);

		auto newImage = Mat(widthOfResultImg, heightOfResultImg, CV_8UC3, Scalar(255, 255, 255));
		Mat insertImageRegion(newImage, Rect((widthOfResultImg - cropingWidth) / 2, (heightOfResultImg - cropingHeight) / 2, cropingWidth, cropingWidth));
		cropingImg.copyTo(insertImageRegion);
		return newImage;
	}
};

int main(int argc, char** argv)
{
	auto argumentsHandler = ConsoleArgumentsHandler();
	if (!argumentsHandler.HandleConsoleArguments(argc, argv))
	{
		std::cerr << "Invalid arguments, try again" << endl;
		return -1;
	}

	Mat image = cv::imread(argumentsHandler.GetPathForInputImage());
	if (image.empty())
	{
		std::cerr << "Could not open or find the image" << endl;
		return -1;
	}

	int widthOfResultImg;
	int heightOfResultImg;
	std::tie(widthOfResultImg, heightOfResultImg) = argumentsHandler.GetCropedSize();

	imwrite(argumentsHandler.GetPathForOutputImage(), ImageCroper::GetCropedImageFromCenterOfOtherImage(image, widthOfResultImg, heightOfResultImg));
	return 0;
}