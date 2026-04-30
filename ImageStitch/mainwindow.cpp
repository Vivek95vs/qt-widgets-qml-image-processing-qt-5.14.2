#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <iostream>
#include <cmath>
#include <fstream>
#include "QDebug"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BasedOnJaw_clicked()
{
    // ========== SYSTEM PARAMETERS ==========
        const double FID_MM = 1200.0;        // Focus to Image Distance in mm
        const double xJawHeight_MM = 80.5;   // X jaw height in mm
        const double yJawHeight_MM = 70.5;   // Y jaw height in mm
        const int IMAGE_SIZE = 3072;         // Image is 3072x3072 pixels
        const double FULL_FIELD_MM = 420.0;  // 42 cm = 420 mm

        // Collimator values - These represent the JAW OPENING in cm
        double xJawOpening_MM = 2.844*10;   // X jaw opening in cm
        double yJawOpening_MM = 2.491*10;   // Y jaw opening in cm

        // ========== CALCULATE ACTUAL FIELD AT DETECTOR ==========
        // Using geometric relationship: Field = Jaw Opening × (FID / Jaw Height)
        double xActualField_MM = xJawOpening_MM * (FID_MM / xJawHeight_MM);
        double yActualField_MM = yJawOpening_MM * (FID_MM / yJawHeight_MM);

        std::cout << "========== COLLIMATOR STATUS ==========" << std::endl;
        std::cout << "Jaw Opening: " << xJawOpening_MM << " mm (X), "
                  << yJawOpening_MM << " mm (Y)" << std::endl;
        std::cout << "Actual Field at Detector: "
                  << xActualField_MM/10.0 << " cm (X), "
                  << yActualField_MM/10.0 << " cm (Y)" << std::endl;
        std::cout << "Full Field Size: " << FULL_FIELD_MM/10.0 << " cm" << std::endl;
        std::cout << std::endl;

        // ========== DECIDE IF CROPPING IS NEEDED ==========
        const double CROP_THRESHOLD_MM = FULL_FIELD_MM * 0.95;  // 5% tolerance

        bool needCropX = (xActualField_MM < CROP_THRESHOLD_MM);
        bool needCropY = (yActualField_MM < CROP_THRESHOLD_MM);

        if (!needCropX && !needCropY) {
            std::cout << "  Copying original image to output..." << std::endl;

            // Just copy the original file
            std::string inputFile = "D:\\DR\\LVI and 2D Images\\testingsample1\\20260327\\LVI\\Shoulder to Knee-LVI\\RAW\\proj_2.raw";
            std::string outputFile = "D:\\DR\\LVI and 2D Images\\testingsample1\\20260327\\LVI\\Shoulder to Knee-LVI\\RAW\\croppedJaw.raw";

            std::ifstream inFile(inputFile, std::ios::binary);
            std::ofstream outFile(outputFile, std::ios::binary);

            if (!inFile.is_open() || !outFile.is_open()) {
                std::cerr << "Error opening files!" << std::endl;
            }

            outFile << inFile.rdbuf();  // Copy entire file
            inFile.close();
            outFile.close();

            std::cout << "Success! Full image saved to " << outputFile << std::endl;
            std::cout << "Original size: " << IMAGE_SIZE << "x" << IMAGE_SIZE << std::endl;
            std::cout << "Cropped size: " << IMAGE_SIZE << "x" << IMAGE_SIZE << " (no cropping)" << std::endl;
        }

        // ========== CALCULATE CROP REGION (only if needed) ==========

        // Half field for cropping
        double xHalfMM = xActualField_MM / 2.0;
        double yHalfMM = yActualField_MM / 2.0;

        // Convert mm to pixels
        double mmToPixels = IMAGE_SIZE / FULL_FIELD_MM;
        int xHalfPixels = (int)std::round(xHalfMM * mmToPixels);
        int yHalfPixels = (int)std::round(yHalfMM * mmToPixels);

        // Calculate crop rectangle (centered)
        int center = IMAGE_SIZE / 2;
        int cropX = center - xHalfPixels;
        int cropY = center - yHalfPixels;
        int cropWidth = xHalfPixels * 2;
        int cropHeight = yHalfPixels * 2;

        // Ensure crop region is within image bounds
        cropX = std::max(0, cropX);
        cropY = std::max(0, cropY);
        cropWidth = std::min(cropWidth, IMAGE_SIZE - cropX);
        cropHeight = std::min(cropHeight, IMAGE_SIZE - cropY);

        std::cout << "Crop Region: x=" << cropX << ", y=" << cropY
                  << ", width=" << cropWidth << ", height=" << cropHeight << std::endl;
        std::cout << std::endl;

        // Load and crop image
        std::string inputFile = "D:\\DR\\LVI and 2D Images\\testingsample1\\20260327\\LVI\\Shoulder to Knee-LVI\\RAW\\proj_2.raw";
        std::string outputFile = "D:\\DR\\LVI and 2D Images\\testingsample1\\20260327\\LVI\\Shoulder to Knee-LVI\\RAW\\croppedJaw.raw";

        std::ifstream file(inputFile, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << inputFile << std::endl;
        }

        std::vector<uint16_t> imageData(IMAGE_SIZE * IMAGE_SIZE);
        file.read(reinterpret_cast<char*>(imageData.data()), IMAGE_SIZE * IMAGE_SIZE * sizeof(uint16_t));
        file.close();

        std::vector<uint16_t> croppedData(cropWidth * cropHeight);

        for (int y = 0; y < cropHeight; y++) {
            int srcY = cropY + y;
            int dstOffset = y * cropWidth;
            int srcOffset = srcY * IMAGE_SIZE + cropX;
            memcpy(&croppedData[dstOffset], &imageData[srcOffset], cropWidth * sizeof(uint16_t));
        }

        std::ofstream outFile(outputFile, std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "Error: Cannot create file " << outputFile << std::endl;
        }

        outFile.write(reinterpret_cast<char*>(croppedData.data()), croppedData.size() * sizeof(uint16_t));
        outFile.close();

        std::cout << "Success! Cropped image saved to " << outputFile << std::endl;
        std::cout << "Original size: " << IMAGE_SIZE << "x" << IMAGE_SIZE << std::endl;
        std::cout << "Cropped size: " << cropWidth << "x" << cropHeight << std::endl;

}

