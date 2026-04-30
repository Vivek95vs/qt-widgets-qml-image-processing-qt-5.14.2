#pragma once

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageReslice.h>
#include <vtkMatrix4x4.h>
#include <string>
#include <vector>

class DicomLoader {
public:
    DicomLoader();
    ~DicomLoader() = default;

    bool loadDirectory(const std::string& directory);
    vtkSmartPointer<vtkImageData> getImageData() const;

    // Get DICOM metadata
    int getNumberOfSlices() const;
    double* getSpacing() const;
    int* getDimensions() const;
    double* getScalarRange() const;
    std::string getPatientName() const;
    std::string getStudyDate() const;

private:
    vtkSmartPointer<vtkDICOMImageReader> reader;
};
