#ifndef RECTIFICATION_PROCESSOR_H
#define RECTIFICATION_PROCESSOR_H

#include "two_image_processor.h"

using namespace cv;

class RectificationProcessor : public TwoImageProcessor
{
  Q_OBJECT
  Q_PROPERTY(QFileInfo CalibrationResults READ calibrationResults
             WRITE setCalibrationResults USER true)
  Q_CLASSINFO("CalibrationResults", "filetype=text;")
  Q_PROPERTY(float FocalLength READ focalLength WRITE setFocalLength USER true)

public:
  RectificationProcessor(QObject *parent = 0);
  ~RectificationProcessor();

  QString name() {return "Rectification";}

  QFileInfo calibrationResults() {QMutexLocker l(&mutex); return calibration_results;}
  void setCalibrationResults(QFileInfo path);

  float focalLength() {QMutexLocker l(&mutex); return focal_length;}
  void setFocalLength(float length);

private:
  void run();
  void loadCalibrationResults();
  void calculateRectMatrix();
  void rectify();
  QFileInfo calibration_results;
  float focal_length;
  Mat R;
  Mat T;
  Mat rect;
};

#endif