void MainWindow::on_BasedOnField_clicked()
{
        // ========== SYSTEM PARAMETERS ==========
        const int IMAGE_SIZE = 3072;              // Image is 3072x3072 pixels
        const double FULL_FIELD_X_MM = 427.0;     // Full image covers 427mm in X direction
        const double FULL_FIELD_Y_MM = 427.0;     // Full image covers 427mm in Y direction

        // Desired crop field size (can be different for X and Y)
        const double CROP_FIELD_X_MM = 420.0;     // Desired crop width in mm (X direction)
        const double CROP_FIELD_Y_MM = 150.0;     // Desired crop height in mm (Y direction)

        // Input/Output files
        std::string inputFile = "D:\\DR\\LVI and 2D Images\\testingsample1\\20260327\\LVI\\Shoulder to Knee-LVI\\RAW\\proj_2.raw";
        std::string outputFile = "D:\\DR\\LVI and 2D Images\\testingsample1\\20260327\\LVI\\Shoulder to Knee-LVI\\RAW\\Crop\\proj_2.raw";

        // ========== CALCULATE CROP REGION ==========
        // Convert physical size to pixels (different scales for X and Y if needed)
        double mmToPixelsX = IMAGE_SIZE / FULL_FIELD_X_MM;
        double mmToPixelsY = IMAGE_SIZE / FULL_FIELD_Y_MM;

        int cropWidthPixels = (int)std::round(CROP_FIELD_X_MM * mmToPixelsX);
        int cropHeightPixels = (int)std::round(CROP_FIELD_Y_MM * mmToPixelsY);

        // Calculate crop region (centered)
        int centerX = IMAGE_SIZE / 2;
        int centerY = IMAGE_SIZE / 2;

        int cropX = centerX - (cropWidthPixels / 2);
        int cropY = centerY - (cropHeightPixels / 2);
        int cropWidth = cropWidthPixels;
        int cropHeight = cropHeightPixels;

        // Ensure crop region is within image bounds
        cropX = std::max(0, cropX);
        cropY = std::max(0, cropY);
        cropWidth = std::min(cropWidth, IMAGE_SIZE - cropX);
        cropHeight = std::min(cropHeight, IMAGE_SIZE - cropY);

        // ========== DISPLAY INFORMATION ==========
        std::cout << "========== CROP INFORMATION ==========" << std::endl;
        std::cout << "Full Image Size: " << IMAGE_SIZE << " x " << IMAGE_SIZE << " pixels" << std::endl;
        std::cout << "Full Field Coverage: " << FULL_FIELD_X_MM << " mm x " << FULL_FIELD_Y_MM << " mm" << std::endl;
        std::cout << std::endl;
        std::cout << "Desired Crop Field: " << CROP_FIELD_X_MM << " mm x " << CROP_FIELD_Y_MM << " mm" << std::endl;
        std::cout << "Conversion (X): " << mmToPixelsX << " pixels/mm" << std::endl;
        std::cout << "Conversion (Y): " << mmToPixelsY << " pixels/mm" << std::endl;
        std::cout << "Crop Size in Pixels: " << cropWidth << " x " << cropHeight << " pixels" << std::endl;
        std::cout << std::endl;
        std::cout << "Crop Region (centered):" << std::endl;
        std::cout << "  X = " << cropX << " (from left)" << std::endl;
        std::cout << "  Y = " << cropY << " (from top)" << std::endl;
        std::cout << "  Width = " << cropWidth << " pixels" << std::endl;
        std::cout << "  Height = " << cropHeight << " pixels" << std::endl;
        std::cout << "=====================================" << std::endl;
        std::cout << std::endl;

        // Validate crop region
        if (cropWidth <= 0 || cropHeight <= 0) {
            std::cerr << "Error: Invalid crop region!" << std::endl;
        }

        // ========== LOAD AND CROP IMAGE ==========
        std::ifstream file(inputFile, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << inputFile << std::endl;
        }

        // Read entire image
        std::vector<uint16_t> imageData(IMAGE_SIZE * IMAGE_SIZE);
        file.read(reinterpret_cast<char*>(imageData.data()), IMAGE_SIZE * IMAGE_SIZE * sizeof(uint16_t));
        file.close();

        // Crop the image
        std::vector<uint16_t> croppedData(cropWidth * cropHeight);

        for (int y = 0; y < cropHeight; y++) {
            int srcY = cropY + y;
            int dstOffset = y * cropWidth;
            int srcOffset = srcY * IMAGE_SIZE + cropX;
            memcpy(&croppedData[dstOffset], &imageData[srcOffset], cropWidth * sizeof(uint16_t));
        }

        // Save cropped image
        std::ofstream outFile(outputFile, std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "Error: Cannot create file " << outputFile << std::endl;
        }

        outFile.write(reinterpret_cast<char*>(croppedData.data()), croppedData.size() * sizeof(uint16_t));
        outFile.close();

        std::cout << "Success! Cropped image saved to " << outputFile << std::endl;
        std::cout << "Original size: " << IMAGE_SIZE << "x" << IMAGE_SIZE << " ("
                  << FULL_FIELD_X_MM << "mm x " << FULL_FIELD_Y_MM << "mm)" << std::endl;
        std::cout << "Cropped size: " << cropWidth << "x" << cropHeight << " ("
                  << CROP_FIELD_X_MM << "mm x " << CROP_FIELD_Y_MM << "mm)" << std::endl;

}

/**
 * Create Gaussian pyramid for multi-band blending
 */
std::vector<cv::Mat> buildGaussianPyramid(const cv::Mat& img, int levels) {
    std::vector<cv::Mat> pyramid;
    pyramid.push_back(img);

    for (int i = 1; i < levels; i++) {
        cv::Mat down;
        cv::pyrDown(pyramid[i-1], down);
        pyramid.push_back(down);
    }

    return pyramid;
}

/**
 * Create Laplacian pyramid from Gaussian pyramid
 */
std::vector<cv::Mat> buildLaplacianPyramid(const std::vector<cv::Mat>& gaussianPyramid) {
    std::vector<cv::Mat> laplacianPyramid;

    for (int i = 0; i < (int)gaussianPyramid.size() - 1; i++) {
        cv::Mat up;
        cv::pyrUp(gaussianPyramid[i+1], up, gaussianPyramid[i].size());
        cv::Mat laplacian;
        cv::subtract(gaussianPyramid[i], up, laplacian);
        laplacianPyramid.push_back(laplacian);
    }

    // Add the last level (lowest frequency)
    laplacianPyramid.push_back(gaussianPyramid.back());

    return laplacianPyramid;
}

/**
 * Multi-band blending for vertical seam
 */
cv::Mat multiBandBlendVertical(const cv::Mat& top, const cv::Mat& bottom,
                               int levels = 5, int blendWidth = 50) {
    // Convert to float for processing
    cv::Mat topFloat, bottomFloat;
    top.convertTo(topFloat, CV_32F);
    bottom.convertTo(bottomFloat, CV_32F);

    int width = top.cols;
    int topHeight = top.rows;
    int bottomHeight = bottom.rows;

    // Extract overlapping region
    cv::Mat topOverlap = topFloat(cv::Rect(0, topHeight - blendWidth, width, blendWidth));
    cv::Mat bottomOverlap = bottomFloat(cv::Rect(0, 0, width, blendWidth));

    // Create weight mask (linear gradient)
    cv::Mat weightMask(blendWidth, width, CV_32F);
    for (int y = 0; y < blendWidth; y++) {
        float alpha = (float)y / blendWidth;
        for (int x = 0; x < width; x++) {
            weightMask.at<float>(y, x) = alpha;
        }
    }

    // Build pyramids for overlapping regions
    std::vector<cv::Mat> topPyramid = buildGaussianPyramid(topOverlap, levels);
    std::vector<cv::Mat> bottomPyramid = buildGaussianPyramid(bottomOverlap, levels);
    std::vector<cv::Mat> maskPyramid = buildGaussianPyramid(weightMask, levels);

    // Build Laplacian pyramids
    std::vector<cv::Mat> topLaplacian = buildLaplacianPyramid(topPyramid);
    std::vector<cv::Mat> bottomLaplacian = buildLaplacianPyramid(bottomPyramid);

    // Blend at each level
    std::vector<cv::Mat> blendedPyramid;
    for (int i = 0; i < levels; i++) {
        cv::Mat blended;
        cv::add(topLaplacian[i].mul(1.0 - maskPyramid[i]),
                bottomLaplacian[i].mul(maskPyramid[i]), blended);
        blendedPyramid.push_back(blended);
    }

    // Reconstruct blended image
    cv::Mat result = blendedPyramid.back();
    for (int i = levels - 2; i >= 0; i--) {
        cv::Mat up;
        cv::pyrUp(result, up, blendedPyramid[i].size());
        cv::add(up, blendedPyramid[i], result);
    }

    // Convert back to 16-bit
    cv::Mat result16bit;
    result.convertTo(result16bit, CV_16UC1);

    // Assemble final image
    cv::Mat finalImage(topHeight + bottomHeight, width, CV_16UC1);

    // Copy top part (excluding blend region)
    top(cv::Rect(0, 0, width, topHeight - blendWidth)).copyTo(
        finalImage(cv::Rect(0, 0, width, topHeight - blendWidth)));

    // Copy blended region
    result16bit.copyTo(
        finalImage(cv::Rect(0, topHeight - blendWidth, width, blendWidth)));

    // Copy bottom part (excluding blend region)
    bottom(cv::Rect(0, blendWidth, width, bottomHeight - blendWidth)).copyTo(
        finalImage(cv::Rect(0, topHeight, width, bottomHeight - blendWidth)));

    return finalImage;
}

cv::Mat loadRawImage16bit(const std::string& filename, int width, int height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return cv::Mat();
    }

    // Create OpenCV Mat (16-bit unsigned, 1 channel)
    cv::Mat image(height, width, CV_16UC1);
    file.read(reinterpret_cast<char*>(image.data), width * height * sizeof(uint16_t));
    file.close();

    return image;
}

/**
 * Save 16-bit raw image
 */
