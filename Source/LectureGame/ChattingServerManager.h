// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ManagerBase.h"

class LECTUREGAME_API ChattingServerManager :
	public ManagerBase
{
private:
	ChattingServerManager();
	~ChattingServerManager();

private:
	class PacketSession * m_pChattingSession;

	FRunnableThread* Thread;
	class RecvThread* RunThread;

public:
	class PacketSession* GetSession()	const
	{
		return m_pChattingSession;
	}

public:
	virtual bool Init(const TCHAR* pIP, short sPort, int32 iUserNumber, const TCHAR* pID);
	void Destroy();

private:
	static ChattingServerManager* m_pInst;

public:
	static ChattingServerManager* GetInst()
	{
		if (!m_pInst)
			m_pInst = new ChattingServerManager;

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
