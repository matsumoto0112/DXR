#include "Path.h"
#include <Windows.h>

namespace {
    constexpr int MAX_SIZE = 1024;
    //実行ファイルまでのパスを取得する
    inline std::wstring getExePath() {
        wchar_t path[MAX_SIZE];
        DWORD ret = GetModuleFileName(nullptr, path, sizeof(path));
        std::wstring res(path);
        return res;
    }
}

namespace Framework::Utility {
    //コンストラクタ
    ExePath::ExePath() {
        mExe = getExePath();
    }
    //デストラクタ
    ExePath::~ExePath() { }
} //Framework::Utility 