void saveRawImage16bit(const cv::Mat& image, const std::string& filename) {
    if (image.empty()) {
        std::cerr << "Error: Cannot save empty image" << std::endl;
        return;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create file " << filename << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(image.data), image.total() * sizeof(uint16_t));
    file.close();

    std::cout << "Saved: " << filename << std::endl;
}


void MainWindow::on_Blending_pixel_clicked()
{

        const int WIDTH = 3022;
        const int height1= 3022;
        const int height2= 3022;
        const int height3= 1079;

       // Load images (you'll need to load your actual images)
       cv::Mat img1 = loadRawImage16bit("D:\\DR\\LVI and 2D Images\\testingsample1\\20260327\\LVI\\Shoulder to Knee-LVI\\RAW\\Crop\\proj_0.raw", WIDTH, height1);
       cv::Mat img2 = loadRawImage16bit("D:\\DR\\LVI and 2D Images\\testingsample1\\20260327\\LVI\\Shoulder to Knee-LVI\\RAW\\Crop\\proj_1.raw", WIDTH, height2);
       cv::Mat img3 = loadRawImage16bit("D:\\DR\\LVI and 2D Images\\testingsample1\\20260327\\LVI\\Shoulder to Knee-LVI\\RAW\\Crop\\proj_2.raw", WIDTH, height3);


       // Method 2: Multi-band blending (better results)
       cv::Mat blendedMulti = multiBandBlendVertical(img1, img2, 5, 150);
       blendedMulti = multiBandBlendVertical(blendedMulti, img3, 5, 150);

       // Save results
       saveRawImage16bit(blendedMulti, "stitched_blended.raw");

       qDebug() << "  Width (cols):" << blendedMulti.cols;
       qDebug() << "  Height (rows):" << blendedMulti.rows;

       // Optional: Save preview images to compare
       cv::Mat preview;
       cv::normalize(blendedMulti, preview, 0, 255, cv::NORM_MINMAX);
       preview.convertTo(preview, CV_8UC1);
       cv::imwrite("stitched_preview.png", preview);
}
//=====================================================================================

/**
 * Remove stitching line using inpainting
 * @param image - Input stitched image
 * @param seamY - Y-coordinate of the stitching line(s)
 * @param lineWidth - Width of the line to remove
 */
cv::Mat removeLineWithInpainting(const cv::Mat& image, int seamY, int lineWidth = 5) {
    cv::Mat result = image.clone();

    // Create mask for the line
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);

    // Draw lines on mask (you can have multiple seams)
    cv::line(mask, cv::Point(0, seamY), cv::Point(image.cols, seamY), cv::Scalar(255), lineWidth);

    // Use inpainting to remove the line
    cv::Mat inpainted;
    cv::inpaint(result, mask, inpainted, 3, cv::INPAINT_TELEA);

    return inpainted;
}

/**
 * Remove multiple stitching lines
 */
cv::Mat removeMultipleLines(const cv::Mat& image, const std::vector<int>& seamYs, int lineWidth = 5) {
    cv::Mat result = image.clone();
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);

    // Draw all seams on mask
    for (int seamY : seamYs) {
        cv::line(mask, cv::Point(0, seamY), cv::Point(image.cols, seamY), cv::Scalar(255), lineWidth);
    }

    // Apply inpainting
    cv::Mat inpainted;
    cv::inpaint(result, mask, inpainted, 3, cv::INPAINT_TELEA);

    return inpainted;
}

/**
 * Apply Gaussian blur specifically at seam lines
 */
cv::Mat blurSeamLines(const cv::Mat& image, const std::vector<int>& seamYs, int blurRadius = 10) {
    cv::Mat result = image.clone();

    for (int seamY : seamYs) {
        int startY = std::max(0, seamY - blurRadius);
        int endY = std::min(image.rows, seamY + blurRadius);
        int height = endY - startY;

        // Extract seam region
        cv::Mat seamRegion = result(cv::Rect(0, startY, image.cols, height));

        // Apply Gaussian blur
        cv::GaussianBlur(seamRegion, seamRegion, cv::Size(0, 0), blurRadius);
    }

    return result;
}

/**
 * Smooth seam using gradient domain processing
 */
cv::Mat smoothSeamGradient(const cv::Mat& image, int seamY, int blendWidth = 20) {
    cv::Mat result = image.clone();

    int startY = std::max(0, seamY - blendWidth);
    int endY = std::min(image.rows, seamY + blendWidth);

    for (int y = startY; y < endY; y++) {
        float alpha = 1.0f - (float)std::abs(y - seamY) / blendWidth;

        for (int x = 0; x < image.cols; x++) {
            // Apply smoothing in vertical direction
            int kernelSize = 5;
            float sum = 0;
            int count = 0;

            for (int ky = -kernelSize/2; ky <= kernelSize/2; ky++) {
                int yk = y + ky;
                if (yk >= 0 && yk < image.rows) {
                    sum += image.at<uint16_t>(yk, x);
                    count++;
                }
            }

            float smoothed = sum / count;
            uint16_t original = image.at<uint16_t>(y, x);

            // Blend original with smoothed
            uint16_t blended = (uint16_t)(original * (1 - alpha) + smoothed * alpha);
            result.at<uint16_t>(y, x) = blended;
        }
    }

    return result;
}

/**
 * Remove vertical lines using FFT filtering
 */
cv::Mat removeVerticalLinesFFT(const cv::Mat& image) {
    // Convert to float
    cv::Mat floatImage;
    image.convertTo(floatImage, CV_32F);

    // Expand image to optimal FFT size
    int m = cv::getOptimalDFTSize(image.rows);
    int n = cv::getOptimalDFTSize(image.cols);
    cv::Mat padded;
    cv::copyMakeBorder(floatImage, padded, 0, m - image.rows, 0, n - image.cols,
                       cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // Create complex matrix for FFT
    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat complex;
    cv::merge(planes, 2, complex);

    // Perform DFT
    cv::dft(complex, complex);

    // Create mask to remove vertical lines (horizontal frequencies)
    cv::Mat mask = cv::Mat::ones(complex.size(), CV_32F);
    int centerX = complex.cols / 2;
    int centerY = complex.rows / 2;

    // Remove horizontal frequencies (vertical lines)
    int radius = 5;
    for (int y = centerY - radius; y <= centerY + radius; y++) {
        for (int x = 0; x < complex.cols; x++) {
            mask.at<float>(y, x) = 0;
        }
    }

    // Apply mask
    cv::Mat filtered;
    cv::mulSpectrums(complex, mask, filtered, 0);

    // Inverse FFT
    cv::Mat inverse;
    cv::idft(filtered, inverse, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);

    // Crop back to original size
    cv::Mat resultFloat = inverse(cv::Rect(0, 0, image.cols, image.rows));

    // Convert back to 16-bit
    cv::Mat result;
    resultFloat.convertTo(result, CV_16UC1);

    return result;
}
/**
 * Apply adaptive median filter at seam lines
 */
cv::Mat adaptiveMedianAtSeam(const cv::Mat& image, int seamY, int windowSize = 5) {
    cv::Mat result = image.clone();
    int radius = windowSize / 2;

    int startY = std::max(0, seamY - 20);
    int endY = std::min(image.rows, seamY + 20);

    for (int y = startY; y < endY; y++) {
        for (int x = 0; x < image.cols; x++) {
            // Collect pixels in window
            std::vector<uint16_t> window;

            for (int ky = -radius; ky <= radius; ky++) {
                int yk = y + ky;
                if (yk >= 0 && yk < image.rows) {
                    for (int kx = -radius; kx <= radius; kx++) {
                        int xk = x + kx;
                        if (xk >= 0 && xk < image.cols) {
                            window.push_back(image.at<uint16_t>(yk, xk));
                        }
                    }
                }
            }

            // Sort and take median
            std::sort(window.begin(), window.end());
            uint16_t median = window[window.size() / 2];

            // Replace with median
            result.at<uint16_t>(y, x) = median;
        }
    }

    return result;
}
//=======================================advanced==============================
// ========== SIMPLE SEAM REMOVAL - PRESERVES ORIGINAL DATA ==========

/**
 * Simple and safe seam removal using Gaussian blur only at the seam line
 */
cv::Mat simpleSeamRemoval(const cv::Mat& image, const std::vector<int>& seams) {
    cv::Mat result = image.clone();

    for (int seamY : seams) {
        // Apply very gentle blur only at the exact seam line
        int blurHeight = 15;  // Small blur area
        int startY = std::max(0, seamY - blurHeight/2);
        int endY = std::min(image.rows, seamY + blurHeight/2);

        if (endY > startY) {
            cv::Rect seamArea(0, startY, image.cols, endY - startY);

            // Apply Gaussian blur with small kernel
            cv::GaussianBlur(result(seamArea), result(seamArea), cv::Size(5, 5), 2);
        }
    }

    return result;
}

/**
 * Feather blending without histogram manipulation
 */
cv::Mat featherBlendSafe(const cv::Mat& image, int seamY, int blendWidth = 30) {
    cv::Mat result = image.clone();

    int startY = std::max(0, seamY - blendWidth);
    int endY = std::min(image.rows, seamY + blendWidth);

    for (int y = startY; y < endY; y++) {
        // Calculate weight (smooth transition)
        float weight = 0.5f + 0.5f * std::cos(M_PI * (y - seamY) / blendWidth);
        weight = std::max(0.0f, std::min(1.0f, weight));

        for (int x = 0; x < image.cols; x++) {
            // Simple weighted average between original and blurred
            uint16_t original = image.at<uint16_t>(y, x);

            // Get average of surrounding pixels (3x3 window)
            int sum = 0;
            int count = 0;
            for (int dy = -1; dy <= 1; dy++) {
                int yk = y + dy;
                if (yk >= 0 && yk < image.rows) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int xk = x + dx;
                        if (xk >= 0 && xk < image.cols) {
                            sum += image.at<uint16_t>(yk, xk);
                            count++;
                        }
                    }
                }
            }
            uint16_t avg = sum / count;

            // Blend original with average
            uint16_t blended = (uint16_t)(original * (1 - weight) + avg * weight);
            result.at<uint16_t>(y, x) = blended;
        }
    }

    return result;
}

