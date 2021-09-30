//===============================================
//
// ポーズ処理 (pause.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "pause.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "sound.h"
#include "fade.h"
#include "ui.h"
#include "camera.h"
#include "texture.h"

//=======================================
// マクロ定義
//=======================================

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CPause::m_apTexture[CPause::TYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 CPause::m_pVtxBuff = NULL;

//========================================
// ポーズのコンストラクタ
// Author : 後藤慎之助
//========================================
CPause::CPause()
{
    m_pVtxBuff = NULL;

    m_nWho = PLAYER_1;
    m_state = STATE_NONE;
    m_typePauseSelect = TYPE_CONTINUE;
    m_colSelect = DEFAULT_COLOR;
    m_bAlphaDown = true;
    m_nSelectCoolTime = 0;
}

//========================================
// ポーズのデストラクタ
// Author : 後藤慎之助
//========================================
CPause::~CPause()
{

}

//========================================
// ポーズの初期化処理
// Author : 後藤慎之助
//========================================
HRESULT CPause::Init(void)
{
    // 画像を結びつける
    m_apTexture[TYPE_BG] = CManager::GetTexture()->GetInfo(16)->pTexture;
    m_apTexture[TYPE_CONTINUE] = CManager::GetTexture()->GetInfo(13)->pTexture;
    m_apTexture[TYPE_RETRY] = CManager::GetTexture()->GetInfo(14)->pTexture;
    m_apTexture[TYPE_QUIT] = CManager::GetTexture()->GetInfo(15)->pTexture;

    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 頂点バッファ生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*TYPE_MAX,
        D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
    VERTEX_2D *pVtx = NULL;	//頂点情報へのポインタ

                            // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

    // 頂点座標の設定（右回りで設定する）
    // 背景
    pVtx[0].pos = PAUSE_BG_POS + D3DXVECTOR3(-(SCREEN_WIDTH / 2), -(SCREEN_HEIGHT / 2), 0.0f);
    pVtx[1].pos = PAUSE_BG_POS + D3DXVECTOR3(+(SCREEN_WIDTH / 2), -(SCREEN_HEIGHT / 2), 0.0f);
    pVtx[2].pos = PAUSE_BG_POS + D3DXVECTOR3(-(SCREEN_WIDTH / 2), +(SCREEN_HEIGHT / 2), 0.0f);
    pVtx[3].pos = PAUSE_BG_POS + D3DXVECTOR3(+(SCREEN_WIDTH / 2), +(SCREEN_HEIGHT / 2), 0.0f);

    // コンティニュー
    pVtx[4].pos = PAUSE_CONTINUE_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[5].pos = PAUSE_CONTINUE_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[6].pos = PAUSE_CONTINUE_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[7].pos = PAUSE_CONTINUE_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);

    // リトライ
    pVtx[8].pos = PAUSE_RETRY_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[9].pos = PAUSE_RETRY_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[10].pos = PAUSE_RETRY_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[11].pos = PAUSE_RETRY_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);

    // やめる
    pVtx[12].pos = PAUSE_QUIT_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[13].pos = PAUSE_QUIT_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), -(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[14].pos = PAUSE_QUIT_POS + D3DXVECTOR3(-(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);
    pVtx[15].pos = PAUSE_QUIT_POS + D3DXVECTOR3(+(PAUSE_MENU_SIZE_X / 2), +(PAUSE_MENU_SIZE_Y / 2), 0.0f);

    for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
    {
        // 色(全て透明にする)
        pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

        // rhwの設定（値は1.0で設定する）
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        // テクスチャ座標の設定
        pVtx[0].tex[0] = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex[0] = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex[0] = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex[0] = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();

    return S_OK;
}

//========================================
// ポーズの終了処理
// Author : 後藤慎之助
//========================================
void CPause::Uninit(void)
{
    // 頂点バッファの破棄
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }
}

//========================================
// ポーズの更新処理
// Author : 後藤慎之助
//========================================
void CPause::Update(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(m_nWho);

    // カメラ取得
    CCamera*pCamera = CManager::GetCamera();

    // 選択しているものの、色を変える
    if (m_bAlphaDown == true)
    {
        m_colSelect.r -= PAUSE_MENU_FADE_RATE;
        m_colSelect.g -= PAUSE_MENU_FADE_RATE;
        m_colSelect.b -= PAUSE_MENU_FADE_RATE;
    }
    else
    {
        m_colSelect.r += PAUSE_MENU_FADE_RATE;
        m_colSelect.g += PAUSE_MENU_FADE_RATE;
        m_colSelect.b += PAUSE_MENU_FADE_RATE;
    }
    if (m_colSelect.r <= PAUSE_MENU_FADE_OUT_MIN)
    {
        m_bAlphaDown = false;
    }
    else if (m_colSelect.r >= 1.0f)
    {
        m_bAlphaDown = true;
    }

    // 選択のクールタイムを数える
    if (m_nSelectCoolTime > 0)
    {
        m_nSelectCoolTime--;
    }

    // 変数宣言
    // 頂点データの範囲をロックし、頂点バッファへのポインタを取得
    VERTEX_2D *pVtx = NULL;
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
    D3DXCOLOR inactive = PAUSE_MENU_INACTIVE_COLOR; // 非アクティブ色

    switch (m_state)
    {
    case STATE_PAUSE:

        // スタートボタンが押されたら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_P) || pInputJoypad->GetJoypadTrigger(m_nWho, CInputJoypad::BUTTON_START))
        {
            // SE
            CManager::SoundPlay(CSound::LABEL_SE_CANCEL);

            // ポーズ解除にする
            CGame::SetQuitPause();
            pCamera->SetState(CCamera::STATE_BUTTLE);

            // 選択肢をコンティニューへ戻す
            m_typePauseSelect = TYPE_CONTINUE;

            // 状態をリセット
            m_state = STATE_NONE;
        }
        else
        {
            // ポーズメニューを選択する
            SelectPauseMenu();
        }

        // ポーズの背景を見えるように
        pVtx[0].col = DEFAULT_COLOR;
        pVtx[1].col = DEFAULT_COLOR;
        pVtx[2].col = DEFAULT_COLOR;
        pVtx[3].col = DEFAULT_COLOR;

        // 選択によって、色を変える
        switch (m_typePauseSelect)
        {
        case TYPE_CONTINUE:

            // コンティニュー
            pVtx[4].col = m_colSelect;
            pVtx[5].col = m_colSelect;
            pVtx[6].col = m_colSelect;
            pVtx[7].col = m_colSelect;

            // リトライ
            pVtx[8].col = inactive;
            pVtx[9].col = inactive;
            pVtx[10].col = inactive;
            pVtx[11].col = inactive;

            // やめる
            pVtx[12].col = inactive;
            pVtx[13].col = inactive;
            pVtx[14].col = inactive;
            pVtx[15].col = inactive;

            break;

        case TYPE_RETRY:

            // コンティニュー
            pVtx[4].col = inactive;
            pVtx[5].col = inactive;
            pVtx[6].col = inactive;
            pVtx[7].col = inactive;

            // リトライ
            pVtx[8].col = m_colSelect;
            pVtx[9].col = m_colSelect;
            pVtx[10].col = m_colSelect;
            pVtx[11].col = m_colSelect;

            // やめる
            pVtx[12].col = inactive;
            pVtx[13].col = inactive;
            pVtx[14].col = inactive;
            pVtx[15].col = inactive;

            break;

        case TYPE_QUIT:

            // コンティニュー
            pVtx[4].col = inactive;
            pVtx[5].col = inactive;
            pVtx[6].col = inactive;
            pVtx[7].col = inactive;

            // リトライ
            pVtx[8].col = inactive;
            pVtx[9].col = inactive;
            pVtx[10].col = inactive;
            pVtx[11].col = inactive;

            // やめる
            pVtx[12].col = m_colSelect;
            pVtx[13].col = m_colSelect;
            pVtx[14].col = m_colSelect;
            pVtx[15].col = m_colSelect;

            break;
        }

        break;
    }

    // 状態がなくなったら、色をリセット
    if (m_state == STATE_NONE)
    {
        for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
        {
            // 色(全て透明にする)
            pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
            pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
            pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
            pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

            pVtx += 4;
        }
    }

    //頂点データをアンロックする
    m_pVtxBuff->Unlock();
}

//========================================
// ポーズの描画処理
// Author : 後藤慎之助
//========================================
void CPause::Draw(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
    {
        // テクスチャの設定
        pDevice->SetTexture(0, m_apTexture[nCnt]);

        // ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, NUM_VERTEX * nCnt, NUM_POLYGON);
    }

    // テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=======================================
// ポーズの作成
// Author : 後藤慎之助
//=======================================
CPause *CPause::Create(void)
{
    CPause *pPause = NULL;

    // メモリを確保
    pPause = new CPause;

    // 初期化
    pPause->Init();

    return pPause;
}

//=======================================
// ポーズのメニュー選択
// Author : 後藤慎之助
//=======================================
void CPause::SelectPauseMenu(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(m_nWho);

    // カメラ取得
    CCamera*pCamera = CManager::GetCamera();

    // フェードしていないなら、選択可能
    if (CFade::GetFade() == CFade::FADE_NONE)
    {
        switch (m_typePauseSelect)
        {
        case TYPE_CONTINUE:

            // Aが押された(決定)
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(m_nWho, CInputJoypad::BUTTON_A))
            {
                // SE
                CManager::SoundPlay(CSound::LABEL_SE_SELECT);

                // ポーズ解除にする
                CGame::SetQuitPause();
                pCamera->SetState(CCamera::STATE_BUTTLE);

                // 状態をリセット
                m_state = STATE_NONE;
            }
            else
            {
                // 上移動
                if (pInputKeyboard->GetKeyboardTrigger(DIK_W) || Controller.lY < 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // 上の選択肢に
                    m_typePauseSelect = TYPE_QUIT;

                    // 選択のクールタイムを設定
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }

                // 下移動
                if (pInputKeyboard->GetKeyboardTrigger(DIK_S) || Controller.lY > 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // 下の選択肢に
                    m_typePauseSelect = TYPE_RETRY;

                    // 選択のクールタイムを設定
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }
            }

            break;

        case TYPE_RETRY:

            // Aが押された(決定)
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(m_nWho, CInputJoypad::BUTTON_A))
            {
                // SE
                CManager::SoundPlay(CSound::LABEL_SE_SELECT);

                // リトライ
                CFade::SetFade(CManager::MODE_GAME);

                // 状態をリセット
                m_state = STATE_NONE;
            }
            else
            {
                // 上移動
                if (pInputKeyboard->GetKeyboardTrigger(DIK_W) || Controller.lY < 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // 上の選択肢に
                    m_typePauseSelect = TYPE_CONTINUE;

                    // 選択のクールタイムを設定
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }

                // 下移動
                if (pInputKeyboard->GetKeyboardTrigger(DIK_S) || Controller.lY > 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // 下の選択肢に
                    m_typePauseSelect = TYPE_QUIT;

                    // 選択のクールタイムを設定
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }
            }

            break;

        case TYPE_QUIT:

            // Aが押された(決定)
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(m_nWho, CInputJoypad::BUTTON_A))
            {
                // SE
                CManager::SoundPlay(CSound::LABEL_SE_SELECT);

                // やめる(タイトルへ)
                CFade::SetFade(CManager::MODE_TITLE);

                // 状態をリセット
                m_state = STATE_NONE;
            }
            else
            {
                // 上移動
                if (pInputKeyboard->GetKeyboardTrigger(DIK_W) || Controller.lY < 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // 上の選択肢に
                    m_typePauseSelect = TYPE_RETRY;

                    // 選択のクールタイムを設定
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }

                // 下移動
                if (pInputKeyboard->GetKeyboardTrigger(DIK_S) || Controller.lY > 0 && m_nSelectCoolTime <= 0)
                {
                    // SE
                    CManager::SoundPlay(CSound::LABEL_SE_CUSTOM);

                    // 下の選択肢に
                    m_typePauseSelect = TYPE_CONTINUE;

                    // 選択のクールタイムを設定
                    m_nSelectCoolTime = MENU_SELECT_COOL_TIME;
                }
            }

            break;
        }
    }
}