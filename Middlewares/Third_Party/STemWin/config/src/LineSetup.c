/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.32                          *
*        Compiled Oct  8 2015, 11:59:02                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "LineSetup.h"
#include "timedate.h"

#include "guivars.h"
#include "callbacks.h"
#include "string.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "mainMenu.h"
#include "LineSetupPulse.h"
#include "lines.h"
#include "backup.h"
extern RTC_TimeTypeDef sTime;

extern TIM_HandleTypeDef htim7;



/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)

extern GUI_CONST_STORAGE GUI_CHARINFO GUI_FontArial18_CharInfo[192];
Lines lineTemp[4];
int16_t diff;
WM_MESSAGE message;
void lineChangeStatus(uint8_t linenum, uint8_t status)
{
	masterClock.line[linenum].Status = status;
	lineTemp[linenum].Status = status;
	switch (status) {
	case LINE_STATUS_STOP:
		BUTTON_SetTextColor(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_DISABLED, GUI_GRAY);
		BUTTON_SetTextColor(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_PRESSED, GUI_RED);
		BUTTON_SetTextColor(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_UNPRESSED, GUI_RED);

		BUTTON_SetText(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), "�����");
		//HEADER_SetItemText(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_HEADER_LINESETUP_VALS), 2, "����");
		//HEADER_SetTextColor(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_HEADER_LINESETUP_VALS), GUI_WHITE);
		break;
	case LINE_STATUS_RUN:
		BUTTON_SetTextColor(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_DISABLED, GUI_GRAY);
		BUTTON_SetTextColor(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_PRESSED, GUI_WHITE);
		BUTTON_SetTextColor(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_UNPRESSED, GUI_WHITE);
		BUTTON_SetText(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), "����");
		//HEADER_SetItemText(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_HEADER_LINESETUP_VALS), 2, "�����");
		//HEADER_SetTextColor(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_HEADER_LINESETUP_VALS), GUI_WHITE);
		break;
	case LINE_STATUS_OFF:
		BUTTON_SetTextColor(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_DISABLED, GUI_GRAY);
		BUTTON_SetTextColor(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_PRESSED, GUI_WHITE);
		BUTTON_SetTextColor(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_UNPRESSED, GUI_WHITE);
		BUTTON_SetText(WM_GetDialogItem(masterClock.handles->hLineSetupMenu, ID_BUTTON_LINESETUP_RUNSTOP), " ");
		break;
	}
}
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_LINESETUP, 1, -3, 320, 240, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_LINESETUP_Hplus, 5, 85, 70, 70, 0, 0x0, 0 },
	{ HEADER_CreateIndirect, "Header", ID_HEADER_LINESETUP_VALSNAMES, 0, 40, 240, 20, 0, 0x0, 0 },
	{ HEADER_CreateIndirect, "Header", ID_HEADER_LINESETUP_VALS, 0, 60, 240, 20, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_LINESETUP_Mplus, 85, 85, 70, 70, 0, 0x0, 0 },
	//{ BUTTON_CreateIndirect, "", ID_BUTTON_LINESETUP_Splus, 165, 85, 70, 70, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "����", ID_BUTTON_LINESETUP_ENTER, 245, 85, 70, 70, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "����", ID_BUTTON_LINESETUP_RUNSTOP, 245, 5, 70, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_LINESETUP_PULSE, 245, 45, 70, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_LINESETUP_Hminus, 5, 165, 70, 70, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_LINESETUP_Mminus, 85, 165, 70, 70, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_LINESETUP_Zplus, 165, 85, 70, 70, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_LINESETUP_Zminus, 165, 165, 70, 70, 0, 0x0, 0 },
	//{ BUTTON_CreateIndirect, "", ID_BUTTON_LINESETUP_Sminus, 165, 165, 70, 70, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "�����", ID_BUTTON_LINESETUP_BACK, 245, 165, 70, 70, 0, 0x0, 0 },
	{ HEADER_CreateIndirect, "Header", ID_HEADER_LINESETUP_STATVALS, 0, 20, 240, 20, 0, 0x0, 0 },
	{ HEADER_CreateIndirect, "Header", ID_HEADER_LINESETUP_TOP, 0, 0, 240, 20, 0, 0x0, 0 },
	// USER START (Optionally insert additional widgets)
	// USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	char str[9];
	uint8_t i = 0;
	uint8_t valsChangedOld = false;

	// USER START (Optionally insert additional variables)
	// USER END

	switch(pMsg->MsgId) {
	case WM_SEC_UPDATE:
		sprintf(str, "%02d:%02d:%02d", hoursToUTC(sTime.Hours, masterClock.daylightSaving->timeZone), sTime.Minutes, sTime.Seconds);
		TFT_LineSetupShowString(173, 17, str, 18, 0xFFFF);
		break;
	case WM_INIT_DIALOG:

		lineTemp[0] = masterClock.line[0];
		lineTemp[1] = masterClock.line[1];
		lineTemp[2] = masterClock.line[2];
		lineTemp[3] = masterClock.line[3];

		//
	// Initialization of Main Window
	//
		hItem = pMsg->hWin;
		WINDOW_SetBkColor(hItem, 0x191615);
		//
		// Initialization of 'Header'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_LINESETUP_VALSNAMES);
		HEADER_AddItem(hItem, 80, "���", 14);
		HEADER_AddItem(hItem, 80, "���", 14);

		HEADER_AddItem(hItem, 80, "����:", 14);

		HEADER_SetTextColor(hItem, GUI_WHITE);
		//
		// Initialization of 'Header'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_LINESETUP_VALS);
		masterClock.handles->hLineSetupVals = hItem;
		sprintf(str, "%02d", masterClock.line[masterClock.guiVars->menuState - 4].Hours);
		HEADER_AddItem(hItem, 80, str, 14);
		sprintf(str, "%02d", masterClock.line[masterClock.guiVars->menuState - 4].Minutes);
		HEADER_AddItem(hItem, 80, str, 14);


		if ((int8_t)masterClock.line[masterClock.guiVars->menuState - 4].TimeZone > 0)
		{
			sprintf(str, "+%d", masterClock.line[masterClock.guiVars->menuState - 4].TimeZone);
		}
		else if ((int8_t)masterClock.line[masterClock.guiVars->menuState - 4].TimeZone <= 0)
		{
			sprintf(str, "%d", (int8_t)masterClock.line[masterClock.guiVars->menuState - 4].TimeZone);
		}

		HEADER_AddItem(hItem, 80, str, 14);
		if (masterClock.guiVars->menuState == MENU_STATE_LINE1SETUP)
		{
			WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_Zminus));
			WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_Zplus));
		}


		switch (masterClock.line[masterClock.guiVars->menuState - 4].Status)
		{
		case LINE_STATUS_RUN:
			BUTTON_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_DISABLED, GUI_GRAY);
			BUTTON_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_PRESSED, GUI_WHITE);
			BUTTON_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_UNPRESSED, GUI_WHITE);
			BUTTON_SetText(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), "����");

			break;
		case LINE_STATUS_STOP:
			BUTTON_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_DISABLED, GUI_GRAY);
			BUTTON_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_PRESSED, GUI_RED);
			BUTTON_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_UNPRESSED, GUI_RED);
			BUTTON_SetText(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), "�����");

			break;
		case LINE_STATUS_OFF:
			BUTTON_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_DISABLED, GUI_GRAY);
			BUTTON_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_PRESSED, GUI_WHITE);
			BUTTON_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), BUTTON_CI_UNPRESSED, GUI_WHITE);
			BUTTON_SetText(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_RUNSTOP), " ");

			HEADER_SetItemText(hItem, 0, "");
			HEADER_SetItemText(hItem, 1, "");
			HEADER_SetItemText(hItem, 2, "");
			WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_Hminus));
			WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_Hplus));
			WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_Mminus));
			WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_Mplus));
			WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_Zminus));
			WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_Zplus));

			break;
		}



		HEADER_SetTextColor(hItem, GUI_WHITE);
		//
		// Initialization of 'Header'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_LINESETUP_STATVALS);

		HEADER_AddItem(hItem, 160, "����� UTC(+0): ", 14);
		HEADER_AddItem(hItem, 80, " ", 14);
		HEADER_SetTextColor(hItem, GUI_WHITE);

		//
		// Initialization of 'Header'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_LINESETUP_TOP);

		switch (masterClock.guiVars->menuState)
		{
		case MENU_STATE_LINE1SETUP:
			HEADER_AddItem(hItem, 240, "����� 1. ��������� �������.", 14);
			break;
		case MENU_STATE_LINE2SETUP:
			HEADER_AddItem(hItem, 240, "����� 2. ��������� �������.", 14);
			break;
		case MENU_STATE_LINE3SETUP:
			HEADER_AddItem(hItem, 240, "����� 3. ��������� �������.", 14);
			break;
		case MENU_STATE_LINE4SETUP:
			HEADER_AddItem(hItem, 240, "����� 4. ��������� �������.", 14);
			break;
		}
		HEADER_SetTextColor(hItem, GUI_WHITE);
		// USER START (Optionally insert additional code for further widget initialization)
		// USER END
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;

		switch (Id) {
		case ID_BUTTON_LINESETUP_Hplus: // Notifications sent by 'h+'
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)

				if(lineTemp[masterClock.guiVars->menuState - 4].Hours != 23)
				{
					pollButton(ID_BUTTON_LINESETUP_Hplus, WM_NOTIFICATION_CLICKED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].Hours);
					lineChangeStatus(masterClock.guiVars->menuState - 4, LINE_STATUS_STOP);
				}
				else
				{
					masterClock.longPressCNT->value = 23;
				}
				if (valsChangedOld != masterClock.guiVars->valsChanged)
				{
					WM_Invalidate(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_ENTER));
				}
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				pollButton(ID_BUTTON_LINESETUP_Hplus, WM_NOTIFICATION_RELEASED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].Hours);

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;

		case ID_BUTTON_LINESETUP_Mplus: // Notifications sent by 'm+'
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				if(lineTemp[masterClock.guiVars->menuState - 4].Minutes != 59)
				{
					pollButton(ID_BUTTON_LINESETUP_Mplus, WM_NOTIFICATION_CLICKED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].Minutes);
					lineChangeStatus(masterClock.guiVars->menuState - 4, LINE_STATUS_STOP);
				}
				else
				{
					masterClock.longPressCNT->value = 59;
				}
				if (valsChangedOld != masterClock.guiVars->valsChanged)
				{
					WM_Invalidate(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_ENTER));
				}
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				pollButton(ID_BUTTON_LINESETUP_Mplus, WM_NOTIFICATION_RELEASED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].Minutes);

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;

		case ID_BUTTON_LINESETUP_ENTER: // Notifications sent by 'Enter'
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)

				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				//diff = get_sTimeLinesDiff(&lineTemp[masterClock.guiVars->menuState - 4], 10);
				xQueueReset(masterClock.line[masterClock.guiVars->menuState - 4].xSemaphore);
				masterClock.line[masterClock.guiVars->menuState - 4] = lineTemp[masterClock.guiVars->menuState - 4];
				sprintf(str, "%02d", masterClock.line[masterClock.guiVars->menuState - 4].Hours);
				masterClock.guiVars->valsChanged = false;
				saveLineToBKP(masterClock.guiVars->menuState - 4);
				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_LINESETUP_RUNSTOP: // Notifications sent by 'RUN/STOP'
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				switch(masterClock.line[masterClock.guiVars->menuState - 4].Status)
				{
				case LINE_STATUS_RUN:
					lineChangeStatus(masterClock.guiVars->menuState - 4, LINE_STATUS_STOP);
					break;
				case LINE_STATUS_STOP:
					lineChangeStatus(masterClock.guiVars->menuState - 4, LINE_STATUS_RUN);
					break;
				case LINE_STATUS_OFF:
					lineChangeStatus(masterClock.guiVars->menuState - 4, LINE_STATUS_OFF);
					break;
				}
				
				diff = get_sTimeLinesDiff(&lineTemp[masterClock.guiVars->menuState - 4], 10);
				masterClock.line[masterClock.guiVars->menuState - 4] = lineTemp[masterClock.guiVars->menuState - 4];
				if (diff > 0)
				{
					masterClock.line[0].pTemp = (uint32_t*)&lineTemp[0];
					masterClock.line[1].pTemp = (uint32_t*)&lineTemp[1];
					masterClock.line[2].pTemp = (uint32_t*)&lineTemp[2];
					masterClock.line[3].pTemp = (uint32_t*)&lineTemp[3];
					masterClock.line[masterClock.guiVars->menuState - 4].Pulses = 0;
					xQueueReset(masterClock.line[masterClock.guiVars->menuState - 4].xSemaphore);
					while (diff > 0)
					{ 
						for (i = 0; i < LINES_AMOUNT; ++i)
						{
							if ((masterClock.guiVars->menuState - 4) == i && (masterClock.line[i].Status == LINE_STATUS_RUN))
							{
								xSemaphoreGive(masterClock.line[i].xSemaphore);
							}
						}
						diff--;
					}
				}
				else
				{
					sprintf(str, "%02d", masterClock.line[masterClock.guiVars->menuState - 4].Hours);
					HEADER_SetItemText(masterClock.handles->hLineSetupVals, 0, str);
					HEADER_SetTextColor(masterClock.handles->hLineSetupVals, GUI_WHITE);
					masterClock.line[masterClock.guiVars->menuState - 4].Pulses = diff;
				}


				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_LINESETUP_PULSE: // Notifications sent by 'Pulse->'
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				masterClock.guiVars->menuState += 4;
				masterClock.guiVars->valsChanged = false;
				CreateLineSetupPulseWindow();
				WM_DeleteWindow(pMsg->hWin);
				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_LINESETUP_Hminus: // Notifications sent by 'h-'
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)

				if(lineTemp[masterClock.guiVars->menuState - 4].Hours != 0)
				{
					pollButton(ID_BUTTON_LINESETUP_Hminus, WM_NOTIFICATION_CLICKED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].Hours);
					lineChangeStatus(masterClock.guiVars->menuState - 4, LINE_STATUS_STOP);
				}
				if (valsChangedOld != masterClock.guiVars->valsChanged)
				{
					WM_Invalidate(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_ENTER));
				}

				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				pollButton(ID_BUTTON_LINESETUP_Hminus, WM_NOTIFICATION_RELEASED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].Hours);

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_LINESETUP_Mminus: // Notifications sent by 'm-'
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				if(lineTemp[masterClock.guiVars->menuState - 4].Minutes != 0)
				{
					pollButton(ID_BUTTON_LINESETUP_Mminus, WM_NOTIFICATION_CLICKED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].Minutes);
					lineChangeStatus(masterClock.guiVars->menuState - 4, LINE_STATUS_STOP);
				}
				if (valsChangedOld != masterClock.guiVars->valsChanged)
				{
					WM_Invalidate(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_ENTER));
				}
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				pollButton(ID_BUTTON_LINESETUP_Mminus, WM_NOTIFICATION_RELEASED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].Minutes);

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_LINESETUP_Zminus: // Notifications sent by 'm-'
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				if(lineTemp[masterClock.guiVars->menuState - 4].TimeZone != -12)
				{
					pollButton(ID_BUTTON_LINESETUP_Zminus, WM_NOTIFICATION_CLICKED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].TimeZone);
					lineChangeStatus(masterClock.guiVars->menuState - 4, LINE_STATUS_STOP);
				}
				else
				{
					masterClock.longPressCNT->value = -12;
				}
				if (valsChangedOld != masterClock.guiVars->valsChanged)
				{
					WM_Invalidate(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_ENTER));
				}
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				pollButton(ID_BUTTON_LINESETUP_Zminus, WM_NOTIFICATION_RELEASED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].TimeZone);

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_LINESETUP_Zplus: // Notifications sent by 'm-'
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				if(lineTemp[masterClock.guiVars->menuState - 4].TimeZone != 12)
				{
					pollButton(ID_BUTTON_LINESETUP_Zplus, WM_NOTIFICATION_CLICKED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].TimeZone);
					lineChangeStatus(masterClock.guiVars->menuState - 4, LINE_STATUS_STOP);
				}
				else
				{
					masterClock.longPressCNT->value = 12;
				}
				if (valsChangedOld != masterClock.guiVars->valsChanged)
				{
					WM_Invalidate(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LINESETUP_ENTER));
				}
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				pollButton(ID_BUTTON_LINESETUP_Zplus, WM_NOTIFICATION_RELEASED, (int8_t*)&lineTemp[masterClock.guiVars->menuState - 4].TimeZone);

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;

		case ID_BUTTON_LINESETUP_BACK: // Notifications sent by 'Back'
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				masterClock.guiVars->menuState = MENU_STATE_MAIN;
				masterClock.guiVars->valsChanged = false;
				message.MsgId = WM_BACKTOMAINMENU;
				message.Data.v = 0xFF;
				masterClock.guiVars->valsChanged = false;
				masterClock.guiVars->delayedLineSetupMenu = false;
				WM_SendMessage(masterClock.handles->hMainMenu, &message);
				WM_DeleteWindow(masterClock.handles->hLineSetupMenu);
				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		}
		break;
		// USER START (Optionally insert additional message handling)
		// USER END
	default :
		WM_DefaultProc(pMsg);
		break;
	}
	valsChangedOld = masterClock.guiVars->valsChanged;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/