/**
 * Median filter at seam (preserves edges)
 */
cv::Mat medianFilterSeam(const cv::Mat& image, int seamY, int filterSize = 5) {
    cv::Mat result = image.clone();

    int startY = std::max(0, seamY - filterSize);
    int endY = std::min(image.rows, seamY + filterSize);

    for (int y = startY; y < endY; y++) {
        for (int x = 0; x < image.cols; x++) {
            // Collect neighborhood pixels
            std::vector<uint16_t> neighbors;
            for (int dy = -filterSize/2; dy <= filterSize/2; dy++) {
                int yk = y + dy;
                if (yk >= 0 && yk < image.rows) {
                    for (int dx = -filterSize/2; dx <= filterSize/2; dx++) {
                        int xk = x + dx;
                        if (xk >= 0 && xk < image.cols) {
                            neighbors.push_back(image.at<uint16_t>(yk, xk));
                        }
                    }
                }
            }

            // Apply median filter
            std::sort(neighbors.begin(), neighbors.end());
            result.at<uint16_t>(y, x) = neighbors[neighbors.size() / 2];
        }
    }

    return result;
}

/**
 * Inpainting at seam (OpenCV built-in)
 */
cv::Mat inpaintSeamSafe(const cv::Mat& image, int seamY, int lineWidth = 3) {
    cv::Mat result = image.clone();

    // Create mask for the seam
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
    cv::line(mask, cv::Point(0, seamY), cv::Point(image.cols, seamY), cv::Scalar(255), lineWidth);

    // Apply inpainting (this is safe and preserves overall structure)
    cv::inpaint(result, mask, result, 2, cv::INPAINT_TELEA);

    return result;
}

/**
 * Gradient domain blending - simple version
 */
cv::Mat gradientBlendSafe(const cv::Mat& image, int seamY, int blendWidth = 40) {
    cv::Mat result = image.clone();

    int startY = std::max(0, seamY - blendWidth);
    int endY = std::min(image.rows, seamY + blendWidth);

    // Calculate vertical gradient
    cv::Mat grad;
    cv::Sobel(image, grad, CV_32F, 0, 1, 3);

    for (int y = startY; y < endY; y++) {
        float weight = 1.0f - std::abs(y - seamY) / (float)blendWidth;

        for (int x = 0; x < image.cols; x++) {
            // Only blend where gradient is high (seam line)
            float gradientMag = std::abs(grad.at<float>(y, x));

            if (gradientMag > 20) {  // Threshold for seam detection
                // Get pixels above and below
                int yAbove = std::max(0, y - 2);
                int yBelow = std::min(image.rows - 1, y + 2);

                uint16_t valAbove = image.at<uint16_t>(yAbove, x);
                uint16_t valBelow = image.at<uint16_t>(yBelow, x);
                uint16_t smoothed = (valAbove + valBelow) / 2;

                // Blend based on gradient magnitude
                float blendWeight = std::min(1.0f, gradientMag / 50.0f) * weight;
                uint16_t blended = (uint16_t)(image.at<uint16_t>(y, x) * (1 - blendWeight) + smoothed * blendWeight);
                result.at<uint16_t>(y, x) = blended;
            }
        }
    }

    return result;
}

/**
 * Combined approach - minimal processing
 */
cv::Mat removeSeamsSafe(const cv::Mat& image, const std::vector<int>& seams) {
    cv::Mat result = image.clone();

    std::cout << "Processing " << seams.size() << " seam(s)..." << std::endl;

    for (size_t i = 0; i < seams.size(); i++) {
        int seamY = seams[i];
        std::cout << "  Processing seam at Y = " << seamY << std::endl;

        // Step 1: Apply gentle inpainting (removes line)
        result = inpaintSeamSafe(result, seamY, 3);

        // Step 2: Apply median filter (smooths artifacts)
        result = medianFilterSeam(result, seamY, 3);

        // Step 3: Apply gradient blending (blends edges)
        result = gradientBlendSafe(result, seamY, 30);

        // Step 4: Final feather blend (optional, very gentle)
        // result = featherBlendSafe(result, seamY, 20);
    }

    return result;
}

/**
 * Detect seam positions from vertical gradients
 */
std::vector<int> detectSeamsSimple(const cv::Mat& image) {
    std::vector<int> seams;

    // Calculate vertical gradient
    cv::Mat grad;
    cv::Sobel(image, grad, CV_32F, 0, 1, 3);

    // Sum gradient per row
    std::vector<float> rowGrad(image.rows, 0);
    for (int y = 0; y < image.rows; y++) {
        float sum = 0;
        for (int x = 0; x < image.cols; x++) {
            sum += std::abs(grad.at<float>(y, x));
        }
        rowGrad[y] = sum / image.cols;
    }

    // Find peaks (potential seam positions)
    int threshold = 30;
    int minDistance = 200;  // Minimum distance between seams

    for (int y = 20; y < image.rows - 20; y++) {
        if (rowGrad[y] > threshold &&
            rowGrad[y] > rowGrad[y-5] &&
            rowGrad[y] > rowGrad[y+5]) {

            // Check distance from last seam
            if (seams.empty() || (y - seams.back()) > minDistance) {
                seams.push_back(y);
            }
        }
    }

    return seams;
}


void MainWindow::on_removelinefromstitched_clicked()
{
    const int IMAGE_SIZE = 3072;
       std::string inputFile = "D:\\DR\\LVI and 2D Images\\stitched_final.raw";
       std::string outputFile = "seam_removed_final.raw";

       // Load image
       std::cout << "Loading image..." << std::endl;
       cv::Mat image = loadRawImage16bit(inputFile, IMAGE_SIZE, IMAGE_SIZE);

       if (image.empty()) {
           std::cerr << "Error: Failed to load image!" << std::endl;
//           return -1;
       }

       std::cout << "Image loaded: " << image.cols << " x " << image.rows << std::endl;

       // Display image statistics
       double minVal, maxVal;
       cv::minMaxLoc(image, &minVal, &maxVal);
       std::cout << "Pixel range: " << minVal << " - " << maxVal << std::endl;

       // Detect seams or use manual
       std::vector<int> seams = detectSeamsSimple(image);

       if (seams.empty()) {
           std::cout << "No seams detected automatically. Using manual seam positions..." << std::endl;
           // For 3 equal parts
           seams = {IMAGE_SIZE / 3, 2 * IMAGE_SIZE / 3};
       }

       std::cout << "Seams at Y positions: ";
       for (int s : seams) std::cout << s << " ";
       std::cout << std::endl;

       // Try different methods and compare

       // Method 1: Simple Gaussian blur
       cv::Mat method1 = simpleSeamRemoval(image, seams);
       saveRawImage16bit(method1, "method1_blur.raw");

       // Method 2: Inpainting only
       cv::Mat method2 = image.clone();
       for (int seam : seams) {
           method2 = inpaintSeamSafe(method2, seam, 3);
       }
       saveRawImage16bit(method2, "method2_inpaint.raw");

       // Method 3: Combined safe method (recommended)
       cv::Mat method3 = removeSeamsSafe(image, seams);
       saveRawImage16bit(method3, outputFile);


       std::cout << "\n========================================" << std::endl;
       std::cout << "Results saved:" << std::endl;
       std::cout << "  original_preview.png (original image)" << std::endl;
       std::cout << "  method1_blur_preview.png (Gaussian blur)" << std::endl;
       std::cout << "  method2_inpaint_preview.png (Inpainting only)" << std::endl;
       std::cout << "  " << outputFile << " (Combined method)" << std::endl;
       std::cout << "========================================" << std::endl;

}
//==========================================================Intensity Correction=======================================

