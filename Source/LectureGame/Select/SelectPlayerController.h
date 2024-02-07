// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "SelectPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API ASelectPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASelectPlayerController();

private:
	bool	bLButtonDown;
	bool	bRButtonDown;
	class ASelectCharacter*	pSelectCharacter;

public:
	class ASelectCharacter* GetSelectCharacter()	const
	{
		return pSelectCharacter;
	}

private:
	virtual void BeginPlay();
	virtual void SetupInputComponent();
	virtual void PlayerTick(float DeltaTime);

public:
	void LButtonClick();
	void LButtonRelease();
	void RButtonClick();
	void RButtonRelease();

	void Clear();

private:
	void Picking();
};
