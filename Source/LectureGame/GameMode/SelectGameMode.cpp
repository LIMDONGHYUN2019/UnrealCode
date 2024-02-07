// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectGameMode.h"
#include "../MainGameInstance.h"
#include "../NetworkManager.h"
#include "../RecvQueue.h"
#include "../LoginServerManager.h"
#include "../NetStream.h"
#include "../GameServerManager.h"
#include "../ChattingServerManager.h"
#include "../Select/SelectPlayerController.h"
#include "../Select/CreateCharacterWidget.h"
#include "../Select/SelectPawn.h"

#include "../Select/SelectCharacter.h"
#include "../Select/CreateCharacterWidget.h"
#include "../Select/SelectStatusWidget.h"

ASelectGameMode::ASelectGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = ASelectPlayerController::StaticClass();
	DefaultPawnClass = ASelectPawn::StaticClass();

	// Login Widget 정보를 읽어온다. 정보를 로딩하는것은 2가지가 있다.
	// Object, Class 2가지이다. Object는 실제 생성된 객체를 얻어오는 방식이고
	// Class는 이게 어떤 클래스 타입인지를 얻어오는 기능이다.
	// FObjectFinder는 애셋을 로딩해서 객체를 만들어내는 방식이고
	// FClassFinder는 애셋을 로딩해서 어떤 클래스 타입인지를 만들어내는 방식이다.
	static ConstructorHelpers::FClassFinder<UUserWidget>	SelectWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/UI_Select.UI_Select_C'"));
	if (SelectWidgetAsset.Succeeded())
		SelectWidgetClass = SelectWidgetAsset.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget>	CreateCharWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/UI_CreateChar.UI_CreateChar_C'"));
	if (CreateCharWidgetAsset.Succeeded())
		CreateChartWidgetClass = CreateCharWidgetAsset.Class;

	static ConstructorHelpers::FClassFinder<ASelectCharacter>	KnightAsset(TEXT("Blueprint'/Game/SelectLevel/BPSelectGreyStone.BPSelectGreyStone_C'"));
	if (KnightAsset.Succeeded())
		KnightCharClass = KnightAsset.Class;

	static ConstructorHelpers::FClassFinder<ASelectCharacter>	ArcherAsset(TEXT("Blueprint'/Game/SelectLevel/BPSelectTwinblast.BPSelectTwinblast_C'"));
	if (ArcherAsset.Succeeded())
		ArcherCharClass = ArcherAsset.Class;

	static ConstructorHelpers::FClassFinder<ASelectCharacter>	MagicionAsset(TEXT("Blueprint'/Game/SelectLevel/BPSelectSerath.BPSelectSerath_C'"));
	if (MagicionAsset.Succeeded())
		MagicionCharClass = MagicionAsset.Class;

	UseCharacterArray.SetNum(3);
}

void ASelectGameMode::StartPlay()
{
	Super::StartPlay();
}

void ASelectGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (SelectWidgetClass)
	{
		SelectWidget = Cast<USelectWidget>(CreateWidget(GetWorld(), SelectWidgetClass));

		if (SelectWidget)
		{
			SelectWidget->AddToViewport();
		}
	}

	if (SelectWidgetClass)
	{
		CreateCharWidget = Cast<UCreateCharacterWidget>(CreateWidget(GetWorld(), CreateChartWidgetClass));

		if (CreateCharWidget)
		{
			CreateCharWidget->AddToViewport();
			CreateCharWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ASelectGameMode::InitGameState()
{
	Super::InitGameState();
}

void ASelectGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RecvQueue* pQueue = GameServerManager::GetInst()->GetRecvQueue();

	int32	iProtocol = 0, iLength = 0;
	uint8	packet[PACKET_LENGTH] = {};

	if (pQueue->pop(iProtocol, iLength, packet))
	{
		switch (iProtocol)
		{
		case GP_CONNECT:
			Connect(packet, iLength);
			break;
		case GP_DISCONNECT:
			DisConnect(packet, iLength);
			break;
		case GP_CONNECT_FAIL:
			ConnectFail(packet, iLength);
			break;
		case GP_CREATECHARACTER:
			CharacterCreate(packet, iLength);
			break;
		case GP_SELECT_CHARACTERINFO: // 보내는곳 없음...
			SelectCharacterInfo(packet, iLength);
			break;
		case GP_CREATE_CHARACTERINFO:
			CreateCharacterInfo(packet, iLength);
			break;
		case GP_DELETE_CHARACTERINFO:
			DeleteCharacterInfo(packet, iLength);
			break;
		case GP_GAMESTART:
			GameStart(packet, iLength);
			break;
		}
	}
}

void ASelectGameMode::Connect(uint8* pPacket, int32 iLength)
{
	NetStream	stream;
	stream.SetBuffer(pPacket);
	int32	iCharacterCount = -1;
	stream.Read(&iCharacterCount, sizeof(int32));

	for (int i = 0; i < iCharacterCount; ++i)
	{
		_tagCharacterInfoTable	tCharInfo = {}; // DB테이블정보까지있는 캐릭터정보구조체
		int32	iJob;
		int32 iSelectPos = 0;

		stream.Read(&iSelectPos, sizeof(int32));

		stream.Read(&iJob, sizeof(int32));
		stream.Read(&tCharInfo, sizeof(_tagCharacterInfoTable));

		FString	strName(tCharInfo.strName);
		ConvertString(strName, tCharInfo.strName);
		FString	strJobName(tCharInfo.strJob);
		AActor*	pSpawnPoint = CharacterSpawnPoint[iSelectPos];

		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride	= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ASelectCharacter* pUserCharacter	= nullptr;
		if (iJob == 0)
		{
			pUserCharacter = GetWorld()->SpawnActor<ASelectCharacter>(KnightCharClass,
				pSpawnPoint->GetActorLocation(), pSpawnPoint->GetActorRotation(),
				param);
		}
		else if (iJob == 1)
		{
			pUserCharacter = GetWorld()->SpawnActor<ASelectCharacter>(ArcherCharClass,
				pSpawnPoint->GetActorLocation(), pSpawnPoint->GetActorRotation(),
				param);
		}
		else if(iJob == 2)
		{
			pUserCharacter = GetWorld()->SpawnActor<ASelectCharacter>(MagicionCharClass,
				pSpawnPoint->GetActorLocation(), pSpawnPoint->GetActorRotation(),
				param);
		}

		//_tagCharacterInfo	tInfo = {};
		//FMemory::Memcpy(&tInfo, ((char*)&tCharInfo) + 12, sizeof(_tagCharacterInfo));
		//pUserCharacter->SetCharacterInfo(&tInfo);
		pUserCharacter->SetCharacterInfo(&tCharInfo);
		pUserCharacter->SetCharacterNumber(tCharInfo.iCharacterNumber);
		pUserCharacter->SetUserNumber(tCharInfo.iUserNumber);

		//UseAddSelectCharacter(pUserCharacter);
		UseCharacterArray[i] = pUserCharacter;
	}
}

void ASelectGameMode::DisConnect(uint8* pPacket, int32 iLength)
{
}

void ASelectGameMode::ConnectFail(uint8* pPacket, int32 iLength)
{
}

void ASelectGameMode::CharacterCreate(uint8* pPacket, int32 iLength)
{
	NetStream	stream;
	stream.SetBuffer(pPacket);
	bool	bCreate = false;
	stream.Read(&bCreate, 1);

	if (bCreate)
	{
		CreateCharWidget->PopUpCreateSuccessMsgBox();

		_tagCharacterInfoTable	tCharInfo = {}; // DB테이블정보까지있는 캐릭터정보구조체
		int32	iJob;
		stream.Read(&iJob, sizeof(int32));
		stream.Read(&tCharInfo, sizeof(_tagCharacterInfoTable));
		FString	strJobName(tCharInfo.strJob);

		int iIndex = 0;

		for (int i = 0; i < 3; ++i)
		{
			if (UseCharacterArray[i] == nullptr)
			{
				iIndex = i;
				break;
			}
		}

		AActor* pSpawnPoint = CharacterSpawnPoint[iIndex];

		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ASelectCharacter* pUserCharacter = nullptr;
		if (iJob == 0)
		{
			pUserCharacter = GetWorld()->SpawnActor<ASelectCharacter>(KnightCharClass,
				pSpawnPoint->GetActorLocation(), pSpawnPoint->GetActorRotation(),
				param);
		}
		else if (iJob == 1)
		{
			pUserCharacter = GetWorld()->SpawnActor<ASelectCharacter>(ArcherCharClass,
				pSpawnPoint->GetActorLocation(), pSpawnPoint->GetActorRotation(),
				param);
		}
		else if (iJob == 2)
		{
			pUserCharacter = GetWorld()->SpawnActor<ASelectCharacter>(MagicionCharClass,
				pSpawnPoint->GetActorLocation(), pSpawnPoint->GetActorRotation(),
				param);
		}

		pUserCharacter->SetCharacterInfo(&tCharInfo);
		pUserCharacter->SetCharacterNumber(tCharInfo.iCharacterNumber);
		pUserCharacter->SetUserNumber(tCharInfo.iUserNumber);

		//UseAddSelectCharacter(pUserCharacter);
		UseCharacterArray[iIndex] = pUserCharacter;

	}

	else
	{
		CreateCharWidget->PopUpCreateFailMsgBox();
	}
	  
	ASelectGameMode* pGameMode = Cast<ASelectGameMode>(GetWorld()->GetAuthGameMode());
	UCreateCharacterWidget* pCharWidget = pGameMode->GetCreateCharacterWidget();
	USelectStatusWidget* pStatusWidget = pCharWidget->GetStatusWidget();

	pStatusWidget->SetVisibility(ESlateVisibility::Collapsed);
	pCharWidget->EnableInputName(false);
}

void ASelectGameMode::SelectCharacterInfo(uint8* pPacket, int32 iLength)
{
	NetStream	stream;

	stream.SetBuffer(pPacket);

	// 생성된 캐릭터 수를 얻어온다.
	int32	iCharacterCount = -1;
	stream.Read(&iCharacterCount, sizeof(int32));

	for (int i = 0; i < iCharacterCount; ++i)
	{
		_tagCharacterInfoTable	tCharInfo = {};
		stream.Read(&tCharInfo, sizeof(_tagCharacterInfoTable));

		FString	strName(tCharInfo.strName);
		LOG(TEXT("Select Character Count : %d"), iCharacterCount);
		LOG(TEXT("Select Character Number : %d"), tCharInfo.iCharacterNumber);
		LOG(TEXT("Select Character Name : %s"), *strName);
	}
}

void ASelectGameMode::ChangeUMG(bool bCreate)
{
	if (bCreate)
	{
		SelectWidget->SetVisibility(ESlateVisibility::Collapsed);
		CreateCharWidget->SetVisibility(ESlateVisibility::Visible);
	}

	else
	{
		SelectWidget->SetVisibility(ESlateVisibility::Visible);
		CreateCharWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

// 캐릭터 생성클릭후 나오는 캐릭터 스폰 + 캐릭터 정보
void ASelectGameMode::CreateCharacterInfo(uint8* pPacket, int32 iLength)
{
	NetStream	stream;
	stream.SetBuffer(pPacket);
	int32	iCharacterCount = -1;
	stream.Read(&iCharacterCount, sizeof(int32));

	for (int32 i = 0; i < iCharacterCount; ++i)
	{
		_tagCharacterInfo* pInfo = new _tagCharacterInfo;
		stream.Read(pInfo, sizeof(_tagCharacterInfo));
		CharInfoArray.Add(pInfo);
		CharacterArray[i]->SetCharacterInfo(pInfo);
	} 
}

void ASelectGameMode::DeleteCharacterInfo(uint8* pPacket, int32 iLength)
{
	ASelectCharacter* pUserCharacter = nullptr;

	NetStream	stream;
	stream.SetBuffer(pPacket);
	int32	iCharacterNumber = -1;
	stream.Read(&iCharacterNumber, sizeof(int32));
	int32	iCharacterCount = -1;
	iCharacterCount = UseCharacterArray.Num();

	for (int32 i = 0; i < iCharacterCount; i++)
	{
		if (UseCharacterArray[i])
		{
			if (UseCharacterArray[i]->GetCharacterNumber() == iCharacterNumber)
			{
				pUserCharacter = UseCharacterArray[i];

				GetWorld()->DestroyActor(UseCharacterArray[i], false, true);
				// 언리얼의 엑터나 컴포넌트는 Destroy를 호출하여 제거를 하게 되면 씬 상에서는 제거된 것으로 보이지만 메모리에서는 Pending Kill 상태가 되어 존재하며
				// 다음 GC에 실행시 메모리에서 해제됩니다.
				// 에디터의 프로젝트 기본 설정상으로 1분단위로 Pending Kill 상태인 오브젝트를 제거하며 사용자가 설정을 변경할 수 있습니다.
				//  nullptr과 IsValid() 함수를 사용하게 되는데 Pending Kill 상태까지 검사하기 위해서는 IsValid() 함수를 사용해야 합니다.

				UseCharacterArray[i] = nullptr;
				//UseCharacterArray.RemoveAt(i);
				break;
			}
		}
	}
}

void ASelectGameMode::GameStart(uint8* pPacket, int32 iLength)
{
	GameServerManager::GetInst()->GetSession();

	NetStream	stream;
	stream.SetBuffer(pPacket);
	int32	iLevelType = -1;
	stream.Read(&iLevelType, sizeof(int32));

	ChattingServerManager::GetInst()->Init(CHATTINGSERVER_IP, CHATTINGSERVER_PORT, NetworkManager::GetInst()->GetUserNumber(),
		NetworkManager::GetInst()->GetID());

	switch ((ELevel)iLevelType)
	{
	case ELevel::Kingdom:
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main"));
		break;
	case ELevel::Desert:
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Desert"));
		break;
	case ELevel::SevarogBossRoom:
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("BossZone"));
		break;
	}
}
