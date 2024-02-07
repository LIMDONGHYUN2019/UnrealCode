// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Select/SelectCharacter.h"
#include "../Select/SelectCameraManager.h"
#include "../Select/SelectWidget.h"
#include "../Select/CreateCharacterWidget.h"
#include "GameFramework/GameModeBase.h"
#include "SelectGameMode.generated.h"

UCLASS()
class LECTUREGAME_API ASelectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASelectGameMode();

private:

	TSubclassOf<UUserWidget>	SelectWidgetClass;
	USelectWidget* SelectWidget;

	TSubclassOf<UUserWidget>	CreateChartWidgetClass;
	UCreateCharacterWidget* CreateCharWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ASelectCameraManager*	CameraManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*>		CharacterSpawnPoint;

	TArray<_tagCharacterInfo*>	CharInfoArray; // 선택창 캐릭터 종류별 스탯
	TArray<ASelectCharacter*>	CharacterArray; // 생성단계 기본 캐릭터 정보

	TArray<ASelectCharacter*>	UseCharacterArray; // 현재 생성된 캐릭터 정보

	TSubclassOf<ASelectCharacter>	KnightCharClass;
	TSubclassOf<ASelectCharacter>	ArcherCharClass;
	TSubclassOf<ASelectCharacter>	MagicionCharClass;

public:
	void SetCameraManager(ASelectCameraManager* pManager)
	{
		CameraManager = pManager;
	}

	ASelectCameraManager* GetCameraManager()	const
	{
		return CameraManager;
	}

	void AddSelectCharacter(class ASelectCharacter* pChar)
	{
		CharacterArray.Add(pChar);
	}

	void UseAddSelectCharacter(class ASelectCharacter* pChar)
	{
		UseCharacterArray.Add(pChar);
	}

	UCreateCharacterWidget* GetCreateCharacterWidget()	const
	{
		return CreateCharWidget;
	}

public:
	virtual void StartPlay();
	virtual void BeginPlay();
	virtual void InitGameState(); //BeginPlay 보다 호출빠름
	virtual void Tick(float DeltaTime) override;

private:
	void Connect(uint8* pPacket, int32 iLength);
	void DisConnect(uint8* pPacket, int32 iLength);
	void ConnectFail(uint8* pPacket, int32 iLength);
	void CharacterCreate(uint8* pPacket, int32 iLength);
	void SelectCharacterInfo(uint8* pPacket, int32 iLength);
	void CreateCharacterInfo(uint8* pPacket, int32 iLength);
	void DeleteCharacterInfo(uint8* pPacket, int32 iLength);
	void GameStart(uint8* pPacket, int32 iLength);

public:
	void ChangeUMG(bool bCreate);
};
