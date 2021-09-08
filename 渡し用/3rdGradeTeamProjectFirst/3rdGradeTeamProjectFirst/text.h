//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//
//	text.h
//	Author:�r�c�I��
//
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#ifndef _TEXT_H_
#define _TEXT_H_

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�C���N���[�h�t�@�C��
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
#include "main.h"
#include "scene.h"

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
//�N���X��`
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
class CText : public CScene
{
public:
    //�񋓌^�錾
    typedef enum
    {
        ALIGN_LEFT = 0,
        ALIGN_CENTER,
        ALIGN_RIGHT
    }ALIGN;

    CText();	//�R���X�g���N�^
    ~CText();	//�f�X�g���N�^
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	//����������
    void Uninit(void);	//�I������
    void Update(void);	//�X�V����
    void Draw(void);	//�`�揈��
    static CText* Create(D3DXVECTOR3 pos, int nSize, char* pStr, ALIGN align, char* pFontName, D3DCOLOR col, int nLimWidth = 0);	//��������
    static void Load(void);	//�t�H���g�t�@�C���̓ǂݍ���
    static void Unload(void);	//�t�H���g�f�[�^�̔j��

    void SetText(char* pStr);	//�e�L�X�g�ύX�֐�
    void SetColor(D3DCOLOR col) { m_col = col; }

private:
    LPD3DXFONT m_pFont;	    //�t�H���g�ۑ��p�ϐ�
    D3DXVECTOR3 m_pos;	    //���W
    int m_nSize;	        //�����̃T�C�Y
    int m_nLimWidth;	    //������̕��̏��
    D3DCOLOR m_col;	        //�F
    ALIGN m_align;	        //�����̐���̕��@
    DWORD m_format;	        //������@�ϊ��p�ϐ�
    char m_fontName[128];	//�t�H���g�̖��O
    char m_str[512];	    //�\�����镶����
};
#endif