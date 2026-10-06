
//check error in below codes

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <chrono>
#include <limits>

#include <omp.h>
#include "PerformanceMonitor.h"
using namespace std;


// ============================================================
// FLOAT IMAGE
// ============================================================

struct FloatImage
{
    int w = 0;
    int h = 0;

    vector<float> data;

    FloatImage() = default;

    FloatImage(int _w, int _h)
        : w(_w),
          h(_h),
          data(static_cast<size_t>(_w) * _h, 0.0F)
    {
    }

    void resize(int _w, int _h)
    {
        w = _w;
        h = _h;
        data.resize(static_cast<size_t>(w) * h);
    }
};


// ============================================================
// FAST BOX BLUR - HORIZONTAL
// ============================================================

static inline void boxBlurH(
    const float* src,
    float* dst,
    int w,
    int h,
    int r)
{
    const float iarr =
        1.0f / static_cast<float>(2 * r + 1);

    #pragma omp parallel for schedule(static)
    for (int y = 0; y < h; ++y)
    {
        const int row = y * w;

        int ti = row;
        int li = row;
        int ri = row + r;

        const float fv = src[row];
        const float lv = src[row + w - 1];

        float val =
            static_cast<float>(r + 1) * fv;

        for (int j = 0; j < r; ++j)
            val += src[row + j];

        for (int j = 0; j <= r; ++j)
        {
            val += src[ri++] - fv;
            dst[ti++] = val * iarr;
        }

        for (int j = r + 1; j < w - r; ++j)
        {
            val += src[ri++] - src[li++];
            dst[ti++] = val * iarr;
        }

        for (int j = w - r; j < w; ++j)
        {
            val += lv - src[li++];
            dst[ti++] = val * iarr;
        }
    }

}

// ============================================================
// FAST BOX BLUR - VERTICAL
// ============================================================

static inline void boxBlurT(
    const float* src,
    float* dst,
    int w,
    int h,
    int r)
{
    const float iarr =
        1.0f / static_cast<float>(2 * r + 1);

    #pragma omp parallel for schedule(static)
    for (int x = 0; x < w; ++x)
    {
        int ti = x;
        int li = x;
        int ri = x + r * w;

        const float fv = src[x];
        const float lv =
            src[x + w * (h - 1)];

        float val =
            static_cast<float>(r + 1) * fv;

        for (int j = 0; j < r; ++j)
            val += src[x + j * w];

        for (int j = 0; j <= r; ++j)
        {
            val += src[ri] - fv;

            dst[ti] =
                val * iarr;

            ri += w;
            ti += w;
        }

        for (int j = r + 1; j < h - r; ++j)
        {
            val += src[ri] - src[li];

            dst[ti] =
                val * iarr;

            li += w;
            ri += w;
            ti += w;
        }

        for (int j = h - r; j < h; ++j)
        {
            val += lv - src[li];

            dst[ti] =
                val * iarr;

            li += w;
            ti += w;
        }
    }
}

// ============================================================
// OPTIMIZED FAST GAUSSIAN
//
// IMPORTANT:
// No:
//      dst.data = src.data;
//
// We reuse one temporary buffer and perform ping-pong
// processing.
// ============================================================

