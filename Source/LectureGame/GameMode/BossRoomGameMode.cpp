// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRoomGameMode.h"
#include "../MainGameInstance.h"
//#include "../MainPlayerController.h"
#include "../BossRoomController.h"
#include "../NetworkManager.h"
#include "../RecvQueue.h"
#include "../PacketSession.h"
#include "../NetStream.h"
#include "../GameServerManager.h"
#include "../ChattingServerManager.h"
#include "../Chatting/ChattingWidget.h"
#include "../Player/Greyston.h"
#include "../Player/Serath.h"
#include "../Player/Twinblast.h"

#include "../Monster/Sevarog.h"

#include "../UI/GameWidget.h"
#include "../UI/Skill/SkillWidget.h"
#include "../UI/Inventory/InventoryWidget.h"
#include "../UI/PlayerUI/PlayerStatusWidget.h"
#include "../UI/PlayerUI/PlayerInfoWidget.h"
#include "../UI/Group/GroupRequestWidget.h"
#include "../UI/Group/GroupWidget.h"
#include "../UI/Quest/QuestWidget.h"
#include "../Etc/EnvironmentActor.h"


ABossRoomGameMode::ABossRoomGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget>	GameWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/UI_Game.UI_Game_C'"));

	if (GameWidgetAsset.Succeeded())
		GameWidgetClass = GameWidgetAsset.Class;

	PlayerControllerClass = ABossRoomController::StaticClass();

	//LoadMonsterClass();
	// 블루프린트 게임모드에서 추가를 해둠...
}

void ABossRoomGameMode::StartPlay()
{
	Super::StartPlay();
}

void ABossRoomGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GameWidgetClass)
	{
		pGameWidget = Cast<UGameWidget>(CreateWidget(GetWorld(), GameWidgetClass));
		if (pGameWidget)
			pGameWidget->AddToViewport();
	}

	UMainGameInstance* GameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	if (GameInst->GetCharacterSet())
	{
		_tagCharacterInfo	CharacterInfo = GameInst->GetCharacterInfo();

		// 캐릭터 생성
		FString	strJob(CharacterInfo.strJob);
		LOG(TEXT("Character Job : %s"), *strJob);
		FString strName(CharacterInfo.strName);
		LOG(TEXT("Character Name : %s"), *strName);

		// 플레이어를 생성한다.
		if (strJob == TEXT("Knight"))
		{
			FActorSpawnParameters	param;

			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			AGreystone* pGreystone = GetWorld()->SpawnActor<AGreystone>(GreystoneClass, PlayerStartLoc, PlayerStartRot,
				param);

			APlayerController* pController = GetWorld()->GetFirstPlayerController();

			pController->Possess(pGreystone);
		}

		else if (strJob == TEXT("Archer"))
		{
			FActorSpawnParameters	param;

			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			ASerath* pSerath = GetWorld()->SpawnActor<ASerath>(SerathClass, PlayerStartLoc, PlayerStartRot,
				param);

			APlayerController* pController = GetWorld()->GetFirstPlayerController();

			pController->Possess(pSerath);
		}

		else if (strJob == TEXT("Magician"))
		{
			FActorSpawnParameters	param;

			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			ATwinblast* pTwinblast = GetWorld()->SpawnActor<ATwinblast>(TwinblastClass, PlayerStartLoc, PlayerStartRot,
				param);

			APlayerController* pController = GetWorld()->GetFirstPlayerController();

			pController->Possess(pTwinblast);
		}

		APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(pPlayer))
		{
			pPlayer->SetCharacterInfo(&CharacterInfo);
			pPlayer->SetHP();
		}

		UPlayerStatusWidget* pStatusWidget = pGameWidget->GetPlayerStatusWidget();
		UInventoryWidget* pInventoryWidget = pGameWidget->GetInventoryWidget();
		if (pStatusWidget)
		{
			pStatusWidget->SetStatus(&CharacterInfo);
		}
		if (pInventoryWidget)
		{
			pInventoryWidget->SetGold(CharacterInfo.iMoney);
		}

		pGameWidget->SetExpPercent(CharacterInfo.iExp / (float)CharacterInfo.iExpMax);
		GameInst->ClearCharacterSet();
	}
}

void ABossRoomGameMode::InitGameState()
{
	Super::InitGameState();
}

void ABossRoomGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RecvQueue* pQueue = GameServerManager::GetInst()->GetRecvQueue();

	int32	iProtocol = 0, iLength = 0;
	uint8	packet[PACKET_LENGTH] = {};

	if (pQueue->pop(iProtocol, iLength, packet))
	{
		int32 iTempo = pQueue->GetCount();
		if (iTempo > 100)
			PrintViewport(1.f, FColor::Blue, FString::Printf(TEXT("Client Packet Count, Protocol : %d // %d"), pQueue->GetCount(), iProtocol));
		switch (iProtocol)
		{
			//=============================================
		case GP_GAMEIN:
			GameIn(packet, iLength);
			break;
		case GP_LEVELSTART: //몬스터 생성
			LevelStart(packet, iLength);
			break;
			//=============================================
		case GP_USERAPPEAR:	// 다른 유저가 접속했을떄 내 화면에 다른 유저를 띄우는 함수
			UserAppear(packet, iLength);
			break;
		case GP_USERDISAPPEAR:
			UserDisAppear(packet, iLength);
			break;
		case GP_OTHERUSERAPPEAR: // 다른유저 화면에 내 캐릭터를 띄우는 함수
			OtherUserAppear(packet, iLength);
			break;
		case GP_OTHERUSERDISAPPEAR:
			OtherUserDisAppear(packet, iLength);
			break;
		case GP_MOVESTART:
			MoveStart(packet, iLength);
			break;
		case GP_MOVE:	// 이동 방향이 바뀌었을때
			MoveDir(packet, iLength);
			break;
		case GP_MOVEEND:
			MoveEnd(packet, iLength);
			break;
		case GP_ROTATION:
			RotationPacket(packet, iLength);
			break;
		case GP_OTHERMOVESTART:
			OtherStartMove(packet, iLength);
			break;
		case GP_OTHERMOVE:
			OtherMove(packet, iLength);
			break;
		case GP_OTHERMOVEEND:
			OtherEndMove(packet, iLength);
			break;
		case GP_SYNC:
			MoveSync(packet, iLength);
			break;
		case GP_ATTACK:
			OtherAttack(packet, iLength);
			break;
			//=============================================
		case GP_PLAYERATTACKDAMAGE:
			PlayerAttackDamage(packet, iLength);
			break;
		case GP_MONSTERCREATE:
			MonsterSpawn(packet, iLength);
			break;
		case GP_MONSTERAI:
			MonsterAI(packet, iLength);
			break;
		case GP_MONSTERDEATH:
			MonsterDeath(packet, iLength);
			break;
		case GP_MONSTERITEMDROP:
			MonsterItemDrop(packet, iLength);
			break;
			//=============================================
		case GP_SkillList:
			SkillList(packet, iLength);
			break;
		case GP_SkillUp:
			SkillUp(packet, iLength);
			break;
			//=============================================
		case GP_INVENTORYLIST:
			InventoryList(packet, iLength);
			break;
		case GP_INVENTORYADD:
			InventoryAdd(packet, iLength);
			break;
		case GP_INVENTORYDELETE:
			InventoryDelete(packet, iLength);
			break;
		case GP_QUESTLIST:
			QuestList(packet, iLength);
			break;
		case GP_QUESTCOMPLETE:
			QuestComplete(packet, iLength);
			break;
			//=============================================
			//=============================================
		case GP_GROUPCREATE:
			GroupCreate(packet, iLength);
			break;
		case GP_GROUPREQUEST:
			GroupRequest(packet, iLength);
			break;
		case GP_GROUPACCEPT:
			GroupAccept(packet, iLength);
			break;
		case GP_GROUPCANCEL:
			GroupCancel(packet, iLength);
			break;
		case GP_GROUPLEAVE:
			GroupLeave(packet, iLength);
			break;
		case GP_GROUPKICK:
			GroupKick(packet, iLength);
			break;
		case GP_GROUPSUCCESS:
			GroupSuccess(packet, iLength);
			break;
			//=============================================
		case GP_MAINPORTAL:
			MainPortal(packet, iLength);
			break;
		case GP_SEVAROGBOSSROOM:
			SevarogBossRoomCreate(packet, iLength);
			break;
		case GP_ALIVE:
			AlivePacket();
			break;
		}
	}

	RecvQueue* pChattingQueue = ChattingServerManager::GetInst()->GetRecvQueue();
	iProtocol = 0, iLength = 0;
	if (pChattingQueue->pop(iProtocol, iLength, packet))
	{
		switch (iProtocol)
		{
		case CP_LOGIN:
			break;
		case CP_LOGOUT:
			break;
		case CP_MESSAGE:
			ChattingMessage(packet, iLength);
			break;
		}
	}
}

