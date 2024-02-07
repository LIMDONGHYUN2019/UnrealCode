// Copyright Epic Games, Inc. All Rights Reserved.


#include "LectureGameGameModeBase.h"
#include "../MainGameInstance.h"
#include "../NetworkManager.h"
#include "../RecvQueue.h"
#include "../PacketSession.h"
#include "../NetStream.h"
#include "../GameServerManager.h"
#include "../ChattingServerManager.h"
#include "../Player/Greyston.h"
#include "../Player/Serath.h"
#include "../Player/Twinblast.h"
#include "../Chatting/ChattingWidget.h"
#include "../UI/GameWidget.h"
#include "../OtherUser/UserCharacter.h"
#include "../UI/ItemBox.h"
#include "../Monster/Minion.h"
#include "../Monster/Boss.h"
#include "../Monster/FinalBoss.h"
#include "../Monster/Sevarog.h"
#include "../Monster/Troll.h"
#include "../Monster/Golem.h"
#include "../MainPlayerController.h"
#include "../UI/Store/ItemData.h"
#include "../UI/Store/StoreWidget.h"
#include "../UI/Skill/SkillWidget.h"
#include "../UI/Inventory/InventoryWidget.h"
#include "../UI/PlayerUI/PlayerStatusWidget.h"
#include "../UI/PlayerUI/PlayerInfoWidget.h"
#include "../UI/PlayerUI/TradeWidget.h"
#include "../UI/PlayerUI/TradeRequestWidget.h"
#include "../UI/Group/GroupRequestWidget.h"
#include "../UI/Group/GroupWidget.h"
#include "../UI/Quest/QuestWidget.h"
#include "../Etc/EnvironmentActor.h"
//#include "D:/UE_4.26/Engine/Plugins/Runtime/nDisplay/Source/DisplayClusterConfigurator/Private/Views/General/DisplayClusterConfiguratorViewGeneral.h"


ALectureGameGameModeBase::ALectureGameGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget>	GameWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/UI_Game.UI_Game_C'"));

	if (GameWidgetAsset.Succeeded())
		GameWidgetClass = GameWidgetAsset.Class;

	PlayerControllerClass = AMainPlayerController::StaticClass();

	//LoadMonsterClass();
	// 블루프린트 게임모드에서 추가를 해둠...
}

void ALectureGameGameModeBase::StartPlay()
{
	Super::StartPlay();
}

void ALectureGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if (GameWidgetClass)
	{
		pGameWidget = Cast<UGameWidget>(CreateWidget(GetWorld(), GameWidgetClass));
		if (pGameWidget)
			pGameWidget->AddToViewport();
	}

	//UMainGameInstance* GameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	//if (GameInst->GetCharacterSet())
	//{
	//	_tagCharacterInfo	CharacterInfo = GameInst->GetCharacterInfo();
	//	// 캐릭터 생성
	//	FString	strJob(CharacterInfo.strJob);
	//	LOG(TEXT("Character Job : %s"), *strJob);
	//	FString strName(CharacterInfo.strName);
	//	LOG(TEXT("Character Name : %s"), *strName);
	//	// 플레이어를 생성한다.
	//	if (strJob == TEXT("Knight"))
	//	{
	//		FActorSpawnParameters	param;

	//		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	//		AGreystone* pGreystone = GetWorld()->SpawnActor<AGreystone>(GreystoneClass, PlayerStartLoc, PlayerStartRot,
	//			param);

	//		APlayerController* pController = GetWorld()->GetFirstPlayerController();

	//		pController->Possess(pGreystone);
	//	}

	//	else if (strJob == TEXT("Archer"))
	//	{
	//		FActorSpawnParameters	param;

	//		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	//		ASerath* pSerath = GetWorld()->SpawnActor<ASerath>(SerathClass, PlayerStartLoc, PlayerStartRot,
	//			param);

	//		APlayerController* pController = GetWorld()->GetFirstPlayerController();

	//		pController->Possess(pSerath);
	//	}

	//	else if (strJob == TEXT("Magician"))
	//	{
	//		FActorSpawnParameters	param;

	//		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	//		ATwinblast* pTwinblast = GetWorld()->SpawnActor<ATwinblast>(TwinblastClass, PlayerStartLoc, PlayerStartRot,
	//			param);

	//		APlayerController* pController = GetWorld()->GetFirstPlayerController();

	//		pController->Possess(pTwinblast);
	//	}

	//	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//	if (IsValid(pPlayer))
	//	{
	//		pPlayer->SetCharacterInfo(&CharacterInfo);
	//		pPlayer->SetHP();
	//	}

	//	UPlayerStatusWidget* pStatusWidget = pGameWidget->GetPlayerStatusWidget();
	//	UInventoryWidget* pInventoryWidget = pGameWidget->GetInventoryWidget();
	//	if (pStatusWidget)
	//	{
	//		pStatusWidget->SetStatus(&CharacterInfo);
	//	}
	//	if (pInventoryWidget)
	//	{
	//		pInventoryWidget->SetGold(CharacterInfo.iMoney);
	//	}

	//	pGameWidget->SetExpPercent(CharacterInfo.iExp / (float)CharacterInfo.iExpMax);
	//	GameInst->ClearCharacterSet();
	//}
}

void ALectureGameGameModeBase::InitGameState()
{
	Super::InitGameState();
}

void ALectureGameGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int job = JobClass.Num();
	int Monster = MonsterClassArray.Num();
	// 33 37 8 9

	RecvQueue* pQueue = GameServerManager::GetInst()->GetRecvQueue();

	int32	iProtocol = 0, iLength = 0;
	uint8	packet[PACKET_LENGTH] = {};

	if (pQueue->pop(iProtocol, iLength, packet))
	{
		int32 iTempo = pQueue->GetCount();
		//if (iTempo > 100)
			//PrintViewport(1.f, FColor::Blue, FString::Printf(TEXT("Client Packet Count, Protocol : %d // %d"), pQueue->GetCount(), iProtocol));
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
		case GP_CHARACTERLEVELUP:
			CharacterLevelup(packet, iLength);
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
		case GP_STORELIST:
			StoreList(packet, iLength);
			break;
		case GP_INVENTORYLIST:
			InventoryList(packet, iLength);
			break;
		case GP_INVENTORYADD:
			InventoryAdd(packet, iLength);
			break;
		case GP_INVENTORYDELETE:
			InventoryDelete(packet, iLength);
			break;
		case GP_INVENTORYGOLD:
			InventoryGold(packet, iLength);
			break;
		case GP_QUESTLIST:
			QuestList(packet, iLength);
			break;
		case GP_QUESTCOMPLETE:
			QuestComplete(packet, iLength);
			break;
			//=============================================
		case GP_TRADEREQUEST:
			TradeRequest(packet, iLength);
			break;
		case GP_TRADEREQUESTACCEPT:
			TradeRequestAccept(packet, iLength);
			break;
		case GP_TRADECANCEL:
			TradeCancel(packet, iLength);
			break;
		case GP_TRADEADD:
			TradeAdd(packet, iLength);
			break;
		case GP_TRADEDELETE:
			TradeDelete(packet, iLength);
			break;
		case GP_TRADEREADY:
			TradeReady(packet, iLength);
			break;
		case GP_TRADESUCCESS:
			TradeSuccess(packet, iLength);
			break;
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

void ALectureGameGameModeBase::LoadMonsterClass()
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

void ALectureGameGameModeBase::GameIn(uint8* pPacket, int32 iLength)
{
	PrintViewport(1.f, FColor::Blue, TEXT("Game In!!!!!!!!!!"));

	NetStream stream;
	stream.SetBuffer(pPacket);

	int32 iCharacterNumber = -1;
	FVector vPos;
	FRotator vRot;
	FVector vScale;
	_tagCharacterInfo CharacterInfo = {};

	stream.Read(&iCharacterNumber, sizeof(int32));
	NetworkManager::GetInst()->SetCharacterNumber(iCharacterNumber);
	stream.Read(&vPos, sizeof(FVector));
	stream.Read(&vRot, sizeof(FVector));
	stream.Read(&vScale, sizeof(FVector));

	stream.Read(&CharacterInfo, sizeof(_tagCharacterInfo));

	FString	strJob(CharacterInfo.strJob);
	LOG(TEXT("Character Job : %s"), *strJob);
	FString strName(CharacterInfo.strName);
	LOG(TEXT("Character Name : %s"), *strName);

	// 플레이어를 생성한다.
	if (strJob == TEXT("GreyStone"))
	{
		FActorSpawnParameters	param;

		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		AGreystone* pGreystone = GetWorld()->SpawnActor<AGreystone>(GreystoneClass, vPos, vRot,
			param);

		APlayerController* pController = GetWorld()->GetFirstPlayerController();

		pController->Possess(pGreystone);
	}

	else if (strJob == TEXT("TwinBlast"))
	{
		FActorSpawnParameters	param;

		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		ATwinblast* pTwinblast = GetWorld()->SpawnActor<ATwinblast>(TwinblastClass, vPos, vRot,
			param);

		APlayerController* pController = GetWorld()->GetFirstPlayerController();

		pController->Possess(pTwinblast);
	}

	else if (strJob == TEXT("Serath"))
	{
		FActorSpawnParameters	param;

		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		ASerath* pSerath = GetWorld()->SpawnActor<ASerath>(SerathClass, vPos, vRot,
			param);

		APlayerController* pController = GetWorld()->GetFirstPlayerController();

		pController->Possess(pSerath);
	}

	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UPlayerStatusWidget* pStatusWidget = pGameWidget->GetPlayerStatusWidget();
	UInventoryWidget* pInventoryWidget = pGameWidget->GetInventoryWidget();

	if (IsValid(pPlayer))
	{
		pPlayer->SetCharacterInfo(&CharacterInfo);
		pPlayer->SetHP();
		pPlayer->SetMP();
		pPlayer->SetActorRelativeScale3D(vScale);
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

	PrintViewport(10.f, FColor::Blue, FString::Printf(TEXT("Exp Max = %d"), CharacterInfo.iExpMax)); // 1 / 4
}

void ALectureGameGameModeBase::UserAppear(uint8* pPacket, int32 iLength)
{
	// 내 화면에 다른 유저가....
	NetStream stream;
	stream.SetBuffer(pPacket);

	int32	iCharacterNumber = -1;
	int32	iJob = -1;

	stream.Read(&iCharacterNumber, sizeof(int32));
	stream.Read(&iJob, sizeof(int32));

	TDoubleLinkedList<int32>::TDoubleLinkedListNode* pNode = m_UserCharacterNumberList.GetHead();

	while (pNode)
	{
		if (pNode->GetValue() == iCharacterNumber)
			return;
		pNode = pNode->GetNextNode();
	}

	FVector	vPos, vScale, vRot;
	_tagCharacterInfo tCharacterInfo = {};
	stream.Read(&vPos, sizeof(FVector));
	stream.Read(&vScale, sizeof(FVector));
	stream.Read(&vRot, sizeof(FVector));
	stream.Read(&tCharacterInfo, sizeof(_tagCharacterInfo));
	m_UserCharacterNumberList.AddTail(iCharacterNumber);

	// FRotator->Vector로 안한것은 순서대로 저장되는지 + 제대로 저장되는지 확인이 안되서
	FRotator	Rot;
	Rot.Roll = vRot.X;
	Rot.Yaw = vRot.Y;
	Rot.Pitch = vRot.Z;

	PrintViewport(5.f, FColor::Blue, TEXT("User Appear - 유저 추가!!"));

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	m_OtherUserCharacter = GetWorld()->SpawnActor<AUserCharacter>(JobClass[iJob], vPos, Rot, param);
	m_OtherUserCharacter->SetActorRelativeScale3D(vScale);
	m_OtherUserCharacter->SetUserName(tCharacterInfo.strName);
	m_OtherUserCharacter->SetCharacterInfo(&tCharacterInfo);
	m_OtherUserCharacter->SetCharacterNumber(iCharacterNumber);
	m_OtherUserList.Emplace(iCharacterNumber, m_OtherUserCharacter);
}

void ALectureGameGameModeBase::UserDisAppear(uint8* pPacket, int32 iLength)
{
	// 나의 화면에서 다른 유저들을 지우는것.
	NetStream stream;
	stream.SetBuffer(pPacket);

	int32	iUserCount = 0;
	int32	iCharacterNumber = -1;

	PrintViewport(5.f, FColor::Blue, TEXT("User DisAppear - 유저 제거!!"));

	stream.Read(&iCharacterNumber, sizeof(int32));

	// 현재 목록에 이 캐릭터의 번호가 있는지 확인한다.
	TDoubleLinkedList<int32>::TDoubleLinkedListNode* pNode = m_UserCharacterNumberList.GetHead();

	while (pNode)
	{
		if (pNode->GetValue() == iCharacterNumber)
		{
			AUserCharacter* OtherUser = FindUser(iCharacterNumber);

			m_UserCharacterNumberList.RemoveNode(iCharacterNumber);

			m_OtherUserList.Remove(iCharacterNumber);

			GetWorld()->DestroyActor(OtherUser);

			break;
		}

		pNode = pNode->GetNextNode();
	}
}

void ALectureGameGameModeBase::OtherUserAppear(uint8* pPacket, int32 iLength)
{
	// 나를 다른 유저 화면에
	NetStream stream;
	stream.SetBuffer(pPacket);
	int32 iChracterCount = 0;
	stream.Read(&iChracterCount, sizeof(int32));
	PrintViewport(5.f, FColor::Blue, TEXT("OtherUser Appear!!"));
	for (int32 i = 0; i < iChracterCount; i++)
	{
		int32	iCharacterNumber = -1;
		int32	iJob = -1;
		FVector	vPos, vScale, vRot;
		_tagCharacterInfo tOtherUserInfo = {};

		stream.Read(&iCharacterNumber, sizeof(int32));
		stream.Read(&iJob, sizeof(int32));
		stream.Read(&vPos, sizeof(FVector));
		stream.Read(&vScale, sizeof(FVector));
		stream.Read(&vRot, sizeof(FVector));
		stream.Read(&tOtherUserInfo, sizeof(_tagCharacterInfo));
		TDoubleLinkedList<int32>::TDoubleLinkedListNode* pNode = m_UserCharacterNumberList.GetHead();
		while (pNode)
		{
			if (pNode->GetValue() == iCharacterNumber)
				return;

			pNode = pNode->GetNextNode();
		}
		m_UserCharacterNumberList.AddTail(iCharacterNumber);
		//FRotator	Rot;
		//Rot.Roll = vRot.X;
		//Rot.Pitch = vRot.Y;
		//Rot.Yaw = vRot.Z;
		FRotator	Rot;
		Rot.Roll = vRot.X;
		Rot.Yaw = vRot.Y;
		Rot.Pitch = vRot.Z;


		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		m_OtherUserCharacter = GetWorld()->SpawnActor<AUserCharacter>(JobClass[iJob], vPos, Rot, param);
		m_OtherUserCharacter->SetActorRelativeScale3D(vScale);
		m_OtherUserCharacter->SetUserName(tOtherUserInfo.strName);
		m_OtherUserCharacter->SetCharacterInfo(&tOtherUserInfo);
		m_OtherUserCharacter->SetHP();
		m_OtherUserCharacter->SetCharacterNumber(iCharacterNumber);

		m_OtherUserList.Emplace(iCharacterNumber, m_OtherUserCharacter);
	}
}

void ALectureGameGameModeBase::OtherUserDisAppear(uint8* pPacket, int32 iLength)
{
	// 주변에 있는 다른 유저의 화면에서 나의 화면을 지우는것.
	NetStream stream;
	stream.SetBuffer(pPacket);

	int32	iUserCount = 0;
	int32	iCharacterNumber = -1;

	PrintViewport(3.f, FColor::Blue, TEXT("OtherUser DisAppear!!"));

	stream.Read(&iUserCount, sizeof(int32));

	for (int32 i = 0; i < iUserCount; ++i)
	{
		stream.Read(&iCharacterNumber, sizeof(int32));
		TDoubleLinkedList<int32>::TDoubleLinkedListNode* pNode = m_UserCharacterNumberList.GetHead();

		while (pNode)
		{
			if (pNode->GetValue() == iCharacterNumber)
			{
				AUserCharacter* OtherUser = FindUser(iCharacterNumber);

				m_UserCharacterNumberList.RemoveNode(iCharacterNumber);

				m_OtherUserList.Remove(iCharacterNumber);

				GetWorld()->DestroyActor(OtherUser);

				break;
			}


			pNode = pNode->GetNextNode();
		}
	}
}

void ALectureGameGameModeBase::MoveSync(uint8* pPacket, int32 iLength)
{
	// 거리가 너무 틀어지면 맞춰주기위해
	// But 패킷을 보내는 사이에 플레이어 이동 가속화로 인해 차이가 더 벌어져서 아예 순간이동을 한다.
	NetStream stream;
	stream.SetBuffer(pPacket);

	FVector	vServerPos;
	FRotator	vServerRot;

	stream.Read(&vServerPos, sizeof(FVector));
	stream.Read(&vServerRot, sizeof(FRotator));


	APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();

	PrintViewport(1.f, FColor::Turquoise, FString::Printf(TEXT("Move Syncing - Server Pos : %s  //  Client Pos : %s"),
		*vServerPos.ToString(), *pPlayer->GetActorLocation().ToString()));
	pPlayer->SetActorLocation(vServerPos);
	pPlayer->SetActorRotation(vServerRot);
}

void ALectureGameGameModeBase::MoveStart(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	FVector		ServerPos;
	FRotator	ServerRot;

	stream.Read(&ServerPos, sizeof(FVector));
	stream.Read(&ServerRot, sizeof(FRotator));
}

void ALectureGameGameModeBase::MoveDir(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	FVector		ServerPos;
	FRotator	ServerRot;

	stream.Read(&ServerPos, sizeof(FVector));
	stream.Read(&ServerRot, sizeof(FRotator));
}

void ALectureGameGameModeBase::MoveEnd(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	FVector		ServerPos;
	FRotator	ServerRot;

	stream.Read(&ServerPos, sizeof(FVector));
	stream.Read(&ServerRot, sizeof(FRotator));

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	FVector	Pos = PlayerCharacter->GetActorLocation();

	//PlayerCharacter->SetActorLocation(ServerPos);

	//PlayerCharacter->SetActorLocation(ServerPos);

	/*FVector	Pos = PlayerCharacter->GetActorLocation();

	ServerPos.Z = Pos.Z;

	float Distance = FVector::Distance(ServerPos, Pos);

	if (Distance > 130.f)
	{
		PlayerCharacter->SetActorLocation(Pos);
	}*/
}

void ALectureGameGameModeBase::RotationPacket(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);
	int32	iCharacterNumber = -1;
	stream.Read(&iCharacterNumber, sizeof(int32));

	float	Yaw = 0.f;
	stream.Read(&Yaw, sizeof(float));

	AUserCharacter* User = FindUser(iCharacterNumber);

	if (!User)
		return;

	FRotator	Rot = User->GetActorRotation();
	Rot.Yaw = Yaw;

	User->SetActorRotation(Rot);
}

void ALectureGameGameModeBase::OtherStartMove(uint8* pPacket, int32 iLength)
{
	// Appear할떄 오류발생
	NetStream stream;
	stream.SetBuffer(pPacket);
	int32	iCharacterNumber = -1;
	stream.Read(&iCharacterNumber, sizeof(int32));

	// 현재 목록에 이 캐릭터의 번호가 있는지 확인한다.
	//TDoubleLinkedList<int32>::TDoubleLinkedListNode* pNode = m_UserCharacterNumberList.GetHead();

	//while (pNode)
	//{
	//	if (pNode->GetValue() == iCharacterNumber)
	//		continue;
	//	pNode = pNode->GetNextNode();
	//}
	FVector	vPos, vDir, vScale;
	FRotator	Rot;
	float	fViewDir, fMoveSpeed;

	stream.Read(&vPos, sizeof(FVector));
	stream.Read(&Rot, sizeof(FRotator));
	stream.Read(&vDir, sizeof(FVector));
	stream.Read(&fViewDir, sizeof(float));
	stream.Read(&fMoveSpeed, sizeof(float));

	//FRotator	Rot;
	//Rot.Roll = vRot.X;
	//Rot.Pitch = vRot.Y;
	//Rot.Yaw = vRot.Z;

	AUserCharacter* User = FindUser(iCharacterNumber);

	if (!User)
		return;

	m_OtherUserCharacter = User;
	//m_OtherUserCharacter = *m_OtherUserList.Find(iCharacterNumber);
	if (m_OtherUserCharacter)
	{
		m_OtherUserCharacter->SetMoveDir(vDir);
		m_OtherUserCharacter->SetMove(true);
		m_OtherUserCharacter->SetViewDir(fViewDir);
		m_OtherUserCharacter->SetViewMoveSpeed(fMoveSpeed);
		m_OtherUserCharacter->SetActorRotation(Rot);
		m_OtherUserCharacter->Move(vPos);
	}
}

void ALectureGameGameModeBase::OtherMove(uint8* pPacket, int32 iLength)
{
	if (m_OtherUserList.Num() == 0)
		return;

	NetStream stream;
	stream.SetBuffer(pPacket);
	int32	iCharacterNumber = -1;
	stream.Read(&iCharacterNumber, sizeof(int32));
	// 현재 목록에 이 캐릭터의 번호가 있는지 확인한다.
	//TDoubleLinkedList<int32>::TDoubleLinkedListNode* pNode = m_UserCharacterNumberList.GetHead();

	//while (pNode)
	//{
	//	if (pNode->GetValue() == iCharacterNumber)
	//		continue;
	//	pNode = pNode->GetNextNode();
	//}
	FVector	vPos, vDir;
	FRotator	Rot;
	float	fViewDir, fMoveSpeed;

	stream.Read(&vPos, sizeof(FVector));
	stream.Read(&Rot, sizeof(FRotator));
	stream.Read(&vDir, sizeof(FVector));
	stream.Read(&fViewDir, sizeof(float));
	stream.Read(&fMoveSpeed, sizeof(float));

	AUserCharacter* User = FindUser(iCharacterNumber);

	if (!User)
		return;

	m_OtherUserCharacter = User;
	//m_OtherUserCharacter = *m_OtherUserList.Find(iCharacterNumber);
	if (m_OtherUserCharacter)
	{
		//PrintViewport(1.f, FColor::Blue, FString::Printf(TEXT("Other Moving Dir : %s"), *vDir.ToString()));
		m_OtherUserCharacter->SetMoveDir(vDir);
		m_OtherUserCharacter->SetMove(true);
		m_OtherUserCharacter->Move(vPos);
		m_OtherUserCharacter->SetViewMoveSpeed(fMoveSpeed);
		m_OtherUserCharacter->SetViewDir(fViewDir);
		m_OtherUserCharacter->SetActorRotation(Rot);
	}
}

void ALectureGameGameModeBase::OtherEndMove(uint8* pPacket, int32 iLength)
{
	if (m_OtherUserList.Num() == 0)
		return;

	NetStream stream;
	stream.SetBuffer(pPacket);
	int32	iCharacterNumber = -1;
	stream.Read(&iCharacterNumber, sizeof(int32));

	FVector		vPos;
	FRotator	Rot;

	stream.Read(&vPos, sizeof(FVector));
	stream.Read(&Rot, sizeof(FRotator));

	AUserCharacter* User = FindUser(iCharacterNumber);

	if (!User)
		return;

	m_OtherUserCharacter = User;
	//m_OtherUserCharacter = *m_OtherUserList.Find(iCharacterNumber);
	if (m_OtherUserCharacter)
	{
		m_OtherUserCharacter->MoveEnd();
		m_OtherUserCharacter->SetMove(false);
		m_OtherUserCharacter->SetViewDir(0.f);
		//m_OtherUserCharacter->SetViewMoveSpeed(0);
		m_OtherUserCharacter->SetActorRotation(Rot);
		m_OtherUserCharacter->Move(vPos);
	}
}

void ALectureGameGameModeBase::OtherAttack(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);
	int iCharacterNumber = -1;
	stream.Read(&iCharacterNumber, sizeof(int));
	ATTACK_TYPE Attack_Type;
	stream.Read(&Attack_Type, sizeof(ATTACK_TYPE));

	int32 AttackIndex = -1;
	stream.Read(&AttackIndex, sizeof(int32));

	float fViewDir;
	stream.Read(&fViewDir, sizeof(float));

	AUserCharacter* User = FindUser(iCharacterNumber);

	if (!User)
		return;

	m_OtherUserCharacter = User;

	switch (Attack_Type)
	{
	case ATTACK_TYPE::NormalAttack:
	{
		m_OtherUserCharacter->NormalAttack(AttackIndex);
	}
	break;
	case ATTACK_TYPE::Skill1:
		m_OtherUserCharacter->SkillAttack1();
		break;
	case ATTACK_TYPE::Skill2:
		m_OtherUserCharacter->SkillAttack2();
		break;
	}

	m_OtherUserCharacter->SetViewDir(fViewDir);
}

