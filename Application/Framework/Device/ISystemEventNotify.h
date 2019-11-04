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
        /**
        * @brief ������
        */
        virtual void onInit() = 0;
        /**
        * @brief �X�V
        */
        virtual void onUpdate() = 0;
        /**
        * @brief �`��
        */
        virtual void onRender() = 0;
        /**
        * @brief �j��
        */
        virtual void onDestroy() = 0;
    };
} //Framework::Device
