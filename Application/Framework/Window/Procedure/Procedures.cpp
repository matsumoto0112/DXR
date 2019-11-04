#include "Procedures.h"

namespace Framework::Window {
    std::vector<std::unique_ptr<IWindowProc>> Procedures::mWindowProcs = {}; //!< �v���V�[�W���������X�g

    //���C���v���V�[�W��
    LRESULT Procedures::mainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        bool isReturn = false;
        //�擪���珇�ɏ����ł���܂Ŏ���
        for (auto&& proc : mWindowProcs) {
            LRESULT res = proc->wndProc(hWnd, msg, wParam, lParam, &isReturn);
            if (isReturn) return res;
        }
        //�����Ȃ���΃f�t�H���g�̏���������
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    //�v���V�[�W���̒ǉ�
    void Procedures::addProc(IWindowProc* proc) {
        mWindowProcs.emplace_back(std::unique_ptr<IWindowProc>(proc));
    }
} //Framework::Window
