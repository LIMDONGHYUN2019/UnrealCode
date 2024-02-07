// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerBase.h"

// 왜 lnit에다 크컬을 넣었을때는 크래쉬가 나고 생성자에다 넣어서 크래쉬안나게 됬다....
// 순서상에 문제로 Tick이 먼저 실행됬다라는것이 일단 추측이다.
ManagerBase::ManagerBase()
{
	m_RecvQueue.SetCriticalSection(&m_Crt);
}

ManagerBase::~ManagerBase()
{
}

bool ManagerBase::Init()
{	
	return true;
}

bool ManagerBase::Init(const TCHAR* pIP, short sPort, int32 iUserNumber, const TCHAR* pID)
{
	return true;
}

