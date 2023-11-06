#include "CommandList.h"

CommandList::CommandList()
{
}

CommandList::~CommandList()
{

}

void CommandList::LobbyCommandPush(CommandList& cmd, void* data)
{
	cmd.buffer.push_back((BYTE*)data);
}

void CommandList::StageCommandPush(CommandList& cmd, void* data)
{
	cmd.buffer.push_back((BYTE*)data);
}

void CommandList::BattleCommandPush(CommandList& cmd, void* data)
{
	cmd.buffer.push_back((BYTE*)data);
}


std::vector<BYTE> CommandList::GetCmdList()
{
	std::vector<BYTE> copyBuffer;
	for (int i = 0; i < buffer.size();++i) {
		copyBuffer.emplace_back(*buffer[i]);
	}
	buffer.clear();

	return copyBuffer;
}
