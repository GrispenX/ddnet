#ifndef GAME_CLIENT_MOD_RUNTIME_H
#define GAME_CLIENT_MOD_RUNTIME_H

#include <game/client/component.h>
#include <sol/sol.hpp>
#include <vector>
#include <string>

class CModRuntime : public CComponent
{
public:
    CModRuntime();
    ~CModRuntime() override;
    
    int Sizeof() const override;
    
    void OnMessage(int Msg, void *pRawMsg) override;
    
    void OnInit() override;

private:
    static int OnModFileFoundCallback(const char *pFilename, int IsDir, int DirType, void *pUser);
    void LoadMod(const std::string &ModFileName, const std::string &ModDir);

    sol::state m_Lua;
    std::vector<std::string> m_LoadedMods;
};

#endif