// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../Player/PlayerCharacter.h"
#include "../OtherUser/UserCharacter.h"
#include "../Monster/Monster.h"
#include "GameFramework/GameModeBase.h"
#include "BossRoomGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API ABossRoomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABossRoomGameMode();

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

	class AUserCharacter* m_OtherUserCharacter;
	TMap<int32, class AUserCharacter*> m_OtherUserList;

	TArray<class AMonster*> m_MonsterList; // 소환수을 넣을까 말까

public:
	class UGameWidget* GetGameWidget()
	{
		return pGameWidget;
	}

public:
	virtual void StartPlay();
	virtual void BeginPlay();
	virtual void InitGameState(); //BeginPlay 보다 호출빠름
	virtual void Tick(float DeltaTime) override;


public:
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

	void SkillList(uint8* pPacket, int32 iLength);
	void SkillUp(uint8* pPacket, int32 iLength);

	void InventoryList(uint8* pPacket, int32 iLength);
	void InventoryAdd(uint8* pPacket, int32 iLength);
	void InventoryDelete(uint8* pPacket, int32 iLength);

	void QuestList(uint8* pPacket, int32 iLength);
	void QuestComplete(uint8* pPacket, int32 iLength);

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
