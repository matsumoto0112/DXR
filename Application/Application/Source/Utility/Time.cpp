#include "Time.h"

namespace Framework::Utility {
    //コンストラクタ
    Time::Time(UINT sampleNum) : mTime(0.0) {
        setSampleCount(sampleNum);
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        mFreq = static_cast<double>(freq.QuadPart);
    }
    //デストラクタ
    Time::~Time() {}
    void Time::update() {
        double diff = getCurrentDefTime();

        mDifTimes.pop_front();

        mDifTimes.push_back(diff);

        double average = (mSumTimes + diff) / mSampleCount;
        if (average != 0) mFPS = 1000.0 / average;

        mSumTimes += diff - mDifTimes.front();
        mDiffTime = diff / 1000.0;
        mTime += mDiffTime;
    }
    //サンプリング数を設定
    void Time::setSampleCount(UINT sample) {
        mSampleCount = sample;
        mSumTimes = 0.0f;
        mDifTimes.resize(mSampleCount, 0.0);
    }
    //前フレームとの差分時間
    double Time::getCurrentDefTime() {
        QueryPerformanceCounter(&mCounter);
        LONGLONG diff = mCounter.QuadPart - mPrevCount;
        mPrevCount = mCounter.QuadPart;
        return diff * 1000.0 / mFreq;
    }
} // namespace Framework::Utility
