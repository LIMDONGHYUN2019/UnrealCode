// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "RecvQueue.h"

class LECTUREGAME_API NetworkManager
{
private:
	NetworkManager();
	~NetworkManager();

private:
	int32	m_iUserNumber;
	int32	m_iCharacterNumber;
	TCHAR	m_strID[256];
	int32	m_iIDLength;
	char	m_strName[32];

public:
	void SetUserNumber(int32 iNumber)
	{
		m_iUserNumber = iNumber;
	}

	int32 GetUserNumber()	const
	{
		return m_iUserNumber;
	}
	void SetCharacterNumber(int32 iNumber)
	{
		m_iCharacterNumber = iNumber;
	}

	int32 GetCharacterNumber()	const
	{
		return m_iCharacterNumber;
	}

	void SetID(const TCHAR* pID, int32 iLength)     
	{
		FMemory::Memcpy(m_strID, pID, sizeof(TCHAR) * iLength);
		m_iIDLength = iLength;
		//memcpy(m_strID, pID, sizeof(TCHAR) * iLength);
	}

	void SetName(const char* pName)
	{
		FMemory::Memcpy(m_strName, pName, 32);
	}

	const char* GetName() const
	{
		return m_strName;
	}

	const TCHAR* GetID()	const
	{
		return m_strID;
	}

public:
	bool Init();
	void LoginDisconnect();

private:
	static NetworkManager* m_pInst;

public:
	static NetworkManager* GetInst()
	{       
		if (!m_pInst)
			m_pInst = new NetworkManager;

		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst)
		{
			delete	m_pInst;
			m_pInst = nullptr;
		}
	}
};
