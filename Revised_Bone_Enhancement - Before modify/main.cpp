#include <QCoreApplication>
#include <QFile>
#include <QImage>
#include <QDebug>
#include <QDir>
#include <vector>
#include <cmath>
#include <algorithm>
#include <omp.h>
#include <cstdint>

#include "PerformanceMonitor.h"

using namespace std;


// Container for image data
struct FloatImage {
   int w, h;
   vector<float> data;
   FloatImage(int _w, int _h) : w(_w), h(_h), data(_w * _h, 0.0f) {}
};

// --- FAST BOX BLUR HELPERS (Part of Fast Gaussian) ---
void boxBlurH(float* src, float* dst, int w, int h, int r) {
   float iarr = 1.0f / (r + r + 1);
   #pragma omp parallel for
   for (int i = 0; i < h; i++) {
       int ti = i * w, li = ti, ri = ti + r;
       float fv = src[ti], lv = src[ti + w - 1], val = (float)(r + 1) * fv;
       for (int j = 0; j < r; j++) val += src[ti + j];
       for (int j = 0; j <= r; j++) { val += src[ri++] - fv; dst[ti++] = val * iarr; }
       for (int j = r + 1; j < w - r; j++) { val += src[ri++] - src[li++]; dst[ti++] = val * iarr; }
       for (int j = w - r; j < w; j++) { val += lv - src[li++]; dst[ti++] = val * iarr; }
   }
}

void boxBlurT(float* src, float* dst, int w, int h, int r) {
   float iarr = 1.0f / (r + r + 1);
   #pragma omp parallel for
   for (int i = 0; i < w; i++) {
       int ti = i, li = ti, ri = ti + r * w;
       float fv = src[ti], lv = src[ti + w * (h - 1)], val = (float)(r + 1) * fv;
       for (int j = 0; j < r; j++) val += src[ti + j * w];
       for (int j = 0; j <= r; j++) { val += src[ri] - fv; dst[ti] = val * iarr; ri += w; ti += w; }
       for (int j = r + 1; j < h - r; j++) { val += src[ri] - src[li]; dst[ti] = val * iarr; li += w; ri += w; ti += w; }
       for (int j = h - r; j < h; j++) { val += lv - src[li]; dst[ti] = val * iarr; li += w; ti += w; }
   }
}

// --- FAST GAUSSIAN BLUR O(1) ---
void fastGaussianBlur(FloatImage& src, FloatImage& dst, float sigma) {
   if (sigma <= 0) { dst.data = src.data; return; }
   int n = 3;
   float wIdeal = sqrt((12.0f * sigma * sigma / n) + 1.0f);
   int wl = floor(wIdeal); if (wl % 2 == 0) wl--;
   int wu = wl + 2;
   float mIdeal = (12.0f * sigma * sigma - n * wl * wl - 4.0f * n * wl - 3.0f * n) / (-4.0f * wl - 4.0f);
   int m = round(mIdeal);
   vector<int> sizes;
   for (int i = 0; i < n; i++) sizes.push_back(i < m ? wl : wu);
   FloatImage temp(src.w, src.h);
   dst.data = src.data;
   for (int s : sizes) {
       int r = (s - 1) / 2;
       boxBlurH(dst.data.data(), temp.data.data(), src.w, src.h, r);
       boxBlurT(temp.data.data(), dst.data.data(), src.w, src.h, r);
   }
}

// --- FAST PERCENTILE (Sampling every 10th pixel) ---
float getPercentileFast(const vector<float>& data, float p) {
   vector<float> sample;
   sample.reserve(data.size() / 10);
   for (size_t i = 0; i < data.size(); i += 10) { if (data[i] > 0) sample.push_back(data[i]); }
   if (sample.empty()) return 0;
   size_t n = (size_t)(p * (sample.size() - 1) / 100.0);
   nth_element(sample.begin(), sample.begin() + n, sample.end());
   return sample[n];
}

void normalize01(FloatImage& img) {
   float minV = img.data[0], maxV = img.data[0];
   for (float v : img.data) { if (v < minV) minV = v; if (v > maxV) maxV = v; }
   float range = maxV - minV;
   if (range < 1e-7f) return;
   float invRange = 1.0f / range;
   #pragma omp parallel for
   for (int i = 0; i < (int)img.data.size(); ++i) img.data[i] = (img.data[i] - minV) * invRange;
}


