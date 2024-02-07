// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainPlayerController();

private:
	bool	bLButtonDown;
	bool	bRButtonDown;
	bool	bControl;

	ELevel	Level;

private:
	virtual void BeginPlay();
	virtual void SetupInputComponent();
	virtual void PlayerTick(float DeltaTime);

public:
	void LButtonClick();
	void LButtonRelease();
	void RButtonClick();
	void RButtonRelease();

private:
	void Inventory();
	void PlayerStatus();
	void Esc();
	void Picking();
	void CharacterInfo();
	void PlayerTurn(float Scale);
	void PlayerLookUp(float Scale);
	void Control();
	void Group();
	void Skill();
	void Quest();
	void Information();
};
