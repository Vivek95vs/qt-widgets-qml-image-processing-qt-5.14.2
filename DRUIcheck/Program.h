#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>

class Program : public QObject
{
    Q_OBJECT

public:
    explicit Program(QObject *parent = nullptr);

    enum PatientOrientation_Standing
    {
        patientOrientationstandNULL = 0,
        Tube_lateral_cal = 1,
        Tube_longitudinal_cal,
        Tube_vertical_cal,
        Tube_vertical_rotation_cal,
        Tube_horizontal_rotation_cal,
        Detector_vertical_cal,
        Detector_tilt_cal,
    };

    enum PatientOrientation_Sleeping
    {
        PatientOrientationsleepNULL = 0,
        Tube_Lateral_cal = 1,
        Tube_Longitudinal_cal,
        Tube_Vertical_cal,
        Tube_Vertical_Rotation_cal,
        Tube_Horizontal_Rotation_cal,
    };

    static PatientOrientation_Standing targetpos_cal_PatientStanding;

    void Main();
};

#endif // PROGRAM_H
