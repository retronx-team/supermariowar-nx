#include "PlayerControlsSelectMenu.h"

#ifdef __SWITCH__
#include "GameValues.h"
#include "ui/MI_SelectField.h"
extern CGameValues game_values;
#endif
#include "ResourceManager.h"

extern CResourceManager* rm;

UI_PlayerControlsSelectMenu::UI_PlayerControlsSelectMenu() : UI_Menu()
{
    miPlayer1ControlsButton = new MI_Button(&rm->spr_selectfield, 120, 140, "Player 1", 400, 1);
    miPlayer1ControlsButton->SetCode(MENU_CODE_TO_PLAYER_1_CONTROLS);

    miPlayer2ControlsButton = new MI_Button(&rm->spr_selectfield, 120, 180, "Player 2", 400, 1);
    miPlayer2ControlsButton->SetCode(MENU_CODE_TO_PLAYER_2_CONTROLS);

    miPlayer3ControlsButton = new MI_Button(&rm->spr_selectfield, 120, 220, "Player 3", 400, 1);
    miPlayer3ControlsButton->SetCode(MENU_CODE_TO_PLAYER_3_CONTROLS);

    miPlayer4ControlsButton = new MI_Button(&rm->spr_selectfield, 120, 260, "Player 4", 400, 1);
    miPlayer4ControlsButton->SetCode(MENU_CODE_TO_PLAYER_4_CONTROLS);

#ifdef __SWITCH__
    miPlayerControlsSingleJoyconModeField = new MI_SelectField(&rm->spr_selectfield, 120, 300, "Split Joycons", 400, 180);
    miPlayerControlsSingleJoyconModeField->Add("Off", 0, "", false, false);
    miPlayerControlsSingleJoyconModeField->Add("On", 1, "", true, false);
    miPlayerControlsSingleJoyconModeField->SetData(NULL, NULL, &game_values.singleJoyconMode);
    miPlayerControlsSingleJoyconModeField->SetKey(game_values.singleJoyconMode ? 1 : 0);
#endif

    miPlayerControlsBackButton = new MI_Button(&rm->spr_selectfield, 544, 432, "Back", 80, 1);
    miPlayerControlsBackButton->SetCode(MENU_CODE_TO_MAIN_MENU);

    miPlayerControlsLeftHeaderBar = new MI_Image(&rm->menu_plain_field, 0, 0, 0, 0, 320, 32, 1, 1, 0);
    miPlayerControlsMenuRightHeaderBar = new MI_Image(&rm->menu_plain_field, 320, 0, 192, 0, 320, 32, 1, 1, 0);
    miPlayerControlsMenuHeaderText = new MI_Text("Player Controls Menu", 320, 5, 0, 2, 1);

    AddControl(miPlayer1ControlsButton, miPlayerControlsBackButton, miPlayer2ControlsButton, NULL, miPlayerControlsBackButton);
    AddControl(miPlayer2ControlsButton, miPlayer1ControlsButton, miPlayer3ControlsButton, NULL, miPlayerControlsBackButton);
    AddControl(miPlayer3ControlsButton, miPlayer2ControlsButton, miPlayer4ControlsButton, NULL, miPlayerControlsBackButton);
#ifdef __SWITCH__
    AddControl(miPlayer4ControlsButton, miPlayer3ControlsButton, miPlayerControlsSingleJoyconModeField, NULL, miPlayerControlsBackButton);
    AddControl(miPlayerControlsSingleJoyconModeField, miPlayer4ControlsButton, miPlayerControlsBackButton, NULL, miPlayerControlsBackButton);
    AddControl(miPlayerControlsBackButton, miPlayerControlsSingleJoyconModeField, miPlayer1ControlsButton, miPlayer1ControlsButton, NULL);
#else
    AddControl(miPlayer4ControlsButton, miPlayer3ControlsButton, miPlayerControlsBackButton, NULL, miPlayerControlsBackButton);
    AddControl(miPlayerControlsBackButton, miPlayer4ControlsButton, miPlayer1ControlsButton, miPlayer1ControlsButton, NULL);
#endif

    AddNonControl(miPlayerControlsLeftHeaderBar);
    AddNonControl(miPlayerControlsMenuRightHeaderBar);
    AddNonControl(miPlayerControlsMenuHeaderText);

    SetHeadControl(miPlayer1ControlsButton);
    SetCancelCode(MENU_CODE_TO_MAIN_MENU);
}

UI_PlayerControlsSelectMenu::~UI_PlayerControlsSelectMenu() {
}