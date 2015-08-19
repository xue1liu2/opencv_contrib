/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2015, Baisheng Lai (laibaisheng@gmail.com), Zhejiang University,
// all rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_RANDOMPATTERN_HPP__
#define __OPENCV_RANDOMPATTERN_HPP__

#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"

namespace cv { namespace randpattern {


//! @addtogroup ccalib
//! @{

/** @brief Class for finding features points and corresponding 3D in world coordinate of
a "random" pattern, which can be to be used in calibration. It is useful when pattern is
partly occluded or only a part of pattern can be observed in multiple cameras calibration.
The pattern can be generated by RandomPatternGenerator class described in this file.

Please refer to paper
    B. Li, L. Heng, K. Kevin  and M. Pollefeys, "A Multiple-Camera System
    Calibration Toolbox Using A Feature Descriptor-Based Calibration
    Pattern", in IROS 2013.
*/

class CV_EXPORTS RandomPatternCornerFinder
{
public:

    /* @brief Construct RandomPatternCornerFinder object

    @param patternWidth the real width of "random" pattern in a user defined unit.
    @param patternHeight the real height of "random" pattern in a user defined unit.
    @param nMiniMatch number of minimal matches, otherwise that image is abandoned
    @depth depth of output objectPoints and imagePoints, set it to be CV_32F or CV_64F.
    @showExtraction whether show feature extraction, 0 for no and 1 for yes.
    @detector feature detector to detect feature points in pattern and images.
    @descriptor feature descriptor.
    @matcher feature matcher.
    */
    RandomPatternCornerFinder(float patternWidth, float patternHeight,
        int nminiMatch = 20, int depth = CV_32F, int verbose = 0, int showExtraction = 0,
        Ptr<FeatureDetector> detector = AKAZE::create(AKAZE::DESCRIPTOR_MLDB, 0, 3, 0.005f),
        Ptr<DescriptorExtractor> descriptor = AKAZE::create(AKAZE::DESCRIPTOR_MLDB,0, 3, 0.005f),
        Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-L1"));

    /* @brief Load pattern image and compute features for pattern
    @param patternImage image for "random" pattern generated by RandomPatternGenerator, run it first.
    */
    void loadPattern(cv::Mat patternImage);

    /* @brief Compute matched object points and image points which are used for calibration
    The objectPoints (3D) and imagePoints (2D) are stored inside the class. Run getObjectPoints()
    and getImagePoints() to get them.

    @param inputImages vector of 8-bit grayscale images containing "random" pattern
    that are used for calibration.
    */
    void computeObjectImagePoints(std::vector<cv::Mat> inputImages);

    //void computeObjectImagePoints2(std::vector<cv::Mat> inputImages);

    /* @brief Compute object and image points for a single image. It returns a vector<Mat> that
    the first element stores the imagePoints and the second one stores the objectPoints.

    @param inputImage single input image for calibration
    */
    std::vector<cv::Mat> computeObjectImagePointsForSingle(cv::Mat inputImage);

    /* @brief Get object(3D) points
    */
    std::vector<cv::Mat> getObjectPoints();

    /* @brief and image(2D) points
    */
    std::vector<cv::Mat> getImagePoints();

private:

    std::vector<cv::Mat> _objectPonits, _imagePoints;
    float _patternWidth, _patternHeight;
    cv::Size _patternImageSize;
    int _nminiMatch;
    int _depth;
	int _verbose;

    Ptr<FeatureDetector> _detector;
    Ptr<DescriptorExtractor> _descriptor;
    Ptr<DescriptorMatcher> _matcher;
    Mat _descriptorPattern;
    std::vector<cv::KeyPoint> _keypointsPattern;
    Mat _patternImage;
    int _showExtraction;

    void keyPoints2MatchedLocation(const std::vector<cv::KeyPoint>& imageKeypoints,
        const std::vector<cv::KeyPoint>& patternKeypoints, const std::vector<cv::DMatch> matchces,
        cv::Mat& matchedImagelocation, cv::Mat& matchedPatternLocation);
    void getFilteredLocation(cv::Mat& imageKeypoints, cv::Mat& patternKeypoints, const cv::Mat mask);
    void getObjectImagePoints(const cv::Mat& imageKeypoints, const cv::Mat& patternKeypoints);
    void crossCheckMatching( cv::Ptr<DescriptorMatcher>& descriptorMatcher,
        const Mat& descriptors1, const Mat& descriptors2,
        std::vector<DMatch>& filteredMatches12, int knn=1 );
    void drawCorrespondence(const Mat& image1, const std::vector<cv::KeyPoint> keypoint1,
        const Mat& image2, const std::vector<cv::KeyPoint> keypoint2, const std::vector<cv::DMatch> matchces,
        const Mat& mask1, const Mat& mask2, const int step);
};

/* @brief Class to generate "random" pattern image that are used for RandomPatternCornerFinder
Please refer to paper
B. Li, L. Heng, K. Kevin  and M. Pollefeys, "A Multiple-Camera System
Calibration Toolbox Using A Feature Descriptor-Based Calibration
Pattern", in IROS 2013.
*/
class RandomPatternGenerator
{
public:
    /* @brief Construct RandomPatternGenerator

    @param imageWidth image width of the generated pattern image
    @param imageHeight image height of the generated pattern image
    */
    RandomPatternGenerator(int imageWidth, int imageHeight);

    /* @brief Generate pattern
    */
    void generatePattern();
    /* @brief Get pattern
    */
    cv::Mat getPattern();
private:
    cv::Mat _pattern;
    int _imageWidth, _imageHeight;
};

//! @}

}} //namespace randpattern, cv
#endif