static void fastGaussianBlur(
    const FloatImage& src,
    FloatImage& dst,
    FloatImage& temp,
    float sigma)
{
    if (sigma <= 0.0f)
    {
        dst.data = src.data;
        return;
    }

    const int w = src.w;
    const int h = src.h;

    constexpr int n = 3;

    const float wIdeal =
        std::sqrt(
            (12.0f * sigma * sigma / n) + 1.0f);

    int wl =
        static_cast<int>(
            std::floor(wIdeal));

    if ((wl & 1) == 0)
        --wl;

    const int wu = wl + 2;

    const float mIdeal =
        (12.0f * sigma * sigma
         - n * wl * wl
         - 4.0f * n * wl
         - 3.0f * n)
        /
        (-4.0f * wl - 4.0f);

    const int m =
        static_cast<int>(
            std::round(mIdeal));

    int sizes[3];

    sizes[0] = (0 < m) ? wl : wu;
    sizes[1] = (1 < m) ? wl : wu;
    sizes[2] = (2 < m) ? wl : wu;

    dst.resize(w, h);
    temp.resize(w, h);

    // First box
    {
        const int r = (sizes[0] - 1) / 2;

        boxBlurH(
            src.data.data(),
            temp.data.data(),
            w,
            h,
            r);

        boxBlurT(
            temp.data.data(),
            dst.data.data(),
            w,
            h,
            r);
    }

    // Second box
    {
        const int r = (sizes[1] - 1) / 2;

        boxBlurH(
            dst.data.data(),
            temp.data.data(),
            w,
            h,
            r);

        boxBlurT(
            temp.data.data(),
            dst.data.data(),
            w,
            h,
            r);
    }

    // Third box
    {
        const int r = (sizes[2] - 1) / 2;

        boxBlurH(
            dst.data.data(),
            temp.data.data(),
            w,
            h,
            r);

        boxBlurT(
            temp.data.data(),
            dst.data.data(),
            w,
            h,
            r);
    }
}
// ============================================================
// FAST PERCENTILE
// ============================================================

static float getPercentileFast(
        const vector<float>& data,
        float p)
{
    vector<float> sample;

    sample.reserve(data.size() / 10 + 1);

    for (size_t i = 0; i < data.size(); i += 10)
    {
        const float v = data[i];

        if (v > 0.0f)
            sample.push_back(v);
    }

    if (sample.empty())
        return 0.0f;

    const size_t n =
            static_cast<size_t>(
                p * (sample.size() - 1) / 100.0f);

    std::nth_element(
        sample.begin(),
        sample.begin() + n,
        sample.end());

    return sample[n];
}


// ============================================================
// OPTIMIZED NORMALIZATION
// ============================================================

static void normalize01(FloatImage& img)
{
    const int count = static_cast<int>(img.data.size());

    if (count <= 0)
        return;

    float minV = std::numeric_limits<float>::max();
    float maxV = std::numeric_limits<float>::lowest();

    #pragma omp parallel
    {
        float localMin = std::numeric_limits<float>::max();
        float localMax = std::numeric_limits<float>::lowest();

        #pragma omp for nowait
        for (int i = 0; i < count; ++i)
        {
            const float v = img.data[i];

            if (v < localMin)
                localMin = v;

            if (v > localMax)
                localMax = v;
        }

        #pragma omp critical
        {
            if (localMin < minV)
                minV = localMin;

            if (localMax > maxV)
                maxV = localMax;
        }
    }

    const float range = maxV - minV;

    if (range < 1e-7f)
        return;

    const float invRange = 1.0f / range;

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < count; ++i)
    {
        img.data[i] =
            (img.data[i] - minV) * invRange;
    }
}

// ============================================================
// 5x5 MEDIAN FILTER
//
// Uses nth_element instead of insertion sort.
//
// nth_element guarantees that element [12] is the median
// without completely sorting all 25 elements.
// ============================================================

