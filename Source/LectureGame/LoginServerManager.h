// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ManagerBase.h"

class LECTUREGAME_API LoginManager	:
	public ManagerBase
{
private:
	LoginManager();
	~LoginManager();

private:
	class PacketSession* m_pLoginSession;

	FRunnableThread* Thread;
	class RecvThread* RunThread;

public:
	class PacketSession* GetSession()	const {
		return m_pLoginSession;
	}

public:
	virtual bool Init();
	void Destroy();

private:
	static LoginManager* m_pInst;

public:
	static LoginManager* GetInst()
	{
		if (!m_pInst)
			m_pInst = new LoginManager;

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
