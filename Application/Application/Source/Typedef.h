#pragma once

#include <wrl.h>
#include <memory>

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

template <class T>
using UniquePtr = std::unique_ptr<T>;

template <class T>
using SharedPtr = std::shared_ptr<T>;

template <class T>
using WeakPtr = std::weak_ptr<T>;

using Vec2 = Framework::Math::Vector2;
using Vec3 = Framework::Math::Vector3;
using Vec4 = Framework::Math::Vector4;
using Mat4 = Framework::Math::Matrix4x4;
using Deg = Framework::Math::Degrees;
using Rad = Framework::Math::Radians;
using Color = Framework::Utility::Color4;