void ALectureGameGameModeBase::ChattingMessage(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);
	int iUserNumber = 0;
	char ChracterName[USERNAME_SIZE] = {};
	char ChatContent[MESSAGE_LENGTH] = {};

	stream.Read(&iUserNumber, sizeof(int));
	stream.Read(ChracterName, USERNAME_SIZE);
	stream.Read(ChatContent, MESSAGE_LENGTH);

	FString ChattingContent = ChatContent;
	FString ChattingName = ChracterName;

	FString Message = UTF8_TO_TCHAR(ChatContent); // TCHAR → UTF16이니까
	//m_Desc->SetText(FText::FromString(Message));

	//FString ChattingContent = FString::Printf(TEXT("%s"), ChatContent);

	pGameWidget->GetChatWidget()->AddChatting(ChattingName + TEXT(" : ") + Message);
}

void ALectureGameGameModeBase::MonsterSpawn(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	FVector	vPos, vScale;
	FRotator	fRot;
	int32 iMonsterNumber = 0;
	int32	iType = 0;
	stream.Read(&iType, sizeof(int32));
	EMonsterType MonsterType = (EMonsterType)iType; // 현재 서버에서 몬스터목록은 enum으로 되어있다.

	stream.Read(&vPos, sizeof(FVector));
	stream.Read(&vScale, sizeof(FVector));
	stream.Read(&fRot, sizeof(FRotator));
	stream.Read(&iMonsterNumber, sizeof(int32));

	AMonster* pMonster = nullptr;
	for (auto& Elem : m_MonsterList)
	{
		if (Elem->GetMonsterNumber() == iMonsterNumber)
		{
			pMonster = Elem;
			break;
		}
	}
	pMonster->Resurrection();
	pMonster->SetMonsterPos(vPos);
	pMonster->SetMonsterNumber(iMonsterNumber);
	pMonster->SetHP(pMonster->GetHPMax());
}

