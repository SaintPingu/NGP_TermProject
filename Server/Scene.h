#pragma once
#include "stdafx.h"

class Scene abstract {
public:
	virtual void Init() abstract;
	virtual void ProcessCommand(int clientID, Command command, void* data) abstract;
};