static void medianFilter5x5(
    const FloatImage& src,
    FloatImage& dst)
{
    const int w = src.w;
    const int h = src.h;

    // Copy source once.
    // This automatically preserves the 2-pixel border.
    dst.data = src.data;

    #pragma omp parallel for schedule(static)
    for (int y = 2; y < h - 2; ++y)
    {
        for (int x = 2; x < w - 2; ++x)
        {
            float window[25];

            int k = 0;

            for (int dy = -2; dy <= 2; ++dy)
            {
                const int row =
                    (y + dy) * w + x;

                window[k++] = src.data[row - 2];
                window[k++] = src.data[row - 1];
                window[k++] = src.data[row];
                window[k++] = src.data[row + 1];
                window[k++] = src.data[row + 2];
            }

            std::nth_element(
                window,
                window + 12,
                window + 25);

            dst.data[y * w + x] = window[12];
        }
    }
}
// ============================================================
// MAIN
// ============================================================

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // ========================================================
    // IMPORTANT PERFORMANCE SETTINGS
    // ========================================================

    // Keep this FIXED across PCs.
    //
    // Try 4 first.
    //
    // If both PCs have good CPUs, try 6 or 8.
    //
    // Do NOT use omp_get_max_threads() if your goal is
    // more consistent execution across different PCs.
    // ========================================================

    constexpr int OMP_THREADS = 24;

    omp_set_dynamic(0);
    omp_set_num_threads(OMP_THREADS);

    qDebug() << "----------------------------------------";
    qDebug() << "OpenMP threads =" << omp_get_max_threads();
    qDebug() << "OpenMP processors =" << omp_get_num_procs();
    qDebug() << "----------------------------------------";


    // ========================================================
    // CONFIGURATION
    // ========================================================

    const QString inputPath =
            "D:/Enhance DR/Originalproj_0.raw";

    const QString outputPath =
            "D:/Enhance DR/ouputImage.raw";


    // ========================================================
    // PARAMETERS
    // ========================================================

    const float ILLUM_SIGMA = 150.0f;
    const float ILLUM_WEIGHT = 0.35f;

    const float SIGMOID_CONTRAST = 6.5f;
    const float SIGMOID_BRIGHT = 0.7f;

    const float DETAIL_W1 = 1.2f;
    const float DETAIL_W2 = 2.5f;
    const float DETAIL_W3 = 0.8f;

    const float CONTRAST_LIMIT = 0.28f;
    const float CONTRAST_STRENGTH = 1.7F;


    // ========================================================
    // TOTAL TIMER
    // ========================================================

    const auto totalStart =
            std::chrono::high_resolution_clock::now();


    // ========================================================
    // LOAD RAW
    // ========================================================

    const auto tLoad =
            std::chrono::high_resolution_clock::now();

    double memoryBefore = getCurrentMemoryMB();

    QFile file(inputPath);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open file:"
                 << inputPath;

        return -1;
    }

    const QByteArray rawData =
            file.readAll();

    file.close();

    const int totalPixels =
            rawData.size() / 2;

    if (totalPixels <= 0)
    {
        qDebug() << "Invalid RAW file.";

        return -1;
    }


    // ========================================================
    // IMAGE SIZE
    // ========================================================

    int width = 3072;
    int height = 3072;

    const double root =
            std::sqrt(
                static_cast<double>(totalPixels));

    if (root == std::floor(root))
    {
        width =
                static_cast<int>(root);

        height = width;
    }

    qDebug() << "Image:"
             << width
             << "x"
             << height;

    qDebug() << "Pixels:"
             << totalPixels;


    // ========================================================
    // CREATE FLOAT IMAGE
    // ========================================================

    FloatImage img(width, height);

    const uint16_t* ptr =
            reinterpret_cast<const uint16_t*>(
                rawData.constData());


    // --------------------------------------------------------
    // Convert uint16 -> float
    // --------------------------------------------------------

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < totalPixels; ++i)
    {
        img.data[i] =
                static_cast<float>(ptr[i]);
    }


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


    // ========================================================
    // 1. LOGARITHMIC NORMALIZATION
    // ========================================================

    auto t =
            std::chrono::high_resolution_clock::now();

    double memoryBefore1 = getCurrentMemoryMB();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < totalPixels; ++i)
    {
        img.data[i] =
                std::log1pf(img.data[i]);
    }

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


    // ========================================================
    // 2. PHOTOMETRIC MASK
    // ========================================================

    t =
            std::chrono::high_resolution_clock::now();
    double memoryBefore2 = getCurrentMemoryMB();


    FloatImage illum(width, height);

    FloatImage gaussianTemp(width, height);

    fastGaussianBlur(
        img,
        illum,
        gaussianTemp,
        ILLUM_SIGMA);

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < totalPixels; ++i)
    {
        img.data[i] =
                img.data[i]
                - ILLUM_WEIGHT * illum.data[i];
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

    // ========================================================
    // 3. INVERSION
    // ========================================================

    t =
            std::chrono::high_resolution_clock::now();

    double memoryBefore3 = getCurrentMemoryMB();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < totalPixels; ++i)
    {
        img.data[i] =
                1.0f - img.data[i];
    }

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


    // ========================================================
    // 4. AUTOMATIC ADAPTIVE CONTRAST
    // ========================================================

    t =
            std::chrono::high_resolution_clock::now();

    double memoryBefore4 = getCurrentMemoryMB();

    double sum = 0.0;
    double sumSq = 0.0;

    #pragma omp parallel for reduction(+:sum,sumSq) schedule(static)
    for (int i = 0; i < totalPixels; ++i)
    {
        const float val =
                img.data[i];

        sum += val;
        sumSq +=
                static_cast<double>(val) *
                static_cast<double>(val);
    }

    const float meanVal =
            static_cast<float>(
                sum / totalPixels);

    const float variance =
            static_cast<float>(
                (sumSq / totalPixels)
                - static_cast<double>(meanVal) *
                  static_cast<double>(meanVal));

    const float stdDev =
            std::sqrt(
                std::max(
                    1e-6f,
                    variance));


    const float autoGain =
            std::max(
                4.0f,
                std::min(
                    10.0f,
                    0.25f /
                    std::max(
                        1e-4f,
                        stdDev)));

    const float autoMidpoint =
            std::max(
                0.2f,
                std::min(
                    0.8f,
                    meanVal));


    qDebug() << "Mean =" << meanVal;
    qDebug() << "StdDev =" << stdDev;
    qDebug() << "AutoGain =" << autoGain;
    qDebug() << "AutoMidpoint =" << autoMidpoint;


    // --------------------------------------------------------
    // Dynamic sigmoid
    // --------------------------------------------------------

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < totalPixels; ++i)
    {
        const float val =
                img.data[i];

        const float contrastVal =
                1.0f /
                (1.0f +
                 std::exp(
                     -autoGain *
                     (val - autoMidpoint)));

        img.data[i] =
                contrastVal;
    }

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
             << memoryAfter4 - memoryBefore4 << "MB";

    qDebug() << "   Peak Memory:"
             << getPeakMemoryMB() << "MB";


    // ========================================================
    // 5. MEDIAN FILTER
    // ========================================================

    t =
            std::chrono::high_resolution_clock::now();

    double memoryBefore5 = getCurrentMemoryMB();

    FloatImage medianImg(width, height);

    medianFilter5x5(
        img,
        medianImg);

    img.data.swap(
        medianImg.data);

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

    // ========================================================
    // 6. CONTRAST ENHANCEMENT
    // ========================================================

    t =
            std::chrono::high_resolution_clock::now();

    double memoryBefore6 = getCurrentMemoryMB();

    const float s_val =
            std::max(
                6.0f,
                0.03f *
                static_cast<float>(
                    std::min(width, height)));


    // --------------------------------------------------------
    // Large blur
    // --------------------------------------------------------

    FloatImage blur_s(width, height);

    fastGaussianBlur(
        img,
        blur_s,
        gaussianTemp,
        s_val);


    // --------------------------------------------------------
    // Percentiles
    // --------------------------------------------------------

    const float p1 =
            getPercentileFast(
                img.data,
                1.0f);

    const float p99 =
            getPercentileFast(
                img.data,
                99.0f);

    const float cap =
            std::max(
                1e-6f,
                CONTRAST_LIMIT *
                (p99 - p1));

    const float invCap =
            1.0f / cap;


    // --------------------------------------------------------
    // Bilateral-like blur
    // --------------------------------------------------------

    FloatImage blur_bf(width, height);

    fastGaussianBlur(
        img,
        blur_bf,
        gaussianTemp,
        std::max(4.0f, s_val * 0.25f));


    // --------------------------------------------------------
    // Detail boost
    // --------------------------------------------------------

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < totalPixels; ++i)
    {
        const float original =
                img.data[i];

        const float detail =
                original -
                blur_s.data[i];

        const float outVal =
                original +
                CONTRAST_STRENGTH *
                cap *
                std::tanh(
                    detail * invCap);

        const float bf =
                std::max(
                    0.0f,
                    std::min(
                        1.0f,
                        blur_bf.data[i]));

        img.data[i] =
                bf * outVal +
                (1.0f - bf) * original;
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


    // ========================================================
    // 7. TONE MAPPING
    // ========================================================

    t =
            std::chrono::high_resolution_clock::now();

    double memoryBefore7 = getCurrentMemoryMB();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < totalPixels; ++i)
    {
        const float val =
                img.data[i];

        img.data[i] =
                1.0f /
                (1.0f +
                 std::exp(
                     -SIGMOID_CONTRAST *
                     (val - SIGMOID_BRIGHT)));
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


    // ========================================================
    // 8. MULTISCALE DETAIL AMPLIFICATION
    // ========================================================

    t =
            std::chrono::high_resolution_clock::now();

    double memoryBefore8 = getCurrentMemoryMB();

    FloatImage b1(width, height);
    FloatImage b2(width, height);
    FloatImage b3(width, height);

    fastGaussianBlur(img, b1, gaussianTemp, 1.0f);
    fastGaussianBlur(img, b2, gaussianTemp, 4.0f);
    fastGaussianBlur(img, b3, gaussianTemp, 12.0f);


    #pragma omp parallel for schedule(static)
    for (int i = 0; i < totalPixels; ++i)
    {
        const float d1 =
                img.data[i] -
                b1.data[i];

        const float d2 =
                b1.data[i] -
                b2.data[i];

        const float d3 =
                b2.data[i] -
                b3.data[i];

        const float result =
                img.data[i]
                + DETAIL_W1 * d1
                + DETAIL_W2 * d2
                + DETAIL_W3 * d3;

        img.data[i] =
                std::max(
                    0.0f,
                    std::min(
                        1.0f,
                        result));
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



    // ========================================================
    // 9. FINAL CLEANUP / SHARPENING
    // ========================================================

    t =
            std::chrono::high_resolution_clock::now();

    double memoryBefore9 = getCurrentMemoryMB();

    normalize01(img);

    FloatImage finalBlur(width, height);

    fastGaussianBlur(
        img,
        finalBlur,
        gaussianTemp,
        1.0f);


    // ========================================================
    // CREATE 16-BIT OUTPUT
    // ========================================================

    vector<uint16_t> res16(
        static_cast<size_t>(totalPixels));


    #pragma omp parallel for schedule(static)
    for (int i = 0; i < totalPixels; ++i)
    {
        const float res_f =
                std::max(
                    0.0f,
                    std::min(
                        1.0f,
                        (img.data[i] * 1.5f)
                        -
                        (finalBlur.data[i] * 0.5f)));

        res16[i] =
                static_cast<uint16_t>(
                    res_f * 65535.0f);
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


    // ========================================================
    // SAVE OUTPUT
    // ========================================================

    t =
            std::chrono::high_resolution_clock::now();

    double memoryBefore10 = getCurrentMemoryMB();

    QFileInfo info(outputPath);

    const QString path =
            info.absolutePath();

    const QString fileName =
            info.fileName();

    QDir().mkpath(path);

    const QString savePath =
            path + "/" + fileName;


    QFile fileOut(savePath);

    if (!fileOut.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to save:"
                 << fileOut.errorString();

        return -1;
    }

    fileOut.write(
        reinterpret_cast<const char*>(
            res16.data()),
        static_cast<qint64>(
            res16.size() *
            sizeof(uint16_t)));

    fileOut.close();


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