///**
// * Method 1: Histogram Matching
// */
//cv::Mat histogramMatching(const cv::Mat& source, const cv::Mat& target) {
//    CV_Assert(source.type() == CV_16UC1 && target.type() == CV_16UC1);

//    cv::Mat result = source.clone();

//    // Calculate histograms (16-bit range 0-65535)
//    int histSize = 65536;
//    float range[] = {0, 65535};
//    const float* histRange = {range};

//    cv::Mat histSource, histTarget;
//    cv::calcHist(&source, 1, 0, cv::Mat(), histSource, 1, &histSize, &histRange);
//    cv::calcHist(&target, 1, 0, cv::Mat(), histTarget, 1, &histSize, &histRange);

//    // Calculate cumulative histograms
//    std::vector<double> cumSource(histSize, 0);
//    std::vector<double> cumTarget(histSize, 0);

//    cumSource[0] = histSource.at<float>(0);
//    cumTarget[0] = histTarget.at<float>(0);

//    for (int i = 1; i < histSize; i++) {
//        cumSource[i] = cumSource[i-1] + histSource.at<float>(i);
//        cumTarget[i] = cumTarget[i-1] + histTarget.at<float>(i);
//    }

//    // Normalize cumulative histograms
//    double totalSource = cumSource[histSize-1];
//    double totalTarget = cumTarget[histSize-1];

//    for (int i = 0; i < histSize; i++) {
//        cumSource[i] /= totalSource;
//        cumTarget[i] /= totalTarget;
//    }

//    // Create lookup table
//    std::vector<ushort> lut(histSize);
//    int j = 0;
//    for (int i = 0; i < histSize; i++) {
//        while (j < histSize && cumTarget[j] < cumSource[i]) {
//            j++;
//        }
//        lut[i] = static_cast<ushort>(j);
//    }

//    // Apply lookup table
//    for (int i = 0; i < result.total(); i++) {
//        ushort val = result.at<ushort>(i);
//        result.at<ushort>(i) = lut[val];
//    }

//    return result;
//}

///**
// * Method 2: Linear Intensity Correction (Mean/Std matching)
// */
//cv::Mat linearIntensityCorrection(const cv::Mat& source, const cv::Mat& target) {
//    CV_Assert(source.type() == CV_16UC1 && target.type() == CV_16UC1);

//    // Calculate mean and standard deviation
//    cv::Scalar meanSource, stdSource, meanTarget, stdTarget;
//    cv::meanStdDev(source, meanSource, stdSource);
//    cv::meanStdDev(target, meanTarget, stdTarget);

//    // Apply correction: I_corrected = (I - meanTarget) * (stdSource/stdTarget) + meanSource
//    double alpha = stdSource[0] / stdTarget[0];
//    double beta = meanSource[0] - alpha * meanTarget[0];

//    cv::Mat corrected;
//    source.convertTo(corrected, CV_32F);
//    corrected = corrected * alpha + beta;
//    corrected.convertTo(corrected, CV_16UC1);

//    return corrected;
//}

///**
// * Method 3: Local Adaptive Intensity Correction
// */
//cv::Mat localIntensityCorrection(const cv::Mat& source, const cv::Mat& target, int windowSize = 100) {
//    CV_Assert(source.type() == CV_16UC1 && target.type() == CV_16UC1);

//    cv::Mat result = source.clone();

//    for (int y = 0; y < source.rows; y += windowSize/2) {
//        for (int x = 0; x < source.cols; x += windowSize/2) {
//            // Define window boundaries
//            int y_end = std::min(y + windowSize, source.rows);
//            int x_end = std::min(x + windowSize, source.cols);

//            cv::Rect window(x, y, x_end - x, y_end - y);

//            cv::Mat sourceWindow = source(window);
//            cv::Mat targetWindow = target(window);

//            // Calculate local mean and std
//            cv::Scalar meanSource, stdSource, meanTarget, stdTarget;
//            cv::meanStdDev(sourceWindow, meanSource, stdSource);
//            cv::meanStdDev(targetWindow, meanTarget, stdTarget);

//            // Apply local correction
//            double alpha = stdSource[0] / stdTarget[0];
//            double beta = meanSource[0] - alpha * meanTarget[0];

//            cv::Mat correctedWindow;
//            sourceWindow.convertTo(correctedWindow, CV_32F);
//            correctedWindow = correctedWindow * alpha + beta;
//            correctedWindow.convertTo(correctedWindow, CV_16UC1);

//            correctedWindow.copyTo(result(window));
//        }
//    }

//    return result;
//}

///**
// * Method 4: Contrast Limited Adaptive Histogram Equalization (CLAHE)
// */
//cv::Mat applyCLAHE(const cv::Mat& image, double clipLimit = 2.0, int tileGridSize = 8) {
//    cv::Mat result;
//    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(clipLimit, cv::Size(tileGridSize, tileGridSize));
//    clahe->apply(image, result);
//    return result;
//}

///**
// * Method 5: Gamma Correction
// */
//cv::Mat gammaCorrection(const cv::Mat& image, double gamma = 1.0) {
//    cv::Mat result;
//    cv::Mat lookUpTable(1, 65536, CV_16UC1);

//    ushort* p = lookUpTable.ptr<ushort>();
//    for (int i = 0; i < 65536; i++) {
//        p[i] = static_cast<ushort>(std::pow(i / 65535.0, gamma) * 65535);
//    }

//    cv::LUT(image, lookUpTable, result);
//    return result;
//}

///**
// * Method 6: Global Intensity Normalization
// */
//cv::Mat globalIntensityNormalization(const cv::Mat& image, int targetMin = 0, int targetMax = 65535) {
//    cv::Mat result;
//    cv::normalize(image, result, targetMin, targetMax, cv::NORM_MINMAX);
//    return result;
//}

///**
// * Method 7: Percentile-based Normalization (removes outliers)
// */
//cv::Mat percentileNormalization(const cv::Mat& image, double lowPercentile = 0.5, double highPercentile = 99.5) {
//    std::vector<ushort> pixels;
//    pixels.assign(image.begin<ushort>(), image.end<ushort>());
//    std::sort(pixels.begin(), pixels.end());

//    int lowIdx = static_cast<int>(pixels.size() * lowPercentile / 100.0);
//    int highIdx = static_cast<int>(pixels.size() * highPercentile / 100.0);

//    ushort lowVal = pixels[lowIdx];
//    ushort highVal = pixels[highIdx];

//    cv::Mat result;
//    image.convertTo(result, CV_32F);
//    result = (result - lowVal) / (highVal - lowVal) * 65535.0;
//    result = cv::max(result, 0.0f);
//    result = cv::min(result, 65535.0f);
//    result.convertTo(result, CV_16UC1);

//    return result;
//}

//// ========== SEAM DETECTION ==========

//std::vector<int> detectSeams(const cv::Mat& image, int threshold = 40, int minDistance = 200) {
//    std::vector<int> seams;

//    // Calculate vertical gradient
//    cv::Mat grad;
//    cv::Sobel(image, grad, CV_32F, 0, 1, 3);

