// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ManagerBase.h"



class LECTUREGAME_API MoveServerManager :
	public ManagerBase
{
private:
	MoveServerManager();
	~MoveServerManager();

private:
	class PacketSession* m_pMoveSession;

	FRunnableThread* Thread;
	class RecvThread* RunThread;

public:
	class PacketSession* GetSession()	const {
		return m_pMoveSession;
	}

public:
	virtual bool Init(const TCHAR* pIP, short sPort, int32 iUserNumber, const TCHAR* pID);
	void Destroy();

private:
	static MoveServerManager* m_pInst;

public:
	static MoveServerManager* GetInst()
	{
		if (!m_pInst)
			m_pInst = new MoveServerManager;

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
