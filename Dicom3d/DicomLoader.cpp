#include "DicomLoader.h"
#include <vtkInformation.h>
#include <vtkStreamingDemandDrivenPipeline.h>

DicomLoader::DicomLoader() {
    reader = vtkSmartPointer<vtkDICOMImageReader>::New();
}

bool DicomLoader::loadDirectory(const std::string& directory) {
    reader->SetDirectoryName(directory.c_str());
    reader->Update();

    // Check if data was loaded
    vtkImageData* image = reader->GetOutput();
    if (!image || image->GetNumberOfPoints() == 0) {
        return false;
    }

    return true;
}

vtkSmartPointer<vtkImageData> DicomLoader::getImageData() const {
    return reader->GetOutput();
}

int DicomLoader::getNumberOfSlices() const {
    if (!reader->GetOutput()) return 0;

    int* dims = reader->GetOutput()->GetDimensions();
    return dims[2];
}

double* DicomLoader::getSpacing() const {
    if (!reader->GetOutput()) return nullptr;
    return reader->GetOutput()->GetSpacing();
}

int* DicomLoader::getDimensions() const {
    if (!reader->GetOutput()) return nullptr;
    return reader->GetOutput()->GetDimensions();
}

double* DicomLoader::getScalarRange() const {
    if (!reader->GetOutput()) return nullptr;

    static double range[2];
    reader->GetOutput()->GetScalarRange(range);
    return range;
}

std::string DicomLoader::getPatientName() const {
    // Note: VTK 8.2 DICOM reader has limited metadata access
    // For full metadata, consider using DCMTK or similar
    return "Patient Name Not Available";
}

std::string DicomLoader::getStudyDate() const {
    return "Study Date Not Available";
}
