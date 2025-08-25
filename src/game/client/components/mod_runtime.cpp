#include "mod_runtime.h"
#include "game/generated/protocol.h"
#include "game/generated/protocol7.h"
#include "base/log.h"
#include "game/client/gameclient.h"

CModRuntime::CModRuntime()
{

}

CModRuntime::~CModRuntime()
{

}

int CModRuntime::Sizeof() const
{
	return sizeof(CModRuntime);
}

void CModRuntime::OnMessage(int Msg, void *pRawMsg)
{
	if(Msg == protocol7::NETMSGTYPE_SV_CHAT)
	{
		CNetMsg_Sv_Chat *pMsg = (CNetMsg_Sv_Chat *)pRawMsg;
		if(str_comp(pMsg->m_pMessage, "q") == 0)
		{
			GameClient()->m_Chat.SendChat(pMsg->m_Team, pMsg->m_pMessage);		
		}
	}
}