int main(int argc, char *argv[]) {
   QCoreApplication a(argc, argv);

   // ============================================================
   // CONFIGURATION & HYPERPARAMETERS
   // ============================================================


   QString inputPath = "D:/Enhance DR/Originalproj_0.raw";
   QString outputPath = "D:/Enhance DR/ouputImage.raw";

   auto start_load = std::chrono::high_resolution_clock::now();

   double memoryBefore = getCurrentMemoryMB();

   float ILLUM_SIGMA = 150.0f;
      float ILLUM_WEIGHT = 0.35f;

      float SIGMOID_CONTRAST = 6.5f;
      float SIGMOID_BRIGHT = 0.7f;

      float DETAIL_W1 = 1.2f; // Fine
      float DETAIL_W2 = 2.5f; // Medium
      float DETAIL_W3 = 0.8f; // Coarse

      float CONTRAST_LIMIT = 0.28f;
      float CONTRAST_STRENGTH = 1.7f;

      const auto totalStart =
          std::chrono::high_resolution_clock::now();

      const auto tLoad =
          std::chrono::high_resolution_clock::now();

      QFile file(inputPath);
      if (!file.open(QIODevice::ReadOnly)) {
          qDebug() << "Could not open file.";
//           return;
      }
      QByteArray rawData = file.readAll();
      file.close();

      int totalPixels = rawData.size() / 2;
      int width = 3072, height = 3072;
      if (sqrt(totalPixels) == floor(sqrt(totalPixels))) width = height = (int)sqrt(totalPixels);

      FloatImage img(width, height);
      uint16_t* ptr = (uint16_t*)rawData.data();
      for (int i = 0; i < totalPixels; ++i) img.data[i] = (float)ptr[i];

      const auto tLoadEnd =
          std::chrono::high_resolution_clock::now();

      double memoryAfter = getCurrentMemoryMB();

      qDebug() << "Load + conversion:"
               << std::chrono::duration<double>(
                      tLoadEnd - tLoad).count()
               << "sec";

      qDebug() << "   Memory Before:"
               << memoryBefore << "MB";

      qDebug() << "   Memory After:"
               << memoryAfter << "MB";

      qDebug() << "   Memory Change:"
               << memoryAfter - memoryBefore << "MB";

      qDebug() << "   Peak Memory:"
               << getPeakMemoryMB() << "MB";

      // 1. LOGARITHMIC NORMALIZATION

      auto t =
          std::chrono::high_resolution_clock::now();

      double memoryBefore1 = getCurrentMemoryMB();


      #pragma omp parallel for
      for (int i = 0; i < totalPixels; ++i) img.data[i] = log1pf(img.data[i]);
      normalize01(img);

      auto tEnd =
          std::chrono::high_resolution_clock::now();

      double memoryAfter1 = getCurrentMemoryMB();

      qDebug() << "1. Log normalization:"
               << std::chrono::duration<double>(
                      tEnd - t).count()
               << "sec";

      qDebug() << "   Memory Before:"
               << memoryBefore1 << "MB";

      qDebug() << "   Memory After:"
               << memoryAfter1 << "MB";

      qDebug() << "   Memory Change:"
               << memoryAfter1 - memoryBefore1 << "MB";

      qDebug() << "   Peak Memory:"
               << getPeakMemoryMB() << "MB";

      // 2. PHOTOMETRIC MASK

      t =
          std::chrono::high_resolution_clock::now();

      double memoryBefore2 = getCurrentMemoryMB();

      FloatImage illum(width, height);
      fastGaussianBlur(img, illum, ILLUM_SIGMA);
      #pragma omp parallel for
      for (int i = 0; i < totalPixels; ++i) {
          img.data[i] = img.data[i] - (ILLUM_WEIGHT * illum.data[i]);
      }
      normalize01(img);

      tEnd =
          std::chrono::high_resolution_clock::now();

      double memoryAfter2 = getCurrentMemoryMB();


      qDebug() << "2. Photometric mask:"
               << std::chrono::duration<double>(
                      tEnd - t).count()
               << "sec";


      qDebug() << "   Memory Before:"
               << memoryBefore2 << "MB";

      qDebug() << "   Memory After:"
               << memoryAfter2 << "MB";

      qDebug() << "   Memory Change:"
               << memoryAfter2 - memoryBefore2 << "MB";

      qDebug() << "   Peak Memory:"
               << getPeakMemoryMB() << "MB";
      // 3. INVERSION

      t =
          std::chrono::high_resolution_clock::now();

      double memoryBefore3 = getCurrentMemoryMB();

          #pragma omp parallel for
          for (int i = 0; i < totalPixels; ++i) img.data[i] = 1.0f - img.data[i];


          tEnd =
              std::chrono::high_resolution_clock::now();

          double memoryAfter3 = getCurrentMemoryMB();

          qDebug() << "3. Inversion:"
                   << std::chrono::duration<double>(
                          tEnd - t).count()
                   << "sec";

          qDebug() << "   Memory Before:"
                   << memoryBefore3 << "MB";

          qDebug() << "   Memory After:"
                   << memoryAfter3 << "MB";

          qDebug() << "   Memory Change:"
                   << memoryAfter3 - memoryBefore3 << "MB";

          qDebug() << "   Peak Memory:"
                   << getPeakMemoryMB() << "MB";
          // ============================================================
          // 4. AUTOMATIC ADAPTIVE CONTRAST ADJUSTMENT (Dynamic Sigmoid)
          // ============================================================
          // Calculate mean intensity and standard deviation across the image

          t =
              std::chrono::high_resolution_clock::now();

          double memoryBefore4 = getCurrentMemoryMB();

          double sum = 0.0, sumSq = 0.0;
          #pragma omp parallel for reduction(+:sum, sumSq)
          for (int i = 0; i < totalPixels; ++i) {
              float val = img.data[i];
              sum += val;
              sumSq += val * val;
          }
          float meanVal = static_cast<float>(sum / totalPixels);
          float stdDev = static_cast<float>(sqrt(max(1e-6, (sumSq / totalPixels) - (meanVal * meanVal))));

          // Calculate dynamic gain (contrast gain inversely proportional to variance)
          // Dark/washed-out images receive higher gain factor automatically
          float autoGain = max(4.0f, min(10.0f, 0.25f / max(1e-4f, stdDev)));
          float autoMidpoint = max(0.2f, min(0.8f, meanVal));

          #pragma omp parallel for
          for (int i = 0; i < totalPixels; ++i) {
              // Apply dynamic sigmoid auto-contrast curve: S(x) = 1 / (1 + exp(-gain * (x - midpoint)))
              float val = img.data[i];
              float contrastVal = 1.0f / (1.0f + expf(-autoGain * (val - autoMidpoint)));
              img.data[i] = contrastVal;
          }

          // Normalizing dynamic range safety check
          normalize01(img);


          tEnd =
              std::chrono::high_resolution_clock::now();
          double memoryAfter4 = getCurrentMemoryMB();


          qDebug() << "4. Adaptive contrast:"
                   << std::chrono::duration<double>(
                          tEnd - t).count()
                   << "sec";

          qDebug() << "   Memory Before:"
                   << memoryBefore4 << "MB";

          qDebug() << "   Memory After:"
                   << memoryAfter4 << "MB";

          qDebug() << "   Memory Change:"
                   << memoryAfter4 - memoryBefore4<< "MB";

          qDebug() << "   Peak Memory:"
                   << getPeakMemoryMB() << "MB";


          // ============================================================
          // 5. 5x5 MEDIAN FILTER (Applied on Contrast-Adjusted Image)
          // ============================================================

          t =
              std::chrono::high_resolution_clock::now();
          double memoryBefore5 = getCurrentMemoryMB();


          FloatImage medianImg(width, height);

          #pragma omp parallel for
          for (int y = 2; y < height - 2; ++y) {
              for (int x = 2; x < width - 2; ++x) {
                  float window[25];
                  int k = 0;
                  for (int dy = -2; dy <= 2; ++dy) {
                      for (int dx = -2; dx <= 2; ++dx) {
                          window[k++] = img.data[(y + dy) * width + (x + dx)];
                      }
                  }

                  // Insertion sort for 25 elements
                  for (int i = 1; i < 25; ++i) {
                      float key = window[i];
                      int j = i - 1;
                      while (j >= 0 && window[j] > key) {
                          window[j + 1] = window[j];
                          j--;
                      }
                      window[j + 1] = key;
                  }
                  medianImg.data[y * width + x] = window[12];
              }
          }

          // Preserve 2-pixel borders
          #pragma omp parallel for
          for (int y = 0; y < height; ++y) {
              for (int x = 0; x < width; ++x) {
                  if (x < 2 || x >= width - 2 || y < 2 || y >= height - 2) {
                      medianImg.data[y * width + x] = img.data[y * width + x];
                  }
              }
          }
          img.data.swap(medianImg.data);

          tEnd =
              std::chrono::high_resolution_clock::now();
          double memoryAfter5 = getCurrentMemoryMB();


          qDebug() << "5. Median filter:"
                   << std::chrono::duration<double>(
                          tEnd - t).count()
                   << "sec";


          qDebug() << "   Memory Before:"
                   << memoryBefore5 << "MB";

          qDebug() << "   Memory After:"
                   << memoryAfter5 << "MB";

          qDebug() << "   Memory Change:"
                   << memoryAfter5 - memoryBefore5 << "MB";

          qDebug() << "   Peak Memory:"
                   << getPeakMemoryMB() << "MB";

      // 6. CONTRAST ENHANCEMENT (Tanh Detail Boost)

        t =
              std::chrono::high_resolution_clock::now();
        double memoryBefore6 = getCurrentMemoryMB();

      float s_val = max(6.0f, 0.03f * min(width, height));
      FloatImage blur_s(width, height);
      fastGaussianBlur(img, blur_s, s_val);

      float p1 = getPercentileFast(img.data, 1.0f);
      float p99 = getPercentileFast(img.data, 99.0f);
      float cap = max(1e-6f, CONTRAST_LIMIT * (p99 - p1));
      float invCap = 1.0f / cap;

      FloatImage blur_bf(width, height);
      fastGaussianBlur(img, blur_bf, max(4.0f, s_val * 0.25f));

      #pragma omp parallel for
      for (int i = 0; i < totalPixels; ++i) {
          float detail = img.data[i] - blur_s.data[i];
          float outVal = img.data[i] + CONTRAST_STRENGTH * cap * tanhf(detail * invCap);
          float bf = max(0.0f, min(1.0f, blur_bf.data[i]));
          img.data[i] = bf * outVal + (1.0f - bf) * img.data[i];
      }

      tEnd =
          std::chrono::high_resolution_clock::now();

      double memoryAfter6 = getCurrentMemoryMB();

      qDebug() << "6. Contrast enhancement:"
               << std::chrono::duration<double>(
                      tEnd - t).count()
               << "sec";

      qDebug() << "   Memory Before:"
               << memoryBefore6 << "MB";

      qDebug() << "   Memory After:"
               << memoryAfter6 << "MB";

      qDebug() << "   Memory Change:"
               << memoryAfter6 - memoryBefore6 << "MB";

      qDebug() << "   Peak Memory:"
               << getPeakMemoryMB() << "MB";


      // 7. TONE MAPPING (Sigmoid LUT)

      t =
          std::chrono::high_resolution_clock::now();

      double memoryBefore7 = getCurrentMemoryMB();

      #pragma omp parallel for
      for (int i = 0; i < totalPixels; ++i) {
          img.data[i] = 1.0f / (1.0f + expf(-SIGMOID_CONTRAST * (img.data[i] - SIGMOID_BRIGHT)));
      }

      tEnd =
          std::chrono::high_resolution_clock::now();

      double memoryAfter7 = getCurrentMemoryMB();

      qDebug() << "7. Tone mapping:"
               << std::chrono::duration<double>(
                      tEnd - t).count()
               << "sec";

      qDebug() << "   Memory Before:"
               << memoryBefore7 << "MB";

      qDebug() << "   Memory After:"
               << memoryAfter7 << "MB";

      qDebug() << "   Memory Change:"
               << memoryAfter7 - memoryBefore7 << "MB";

      qDebug() << "   Peak Memory:"
               << getPeakMemoryMB() << "MB";

      // 8. MULTISCALE DETAIL AMPLIFICATION

      t =
          std::chrono::high_resolution_clock::now();

      double memoryBefore8 = getCurrentMemoryMB();

      FloatImage b1(width, height), b2(width, height), b3(width, height);
      fastGaussianBlur(img, b1, 1.0f);
      fastGaussianBlur(img, b2, 4.0f);
      fastGaussianBlur(img, b3, 12.0f);

      #pragma omp parallel for
      for (int i = 0; i < totalPixels; ++i) {
          float d1 = img.data[i] - b1.data[i];
          float d2 = b1.data[i] - b2.data[i];
          float d3 = b2.data[i] - b3.data[i];
          img.data[i] = max(0.0f, min(1.0f, img.data[i] + (DETAIL_W1 * d1) + (DETAIL_W2 * d2) + (DETAIL_W3 * d3)));
      }

      tEnd =
          std::chrono::high_resolution_clock::now();

      double memoryAfter8 = getCurrentMemoryMB();

      qDebug() << "8. Multiscale detail:"
               << std::chrono::duration<double>(
                      tEnd - t).count()
               << "sec";

      qDebug() << "   Memory Before:"
               << memoryBefore8 << "MB";

      qDebug() << "   Memory After:"
               << memoryAfter8 << "MB";

      qDebug() << "   Memory Change:"
               << memoryAfter8 - memoryBefore8 << "MB";

      qDebug() << "   Peak Memory:"
               << getPeakMemoryMB() << "MB";
      // 9. FINAL CLEANUP & SHARPENING

      t =
          std::chrono::high_resolution_clock::now();

      double memoryBefore9 = getCurrentMemoryMB();

      normalize01(img);
      FloatImage finalBlur(width, height);
      fastGaussianBlur(img, finalBlur, 1.0f);

      vector<uint16_t> res16(totalPixels);
      QImage resultImg(width, height, QImage::Format_Grayscale8);

      #pragma omp parallel for
      for (int i = 0; i < totalPixels; ++i) {
          float res_f = max(0.0f, min(1.0f, (img.data[i] * 1.5f) + (finalBlur.data[i] * -0.5f)));
          ((uint8_t*)resultImg.bits())[i] = (uint8_t)(res_f * 255.0f);
          res16[i] = (uint16_t)(res_f * 65535.0f);
      }

      tEnd =
          std::chrono::high_resolution_clock::now();

      double memoryAfter9 = getCurrentMemoryMB();

      qDebug() << "9. Final processing:"
               << std::chrono::duration<double>(
                      tEnd - t).count()
               << "sec";

      qDebug() << "   Memory Before:"
               << memoryBefore9 << "MB";

      qDebug() << "   Memory After:"
               << memoryAfter9 << "MB";

      qDebug() << "   Memory Change:"
               << memoryAfter9 - memoryBefore9 << "MB";

      qDebug() << "   Peak Memory:"
               << getPeakMemoryMB() << "MB";


      // SAVE FINAL OUTPUTS
      t =
          std::chrono::high_resolution_clock::now();

      double memoryBefore10 = getCurrentMemoryMB();

      QFileInfo info(outputPath);

      QString path = info.absolutePath();
      QString fileName = info.fileName();

      QDir().mkpath(path);

      QString savePath = path + "/" + fileName;

      QFile fileOut(savePath);

      if (fileOut.open(QIODevice::WriteOnly)) {
          fileOut.write(reinterpret_cast<const char*>(res16.data()),
                        res16.size() * sizeof(uint16_t));
          fileOut.close();
          qDebug() << "Saved:" << savePath;
      }
      else {
          qDebug() << "Failed:" << fileOut.errorString();
      }

      tEnd =
          std::chrono::high_resolution_clock::now();

      double memoryAfter10 = getCurrentMemoryMB();
      qDebug() << "10. Save:"
               << std::chrono::duration<double>(
                      tEnd - t).count()
               << "sec";

      qDebug() << "   Memory Before:"
               << memoryBefore10 << "MB";

      qDebug() << "   Memory After:"
               << memoryAfter10 << "MB";

      qDebug() << "   Memory Change:"
               << memoryAfter10 - memoryBefore10 << "MB";

      qDebug() << "   Peak Memory:"
               << getPeakMemoryMB() << "MB";




      // ========================================================
      // TOTAL TIME
      // ========================================================

      const auto totalEnd =
          std::chrono::high_resolution_clock::now();

      const double totalTime =
          std::chrono::duration<double>(
              totalEnd - totalStart).count();


      qDebug() << "========================================";
      qDebug() << "FINAL OUTPUT:";
      qDebug() << savePath;
      qDebug() << "TOTAL PROCESSING TIME:"
               << totalTime
               << "seconds";
      qDebug() << "========================================";

   return 0;
}
