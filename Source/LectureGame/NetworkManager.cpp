// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkManager.h"
#include "LoginServerManager.h"
#include "GameServerManager.h"
#include "MoveServerManager.h"
#include "ChattingServerManager.h"

NetworkManager* NetworkManager::m_pInst = nullptr;

NetworkManager::NetworkManager()
{
	m_iUserNumber = -1;
	memset(m_strID, 0, sizeof(TCHAR) * 256);
}

NetworkManager::~NetworkManager()
{
	GameServerManager::DestroyInst();
	LoginManager::DestroyInst();

	ChattingServerManager::DestroyInst();
}

bool NetworkManager::Init()
{
	return true;
}

void NetworkManager::LoginDisconnect()
{
	LoginManager::GetInst()->Destroy();
}