void ALectureGameGameModeBase::MonsterAI(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	int32 iMonsterNumber = 0;
	MonsterAIProtocol AIProtocol;

	stream.Read(&iMonsterNumber, sizeof(int32));
	stream.Read(&AIProtocol, sizeof(MonsterAIProtocol));

	AMonster* pMonster = nullptr;
	for (auto& Elem : m_MonsterList)
	{
		if (Elem->GetMonsterNumber() == iMonsterNumber)
		{
			pMonster = Elem;
			break;
		}
	}

	if (!pMonster)
		return;

	switch (AIProtocol)
	{
	case MA_IDLE:
		pMonster->MoveEnd();
		pMonster->SetMove(false);
		break;
	case MA_TRACESTART:
	{
		//PrintViewport(1.0f, FColor::Yellow, FString::Printf(TEXT("TraceStarting.....")));
		pMonster->Move();
	}
	break;
	case MA_TRACE:
	{
		FVector	vPos, vDir;
		//PrintViewport(1.0f, FColor::Green, FString::Printf(TEXT("!!!!Tracing.....")));
		stream.Read(&vDir, sizeof(FVector));
		stream.Read(&vPos, sizeof(FVector));

		pMonster->SetMove(true);
		pMonster->SetMoveDir(vDir);
		pMonster->SetMonsterPos(vPos);
		pMonster->Move();
	}
	break;
	case MA_TRACEEND:
	{
		FVector	vPos;
		stream.Read(&vPos, sizeof(FVector));
		pMonster->SetMonsterPos(vPos);
		pMonster->SetMove(false);
		pMonster->MoveEnd();
	}
	break;
	case MA_PATROLSTART:
	{
		FVector	vDir;
		stream.Read(&vDir, sizeof(FVector));

		pMonster->SetMoveDir(vDir);
		pMonster->SetMove(true);
		pMonster->Move();
	}
	break;
	case MA_PATROL:
	{
		FVector	vPos, vDir;
		stream.Read(&vDir, sizeof(FVector));
		stream.Read(&vPos, sizeof(FVector));
		pMonster->SetMoveDir(vDir);
		pMonster->SetMonsterPos(vPos);
		pMonster->Move();
	}
	break;
	case MA_PATROLEND:
	{
		FVector	vPos;
		stream.Read(&vPos, sizeof(FVector));
		pMonster->SetMonsterPos(vPos);
		pMonster->SetMove(false);
		pMonster->MoveEnd();
	}
	break;

	case MA_DAMAGE: // 몬스터 공격
	{
		int32	Damage;
		stream.Read(&Damage, sizeof(int32));

		int32	HP;
		stream.Read(&HP, sizeof(int32));

		int32	iCharacterNumber = -1;
		stream.Read(&iCharacterNumber, sizeof(int32));

		// 유저번호가 현재 이 클라이언트의 유저라면 플레이어의 체력을 받아온 데미지만큼 감소시키고
		// 이 클라이언트의 유저가 아니라면 다른 유저들 목록중 찾아서 체력을 감소시킨다.
		if (iCharacterNumber == NetworkManager::GetInst()->GetCharacterNumber())
		{
			APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

			/*PrintViewport(2.0f, FColor::Blue, FString::Printf(TEXT("Player Pos : %s"), *pPlayer->GetActorLocation().ToString()));
			PrintViewport(2.0f, FColor::Blue, FString::Printf(TEXT("Monster Pos : %s"), *pMonster->GetActorLocation().ToString()));
			PrintViewport(2.0f, FColor::Blue, FString::Printf(TEXT("Monster Client Pos : %s"), *pMonster->GetMonsterPos().ToString()));*/
			if (IsValid(pPlayer))
			{
				if (pPlayer->AddHP(Damage))
				{
					pPlayer->Death();
				}
			}
		}
		else
		{
			// 다른 유저중 찾아야 한다.
			AUserCharacter* pFindUser = FindUser(iCharacterNumber);
			if (pFindUser)
			{
				// 죽었을 경우
				if (pFindUser->AddHP(Damage))
				{
					pFindUser->Death();
				}
			}
		}
	}
	break;

	case MA_ATTACK:
	{
		pMonster->SetMove(false);
		pMonster->Attack();
	}
	break;
	case MA_ATTACKEND:
	{
		FVector	vDir;
		stream.Read(&vDir, sizeof(FVector));
		pMonster->SetMoveDir(vDir);
	}

	case MA_SKILL1:
	{
		pMonster->SetMove(false);
		pMonster->Skill_1();
	}
	break;

	case MA_SKILL1END:
	{
		FVector	vDir;
		stream.Read(&vDir, sizeof(FVector));
		pMonster->SetMoveDir(vDir);
	}
	break;

	case MA_SKILL2:
	{
		pMonster->SetMove(false);
		pMonster->Skill_2();
	}
	break;
	case MA_SKILL2END:
	{
		FVector	vDir;
		stream.Read(&vDir, sizeof(FVector));
		pMonster->SetMoveDir(vDir);
	}
	break;
	}
}