//    // Sum gradient per row
//    std::vector<float> rowGrad(image.rows, 0);
//    for (int y = 0; y < image.rows; y++) {
//        float sum = 0;
//        for (int x = 0; x < image.cols; x++) {
//            sum += std::abs(grad.at<float>(y, x));
//        }
//        rowGrad[y] = sum / image.cols;
//    }

//    // Find peaks
//    for (int y = 20; y < image.rows - 20; y++) {
//        if (rowGrad[y] > threshold &&
//            rowGrad[y] > rowGrad[y-5] &&
//            rowGrad[y] > rowGrad[y+5]) {

//            if (seams.empty() || (y - seams.back()) > minDistance) {
//                seams.push_back(y);
//            }
//        }
//    }

//    return seams;
//}

//// ========== INTENSITY CORRECTION ACROSS SEAMS ==========

///**
// * Apply intensity correction at seam locations
// */
//cv::Mat correctIntensityAtSeams(const cv::Mat& image, const std::vector<int>& seams,
//                                int correctionMethod = 1, int blendWidth = 100) {
//    cv::Mat result = image.clone();

//    for (size_t i = 0; i < seams.size(); i++) {
//        int seamY = seams[i];

//        // Get regions above and below the seam
//        int startY = std::max(0, seamY - blendWidth);
//        int endY = std::min(image.rows, seamY + blendWidth);

//        cv::Mat regionAbove = result(cv::Rect(0, startY, image.cols, seamY - startY));
//        cv::Mat regionBelow = result(cv::Rect(0, seamY, image.cols, endY - seamY));

//        if (regionAbove.empty() || regionBelow.empty()) continue;

//        cv::Mat correctedBelow;

//        // Apply selected correction method
//        switch (correctionMethod) {
//            case 1: // Histogram matching
//                correctedBelow = histogramMatching(regionBelow, regionAbove);
//                break;
//            case 2: // Linear correction
//                correctedBelow = linearIntensityCorrection(regionBelow, regionAbove);
//                break;
//            case 3: // Local adaptive
//                correctedBelow = localIntensityCorrection(regionBelow, regionAbove, 50);
//                break;
//            case 4: // CLAHE
//                correctedBelow = applyCLAHE(regionBelow, 2.0, 8);
//                break;
//            case 5: // Gamma correction
//                correctedBelow = gammaCorrection(regionBelow, 1.2);
//                break;
//            default:
//                correctedBelow = regionBelow;
//        }

//        // Apply feather blending at the seam for smooth transition
//        int featherWidth = 30;
//        for (int y = 0; y < featherWidth; y++) {
//            float alpha = static_cast<float>(y) / featherWidth;
//            float beta = 1.0f - alpha;

//            int rowAbove = seamY - featherWidth + y;
//            int rowBelow = seamY + y;

//            if (rowAbove >= 0 && rowAbove < image.rows && rowBelow < image.rows) {
//                for (int x = 0; x < image.cols; x++) {
//                    ushort valAbove = result.at<ushort>(rowAbove, x);
//                    ushort valBelow = correctedBelow.at<ushort>(y, x);
//                    result.at<ushort>(rowBelow, x) = static_cast<ushort>(valAbove * beta + valBelow * alpha);
//                }
//            }
//        }

//        // Copy corrected region
//        correctedBelow.copyTo(result(cv::Rect(0, seamY, image.cols, endY - seamY)));
//    }

//    return result;
//}

//// ========== COMPLETE INTENSITY CORRECTION PIPELINE ==========

//cv::Mat applyCompleteIntensityCorrection(const cv::Mat& image, bool autoDetectSeams = true) {
//    cv::Mat result = image.clone();

//    std::cout << "=== Intensity Correction Pipeline ===" << std::endl;

//    // Step 1: Detect seams
//    std::vector<int> seams;
//    if (autoDetectSeams) {
//        seams = detectSeams(image, 40, 200);
//        std::cout << "Detected " << seams.size() << " seam(s) at positions: ";
//        for (int s : seams) std::cout << s << " ";
//        std::cout << std::endl;
//    } else {
//        // Manual seams for 3 equal parts
//        seams = {image.rows / 3, 2 * image.rows / 3};
//        std::cout << "Using manual seam positions: " << seams[0] << ", " << seams[1] << std::endl;
//    }

//    if (seams.empty()) {
//        std::cout << "No seams detected. Applying global correction only." << std::endl;
//    } else {
//        // Step 2: Apply intensity correction at seams
//        std::cout << "Applying histogram matching at seams..." << std::endl;
//        result = correctIntensityAtSeams(result, seams, 1, 100);
//    }

//    // Step 3: Apply global normalization
//    std::cout << "Applying global percentile normalization..." << std::endl;
//    result = percentileNormalization(result, 0.5, 99.5);

//    // Step 4: Optional CLAHE for local contrast enhancement
//    std::cout << "Applying CLAHE for contrast enhancement..." << std::endl;
//    result = applyCLAHE(result, 2.0, 8);

//    std::cout << "Intensity correction completed!" << std::endl;

//    return result;
//}


void MainWindow::on_Intensity_correction_clicked()
{
//    const int IMAGE_SIZE = 3072;
//     std::string inputFile = "D:\\DR\\LVI and 2D Images\\stitched_final.raw";
//     std::string outputFile = "D:\\DR\\LVI and 2D Images\\intensity_corrected.raw";

//     // Load image
//     std::cout << "Loading image..." << std::endl;
//     cv::Mat image = loadRawImage16bit(inputFile, IMAGE_SIZE, IMAGE_SIZE);

//     if (image.empty()) {
//         std::cerr << "Error: Failed to load image!" << std::endl;
//     }

//     // Display original statistics
//     double minVal, maxVal;
//     cv::minMaxLoc(image, &minVal, &maxVal);
//     cv::Scalar meanVal = cv::mean(image);

//     std::cout << "\nOriginal Image Statistics:" << std::endl;
//     std::cout << "  Min: " << minVal << std::endl;
//     std::cout << "  Max: " << maxVal << std::endl;
//     std::cout << "  Mean: " << meanVal[0] << std::endl;
////     std::cout << "  StdDev: " << cv::meanStdDev(image)[1][0] << std::endl;

//     // Try different correction methods

//     // Method 1: Complete pipeline (Recommended)
//     std::cout << "\n--- Method 1: Complete Pipeline ---" << std::endl;
//     cv::Mat result1 = applyCompleteIntensityCorrection(image, true);
//     saveRawImage16bit(result1, "D:\\\DR\\\LVI and 2D Images\\result_complete_pipeline.raw");

//     // Method 2: Histogram matching only
//     std::cout << "\n--- Method 2: Histogram Matching Only ---" << std::endl;
//     std::vector<int> seams = detectSeams(image, 40, 200);
//     cv::Mat result2 = correctIntensityAtSeams(image, seams, 1, 100);
//     saveRawImage16bit(result2, "D:\\DR\\\LVI and 2D Images\\result_histogram_match.raw");

//     // Method 3: Linear correction only
//     std::cout << "\n--- Method 3: Linear Correction Only ---" << std::endl;
//     cv::Mat result3 = correctIntensityAtSeams(image, seams, 2, 100);
//     saveRawImage16bit(result3, "D:\\DR\\LVI and 2D Images\\result_linear_correction.raw");

//     // Method 4: CLAHE only
//     std::cout << "\n--- Method 4: CLAHE Only ---" << std::endl;
//     cv::Mat result4 = applyCLAHE(image, 2.0, 8);
//     saveRawImage16bit(result4, "D:\\DR\\LVI and 2D Images\\result_clahe_only.raw");

//     // Method 5: Global normalization only
//     std::cout << "\n--- Method 5: Global Normalization Only ---" << std::endl;
//     cv::Mat result5 = percentileNormalization(image, 0.5, 99.5);
//     saveRawImage16bit(result5, "D:\\DR\\LVI and 2D Images\\result_global_norm.raw");

//     // Method 6: Gamma correction
//     std::cout << "\n--- Method 6: Gamma Correction ---" << std::endl;
//     cv::Mat result6 = gammaCorrection(image, 1.2);
//     saveRawImage16bit(result6, "D:\\DR\\LVI and 2D Images\\result_gamma_correction.raw");

//     // Display results statistics
//     std::cout << "\n=== Results Statistics ===" << std::endl;

//     cv::minMaxLoc(result1, &minVal, &maxVal);
//     meanVal = cv::mean(result1);
//     std::cout << "Complete Pipeline: Min=" << minVal << ", Max=" << maxVal
//               << ", Mean=" << meanVal[0] << std::endl;

//     cv::minMaxLoc(result2, &minVal, &maxVal);
//     meanVal = cv::mean(result2);
//     std::cout << "Histogram Match: Min=" << minVal << ", Max=" << maxVal
//               << ", Mean=" << meanVal[0] << std::endl;

//     cv::minMaxLoc(result3, &minVal, &maxVal);
//     meanVal = cv::mean(result3);
//     std::cout << "Linear Correction: Min=" << minVal << ", Max=" << maxVal
//               << ", Mean=" << meanVal[0] << std::endl;

//     std::cout << "\nAll results saved!" << std::endl;
//     std::cout << "Preview images saved as PNG files for comparison." << std::endl;

}

