// Fill out your copyright notice in the Description page of Project Settings.


#include "Sync.h"

Sync::Sync(FCriticalSection* pCrt)
{
	m_pCrt = pCrt;
	m_pCrt->Lock();
}

Sync::~Sync()
{
	m_pCrt->Unlock();
}