void ALectureGameGameModeBase::CharacterLevelup(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);
}

void ALectureGameGameModeBase::LevelStart(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	int32	iMonsterCount = 0;
	stream.Read(&iMonsterCount, sizeof(int32));

	PrintViewport(3.0f, FColor::Cyan, FString::Printf(TEXT("Monster Count : %d"), iMonsterCount));

	for (int32 i = 0; i < iMonsterCount; ++i)
	{
		int32	iType = 0;
		stream.Read(&iType, sizeof(int32));
		EMonsterType	MonsterType = (EMonsterType)iType;

		FVector		vPos, vScale;
		FVector		vRot;
		FRotator	Rot;

		stream.Read(&vPos, sizeof(FVector));
		stream.Read(&vScale, sizeof(FVector));
		stream.Read(&vRot, sizeof(FVector));
		Rot.Roll = vRot.X;
		Rot.Yaw = vRot.Y;
		Rot.Pitch = vRot.Z;

		vPos.Z += 1000;

		AMonster* pMonster = nullptr;

		switch (MonsterType)
		{
		case EMonsterType::Minion:
			pMonster = GetWorld()->SpawnActor<AMinion>(MonsterClassArray[0], vPos, Rot);
			break;
		case EMonsterType::Golem:
			pMonster = GetWorld()->SpawnActor<AGolem>(MonsterClassArray[1], vPos, Rot);
			break;
		case EMonsterType::Troll:
			pMonster = GetWorld()->SpawnActor<ATroll>(MonsterClassArray[2], vPos, Rot);
			break;
		case EMonsterType::Boss:
			pMonster = GetWorld()->SpawnActor<ABoss>(MonsterClassArray[3], vPos, Rot);
			break;
		case EMonsterType::FinalBoss:
			pMonster = GetWorld()->SpawnActor<AFinalBoss>(MonsterClassArray[4], vPos, Rot);
			break;
		case EMonsterType::Sevarog:
			pMonster = GetWorld()->SpawnActor<ASevarog>(MonsterClassArray[5], vPos, Rot);
			break;
		}

		int32 iMonsterNumber = 0;
		stream.Read(&iMonsterNumber, sizeof(int32));

		if (pMonster)
		{
			pMonster->SetActorRelativeScale3D(vScale);
			pMonster->SetMonsterPos(vPos);

			pMonster->SetMonsterNumber(iMonsterNumber);
			pMonster->SetHP(pMonster->GetHPMax());

			m_MonsterList.Emplace(pMonster);
		}
		else if (!pMonster)
		{
			//PrintViewport(5.f, FColor::Emerald, FString::Printf(TEXT("%d Number Monster Spawn Failed.....!!"), iMonsterNumber));
		}
	}
	// 모든 몬스터가 만들어진 후에 게임이 시작되어야 한다.

	// Level이 시작되므로 여기에서 서버로 해당 Level이 시작되었다는 패킷을 보내준다.
	PacketSession* NetSession = GameServerManager::GetInst()->GetSession();
	NetStream	stream2;
	uint8	strPacket[PACKET_LENGTH] = {};
	stream2.SetBuffer(strPacket);
	NetSession->Write(GP_LEVELSTART, stream2.GetLength(), strPacket);
}

void ALectureGameGameModeBase::SkillList(uint8* pPacket, int32 iLength)
{
	NetStream	stream;
	stream.SetBuffer(pPacket);

	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UMainGameInstance* GameInst = GetWorld()->GetGameInstance<UMainGameInstance>();

	USkillWidget* SkillWidget = pGameWidget->GetSkillWidget();

	int32	SkillCount = 0;
	stream.Read(&SkillCount, 4);

	TArray<FString>	SkillNameArray;
	EJob	SkillJob = EJob::End;
	int32 iSkillLevel = 0;

	for (int32 i = 0; i < SkillCount; ++i)
	{
		int32	Type, Job, Propt;

		SkillInfo* pSkillInfo = new SkillInfo;

		stream.Read(&iSkillLevel, sizeof(int32));

		stream.Read(&Type, sizeof(int32));
		pSkillInfo->eType = (ESkillType)Type;

		stream.Read(&Job, sizeof(int32));
		pSkillInfo->eJob = (EJob)Job;

		SkillJob = pSkillInfo->eJob;

		stream.Read(&Propt, sizeof(int32));
		pSkillInfo->ePropt = (ESkillProperty)Propt;

		stream.Read(&pSkillInfo->iSkillNumber, sizeof(int32));
		stream.Read(&pSkillInfo->iSkillLevelMax, sizeof(int32));
		stream.Read(&pSkillInfo->iRequiredLevel, sizeof(int32));
		stream.Read(&pSkillInfo->fCooldown, sizeof(float));

		int32	NameLength = 0;
		stream.Read(&NameLength, sizeof(int32));
		stream.Read(pSkillInfo->strName, NameLength);

		NameLength = 0;
		stream.Read(&NameLength, sizeof(int32));
		stream.Read(pSkillInfo->strDesc, NameLength);

		int32	iOptionCount = 0;
		stream.Read(&iOptionCount, sizeof(int32));
		for (int32 j = 0; j < iOptionCount; ++j)
		{
			SkillOption tOption;
			int32	Option;
			stream.Read(&Option, sizeof(int32));
			tOption.SkillOptionType = (ESkillOptionType)Option;
			stream.Read(&tOption.SkillOptionData, sizeof(float));

			pSkillInfo->vecOption.Add(tOption);
		}

		int32	iActionCount = 0;
		stream.Read(&iActionCount, sizeof(int32));
		for (int32 j = 0; j < iActionCount; ++j)
		{
			SkillAction tAction;
			int	Action;
			stream.Read(&Action, sizeof(int32));
			tAction.SkillActionType = (ESkillActionType)Action;

			stream.Read(&tAction.SkillActionData, sizeof(float));

			pSkillInfo->vecAction.Add(tAction);
		}

		int32 iNextSkillNumberCount = 0;
		stream.Read(&iNextSkillNumberCount, sizeof(int32));
		for (int32 j = 0; j < iNextSkillNumberCount; ++j)
		{
			int32 iNextSkillNumber = 0;
			stream.Read(&iNextSkillNumber, sizeof(int32));

			pSkillInfo->vecNextSkillNumber.Add(iNextSkillNumber);
		}
		int32 iPrevSkillNumberCount = 0;
		stream.Read(&iPrevSkillNumberCount, sizeof(int32));
		for (int32 j = 0; j < iPrevSkillNumberCount; ++j)
		{
			int32 iPrevSkillNumber = 0;
			stream.Read(&iPrevSkillNumber, sizeof(int32));

			pSkillInfo->vecNextSkillNumber.Add(iPrevSkillNumber);
		}

		SkillNameArray.Add(pSkillInfo->strName);
		pPlayer->AddSkillInfo(pSkillInfo);
		SkillWidget->AddSkillInfo(pSkillInfo);
		SkillWidget->ChangeSkillLevel(i, iSkillLevel);
	}

	FString	JobName;
	switch (SkillJob)
	{
	case EJob::GreyStone:
		JobName = "GreyStone";
		break;
	case EJob::TwinBlast:
		JobName = "TwinBlast";
		break;
	case EJob::Serath:
		JobName = "Serath";
		break;
	}

	const FSkillIconImage* SkillIcon = GameInst->GetSkillIcon(JobName);

	for (int32 i = 0; i < SkillNameArray.Num(); ++i)
	{
		int32	Index = -1;

		for (int32 j = 0; j < SkillIcon->IconJobArray.IconDataArray.Num(); ++j)
		{
			if (SkillNameArray[i] == SkillIcon->IconJobArray.IconDataArray[j].Name)
			{
				Index = i;

				LOG(TEXT("SkillIndex : %d, SkillName : %s"), Index, *SkillNameArray[Index]);
				SkillWidget->ChangeIcon(Index, SkillIcon->IconJobArray.IconDataArray[j].Image);

				break;
			}
		}

		if (Index == -1)
			continue;
	}

	SkillWidget->SetSkillPoint(pPlayer->GetSkillPoint());
}