//=========================================================Intensity Correction 2=======================================
/**
 * Method 1: Gamma Correction for 16-bit images
 */
cv::Mat gammaCorrection16bit(const cv::Mat& image, double gamma = 1.2) {
    CV_Assert(image.type() == CV_16UC1);

    cv::Mat result(image.size(), CV_16UC1);

    // Create lookup table for 16-bit (0-65535)
    std::vector<ushort> lut(65536);
    for (int i = 0; i < 65536; i++) {
        double val = i / 65535.0;
        val = std::pow(val, gamma);
        lut[i] = static_cast<ushort>(val * 65535.0);
    }

    // Apply lookup table manually
    for (int y = 0; y < image.rows; y++) {
        const ushort* src = image.ptr<ushort>(y);
        ushort* dst = result.ptr<ushort>(y);
        for (int x = 0; x < image.cols; x++) {
            dst[x] = lut[src[x]];
        }
    }

    return result;
}

/**
 * Method 2: Histogram Matching for 16-bit
 */
cv::Mat histogramMatching16bit(const cv::Mat& source, const cv::Mat& target) {
    CV_Assert(source.type() == CV_16UC1 && target.type() == CV_16UC1);

    cv::Mat result = source.clone();

    // Calculate histograms (16-bit range 0-65535)
    int histSize = 65536;
    float range[] = {0, 65536};
    const float* histRange = {range};

    cv::Mat histSource, histTarget;
    cv::calcHist(&source, 1, 0, cv::Mat(), histSource, 1, &histSize, &histRange);
    cv::calcHist(&target, 1, 0, cv::Mat(), histTarget, 1, &histSize, &histRange);

    // Calculate cumulative histograms
    std::vector<double> cumSource(histSize, 0);
    std::vector<double> cumTarget(histSize, 0);

    cumSource[0] = histSource.at<float>(0);
    cumTarget[0] = histTarget.at<float>(0);

    for (int i = 1; i < histSize; i++) {
        cumSource[i] = cumSource[i-1] + histSource.at<float>(i);
        cumTarget[i] = cumTarget[i-1] + histTarget.at<float>(i);
    }

    // Normalize cumulative histograms
    double totalSource = cumSource[histSize-1];
    double totalTarget = cumTarget[histSize-1];

    if (totalSource > 0 && totalTarget > 0) {
        for (int i = 0; i < histSize; i++) {
            cumSource[i] /= totalSource;
            cumTarget[i] /= totalTarget;
        }
    }

    // Create lookup table
    std::vector<ushort> lut(histSize);
    int j = 0;
    for (int i = 0; i < histSize; i++) {
        while (j < histSize && cumTarget[j] < cumSource[i]) {
            j++;
        }
        lut[i] = static_cast<ushort>(j);
    }

    // Apply lookup table
    for (int y = 0; y < result.rows; y++) {
        ushort* row = result.ptr<ushort>(y);
        for (int x = 0; x < result.cols; x++) {
            row[x] = lut[row[x]];
        }
    }

    return result;
}

/**
 * Method 3: Linear Intensity Correction (Mean/Std matching)
 */
cv::Mat linearIntensityCorrection16bit(const cv::Mat& source, const cv::Mat& target) {
    CV_Assert(source.type() == CV_16UC1 && target.type() == CV_16UC1);

    // Calculate mean and standard deviation
    cv::Scalar meanSource, stdSource, meanTarget, stdTarget;
    cv::meanStdDev(source, meanSource, stdSource);
    cv::meanStdDev(target, meanTarget, stdTarget);

    // Apply correction: I_corrected = (I - meanTarget) * (stdSource/stdTarget) + meanSource
    double alpha = stdSource[0] / stdTarget[0];
    double beta = meanSource[0] - alpha * meanTarget[0];

    cv::Mat corrected;
    source.convertTo(corrected, CV_32F);
    corrected = corrected * alpha + beta;
    corrected = cv::max(corrected, 0.0f);
    corrected = cv::min(corrected, 65535.0f);
    corrected.convertTo(corrected, CV_16UC1);

    return corrected;
}

/**
 * Method 4: Local Adaptive Intensity Correction
 */
cv::Mat localIntensityCorrection16bit(const cv::Mat& source, const cv::Mat& target, int windowSize = 100) {
    CV_Assert(source.type() == CV_16UC1 && target.type() == CV_16UC1);

    cv::Mat result = source.clone();

    for (int y = 0; y < source.rows; y += windowSize/2) {
        for (int x = 0; x < source.cols; x += windowSize/2) {
            // Define window boundaries
            int y_end = std::min(y + windowSize, source.rows);
            int x_end = std::min(x + windowSize, source.cols);

            cv::Rect window(x, y, x_end - x, y_end - y);

            cv::Mat sourceWindow = source(window);
            cv::Mat targetWindow = target(window);

            // Calculate local mean and std
            cv::Scalar meanSource, stdSource, meanTarget, stdTarget;
            cv::meanStdDev(sourceWindow, meanSource, stdSource);
            cv::meanStdDev(targetWindow, meanTarget, stdTarget);

            // Apply local correction
            double alpha = stdSource[0] / stdTarget[0];
            double beta = meanSource[0] - alpha * meanTarget[0];

            cv::Mat correctedWindow;
            sourceWindow.convertTo(correctedWindow, CV_32F);
            correctedWindow = correctedWindow * alpha + beta;
            correctedWindow = cv::max(correctedWindow, 0.0f);
            correctedWindow = cv::min(correctedWindow, 65535.0f);
            correctedWindow.convertTo(correctedWindow, CV_16UC1);

            correctedWindow.copyTo(result(window));
        }
    }

    return result;
}

/**
 * Method 5: CLAHE for 16-bit images
 */
cv::Mat applyCLAHE16bit(const cv::Mat& image, double clipLimit = 2.0, int tileGridSize = 8) {
    CV_Assert(image.type() == CV_16UC1);

    cv::Mat result;
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(clipLimit, cv::Size(tileGridSize, tileGridSize));
    clahe->apply(image, result);
    return result;
}

/**
 * Method 6: Global Percentile Normalization
 */
cv::Mat percentileNormalization16bit(const cv::Mat& image, double lowPercentile = 0.5, double highPercentile = 99.5) {
    CV_Assert(image.type() == CV_16UC1);

    // Collect all pixels
    std::vector<ushort> pixels;
    pixels.reserve(image.total());
    for (int y = 0; y < image.rows; y++) {
        const ushort* row = image.ptr<ushort>(y);
        for (int x = 0; x < image.cols; x++) {
            pixels.push_back(row[x]);
        }
    }

    // Sort to find percentiles
    std::sort(pixels.begin(), pixels.end());

    int lowIdx = static_cast<int>(pixels.size() * lowPercentile / 100.0);
    int highIdx = static_cast<int>(pixels.size() * highPercentile / 100.0);

    ushort lowVal = pixels[lowIdx];
    ushort highVal = pixels[highIdx];

    if (highVal <= lowVal) {
        return image.clone();
    }

    // Apply normalization
    cv::Mat result(image.size(), CV_16UC1);
    for (int y = 0; y < image.rows; y++) {
        const ushort* src = image.ptr<ushort>(y);
        ushort* dst = result.ptr<ushort>(y);
        for (int x = 0; x < image.cols; x++) {
            double val = (src[x] - lowVal) / static_cast<double>(highVal - lowVal) * 65535.0;
            dst[x] = static_cast<ushort>(std::max(0.0, std::min(65535.0, val)));
        }
    }

    return result;
}