void ABossRoomGameMode::GameIn(uint8* pPacket, int32 iLength)
{
	PrintViewport(1.f, FColor::Blue, TEXT("Game In!!!!!!!!!!"));

	NetStream stream;
	stream.SetBuffer(pPacket);

	int32 iCharacterNumber = -1;
	_tagCharacterInfo CharacterInfo = {};

	stream.Read(&iCharacterNumber, sizeof(int32));
	NetworkManager::GetInst()->SetCharacterNumber(iCharacterNumber);

	EJob	iJob = EJob::End;
	stream.Read(&iJob, sizeof(EJob));

	stream.Read(&CharacterInfo, sizeof(_tagCharacterInfo));

	FString	strJob(CharacterInfo.strJob);
	FString strName(CharacterInfo.strName);

	// 플레이어를 생성한다.
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	switch (iJob)
	{
	case EJob::GreyStone:
	{
		AGreystone* pGreystone = GetWorld()->SpawnActor<AGreystone>(GreystoneClass, PlayerStartLoc, PlayerStartRot,
			param);

		APlayerController* pController = GetWorld()->GetFirstPlayerController();
		pController->Possess(pGreystone);
	}
	break;
	case EJob::TwinBlast:
	{
		ATwinblast* pTwinblast = GetWorld()->SpawnActor<ATwinblast>(TwinblastClass, PlayerStartLoc, PlayerStartRot,
			param);

		APlayerController* pController = GetWorld()->GetFirstPlayerController();
		pController->Possess(pTwinblast);
	}
		break;
	case EJob::Serath:
	{
		ASerath* pSerath = GetWorld()->SpawnActor<ASerath>(SerathClass, PlayerStartLoc, PlayerStartRot,
			param);

		APlayerController* pController = GetWorld()->GetFirstPlayerController();
		pController->Possess(pSerath);
	}
		break;
	case EJob::Modekaiser:
		break;
	case EJob::End:
		break;
	}

	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UPlayerStatusWidget* pStatusWidget = pGameWidget->GetPlayerStatusWidget();
	UInventoryWidget* pInventoryWidget = pGameWidget->GetInventoryWidget();

	if (IsValid(pPlayer))
	{
		pPlayer->SetCharacterInfo(&CharacterInfo);
		pPlayer->SetHP();
		pPlayer->SetMP();
		//pPlayer->SetActorRelativeScale3D(vScale);
	}
	if (pStatusWidget)
	{
		pStatusWidget->SetStatus(&CharacterInfo);
	}
	if (pInventoryWidget)
	{
		pInventoryWidget->SetGold(CharacterInfo.iMoney);
	}
	pGameWidget->SetExpPercent(CharacterInfo.iExp / (float)CharacterInfo.iExpMax);

}

void ABossRoomGameMode::UserAppear(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::UserDisAppear(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::OtherUserDisAppear(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::MoveSync(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::OtherUserAppear(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::MoveStart(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::MoveDir(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::MoveEnd(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::RotationPacket(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::OtherStartMove(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::OtherMove(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::OtherEndMove(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::OtherAttack(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::ChattingMessage(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::LevelStart(uint8* pPacket, int32 iLength)
{

}

void ABossRoomGameMode::MonsterSpawn(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::MonsterAI(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::SkillList(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::SkillUp(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::InventoryList(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::InventoryAdd(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::InventoryDelete(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::QuestList(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::QuestComplete(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::GroupCreate(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::GroupRequest(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::GroupAccept(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::GroupCancel(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::GroupLeave(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::GroupKick(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::GroupSuccess(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::PlayerAttackDamage(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::MonsterDeath(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::MonsterItemDrop(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::MonsterAttackDamage(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::MainPortal(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::SevarogBossRoomCreate(uint8* pPacket, int32 iLength)
{
}

void ABossRoomGameMode::AlivePacket()
{
}

void ABossRoomGameMode::LoadMonsterClass()
{
	static ConstructorHelpers::FClassFinder<AMonster>	MinionAsset(TEXT("Blueprint'/Game/Monster/BPMinion.BPMinion_C'"));

	if (MinionAsset.Succeeded())
		MonsterClassArray.Add(MinionAsset.Class);

	static ConstructorHelpers::FClassFinder<AMonster>	BossAsset(TEXT("Blueprint'/Game/Monster/BPBoss.BPBoss_C'"));

	if (BossAsset.Succeeded())
		MonsterClassArray.Add(BossAsset.Class);

	static ConstructorHelpers::FClassFinder<AMonster>	FinalBossAsset(TEXT("Blueprint'/Game/Monster/BPFinalBoss.BPFinalBoss_C'"));

	if (FinalBossAsset.Succeeded())
		MonsterClassArray.Add(FinalBossAsset.Class);

	static ConstructorHelpers::FClassFinder<AMonster>	SevarogBossAsset(TEXT("Blueprint'/Game/Monster/BPSevarog.BPSevarog_C'"));
	if (SevarogBossAsset.Succeeded())
		MonsterClassArray.Add(SevarogBossAsset.Class);

	static ConstructorHelpers::FClassFinder<AMonster>	GolemAsset(TEXT("Blueprint'/Game/Monster/BPGolem.BPGolem_C'"));
	if (GolemAsset.Succeeded())
		MonsterClassArray.Add(GolemAsset.Class);

	static ConstructorHelpers::FClassFinder<AMonster>	TrollAsset(TEXT("Blueprint'/Game/Monster/BPTroll.BPTroll_C'"));
	if (TrollAsset.Succeeded())
		MonsterClassArray.Add(TrollAsset.Class);
}
