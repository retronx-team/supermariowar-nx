#ifndef MENU_PLAYERCONTROLSSELECT_H
#define MENU_PLAYERCONTROLSSELECT_H

#include "uimenu.h"
#include "uicontrol.h"

#ifdef __SWITCH__
class MI_SelectField;
#endif

/*
	This menu leads to the control settings of player 1-4.
*/
class UI_PlayerControlsSelectMenu : public UI_Menu
{
public:
	UI_PlayerControlsSelectMenu();
	~UI_PlayerControlsSelectMenu();

private:
	MI_Button * miPlayer1ControlsButton;
	MI_Button * miPlayer2ControlsButton;
	MI_Button * miPlayer3ControlsButton;
	MI_Button * miPlayer4ControlsButton;

#ifdef __SWITCH__
	MI_SelectField * miPlayerControlsSingleJoyconModeField;
#endif

	MI_Button * miPlayerControlsBackButton;

	MI_Image * miPlayerControlsLeftHeaderBar;
	MI_Image * miPlayerControlsMenuRightHeaderBar;
	MI_Text * miPlayerControlsMenuHeaderText;
};

#endif // MENU_PLAYERCONTROLSSELECT_H
