// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerBase.h"

// �� lnit���� ũ���� �־������� ũ������ ���� �����ڿ��� �־ ũ�����ȳ��� ���....
// ������ ������ Tick�� ���� ������ٶ�°��� �ϴ� �����̴�.
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

