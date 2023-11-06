#pragma once
#include "TermProject_Final/stdafx.h"

class CommandList
{
public:
	CommandList();
	~CommandList();

	std::vector<BYTE*> buffer;

	void LobbyCommandPush(CommandList& cmd, void* data);
	void StageCommandPush(CommandList& cmd, void* data);
	void BattleCommandPush(CommandList& cmd, void* data);

	std::vector<BYTE> GetCmdList();
};

