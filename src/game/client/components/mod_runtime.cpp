#include "mod_runtime.h"
#include "game/generated/protocol.h"
#include "game/generated/protocol7.h"
#include "base/log.h"
#include "game/client/gameclient.h"

CModRuntime::CModRuntime()
{
	m_Lua.set_function("SendChat", [this] (int team, std::string message) {GameClient()->m_Chat.SendChat(team, message.c_str());});
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

		for(auto mod : m_LoadedMods)
		{
			try
			{
				m_Lua[mod]["OnMessage"].call(pMsg->m_Team, pMsg->m_pMessage);
			}
			catch(...)
			{
				log_error("mods", "Something went wrong in mod '%s'", mod);
			}
		}
	}
}

void CModRuntime::OnInit()
{	
	if(!Storage()->FolderExists("mods", IStorage::TYPE_SAVE))
	{
		Storage()->CreateFolder("mods", IStorage::TYPE_SAVE);
	}

	Storage()->ListDirectory(IStorage::TYPE_SAVE, "mods/", OnModFileFoundCallback, this);
}

int CModRuntime::OnModFileFoundCallback(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CModRuntime* modRuntime = reinterpret_cast<CModRuntime*>(pUser);
	char modAbsoluteDirPath[1024] = {0};
	
	if(!str_endswith(pFilename, ".lua"))
	{
		return 0;
	}
	
	modRuntime->Storage()->GetCompletePath(IStorage::TYPE_SAVE, "mods/", modAbsoluteDirPath, sizeof(modAbsoluteDirPath));

	modRuntime->LoadMod(pFilename, modAbsoluteDirPath);

	return 0;
}

void CModRuntime::LoadMod(const std::string &ModFileName, const std::string &ModDir)
{
	const std::string modPath = ModDir + ModFileName;
	log_info("mods", "Loading mod: %s", modPath.c_str());

	try
	{
		sol::object mod = m_Lua.script_file(modPath);
		m_Lua[ModFileName] = mod;
	
		m_LoadedMods.push_back(ModFileName);
	}
	catch(...)
	{
		log_error("mods", "An error occured while loading mod '%s'", modPath.c_str());
	}
}