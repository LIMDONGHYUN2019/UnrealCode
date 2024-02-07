// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"

class LECTUREGAME_API RecvThread	:
	public FRunnable
{ //runnable 클래스, fRunnable 변수
public:
	RecvThread();
	~RecvThread();

public:
	bool bLoop; 
	//FSocket* m_pMainGameSocket;
	class PacketSession* m_pSession;
	class ManagerBase* m_pManager;

public:
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();
};
