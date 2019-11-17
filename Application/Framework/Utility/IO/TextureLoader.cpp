#include "TextureLoader.h"
#include <vector>
#include <atlstr.h>
#include "Framework/Utility/Debug.h"

namespace Framework::Utility {
    TextureLoader::TextureLoader() {
        CoInitialize(NULL);
        //イメージファクトリ作成
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory,
            nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&mFactory));
        MY_THROW_IF_FAILED(hr);

        //フォーマット変換器作成
        hr = mFactory->CreateFormatConverter(&mConverter);
        MY_THROW_IF_FAILED(hr);
    }

    TextureLoader::~TextureLoader() { }

    Desc::TextureDesc TextureLoader::load(const std::wstring& filepath) {
           //パスからデコーダーを作成
        HRESULT hr = mFactory->CreateDecoderFromFilename(
            filepath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &mDecoder);
        MY_THROW_IF_FAILED_LOG(hr, "テクスチャの読み込みに失敗しました。");

        //フレームの取得
        hr = mDecoder->GetFrame(0, &mFrame);
        MY_THROW_IF_FAILED(hr);

        Desc::TextureDesc desc = {};
        mFrame->GetSize(&desc.width, &desc.height);
        //ピクセル形式を取得
        WICPixelFormatGUID pixelFormat;
        hr = mFrame->GetPixelFormat(&pixelFormat);
        MY_THROW_IF_FAILED(hr);

        int stride = desc.width * 4;
        int bufferSize = stride * desc.height;
        desc.pixels.resize(bufferSize);
        //ピクセル形式が32bitRGBAでなかったら変換する
        if (pixelFormat != GUID_WICPixelFormat32bppRGBA) {
            hr = mFactory->CreateFormatConverter(&mConverter);
            MY_THROW_IF_FAILED(hr);

            hr = mConverter->Initialize(mFrame, GUID_WICPixelFormat32bppRGBA,
                WICBitmapDitherTypeErrorDiffusion, nullptr, 0, WICBitmapPaletteTypeCustom);
            MY_THROW_IF_FAILED(hr);

            hr = mConverter->CopyPixels(0, stride, bufferSize, desc.pixels.data());
            MY_THROW_IF_FAILED(hr);
        }
        else {
            hr = mFrame->CopyPixels(nullptr, stride, bufferSize, desc.pixels.data());
            MY_THROW_IF_FAILED(hr);
        }

        return desc;
    }
} //Framework::Utility
