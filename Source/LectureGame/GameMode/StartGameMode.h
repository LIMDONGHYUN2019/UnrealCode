// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "StartGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API AStartGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AStartGameMode();

private:
	TSubclassOf<UUserWidget>	LoginWidgetClass;
	class ULoginWidget* LoginWidget;

public:
	virtual void StartPlay();
	virtual void BeginPlay();
	virtual void InitGameState(); //BeginPlay 보다 호출빠름
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void Tick(float DeltaTime) override;

private:
	void LoginSuccess(uint8* pPacket);
};
