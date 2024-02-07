// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ManagerBase.h"

class LECTUREGAME_API GameServerManager :
	public ManagerBase
{
private:
	GameServerManager();
	~GameServerManager();

private:
	class PacketSession* m_pGameSession;

	FRunnableThread* Thread;
	class RecvThread* RunThread;

public:
	class PacketSession* GetSession()	const
	{
		return m_pGameSession;
	}

public:
	virtual bool Init(const TCHAR* pIP, short sPort, int32 iUserNumber, const TCHAR* pID);
	void Destroy();

private:
	static GameServerManager* m_pInst;

public:
	static GameServerManager* GetInst()
	{
		if (!m_pInst)
			m_pInst = new GameServerManager;

		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst)
		{
			delete m_pInst;
			m_pInst = nullptr;
		}
	}
};
