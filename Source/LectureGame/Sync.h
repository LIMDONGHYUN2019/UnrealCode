// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"

class LECTUREGAME_API Sync
{
public:
	Sync(FCriticalSection* pCrt);
	~Sync();

private:
	FCriticalSection* m_pCrt;
};
