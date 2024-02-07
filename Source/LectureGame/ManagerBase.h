// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "RecvQueue.h"

class LECTUREGAME_API ManagerBase
{
public:
	ManagerBase();
	virtual ~ManagerBase();

private:
	RecvQueue		m_RecvQueue;
	FCriticalSection	m_Crt;

public:
	RecvQueue* GetRecvQueue()
	{
		return &m_RecvQueue;
	}

public:
	virtual bool Init();
	virtual bool Init(const TCHAR* pIP, short sPort, int32 iUserNumber, const TCHAR* pID);
};