void ALectureGameGameModeBase::SkillUp(uint8* pPacket, int32 iLength)
{
	NetStream	stream;
	stream.SetBuffer(pPacket);
	int32 iSkillNumber = -1;
	int32 iSkillPoint = -1;
	int32 iIndex = -1;
	int32 iSkillLevel = -1;
	int32 iSkillUpMsg = 0;

	stream.Read(&iSkillUpMsg, sizeof(int32));

	ESkillUpMessage	Msg = (ESkillUpMessage)iSkillUpMsg;

	if (Msg == ESkillUpMessage::Success)
	{
		stream.Read(&iSkillNumber, sizeof(int32));
		stream.Read(&iSkillPoint, sizeof(int32));
		stream.Read(&iIndex, sizeof(int32));
		stream.Read(&iSkillLevel, sizeof(int32));

		APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		UMainGameInstance* GameInst = GetWorld()->GetGameInstance<UMainGameInstance>();
		USkillWidget* SkillWidget = pGameWidget->GetSkillWidget();

		TArray<SkillInfo*>* SkillInfoArray = SkillWidget->GetSkillInfo();

		pPlayer->SetSkillPoint(iSkillPoint);
		SkillWidget->SetSkillPoint(iSkillPoint);

		if (iSkillNumber == (*SkillInfoArray)[iIndex]->iSkillNumber)
		{
			SkillWidget->ChangeSkillLevel(iIndex, iSkillLevel);
		}
	}

	else if (Msg == ESkillUpMessage::MaxLevelFail)
	{
		pGameWidget->GetChatWidget()->AddChatting(TEXT("[ Notice ] : 스킬레벨이 최대치입니다..."));
	}
	else if (Msg == ESkillUpMessage::SkillPointFail)
	{
		pGameWidget->GetChatWidget()->AddChatting(TEXT("[ Notice ] : 스킬포인트가 부족합니다..."));
	}
	else if (Msg == ESkillUpMessage::RequiredLevelFail)
	{
		pGameWidget->GetChatWidget()->AddChatting(TEXT("[ Notice ] : 플레이어 레벨이 너무 낮습니다."));
	}
	else if (Msg == ESkillUpMessage::LinkedSkillFail)
	{
		pGameWidget->GetChatWidget()->AddChatting(TEXT("[ Notice ] : 이전 스킬을 찍어야합니다."));
	}
}

void ALectureGameGameModeBase::StoreList(uint8* pPacket, int32 iLength)
{
	// 상점 목록에 받아온 목록을 넣어준다.
	NetStream	stream;
	stream.SetBuffer(pPacket);

	int32	ItemCount = 0;
	stream.Read(&ItemCount, 4);

	UStoreWidget* pStoreWidget = pGameWidget->GetStoreWidget();
	if (!pStoreWidget)
		return;

	for (int32 i = 0; i < ItemCount; i++)
	{
		FItemInfoData StoreList;
		stream.Read(StoreList.strName, ITEMNAME_SIZE);
		stream.Read(&StoreList.Grade, sizeof(EItemGrade));
		stream.Read(&StoreList.Type, sizeof(ItemType));
		stream.Read(&StoreList.Buy, sizeof(int32));
		stream.Read(&StoreList.Sell, sizeof(int32));

		int32 iOptionCount;
		stream.Read(&iOptionCount, sizeof(int32));

		for (int32 j = 0; j < iOptionCount; j++)
		{
			FItemOption tOption;

			stream.Read(&tOption.Type, sizeof(ItemOptionType));
			stream.Read(&tOption.Option, sizeof(int32));

			StoreList.tOption.Add(tOption);
		}

		FString ItemName = StoreList.strName;
		if (pStoreWidget)
		{
			pStoreWidget->AddItem(ItemName, StoreList.Grade, StoreList.Type,
				StoreList.Buy, StoreList.Sell, StoreList.tOption);
		}
	}

	if (pStoreWidget)
	{
		pStoreWidget->SetVisibility(ESlateVisibility::Visible);
	}

	//PrintViewport(10.f, FColor::Blue, FString::Printf(TEXT(" Size~~~~~~ : %d \\ %d"), sizeof(ItemType), sizeof(ItemOptionType))); // 1 / 4
}

void ALectureGameGameModeBase::InventoryList(uint8* pPacket, int32 iLength)
{
	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());

	if (pGameMode)
	{
		NetStream	stream;
		stream.SetBuffer(pPacket);

		int32	ItemCount = 0;
		stream.Read(&ItemCount, 4);

		UInventoryWidget* pInventoryWidget = pGameWidget->GetInventoryWidget();

		for (int32 i = 0; i < ItemCount; ++i)
		{
			FItemInfoData	ItemInfo;
			int32 Tempo;

			stream.Read(ItemInfo.strName, ITEMNAME_SIZE);
			stream.Read(&Tempo, sizeof(int32));
			ItemInfo.Grade = (EItemGrade)Tempo;
			stream.Read(&Tempo, sizeof(int32));
			ItemInfo.Type = (ItemType)Tempo;
			stream.Read(&ItemInfo.Buy, sizeof(int32));
			stream.Read(&ItemInfo.Sell, sizeof(int32));

			int32	OptionCount = 0;

			stream.Read(&OptionCount, sizeof(int32));

			for (int32 j = 0; j < OptionCount; ++j)
			{
				FItemOption tOption;

				stream.Read(&tOption.Type, sizeof(ItemOptionType));
				stream.Read(&tOption.Option, sizeof(int32));

				ItemInfo.tOption.Add(tOption);
			}

			FString ItemName = ItemInfo.strName;
			if (pInventoryWidget)
			{
				pInventoryWidget->AddItem(ItemName, ItemInfo.Grade, ItemInfo.Type, ItemInfo.Buy, ItemInfo.Sell, ItemInfo.tOption);
			}
		}
	}
}

void ALectureGameGameModeBase::InventoryAdd(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);
	bool bSuccess = false;

	stream.Read(&bSuccess, sizeof(bool));
	if (bSuccess)
	{
		UInventoryWidget* pInventoryWidget = pGameWidget->GetInventoryWidget();

		int32 Tempo;
		FItemInfoData	ItemInfo;

		stream.Read(ItemInfo.strName, ITEMNAME_SIZE);

		stream.Read(&Tempo, sizeof(int32));
		ItemInfo.Grade = (EItemGrade)Tempo;

		stream.Read(&Tempo, sizeof(int32));
		ItemInfo.Type = (ItemType)Tempo;

		stream.Read(&ItemInfo.Buy, sizeof(int32));
		stream.Read(&ItemInfo.Sell, sizeof(int32));

		int32	OptionCount = 0;

		stream.Read(&OptionCount, sizeof(int32));

		for (int32 j = 0; j < OptionCount; ++j)
		{
			FItemOption tOption;

			stream.Read(&tOption.Type, sizeof(ItemOptionType));
			stream.Read(&tOption.Option, sizeof(int32));

			ItemInfo.tOption.Add(tOption);
		}

		FString ItemName = ItemInfo.strName;
		if (pInventoryWidget)
		{
			pInventoryWidget->AddItem(ItemName, ItemInfo.Grade, ItemInfo.Type, ItemInfo.Buy, ItemInfo.Sell, ItemInfo.tOption);


		}
	}
	else
	{
		PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("Add Item Failed !!!!!")));
	}

}

void ALectureGameGameModeBase::InventoryDelete(uint8* pPacket, int32 iLength)
{
}

void ALectureGameGameModeBase::InventoryGold(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	int32 iGold = 0;
	stream.Read(&iGold, sizeof(int32));

	UInventoryWidget* pInventoryWidget = pGameWidget->GetInventoryWidget();

	pInventoryWidget->SetGold(iGold);
}

