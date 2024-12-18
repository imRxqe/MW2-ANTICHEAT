# MW2 ist voller Nuttensöhne die ungefragt XP lobbys starten upranken oder deranken
https://www.youtube.com/watch?v=UGELwE3k-Lc
* THE COMMANDS LIKE END GAME MUST GET SENDED SOMEWHERE; AND THE MOD MENU CAN PREVENT THEM
* SO I HAVE TO FIND OUT HOW IT WORKS

### MW2/MW3 RCE Exploit
https://www.youtube.com/watch?v=rdhcYNAcYrc
https://www.youtube.com/watch?v=HoS8dhoT01o

https://github.com/skiff/PS3-Toolbox
https://consolecrunch.com/threads/ps3-sdk-4-75-and-prodg-v4-70-and-visual-studio-2010-and-2013-professional.30195/
https://www.youtube.com/watch?v=tROYWX9NPPQ
* ProDG, Targetmanger etc, TMAPI
* Visual Studio 2010

* ForceHost/SuperForceHost
* How do non-host fuckers enable XP hack etc? -> RCE?


1. Code a menu base that runs and does basically nothing
2. Find out how netcheat works
3. Maybe reverse stuff if possible
4. Kernel Plugin to go to XMB if crash in userland
5. Test RTM/Reading values from C# code


Mod Menu - AntiCheat

Blacklist names:
LewisRTM
PAPPY-_-SHOT
sinercal
chuhses -> real name
squatd -> sinercal
Gee-Kay-Sinercal
ArmedResponse_K7

FIND OUT WHO HAS MENU INSTALLED


Show name of host in lobby
Show name of host ingame
How to log what codes are executed? For example endgame
How to override everyones binds to render other mod menus useless?


### RESEARCH
* The RCE Menu by Soul uses the "CL_Netchan_Transmit" ecploit, sane as the already released mw3 rce
* https://nextgenupdate.com/forums/modern-warfare-3-mods-patches-tutorials/1023160-mw3-rce-poc-1.html
* https://pastebin.com/A3KaUyGt

    all RCE patches
    anti-crash/freeze/xmb-kick
    xuid/npid/ip blacklist
    map/gamemode settings
    stats/rename options (self only)