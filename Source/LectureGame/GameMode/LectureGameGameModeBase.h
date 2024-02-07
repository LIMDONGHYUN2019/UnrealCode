// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../GameInfo.h"
#include "../Player/PlayerCharacter.h"
#include "../OtherUser/UserCharacter.h"
#include "../Monster/Monster.h"
#include "GameFramework/GameModeBase.h"
#include "LectureGameGameModeBase.generated.h"


UCLASS()
class LECTUREGAME_API ALectureGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALectureGameGameModeBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APlayerCharacter>	GreystoneClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APlayerCharacter>	SerathClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APlayerCharacter>	TwinblastClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector		PlayerStartLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FRotator	PlayerStartRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UClass*>	JobClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<AMonster>>	MonsterClassArray;

	TSubclassOf<UUserWidget>	GameWidgetClass;
	class UGameWidget* pGameWidget;

	TDoubleLinkedList<int32>	m_UserCharacterNumberList;
	class AUserCharacter* m_OtherUserCharacter;

	TMap<int32, class AUserCharacter*> m_OtherUserList;

	TArray<class AMonster*> m_MonsterList;

public:
	class AUserCharacter* FindUser(int32 CharacterNumber)
	{
		class AUserCharacter** ppChar = m_OtherUserList.Find(CharacterNumber);

		if (ppChar)
			return *ppChar;

		return nullptr;
	}
	class UGameWidget* GetGameWidget()
	{
		return pGameWidget;
	}
	
public:
	virtual void StartPlay();
	virtual void BeginPlay();
	virtual void InitGameState(); //BeginPlay 보다 호출빠름
	virtual void Tick(float DeltaTime) override;

	/*
		ASelectGameMode* pGameMode = Cast<ASelectGameMode>(GetWorld()->GetAuthGameMode());
		ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
		UGameWidget* pGameWidget = pGameMode->GetGameWidget();
	*/

private:
	void GameIn(uint8* pPacket, int32 iLength);
	void UserAppear(uint8* pPacket, int32 iLength);
	void UserDisAppear(uint8* pPacket, int32 iLength);
	void OtherUserDisAppear(uint8* pPacket, int32 iLength);
	void MoveSync(uint8* pPacket, int32 iLength);
	void OtherUserAppear(uint8* pPacket, int32 iLength);
	void MoveStart(uint8* pPacket, int32 iLength);
	void MoveDir(uint8* pPacket, int32 iLength);
	void MoveEnd(uint8* pPacket, int32 iLength);
	void RotationPacket(uint8* pPacket, int32 iLength);
	void OtherStartMove(uint8* pPacket, int32 iLength);
	void OtherMove(uint8* pPacket, int32 iLength);
	void OtherEndMove(uint8* pPacket, int32 iLength);
	void OtherAttack(uint8* pPacket, int32 iLength);
	void ChattingMessage(uint8* pPacket, int32 iLength);
	void LevelStart(uint8* pPacket, int32 iLength);
	void MonsterSpawn(uint8* pPacket, int32 iLength);
	void MonsterAI(uint8* pPacket, int32 iLength);

	void CharacterLevelup(uint8* pPacket, int32 iLength);

	void SkillList(uint8* pPacket, int32 iLength);
	void SkillUp(uint8* pPacket, int32 iLength);

	void StoreList(uint8* pPacket, int32 iLength);
	void InventoryList(uint8* pPacket, int32 iLength);
	void InventoryAdd(uint8* pPacket, int32 iLength);
	void InventoryDelete(uint8* pPacket, int32 iLength);
	void InventoryGold(uint8* pPacket, int32 iLength);

	void QuestList(uint8* pPacket, int32 iLength);
	void QuestComplete(uint8* pPacket, int32 iLength);

	void TradeRequest(uint8* pPacket, int32 iLength);
	void TradeRequestAccept(uint8* pPacket, int32 iLength);
	void TradeCancel(uint8* pPacket, int32 iLength);
	void TradeAdd(uint8* pPacket, int32 iLength);
	void TradeDelete(uint8* pPacket, int32 iLength);
	void TradeReady(uint8* pPacket, int32 iLength);
	void TradeSuccess(uint8* pPacket, int32 iLength);
	
	void GroupCreate(uint8* pPacket, int32 iLength);
	void GroupRequest(uint8* pPacket, int32 iLength);
	void GroupAccept(uint8* pPacket, int32 iLength);
	void GroupCancel(uint8* pPacket, int32 iLength);
	void GroupLeave(uint8* pPacket, int32 iLength);
	void GroupKick(uint8* pPacket, int32 iLength);
	void GroupSuccess(uint8* pPacket, int32 iLength);

	void PlayerAttackDamage(uint8* pPacket, int32 iLength);
	void MonsterDeath(uint8* pPacket, int32 iLength);
	void MonsterItemDrop(uint8* pPacket, int32 iLength);
	void MonsterAttackDamage(uint8* pPacket, int32 iLength);
	void MainPortal(uint8* pPacket, int32 iLength);
	void SevarogBossRoomCreate(uint8* pPacket, int32 iLength);
	void AlivePacket();

private:
	void LoadMonsterClass();
};