void ALectureGameGameModeBase::QuestList(uint8* pPacket, int32 iLength)
{
	UQuestWidget* QuestWidget = pGameWidget->GetQuestWidget();

	NetStream stream;
	stream.SetBuffer(pPacket);

	int32 iQuestCount = 0;
	stream.Read(&iQuestCount, sizeof(int32));

	for (int32 i = 0; i < iQuestCount; ++i)
	{
		_tagQuestInfo Quest;

		stream.Read(&Quest.QuestNumber, sizeof(int32));
		stream.Read(Quest.strName, QUESTNAME_SIZE);
		stream.Read(Quest.strDesc, DESC_SIZE);

		_tagQuestOption Option;
		{
			stream.Read(&Option.Type, sizeof(EQuestType));
			stream.Read(&Option.SerialNumber, sizeof(int32));
			stream.Read(&Option.OptionData, sizeof(int32));
		}

		stream.Read(&Quest.QuestExp, sizeof(int32));

		// 퀘스트창에 표시한다.
		QuestWidget->AddQuest(Quest);
	}
}

void ALectureGameGameModeBase::QuestComplete(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	int32 iQuestExp = 0;
	stream.Read(&iQuestExp, sizeof(int32));

	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//pPlayer->SetExp(iQuestExp);
	pGameWidget->SetExpPercent(pPlayer->GetCharacterInfo().iExp / (float)pPlayer->GetCharacterInfo().iExpMax);
}

void ALectureGameGameModeBase::PlayerAttackDamage(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	int32 iMonsterNumber = 0;
	stream.Read(&iMonsterNumber, sizeof(int32));
	int32 iMonsterRemainHP = 0;
	stream.Read(&iMonsterRemainHP, sizeof(int32));

	AMonster* pMonster = nullptr;
	for (auto& Elem : m_MonsterList)
	{
		if (Elem->GetMonsterNumber() == iMonsterNumber)
		{
			pMonster = Elem;
			break;
		}
	}

	if (iMonsterRemainHP > 0)
	{
		int32	iDamage = pMonster->GetHP() - iMonsterRemainHP;

		pMonster->SetHP(iMonsterRemainHP);
		pMonster->Hit(iDamage);
	}
}

void ALectureGameGameModeBase::MonsterDeath(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	int32 iMonsterNumber = 0;
	stream.Read(&iMonsterNumber, sizeof(int32));

	int32	iExp = 0, iGold = 0;

	if (iLength > stream.GetLength())
	{
		stream.Read(&iExp, sizeof(int32));
		stream.Read(&iGold, sizeof(int32));
	}

	AMonster* pMonster = nullptr;
	for (auto& Elem : m_MonsterList)
	{
		if (Elem->GetMonsterNumber() == iMonsterNumber)
		{
			pMonster = Elem;
			break;
		}
	}

	if (!pMonster)
		return;

	pMonster->SetHP(0);
	pMonster->Hit(0);

	APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	UInventoryWidget* pInventoryWidget = pGameWidget->GetInventoryWidget();
	UPlayerStatusWidget* pStatusWidget = pGameWidget->GetPlayerStatusWidget();
	USkillWidget* pSkillWidget = pGameWidget->GetSkillWidget();
	if (pGameWidget)
	{
		int32 iLevel = pPlayer->AddExp(iExp);
		if (iLevel > 0)
		{
			UMainGameInstance* GameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
			const FJobLevelStatus* LevelStatus = nullptr;
			
			FString	strJob(pPlayer->GetCharacterInfo().strJob);
			FString Level = FString::Printf(TEXT("Level%d"), iLevel);

			pGameWidget->GetChatWidget()->AddChatting(iLevel + TEXT(" 로 레벨이 올랐습니다."));

			if (strJob == TEXT("GreyStone"))
			{
				LevelStatus = GameInst->GetGreystoneLevelInfo(Level);
			}

			else if (strJob == TEXT("TwinBlast"))
			{
				LevelStatus = GameInst->GetTwinblastLevelInfo(Level);
			}

			else if (strJob == TEXT("Serath"))
			{
				LevelStatus = GameInst->GetSerathLevelInfo(Level);
			}

			if (!LevelStatus)
			{
				pGameWidget->GetChatWidget()->AddChatting(TEXT("  최대 레벨 입니다.!!!!"));
				
			}
			else
			{
				pPlayer->SetLevelUp(LevelStatus);

				if (pStatusWidget)
				{
					_tagCharacterInfo Info = pPlayer->GetCharacterInfo();
					pStatusWidget->SetStatus(&Info);
				}
			}

			pPlayer->Levelup();
		}

		pGameWidget->SetExpPercent(pPlayer->GetCharacterInfo().iExp / (float)pPlayer->GetCharacterInfo().iExpMax);

		if (pInventoryWidget)
		{
			pPlayer->SetGold(iGold);
			pInventoryWidget->SetGold(pPlayer->GetGold());
		}

		if (pSkillWidget)
		{
			pSkillWidget->SkillPointUp(1);
		}

		UMinimapWidget* pMinimap = pGameWidget->GetMinimapWidget();

		FString Money = FString::Printf(TEXT("%d"), iGold);
		FString Exp = FString::Printf(TEXT("%d"), iExp);
		pGameWidget->GetChatWidget()->AddChatting(Money + TEXT(" Gold를  획득했습니다."));
		pGameWidget->GetChatWidget()->AddChatting(Exp + TEXT(" 경험치를  획득했습니다."));

		NetStream	stream1;
		uint8	packet[PACKET_LENGTH] = {};
		stream1.SetBuffer(packet);
		PacketSession* pSession = GameServerManager::GetInst()->GetSession();
		_tagCharacterInfo Info = pPlayer->GetCharacterInfo();
		stream1.Write(&Info, sizeof(_tagCharacterInfo));

		pSession->Write(GP_CHARACTERLEVELUP, stream1.GetLength(), packet);
	}

	//pMonster->ItemDropBox();
	pMonster->Death();

	//m_MonsterList.Remove(pMonster);
	//GetWorld()->DestroyActor(pMonster);
	//GetWorld()->RemoveActor(pMonster, true);

	if(!pMonster)
		PrintViewport(3.f, FColor::Blue, FString::Printf(TEXT("After Monster Death Count : %d"), m_MonsterList.Num()));
}

void ALectureGameGameModeBase::MonsterItemDrop(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);
	int32 iMonsterNumber = 0;
	stream.Read(&iMonsterNumber, sizeof(int32));

	AMonster* pMonster = nullptr;
	for (auto& Elem : m_MonsterList)
	{
		if (Elem->GetMonsterNumber() == iMonsterNumber)
		{
			pMonster = Elem;
			break;
		}
	}
	if (!pMonster)
		return;

	FVector vLoc = pMonster->GetTargetLocation();

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AItemBox* DropItemBox = GetWorld()->SpawnActor<AItemBox>(AItemBox::StaticClass(),
		vLoc, FRotator(0.f, 0.f, 0.f), param);

	// 아이템 확률에 따른 종류별로 서버에서 받아온다.
	int32 iItemDropCount = 0;
	stream.Read(&iItemDropCount, sizeof(int32));

	for (int i = 0; i < iItemDropCount; ++i)
	{
		FItemInfoData DropItem;

		stream.Read(DropItem.strName, ITEMNAME_SIZE);
		stream.Read(&DropItem.Grade, sizeof(EItemGrade));
		stream.Read(&DropItem.Type, sizeof(ItemType));
		stream.Read(&DropItem.Buy, sizeof(int32));
		stream.Read(&DropItem.Sell, sizeof(int32));

		FString ItemName = DropItem.strName;

		int32 iOptionCount;
		stream.Read(&iOptionCount, sizeof(int32));

		for (int32 j = 0; j < iOptionCount; j++)
		{
			FItemOption tOption;

			stream.Read(&tOption.Type, sizeof(ItemOptionType));
			stream.Read(&tOption.Option, sizeof(int32));

			DropItem.tOption.Add(tOption);
		}

		DropItemBox->AddDropItem(DropItem);

		//UInventoryWidget* pInventoryWidget = pGameWidget->GetInventoryWidget();
		//if (pInventoryWidget)
		//{
		//	pInventoryWidget->AddItem(ItemName, DropItem.Grade, DropItem.Type, DropItem.Buy, DropItem.Sell, DropItem.tOption);
		//	pGameWidget->GetChatWidget()->AddChatting(ItemName + TEXT("을  획득했습니다."));
		//}

	}
}