WM_HWIN CreateLineSetupWindow(void) {
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	masterClock.handles->hLineSetupMenu = hWin;
	WM_SetCallback(WM_GetDialogItem(hWin, ID_BUTTON_LINESETUP_Hplus), _cbArrowUpButton);
	WM_SetCallback(WM_GetDialogItem(hWin, ID_BUTTON_LINESETUP_Mplus), _cbArrowUpButton);
	WM_SetCallback(WM_GetDialogItem(hWin, ID_BUTTON_LINESETUP_Zplus), _cbArrowUpButton);
	WM_SetCallback(WM_GetDialogItem(hWin, ID_BUTTON_LINESETUP_Hminus), _cbArrowDownButton);
	WM_SetCallback(WM_GetDialogItem(hWin, ID_BUTTON_LINESETUP_Mminus), _cbArrowDownButton);
	WM_SetCallback(WM_GetDialogItem(hWin, ID_BUTTON_LINESETUP_Zminus), _cbArrowDownButton);
	WM_SetCallback(WM_GetDialogItem(hWin, ID_BUTTON_LINESETUP_ENTER), _cbEnterButton);
	WM_SetCallback(WM_GetDialogItem(hWin, ID_HEADER_LINESETUP_STATVALS), _cbLineSetupSystemTime);
	WM_SetCallback(WM_GetDialogItem(hWin, ID_BUTTON_LINESETUP_PULSE), _cbPulseSetupButton);


	return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/