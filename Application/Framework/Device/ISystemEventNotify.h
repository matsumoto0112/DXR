#pragma once

namespace Framework::Device {
    /**
    * @class ISystemEventNotify
    * @brief �V�X�e���̃C�x���g�ʒm�C���^�[�t�F�[�X
    */
    class ISystemEventNotify {
    public:
        /**
        * @brief �f�X�g���N�^
        */
        virtual ~ISystemEventNotify() = default;
        virtual void onInit() = 0;
        virtual void onUpdate() = 0;
        virtual void onRender() = 0;
        virtual void onDestroy() = 0;
    };
} //Framework::Device
