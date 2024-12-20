### Check out next
* xuid? NPID? https://nextgenupdate.com/forums/call-duty-series/936613-ps3xuid-generator.html
* Anti CFG SPRX by SC58 https://nextgenupdate.com/forums/modern-warfare-2-mods-patches-tutorials/882273-114sprxiw4server.html
* good resource for offsets etc https://nextgenupdate.com/forums/members/845060-sc58.html


### TOC pointer (table of contents)


How is RPC enabled?

It somehow uses PS3.SetMemory to modify memory in a way that spawns a function that can then be called from RTM menu



Quote from chco ngu
https://nextgenupdate.com/forums/modern-warfare-2-mods-patches-tutorials/734045-114-rpc-fixed-1.html

It's pretty clear that you don't really understand why this works or doesn't work. For MW2, you can't just copy and paste the code from Ghosts/BO2/MW3/whatever. MW2 uses the TOC pointer in most functions, so it will cause lots of bugs and freezes if you don't properly store that pointer somewhere before calling a branch. 

A custom function is placed into the code