void ALectureGameGameModeBase::TradeRequest(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);
	char PlayerCharacterName[USERNAME_SIZE] = {};
	int32 RequestCharacterNumber = 0;

	stream.Read(&RequestCharacterNumber, sizeof(int32));
	stream.Read(PlayerCharacterName, USERNAME_SIZE);

	FString PlayerName = PlayerCharacterName;

	pGameWidget->GetTradeRequestWidget()->SetRequestDetails(PlayerName, RequestCharacterNumber);
	pGameWidget->OpenTradeRequestWidget();
}

void ALectureGameGameModeBase::TradeRequestAccept(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);
	char PlayerCharacterName[USERNAME_SIZE] = {};

	APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	UTradeWidget* pTrade = pGameWidget->GetTradeWidget();
	AUserCharacter* OtherPlayer = pGameWidget->GetPlayerInfoWidget()->GetOtherPlayer();

	pGameWidget->OpenTradeWidget();
	pGameWidget->OpenInventoryWidget();

	FString PlayerName = pPlayer->GetCharacterInfo().strName;
	pTrade->SetPlayerName(PlayerName);

	FString OtherPlayerName = OtherPlayer->GetUserName();
	pTrade->SetOtherPlayerName(OtherPlayerName);

	pTrade->SetOtherPlayerCharacterNumber(OtherPlayer->GetCharacterNumber());
}
void ALectureGameGameModeBase::TradeCancel(uint8* pPacket, int32 iLength)
{
	/*ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());*/
	//UChattingWidget* pChatting = pGameMode->GetGameWidget()->GetChatWidget();
	//FString string = TEXT("[ Notice ] : The Trade has been Canceled.");
	//pChatting->AddChatting(string);

	//pGameMode->GetGameWidget()->GetInventoryWidget()->SetTrade(false);

	//for (int32 i = 0; i < PlayerView->GetNumItems(); ++i)
	//{
	//	UStoreItemData* TradeItem = Cast<UStoreItemData>(PlayerView->GetItemAt(i));
	//	pGameMode->GetGameWidget()->GetInventoryWidget()->AddItem(TradeItem);
	//}

	pGameWidget->GetTradeWidget()->Cancel();
}
void ALectureGameGameModeBase::TradeAdd(uint8* pPacket, int32 iLength)
{
	UItemData* pItem = NewObject<UItemData>(this, UItemData::StaticClass());

	FItemInfoData Item;

	NetStream stream;
	stream.SetBuffer(pPacket);

	int32 Tempo;

	stream.Read(Item.strName, ITEMNAME_SIZE);

	stream.Read(&Tempo, sizeof(EItemGrade));
	Item.Grade = (EItemGrade)Tempo;

	stream.Read(&Tempo, sizeof(ItemType));
	Item.Type = (ItemType)Tempo;

	stream.Read(&Item.Buy, sizeof(int32));
	stream.Read(&Item.Sell, sizeof(int32));

	int32 iOptionCount;
	stream.Read(&iOptionCount, sizeof(int32));
	for (int32 i = 0; i < iOptionCount; ++i)
	{
		FItemOption Option;
		stream.Read(&Option, sizeof(Option));

		Item.tOption.Add(Option);
	}

	FString ItemName = Item.strName;

	UTradeWidget* pTrade = pGameWidget->GetTradeWidget();
	pTrade->AddItem(ItemName, Item.Grade, Item.Type, Item.Buy, Item.Sell, Item.tOption);
}
void ALectureGameGameModeBase::TradeDelete(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	FItemInfoData Item;
	int32 Index;

	stream.Read(&Index, sizeof(int32));

	UTradeWidget* pTrade = pGameWidget->GetTradeWidget();

	pTrade->DeleteItem(Index);
}
void ALectureGameGameModeBase::TradeReady(uint8* pPacket, int32 iLength)
{
	UTradeWidget* pTrade = pGameWidget->GetTradeWidget();

	if (pTrade->GetOtherUserReady() == true)
	{
		pTrade->SetOtherPlayerReady(false);
	}
	else
	{
		pTrade->SetOtherPlayerReady(true);
	}

	pTrade->TradeReady();
}
void ALectureGameGameModeBase::TradeSuccess(uint8* pPacket, int32 iLength)
{
	UInventoryWidget* pInventory = pGameWidget->GetInventoryWidget();
	UTradeWidget* pTrade = pGameWidget->GetTradeWidget();

	NetStream stream;
	stream.SetBuffer(pPacket);
	// ------------------------

	pTrade->TradeSuccess();
}

void ALectureGameGameModeBase::GroupCreate(uint8* pPacket, int32 iLength)
{

}
void ALectureGameGameModeBase::GroupRequest(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	int32 RequestCharacterNumber = 0;
	char RequestPlayerName[USERNAME_SIZE] = {};

	stream.Read(&RequestCharacterNumber, sizeof(int32));
	stream.Read(RequestPlayerName, USERNAME_SIZE);

	FString Name = RequestPlayerName;

	pGameWidget->GetGroupRequestWidget()->SetRequestCharacter(RequestCharacterNumber, Name);
	pGameWidget->OpenGroupRequestWidget();
}
void ALectureGameGameModeBase::GroupAccept(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	int32 iAcceptCharacterNumber = 0;
	char AcceptCharacterName[USERNAME_SIZE] = {};
	char AcceptCharacterJob[USERNAME_SIZE] = {};
	int32 iAcceptCharacterLevel = 0;

	stream.Read(&iAcceptCharacterNumber, sizeof(int32));
	stream.Read(AcceptCharacterName, USERNAME_SIZE);
	stream.Read(AcceptCharacterJob, USERNAME_SIZE);
	stream.Read(&iAcceptCharacterLevel, sizeof(int32));

	FString Name = AcceptCharacterName;
	FString Job = AcceptCharacterJob;

	PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("번호 : %d"), iAcceptCharacterNumber));
	PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("이름 : %s"), *Name));
	PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("직업 : %s"), *Job));

	UGroupWidget* pGroup = pGameWidget->GetGroupWidget();

	pGroup->AddPlayer(Name, Job, iAcceptCharacterLevel);

	// 마지막엔 내 정보를 입력해주자.
	APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (!pPlayer->IsGroupAdmin())
	{
		FString Myname = pPlayer->GetCharacterInfo().strName;
		FString Myjob = pPlayer->GetCharacterInfo().strJob;
		int Mylevel = pPlayer->GetCharacterInfo().iLevel;


		pGroup->AddPlayer(Myname, Myjob, Mylevel);
	}
}
void ALectureGameGameModeBase::GroupCancel(uint8* pPacket, int32 iLength)
{

}
void ALectureGameGameModeBase::GroupLeave(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);
	UGroupWidget* pGroup = pGameWidget->GetGroupWidget();

	pGroup->LeavePlayer();

}
void ALectureGameGameModeBase::GroupKick(uint8* pPacket, int32 iLength)
{

}
void ALectureGameGameModeBase::GroupSuccess(uint8* pPacket, int32 iLength)
{

}

void ALectureGameGameModeBase::MonsterAttackDamage(uint8* pPacket, int32 iLength)
{
}

void ALectureGameGameModeBase::MainPortal(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	bool bCreate;

	stream.Read(&bCreate, sizeof(bool));

	if (bCreate)
	{
		PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("Main포탈을 이용합니다")));

		// OpenLevel을 하면 기존 레벨에 있던 것들은 다 사라지게된다.
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("BossZone"));
	}
}

void ALectureGameGameModeBase::SevarogBossRoomCreate(uint8* pPacket, int32 iLength)
{
	NetStream stream;
	stream.SetBuffer(pPacket);

	PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("보스방에 입장을 합니다?")));
}

void ALectureGameGameModeBase::AlivePacket()
{
	PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("Player Alive.....")));

	NetStream stream;
	uint8	packet[PACKET_LENGTH] = {};
	stream.SetBuffer(packet);
	PacketSession* pSession = GameServerManager::GetInst()->GetSession();
	pSession->Write(GP_ALIVE, stream.GetLength(), packet);
}
