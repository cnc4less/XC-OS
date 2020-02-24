#include "Basic/FileGroup.h"
#include "DisplayPrivate.h"
#include "BV_Player/BV_Player.h"

static lv_obj_t * appWindow;

#define BV_BUFF_SIZE (2 * 1024)
static uint8_t BvBuffer[BV_BUFF_SIZE];

BV_Player BvPlayer(BvBuffer, BV_BUFF_SIZE);
static lv_task_t * taskBvPlayer;

static String BvFilePath;
void Bv_OpenFile(const char* path)
{
    BvFilePath = String(path);
    page.PagePush(PAGE_BvPlayer);
}

static void Screen_FastDrawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
    screen.drawFastRGBBitmap(x, y, bitmap, w, h);
}

static void Screen_FastDrawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h)
{
    screen.drawFastBitmap(x, y, bitmap, w, h);
}

static void Task_BvPlyaerUpdate(lv_task_t * task)
{
    BvPlayer.UpdatePlayerState();
    if(BvPlayer.IsEnd())
    {
        page.PagePop();
    }
}

/**
  * @brief  ҳ���ʼ���¼�
  * @param  ��
  * @retval ��
  */
static void Setup()
{
    lv_obj_move_foreground(appWindow);
    taskBvPlayer = lv_task_create(Task_BvPlyaerUpdate, 33, LV_TASK_PRIO_MID, 0);
    memset(BvBuffer, 0, BV_BUFF_SIZE);
    if(!BvPlayer.OpenVideo(BvFilePath))
    {
        return;
    }
    BvPlayer.BaseX = (APP_WIN_WIDTH - BvPlayer.Head.Width) * 0.5f;
    BvPlayer.BaseY = (APP_WIN_HEIGHT - BvPlayer.Head.Height) * 0.5f + BarStatus_GetHeight();
    BvPlayer.SetPlayerState(BvPlayer.Run);
    BvPlayer.DrawPic_16bit = Screen_FastDrawRGBBitmap;
    BvPlayer.DrawPic_8bit = Screen_FastDrawBitmap;
}

/**
  * @brief  ҳ���˳��¼�
  * @param  ��
  * @retval ��
  */
static void Exit()
{
    BvPlayer.CloseVideo();
    lv_task_del(taskBvPlayer);
}

/**
  * @brief  ҳ���¼�
  * @param  event:�¼����
  * @param  param:�¼�����
  * @retval ��
  */
static void Event(int event, void* param)
{
    lv_obj_t * btn = (lv_obj_t*)param;
    if(event == LV_EVENT_CLICKED)
    {
        if(btn == btnBack)
        {
            page.PagePop();
        }
    }
}

/**
  * @brief  ҳ��ע��
  * @param  pageID:Ϊ��ҳ������ID��
  * @retval ��
  */
void PageRegister_BvPlayer(uint8_t pageID)
{
    appWindow = AppWindow_GetCont(pageID);
    lv_style_t * style = (lv_style_t *)lv_cont_get_style(appWindow, LV_CONT_STYLE_MAIN);
    *style = lv_style_pretty;
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}