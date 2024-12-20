#include "functions.h"
#include <cellstatus.h>
#include <sys/prx.h>
#include <string.h>
#include <sys/timer.h>
#include <sys/ppu_thread.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <sysutil/sysutil_oskdialog.h>
#include <sysutil/sysutil_oskdialog_ext.h>
 
SYS_MODULE_INFO( Plugin, 0, 1, 1);
SYS_MODULE_START( Main );
#define Sleep(x)    (sys_timer_usleep(x*1000))
#define ARRAY_SIZE(array) (sizeof((array)) / sizeof((array[0])))
int (*stdc_snprintf)(char* destination, size_t len, const char* format, ...) = (int(*)(char*, size_t, const char*, ...))0x0071C2C0;//open up IDA and look at this address
int (*stdc_printf)(const char*, ...) = (int(*)(const char*, ...))0x0071C2B0;
int (*SV_GameSendServerCommand_AddTestClient)(void) = (int(*)(void))0x002189D8;
void (*SV_GameSendServerCommand_ExecuteClientCommand)(int *cl, const char *s, int clientOK) = (void(*)(int *cl, const char *s, int clientOK))0x00215310;
 
opd_s HDA = { 0x001826B0, TOC };
void(*HudElem_DestroyAll)() = (void(*)())&HDA;
opd_s GMI = { 0x001BE758, TOC };
int(*GetMaterialIndex)(const char* Material) = (int(*)(const char*))&GMI;
opd_s HEA = { 0x001806E0, TOC };
game_hudelem_s*(*HudElemAlloc)(int32_t client, int32_t teamNum) = (game_hudelem_s*(*)(int32_t, int32_t))&HEA;
void(*SV_DropClient)(int client_s, const char *reason, bool tellThem) = (void(*)(int, const char*, bool))0x0070A890;
void(*G_DamageClient)(int *targ, int *inflictor, int *attacker, const float *dir, const float *point, int damage, int dflags, int mod, int weapon, int hitLoc, int timeOffset) = (void(*)(int*, int*, int*, const float*, const float*, int, int, int, int, int, int))0x007061F0;
void(*Player_Die)(int* self, int* inflictor, int* attacker, int damage, int meansOfDeath, int WeaponIndex, const float *vDir, int hitLocation, int psTime) = (void(*)(int*, int*, int*, int, int, int, const float*, int, int))0x00706238;
inline int G_Client(int clientIndex)
{
    return 0x14E2200 + (0x3700 * clientIndex);
}
bool isAlive(int clientIndex)
{
    return *(bool*)(G_Client(clientIndex) + 0x3193) == false;
}
inline int client_s(int clientIndex)
{
    return *(int*)0x1BF911C + (0x97F80 * clientIndex);
}
inline int G_Entity(int clientIndex)
{
    return *(int*)0x1319800 + (0x280 * clientIndex);
}
void KickPlayer(int clientIndex, const char* reason, bool tellThem)
{
    SV_DropClient(client_s(clientIndex), reason, true);
}
void KillPlayer(int Killer, int Victim)
{
    int killer = G_Entity(Killer), victim = G_Entity(Victim);
    Player_Die(&victim, &killer, &killer, 0x186A0, 0xC, 0x47C, (float*)0xD014ABF0, 0x1BF6000, 0x14EFE00);
    //G_DamageClient(&victim, &killer, &killer, (float*)0xD014ABF0, (float*)0xD014ACB4, 0x3C, 2, 2, 0x10A, 0x1BF6000, 0x10009220);
}
bool DetectBtn(int client, const char* btn)
{
    return !strcmp((char*)client_s(client) + 0x20E9F, btn);
}
opd_s SV = { 0x0021A0A0, TOC };
void(*SV_GameSendServerCommand)(int, int, const char*) = (void(*)(int, int, const char*))&SV;
void iPrintlnBold(int client, const char* txt)
{
    char cmd[0x50];
    stdc_snprintf(cmd, sizeof(cmd), "c \"%s\"", txt);
    SV_GameSendServerCommand(client, 0, cmd);
}
void iPrintln(int client, const char* txt)
{
    char cmd[0x50];
    stdc_snprintf(cmd, sizeof(cmd), "f \"%s\"", txt);
    SV_GameSendServerCommand(client, 0, cmd);
}
void setDvar(int client, const char* txt)
{
    char cmd[0x50];
    stdc_snprintf(cmd, sizeof(cmd), "v %s", txt);
    SV_GameSendServerCommand(client, 0, cmd);
}
 