/**
 * Method 7: Global Min-Max Normalization
 */
cv::Mat minMaxNormalization16bit(const cv::Mat& image) {
    CV_Assert(image.type() == CV_16UC1);

    double minVal, maxVal;
    cv::minMaxLoc(image, &minVal, &maxVal);

    if (maxVal <= minVal) {
        return image.clone();
    }

    cv::Mat result(image.size(), CV_16UC1);
    for (int y = 0; y < image.rows; y++) {
        const ushort* src = image.ptr<ushort>(y);
        ushort* dst = result.ptr<ushort>(y);
        for (int x = 0; x < image.cols; x++) {
            double val = (src[x] - minVal) / (maxVal - minVal) * 65535.0;
            dst[x] = static_cast<ushort>(std::max(0.0, std::min(65535.0, val)));
        }
    }

    return result;
}

// ========== SEAM DETECTION ==========

std::vector<int> detectSeams(const cv::Mat& image, int threshold = 40, int minDistance = 200) {
    std::vector<int> seams;

    // Calculate vertical gradient
    cv::Mat grad;
    cv::Sobel(image, grad, CV_32F, 0, 1, 3);

    // Sum gradient per row
    std::vector<float> rowGrad(image.rows, 0);
    for (int y = 0; y < image.rows; y++) {
        float sum = 0;
        for (int x = 0; x < image.cols; x++) {
            sum += std::abs(grad.at<float>(y, x));
        }
        rowGrad[y] = sum / image.cols;
    }

    // Find peaks
    for (int y = 20; y < image.rows - 20; y++) {
        if (rowGrad[y] > threshold &&
            rowGrad[y] > rowGrad[y-5] &&
            rowGrad[y] > rowGrad[y+5]) {

            if (seams.empty() || (y - seams.back()) > minDistance) {
                seams.push_back(y);
            }
        }
    }

    return seams;
}

// ========== INTENSITY CORRECTION ACROSS SEAMS ==========

cv::Mat correctIntensityAtSeams(const cv::Mat& image, const std::vector<int>& seams,
                                int correctionMethod = 1, int blendWidth = 100) {
    cv::Mat result = image.clone();

    for (size_t i = 0; i < seams.size(); i++) {
        int seamY = seams[i];

        // Get regions above and below the seam
        int startY = std::max(0, seamY - blendWidth);
        int endY = std::min(image.rows, seamY + blendWidth);

        cv::Mat regionAbove = result(cv::Rect(0, startY, image.cols, seamY - startY));
        cv::Mat regionBelow = result(cv::Rect(0, seamY, image.cols, endY - seamY));

        if (regionAbove.empty() || regionBelow.empty()) continue;

        cv::Mat correctedBelow;

        // Apply selected correction method
        switch (correctionMethod) {
            case 1: // Histogram matching
                correctedBelow = histogramMatching16bit(regionBelow, regionAbove);
                break;
            case 2: // Linear correction
                correctedBelow = linearIntensityCorrection16bit(regionBelow, regionAbove);
                break;
            case 3: // Local adaptive
                correctedBelow = localIntensityCorrection16bit(regionBelow, regionAbove, 50);
                break;
            default:
                correctedBelow = regionBelow;
        }

        // Copy corrected region
        correctedBelow.copyTo(result(cv::Rect(0, seamY, image.cols, endY - seamY)));
    }

    return result;
}
void MainWindow::on_intensity_correction2_clicked()
{
    const int IMAGE_SIZE = 3072;
      std::string inputFile = "D:\\DR\\LVI and 2D Images\\stitched_final.raw";
      std::string outputDir = "D:\\DR\\LVI and 2D Images\\";

      // Load image
      std::cout << "Loading image..." << std::endl;
      cv::Mat image = loadRawImage16bit(inputFile, IMAGE_SIZE, IMAGE_SIZE);

      if (image.empty()) {
          std::cerr << "Error: Failed to load image!" << std::endl;
//          return -1;
      }

      // Display original statistics
      double minVal, maxVal;
      cv::minMaxLoc(image, &minVal, &maxVal);
      cv::Scalar meanVal = cv::mean(image);
      cv::Scalar stdVal;
      cv::meanStdDev(image, meanVal, stdVal);

      std::cout << "\n=== Original Image Statistics ===" << std::endl;
      std::cout << "  Min: " << minVal << std::endl;
      std::cout << "  Max: " << maxVal << std::endl;
      std::cout << "  Mean: " << meanVal[0] << std::endl;
      std::cout << "  StdDev: " << stdVal[0] << std::endl;

      // Detect seams
      std::vector<int> seams = detectSeams(image, 40, 200);
      std::cout << "\nDetected " << seams.size() << " seam(s) at positions: ";
      for (int s : seams) std::cout << s << " ";
      std::cout << std::endl;

      // Method 1: Global Min-Max Normalization
      std::cout << "\n--- Method 1: Global Min-Max Normalization ---" << std::endl;
      cv::Mat result1 = minMaxNormalization16bit(image);
      saveRawImage16bit(result1, outputDir + "result_minmax_norm.raw");

      // Method 2: Percentile Normalization
      std::cout << "\n--- Method 2: Percentile Normalization ---" << std::endl;
      cv::Mat result2 = percentileNormalization16bit(image, 0.5, 99.5);
      saveRawImage16bit(result2, outputDir + "result_percentile_norm.raw");

      // Method 3: CLAHE
      std::cout << "\n--- Method 3: CLAHE ---" << std::endl;
      cv::Mat result3 = applyCLAHE16bit(image, 2.0, 8);
      saveRawImage16bit(result3, outputDir + "result_clahe.raw");

      // Method 4: Gamma Correction
      std::cout << "\n--- Method 4: Gamma Correction ---" << std::endl;
      cv::Mat result4 = gammaCorrection16bit(image, 1.2);
      saveRawImage16bit(result4, outputDir + "result_gamma.raw");

      // Method 5: Intensity correction at seams + normalization
      std::cout << "\n--- Method 5: Seam Correction + Normalization ---" << std::endl;
      cv::Mat result5 = correctIntensityAtSeams(image, seams, 1, 100);
      result5 = percentileNormalization16bit(result5, 0.5, 99.5);
      saveRawImage16bit(result5, outputDir + "result_seam_corrected.raw");

      // Method 6: Complete pipeline (Recommended)
      std::cout << "\n--- Method 6: Complete Pipeline (Recommended) ---" << std::endl;
      cv::Mat result6 = correctIntensityAtSeams(image, seams, 1, 100);
      result6 = applyCLAHE16bit(result6, 2.0, 8);
      result6 = percentileNormalization16bit(result6, 0.5, 99.5);
      saveRawImage16bit(result6, outputDir + "result_complete.raw");


      cv::minMaxLoc(result1, &minVal, &maxVal);
      meanVal = cv::mean(result1);
      std::cout << "Min-Max Norm: Min=" << minVal << ", Max=" << maxVal << ", Mean=" << meanVal[0] << std::endl;

      cv::minMaxLoc(result2, &minVal, &maxVal);
      meanVal = cv::mean(result2);
      std::cout << "Percentile Norm: Min=" << minVal << ", Max=" << maxVal << ", Mean=" << meanVal[0] << std::endl;

      cv::minMaxLoc(result3, &minVal, &maxVal);
      meanVal = cv::mean(result3);
      std::cout << "CLAHE: Min=" << minVal << ", Max=" << maxVal << ", Mean=" << meanVal[0] << std::endl;

      cv::minMaxLoc(result4, &minVal, &maxVal);
      meanVal = cv::mean(result4);
      std::cout << "Gamma: Min=" << minVal << ", Max=" << maxVal << ", Mean=" << meanVal[0] << std::endl;

      cv::minMaxLoc(result6, &minVal, &maxVal);
      meanVal = cv::mean(result6);
      std::cout << "Complete Pipeline: Min=" << minVal << ", Max=" << maxVal << ", Mean=" << meanVal[0] << std::endl;

      std::cout << "\n=== All results saved successfully! ===" << std::endl;
      std::cout << "Check the output directory for RAW files and PNG previews." << std::endl;
      std::cout << "Recommended result: result_complete.raw" << std::endl;

//      return 0;

}
