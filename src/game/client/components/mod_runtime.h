#ifndef GAME_CLIENT_MOD_RUNTIME_H
#define GAME_CLIENT_MOD_RUNTIME_H

#include <game/client/component.h>

class CModRuntime : public CComponent
{
private:
    /* data */
public:
    CModRuntime(/* args */);
    ~CModRuntime() override;

    int Sizeof() const override;

    void OnMessage(int Msg, void *pRawMsg) override;
};

#endif