/*void SV_ExecuteClientCommandHook(int cl, const char * s, bool clientOK, bool Unknown)
{
    stdc_printf("SV_ExecuteClientCommand Hooked\n");
    if(!*(bool*)0x01D17A8C)
        return;
    int Client = (cl - *(int*)0x1BF911C) / 0x97F80;
    SV_Cmd_TokenizeString(s);
    ClientCommand(Client);
    SV_Cmd_EndTokenizedString();
    
    if(strstr(s, DPAD_UP)) 
    {
        stdc_printf("DPAD Up Pressed\n");
        iPrintlnBold(Client, "Dpad up pressed!");
    }
}*/
game_hudelem_s* setShader(int clientIndex, const char* Shader, int Width, int Height, float X, float Y, int Allign = 5, unsigned char R = 0, unsigned char G = 0, unsigned char B = 0, unsigned char A = 0)
{
    game_hudelem_s* elem = HudElemAlloc(clientIndex, 0);
    elem->clientNum = clientIndex;
    elem->elem.type = 6;
    elem->elem.materialIndex = GetMaterialIndex(Shader);
    elem->elem.width = Width;
    elem->elem.height = Height;
    elem->elem.x = X;
    elem->elem.y = Y;
    elem->elem.alignOrg = Allign;
    elem->elem.color.r = R;
    elem->elem.color.g = G;
    elem->elem.color.b = B;
    elem->elem.color.a = A;
    return elem;
}
game_hudelem_s* setText(int clientIndex, const char* Text, int Font, float FontScale, float X, float Y, int Allign, unsigned char R = 0, unsigned char G = 0, unsigned char B = 0, unsigned char A = 0,unsigned char glowR = 0, unsigned char glowG = 0, unsigned char glowB = 0, unsigned char glowA = 0)
{
    game_hudelem_s* elem = HudElemAlloc(clientIndex, 0);
    elem->clientNum = clientIndex;
    elem->elem.type = 1;
    elem->elem.text = G_LocalizedStringIndex(Text);
    elem->elem.font = Font;
    elem->elem.fontScale = FontScale;
    elem->elem.x = X;
    elem->elem.y = Y;
    elem->elem.alignOrg = Allign;
    elem->elem.alignScreen = 6;
    elem->elem.color.r = R;
    elem->elem.color.g = G;
    elem->elem.color.b = B;
    elem->elem.color.a = A;
    elem->elem.glowColor.r = glowR;
    elem->elem.glowColor.g = glowG;
    elem->elem.glowColor.b = glowB;
    elem->elem.glowColor.a = glowA;
    return elem;
}
int(*Dvar_FindMallableVar)(const char* DvarName) = (int(*)(const char*))0x0070DF70;
char* Dvar_GetString(const char* Dvar)
{
    return (char*)(*(int*)(Dvar_FindMallableVar(Dvar) + 0xC));
}
int32_t getHost()
{
    for (int i = 0; i < 18; i++)
    {
        if (!strcmp((char*)Dvar_GetString("sv_hostname"), (char*)(G_Client(i) + 0x3208)))
            return i;
    }
    return -1;
}
bool isHost(int clientIndex)
{
    return !strcmp((char*)Dvar_GetString("sv_hostname"), (char*)(G_Client(clientIndex) + 0x3208));
}
typedef enum Verification
{
    None,
    Vip,
    Admin,
    Host
} Verification;
float TextDifference = 18;
const int MaxMenus = 20, MaxOptions = 20;
struct PlayerVars
{
    bool Test;
    bool isAlive;
};
struct ModMenu_t
{
    bool Loaded;
    bool isOpened;
    bool isLocked;
    int CurrentMenu;
    int ScrollPos[MaxMenus];
    const char* Title[MaxMenus];
    const char* Options[MaxMenus][MaxOptions];
    int OptionCount[MaxMenus];
    int ParentID[MaxMenus];
    bool ToggleFunc[MaxMenus][MaxOptions];
    int SubMenuID[MaxMenus][MaxOptions];
    Verification AccessLevel;
    game_hudelem_s* Shader_BG;
    game_hudelem_s* Shader_Scrollbar;
    game_hudelem_s* TextElem[MaxOptions];
    game_hudelem_s* TitleElem;
    PlayerVars Vars;
};
ModMenu_t ModMenu[18];
char* getClientName(int clientIndex)
{
    char* temp = (char*)(G_Client(clientIndex) + 0x3208);
    if (strlen(temp) > 0)
        return temp;
    return "";
}
Verification getAccessLevel(int clientIndex)
{
    return ModMenu[clientIndex].AccessLevel;
}
bool isAllowed(int clientIndex, Verification MinLevel = Vip)
{
    return getAccessLevel(clientIndex) >= MinLevel;
}
const char* getAccessLevelStr(int clientIndex)
{
    if (isAllowed(clientIndex, Host)) return "^1Host^7";
    if (isAllowed(clientIndex, Admin)) return "^1Admin^7";
    if (isAllowed(clientIndex, Vip)) return "^5Vip^7";
    else return "";
}
void CloseMenu(int clientIndex)
{
    FadeOverTime(ModMenu[clientIndex].Shader_BG, 1, 0);
    FadeOverTime(ModMenu[clientIndex].Shader_Scrollbar, 1, 0);
    FadeOverTime(ModMenu[clientIndex].TitleElem, 1, 0);
    for (int i = 0; i < ModMenu[clientIndex].OptionCount[ModMenu[clientIndex].CurrentMenu]; i++) FadeOverTime(ModMenu[clientIndex].TextElem[i], 1, 0);
    SV_GameSendServerCommand(clientIndex, 0, "v r_blur \"0\" hud_enable \"1\" xblive_privatematch \"0\"");
    iPrintln(clientIndex, "Menu Closed");
    ModMenu[clientIndex].isOpened = false;
}
void setAccess(int Owner, int Target, Verification AccessLevel)
{
    if (isHost(Target))
        iPrintlnBold(Owner, "^1You cannot change the Accesslevel of the Host!");
    else
    {
        if (isAllowed(Target))
            CloseMenu(Target);
        ModMenu[Target].AccessLevel = AccessLevel;
        char temp[0x50];
        stdc_snprintf(temp, sizeof(temp), "You have changed %s Accesslevel to: %s", getClientName(Target), getAccessLevelStr(Target));
        iPrintln(Owner, temp);
        stdc_snprintf(temp, sizeof(temp), "Your Accesslevel has been changed to %s by %s", getAccessLevelStr(Target), getClientName(Owner));
    }
}
void AddMenu(int clientIndex, int MenuID, int ParentID)
{
    ModMenu[clientIndex].ParentID[MenuID] = ParentID;
}
void AddFunc(int clientIndex, int MenuID, const char* OptName, bool Toggle = NULL, int SubID = NULL)
{
    ModMenu[clientIndex].Options[MenuID][ModMenu[clientIndex].OptionCount[MenuID]] = OptName;
    ModMenu[clientIndex].ToggleFunc[MenuID][ModMenu[clientIndex].OptionCount[MenuID]] = Toggle == NULL ? NULL : Toggle;
    ModMenu[clientIndex].SubMenuID[MenuID][ModMenu[clientIndex].OptionCount[MenuID]] = SubID;
    if (strlen(OptName) > 0) ModMenu[clientIndex].OptionCount[MenuID]++;
}
const char* Players[18];
void MenuStructure(int clientIndex)
{
    for (int i = 0; i < ARRAY_SIZE(ModMenu[clientIndex].OptionCount) - 2; i++)// Reset Option Count
        ModMenu[clientIndex].OptionCount[i] = 0;
    if (isAllowed(clientIndex, Admin))
    {
        //Main Menu
        AddMenu(clientIndex, 1, NULL);
        AddFunc(clientIndex, 1, "Option 1");
        AddFunc(clientIndex, 1, "Sub Menu", NULL, 2);
        AddFunc(clientIndex, 1, "SnaY ^1<3");
        AddFunc(clientIndex, 1, "seb5594s is da bes");
        AddFunc(clientIndex, 1, "blow ma dick enstone");
        AddFunc(clientIndex, 1, "RTM is gay");
        AddFunc(clientIndex, 1, "RDC is full with Fags");
        AddFunc(clientIndex, 1, "Player Menu", 1, 98);
        AddFunc(clientIndex, 1, "Suicide");
 
        //Sub Menu 1
        AddMenu(clientIndex, 2, 1);
        AddFunc(clientIndex, 2, "Option 1");
        AddFunc(clientIndex, 2, "Option 2");
        AddFunc(clientIndex, 2, "Option 3");
    }
    if (isAllowed(clientIndex, Host))
    {
        //Load Player's
        ModMenu[clientIndex].OptionCount[98] = 0;
        AddMenu(clientIndex, 98, 1);
        for (int i = 0; i < 18; i++)
        {
            Players[i] = getClientName(i);
            if (strcmp(Players[i], ""))
            {
                AddFunc(clientIndex, 98, Players[i], NULL, 99);
            }
        }
        
        //Player Options
        ModMenu[clientIndex].OptionCount[99] = 0;
        AddMenu(clientIndex, 99, 98);
        AddFunc(clientIndex, 99, "Kill");
        AddFunc(clientIndex, 99, "Kick");
        AddFunc(clientIndex, 99, "Remove Access");
        AddFunc(clientIndex, 99, "Give Vip");
        AddFunc(clientIndex, 99, "Give Admin");
    }
}
void ProcessScroll(int clientIndex)
{
    int ID = ModMenu[clientIndex].CurrentMenu;
    if (ModMenu[clientIndex].ScrollPos[ID] > ModMenu[clientIndex].OptionCount[ID] - 1)
        ModMenu[clientIndex].ScrollPos[ID] = 0;
    else if (ModMenu[clientIndex].ScrollPos[ID] < 0)
        ModMenu[clientIndex].ScrollPos[ID] = ModMenu[clientIndex].OptionCount[ID] - 1;
    ModMenu[clientIndex].Shader_Scrollbar->elem.x = 88 + (ModMenu[clientIndex].ScrollPos[ID] * 15.50);
    //MoveOverTime(ModMenu[clientIndex].Shader_Scrollbar, 1, 88 + (CurrentOpt * 20), 300);
}
void LoadMenu(int clientIndex, int MenuID)
{
    ModMenu[clientIndex].CurrentMenu = MenuID;
    stdc_printf("Menu ID: %i\nOptioncount: %i\n", MenuID, ModMenu[clientIndex].OptionCount[ModMenu[clientIndex].CurrentMenu]);
    for (int i = 0; i < ModMenu[clientIndex].OptionCount[MenuID]; i++)
    {
        ChangeText(ModMenu[clientIndex].TextElem[i], ModMenu[clientIndex].Options[ModMenu[clientIndex].CurrentMenu][i]);
        FadeOverTime(ModMenu[clientIndex].TextElem[i], 1, 255);
    }
    for (int i = 0; i < MaxOptions - ModMenu[clientIndex].OptionCount[MenuID] - 1; i++) FadeOverTime(ModMenu[clientIndex].TextElem[i + ModMenu[clientIndex].OptionCount[MenuID]], 0.5, 0);
    ChangeText(ModMenu[clientIndex].TitleElem, ModMenu[clientIndex].Title[ModMenu[clientIndex].CurrentMenu]);
    ProcessScroll(clientIndex);
}
void OpenMenu(int clientIndex)
{
    SV_GameSendServerCommand(clientIndex, 0, "v r_blur \"7\" hud_enable \"0\" cg_crosshairs \"0\" ui_hud_hardcore \"1\"");
    MenuStructure(clientIndex);
    LoadMenu(clientIndex, ModMenu[clientIndex].CurrentMenu);
    ModMenu[clientIndex].Shader_Scrollbar->elem.x = 88 + (ModMenu[clientIndex].ScrollPos[ModMenu[clientIndex].CurrentMenu] * 15.50);
    FadeOverTime(ModMenu[clientIndex].Shader_BG, 1, 175);
    FadeOverTime(ModMenu[clientIndex].TitleElem, 1, 255);
    FadeOverTime(ModMenu[clientIndex].Shader_Scrollbar, 1, 255);
    sys_timer_sleep(1);
    iPrintln(clientIndex, "Menu Opened");
    ModMenu[clientIndex].isOpened = true;
}
int atoi(const char *s)
{
    int sign = 1, num = 0;
    if (*s == '-') sign = -1;
    s++;
    while (*s)
    {
        num = ((*s) - '0') + num * 10;
        s++;
    }
    return num * sign;
}
void ExecuteOption(int clientIndex)
{
    int ID = ModMenu[clientIndex].CurrentMenu, TempSub = ModMenu[clientIndex].SubMenuID[ID][ModMenu[clientIndex].ScrollPos[ID]];
    const char* CurrentOpt = ModMenu[clientIndex].Options[ID][ModMenu[clientIndex].ScrollPos[ID]];
    if (TempSub != NULL)
    {
        if (TempSub == 99)
        {
            char dest[0x50];
            stdc_snprintf(dest, 0x50, "[%s] %s :)", getAccessLevelStr(clientIndex), CurrentOpt);
            ModMenu[clientIndex].Title[ModMenu[clientIndex].SubMenuID[ID][ModMenu[clientIndex].ScrollPos[ID]]] = dest;
        }
        else
            ModMenu[clientIndex].Title[ModMenu[clientIndex].SubMenuID[ID][ModMenu[clientIndex].ScrollPos[ID]]] = CurrentOpt;
        LoadMenu(clientIndex, ModMenu[clientIndex].SubMenuID[ID][ModMenu[clientIndex].ScrollPos[ID]]);
    }
    else
    {
        if (ID == 99)
        {
            int Target = 0;
            for (int i = 0; i < 18; i++)
            {
                if (!strcmp(getClientName(i), CurrentOpt))
                    Target = i;
            }
            char test[0x50];
            stdc_snprintf(test, 0x50, "DEBUG - Client: %s = Client Number: %i", getClientName(Target), Target);
            iPrintlnBold(-1, test);
            if (CurrentOpt == "Kick")
            {
                KickPlayer(Target, "Kicked for beeing a Faggot", true);
                LoadMenu(clientIndex, 98);
            }
            else if (CurrentOpt == "Kill")
            {
                
                LoadMenu(clientIndex, 98);
            }
            else if (CurrentOpt == "Remove Access Level")
            {
                setAccess(clientIndex, Target, None);
                LoadMenu(clientIndex, 98);
            }
            else if (CurrentOpt == "Give Vip")
            {
                setAccess(clientIndex, Target, Vip);
                LoadMenu(clientIndex, 98);
            }
            else if (CurrentOpt == "Give Admin")
            {
                setAccess(clientIndex, Target, Admin);
                LoadMenu(clientIndex, 98);
            }
        }
        else
        {
            char opt[100];
            stdc_snprintf(opt, 100, "Selected Option: %s", CurrentOpt);
            iPrintlnBold(clientIndex, opt);
 
            if (CurrentOpt == "Suicide")
            {
                KillPlayer(clientIndex, clientIndex);
            }
        }
    }
}
void OnPlayerSpawned(int clientIndex)
{
    char dest[0x80];
    stdc_snprintf(dest, 0x80, "Open the Menu with [{+actionslot 1}]");
    iPrintln(clientIndex, dest);
}
void Start(uint64_t arg)
{
    for (;;)
    {
        if (*(bool*)0x01D17A8C)//check if ingame or not
        {
            for (int i = 0; i < 18; i++)
            {
                if (!ModMenu[i].Loaded && *(int*)(G_Client(i) + 0x2B4) > 0)
                {
                    *(int*)0x131689C = 1;//to use precached shaders
                    ModMenu[i].AccessLevel = isHost(i) ? Host : None;
                    if (isAllowed(i))
                    {
                        SV_GameSendServerCommand(i, 0, "v loc_warningsUI \"0\" loc_warnings \"0\"");//make our strings localized
                        sys_timer_sleep(1);
                        ModMenu[i].CurrentMenu = 1;//set first menu Main Menu
                        ModMenu[i].Title[ModMenu[i].CurrentMenu] = "Main Menu";
                        ModMenu[i].Shader_BG = setShader(i, "black", 640, 480, 0, 224, 8);
                        ModMenu[i].Shader_Scrollbar = setShader(i, "menu_button_selection_bar", 400, 19, 88, 300, 8);
                        ModMenu[i].TitleElem = setText(i, "", 7, 2, 120, 220, 8, 255, 255, 255, 0, 0, 100, 255);
                        for (int x = 0; x < MaxOptions; x++)
                            ModMenu[i].TextElem[x] = setText(i, "", 0, 1.3, 200 + (35 * x), 220, 8, 255, 255, 255, 255);
                        ModMenu[i].Loaded = true;
                        OnPlayerSpawned(i);
                    }
                }
                else if (ModMenu[i].Loaded)
                {
                    if (!ModMenu[i].isOpened && !ModMenu[i].isLocked && DetectBtn(i, "+actionslot 1"))
                        OpenMenu(i);
                    else if (ModMenu[i].isOpened && !ModMenu[i].isLocked)
                    {
                        bool Up = DetectBtn(i, "+actionslot 1"), Down = DetectBtn(i, "+actionslot 2");
                        if (Up || Down)
                        {
                            ModMenu[i].ScrollPos[ModMenu[i].CurrentMenu] -= Up;
                            ModMenu[i].ScrollPos[ModMenu[i].CurrentMenu] += Down;
                            ProcessScroll(i);
                        }
                        else if (DetectBtn(i, "+usereload"))
                        {
                            if (ModMenu[i].ParentID[ModMenu[i].CurrentMenu] == NULL)
                                CloseMenu(i);
                            else
                                LoadMenu(i, ModMenu[i].ParentID[ModMenu[i].CurrentMenu]);
                        }
                        else if (DetectBtn(i, "+gostand"))
                            ExecuteOption(i);
                    }
                    if (!isAlive(i))
                    {
                        sys_timer_sleep(2);
                        OnPlayerSpawned(i);
                    }
                }
                Sleep(5);
            }
        }
        else
        {
            for (int i = 0; i < 18; i++)//Reset Menu if not ingame
            {
                if (ModMenu[i].Loaded)
                    ModMenu[i].Loaded = false;
            }
        }
        Sleep(25);
    }
}
 
extern "C" int Main(void)
{
    sys_ppu_thread_t id;
    sys_ppu_thread_create(&id, Start, 0, 2000, 0x80000, SYS_PPU_THREAD_CREATE_JOINABLE, "seb5594's MW2 Plugin");
    return SYS_PRX_RESIDENT;
}