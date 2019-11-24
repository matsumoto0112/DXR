#pragma once
#include <memory>
#include <wrl.h>

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

template <class T>
using UniquePtr = std::unique_ptr<T>;

template <class T>
using SharedPtr = std::shared_ptr<T>;

template <class T>
using WeakPtr = std::weak_ptr<T>;
