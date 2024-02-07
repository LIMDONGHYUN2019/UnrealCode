// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BossRoomGameMode.h"
#include "BossRoomController.h"
#include "NPC.h"
#include "UI/GameWidget.h"
#include "NetStream.h"
#include "NetSession.h"
#include "PacketSession.h"
#include "GameServerManager.h"
#include "Player/PlayerCharacter.h"
#include "OtherUser/UserCharacter.h"
#include "UI/Inventory/InventoryWidget.h"
#include "UI/PlayerUI/PlayerStatusWidget.h"
#include "UI/PlayerUI/TradeWidget.h"
#include "UI/PlayerUI/PlayerInfoWidget.h"
#include "UI/Group/GroupWidget.h"
#include "UI/Store/StoreWidget.h"
#include "UI/Skill/SkillWidget.h"
#include "UI/SevarogRoomCreateWidget.h"
#include "UI/PlayerUI/PlayerStatusWidget.h"
#include "UI/Quest/QuestWidget.h"
#include "UI/InformationWidget.h"

ABossRoomController::ABossRoomController()
{
	bLButtonDown = false;
	bRButtonDown = false;

	bShowMouseCursor = true;

	bEnableClickEvents = false;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;
}

void ABossRoomController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI	inputMode;
	SetInputMode(inputMode);
	bControl = true;

	InputPitchScale = 0.f;
	//InputYawScale = 0.f;
	InputRollScale = 0.f;
}

void ABossRoomController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 프로젝트세팅에서 키 / 마우스 입력시
	InputComponent->BindAction(TEXT("LButton"), EInputEvent::IE_Pressed, this, &ABossRoomController::LButtonClick);
	InputComponent->BindAction(TEXT("LButton"), EInputEvent::IE_Released, this, &ABossRoomController::LButtonRelease);

	InputComponent->BindAction(TEXT("RButton"), EInputEvent::IE_Pressed, this, &ABossRoomController::RButtonClick);
	InputComponent->BindAction(TEXT("RButton"), EInputEvent::IE_Released, this, &ABossRoomController::RButtonRelease);

	InputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &ABossRoomController::Inventory);
	InputComponent->BindAction(TEXT("Status"), EInputEvent::IE_Pressed, this, &ABossRoomController::PlayerStatus);
	InputComponent->BindAction(TEXT("Esc"), EInputEvent::IE_Pressed, this, &ABossRoomController::Esc);
	InputComponent->BindAction(TEXT("Control"), EInputEvent::IE_Pressed, this, &ABossRoomController::Control);
	InputComponent->BindAction(TEXT("Group"), EInputEvent::IE_Pressed, this, &ABossRoomController::Group);
	InputComponent->BindAction(TEXT("Skill"), EInputEvent::IE_Pressed, this, &ABossRoomController::Skill);
	InputComponent->BindAction(TEXT("Quest"), EInputEvent::IE_Pressed, this, &ABossRoomController::Quest);
	InputComponent->BindAction(TEXT("Information"), EInputEvent::IE_Pressed, this, &ABossRoomController::Information);
	InputComponent->BindAxis(TEXT("Turn"), this, &ABossRoomController::PlayerTurn);
	InputComponent->BindAxis(TEXT("LookUp"), this, &ABossRoomController::PlayerLookUp);
}

void ABossRoomController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bLButtonDown)
	{
		Picking();
	}
	else if (bRButtonDown)
	{
		CharacterInfo();
	}
}

void ABossRoomController::PlayerTurn(float Scale)
{
	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetPawn());
	//APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();

	if (pPlayer)
	{
		pPlayer->Turn(Scale);
	}
}
void ABossRoomController::PlayerLookUp(float Scale)
{
	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetPawn());
	//APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();

	if (pPlayer)
	{
		pPlayer->LookUp(Scale);
	}

}

void ABossRoomController::Control()
{
	if (bControl)
	{
		FInputModeGameOnly	inputMode;
		SetInputMode(inputMode);
		bShowMouseCursor = false;
		bControl = false;
	}
	else
	{
		FInputModeGameAndUI	inputMode;
		SetInputMode(inputMode);
		bShowMouseCursor = true;
		bControl = true;
	}

}

void ABossRoomController::LButtonClick()
{
	bLButtonDown = true;
}

void ABossRoomController::LButtonRelease()
{
	bLButtonDown = false;
}

void ABossRoomController::RButtonClick()
{
	bRButtonDown = true;
}

void ABossRoomController::RButtonRelease()
{
	bRButtonDown = false;
}

void ABossRoomController::Group()
{
	ABossRoomGameMode* pGame = Cast<ABossRoomGameMode>(GetWorld()->GetAuthGameMode());
	UGroupWidget* pGroupWidget = pGame->GetGameWidget()->GetGroupWidget();

	if (pGroupWidget)
	{
		if (pGroupWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			pGroupWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			pGroupWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABossRoomController::Skill()
{
	ABossRoomGameMode* pGame = Cast<ABossRoomGameMode>(GetWorld()->GetAuthGameMode());
	USkillWidget* pSkillWidget = pGame->GetGameWidget()->GetSkillWidget();

	if (pSkillWidget)
	{
		if (pSkillWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			pSkillWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			pSkillWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABossRoomController::Quest()
{
	ABossRoomGameMode* pGame = Cast<ABossRoomGameMode>(GetWorld()->GetAuthGameMode());
	UQuestWidget* pQuestWidget = pGame->GetGameWidget()->GetQuestWidget();

	if (pQuestWidget)
	{
		if (pQuestWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			pQuestWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			pQuestWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABossRoomController::Information()
{
	ABossRoomGameMode* pGame = Cast<ABossRoomGameMode>(GetWorld()->GetAuthGameMode());
	UInformationWidget* pInformationWidget = pGame->GetGameWidget()->GetInformationWidget();

	if (pInformationWidget)
	{
		if (pInformationWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			pInformationWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			pInformationWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABossRoomController::Inventory()
{
	ABossRoomGameMode* pGameMode = Cast<ABossRoomGameMode>(GetWorld()->GetAuthGameMode());
	UInventoryWidget* pInventoryWidget = pGameMode->GetGameWidget()->GetInventoryWidget();

	if (pInventoryWidget)
	{
		if (pInventoryWidget->IsOpen() == false)
			pInventoryWidget->SetOpen(true);

		if (pInventoryWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			pInventoryWidget->CloseInventory();
		}
		else
		{
			pInventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABossRoomController::PlayerStatus()
{
	ABossRoomGameMode* pGameMode = Cast<ABossRoomGameMode>(GetWorld()->GetAuthGameMode());
	UPlayerStatusWidget* pPlayerStatusWidget = pGameMode->GetGameWidget()->GetPlayerStatusWidget();

	if (pPlayerStatusWidget)
	{
		if (pPlayerStatusWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			pPlayerStatusWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			pPlayerStatusWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

}

void ABossRoomController::Esc()
{
	ABossRoomGameMode* pGameMode = Cast<ABossRoomGameMode>(GetWorld()->GetAuthGameMode());
	if (pGameMode)
	{
		UStoreWidget* pStore = pGameMode->GetGameWidget()->GetStoreWidget();
		UInventoryWidget* pInventory = pGameMode->GetGameWidget()->GetInventoryWidget();
		UPlayerStatusWidget* pPlayerStatus = pGameMode->GetGameWidget()->GetPlayerStatusWidget();
		UGroupWidget* pGroup = pGameMode->GetGameWidget()->GetGroupWidget();
		USkillWidget* pSkill = pGameMode->GetGameWidget()->GetSkillWidget();
		UInformationWidget* pInformationWidget = pGameMode->GetGameWidget()->GetInformationWidget();
		UQuestWidget* pQuestWidget = pGameMode->GetGameWidget()->GetQuestWidget();

		if (pPlayerStatus->GetVisibility() == ESlateVisibility::Visible)
		{
			pPlayerStatus->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (pStore->GetVisibility() == ESlateVisibility::Visible)
		{
			pStore->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (pInventory->GetVisibility() == ESlateVisibility::Visible)
		{
			pInventory->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (pSkill->GetVisibility() == ESlateVisibility::Visible)
		{
			pSkill->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (pInformationWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			pInformationWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (pQuestWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			pQuestWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ABossRoomController::Picking()
{
	FHitResult	result;
	bool	bHit = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, result);
	ABossRoomGameMode* pGameMode = Cast<ABossRoomGameMode>(GetWorld()->GetAuthGameMode());

	// 선택시 무엇을 선택을 했는지
	if (bHit)
	{
		ANPC* pNPC = Cast<ANPC>(result.GetActor());

		if (IsValid(pNPC))
		{
			// NPC와 일정 거리 안에 있는지 판단한다.
			APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetPawn());

			FVector fNpcPos = pNPC->GetActorLocation();
			FVector fPlayerPos = pPlayer->GetActorLocation();
			FVector fDir = fPlayerPos - fNpcPos;

			float fDist = fDir.Size();

			if (fDist > 5000)
			{
				return;
			}

			switch (pNPC->GetNPCType())
			{
			case ENPCType::Store:
			{
				if (pGameMode)
				{
					UStoreWidget* Store = pGameMode->GetGameWidget()->GetStoreWidget();
					UGameWidget* pGameWidget = pGameMode->GetGameWidget();
					if (!Store->IsFirstOpen())
					{
						// 서버로 상점 목록 패킷을 보내주도록 한다.
						uint8	strPacket[PACKET_LENGTH] = {};
						PacketSession* Session = GameServerManager::GetInst()->GetSession();
						Session->Write(GP_STORELIST, 0, strPacket);
						Store->IsFirstClose();
						Store->Open();
						pGameWidget->OpenInventoryWidget();
					}
					if (!Store->IsOpen())
					{
						pGameWidget->OpenStoreWidget();
						pGameWidget->OpenInventoryWidget();
						Store->Open();
					}
				}
			}
			break;

			case ENPCType::SevarogRoomCreate:
			{
				if (pGameMode)
				{
					USevarogRoomCreateWidget* BossRoomWidget = pGameMode->GetGameWidget()->GetSevarogRoomCreateWidget();

					if (BossRoomWidget)
					{
						BossRoomWidget->SetVisibility(ESlateVisibility::Visible);
					}
				}
			}
			break;
			}
		}
		else
		{
			APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetPawn());
			if (pPlayer)
			{
				pPlayer->NormalAttack();
			}
		}
	}

	else
	{
		APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetPawn());

		if (pPlayer)
		{
			pPlayer->NormalAttack();
		}
	}
}

void ABossRoomController::CharacterInfo()
{
	FHitResult result;
	bool bHit = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, result);
	ABossRoomGameMode* pGameMode = Cast<ABossRoomGameMode>(GetWorld()->GetAuthGameMode());

	if (bHit)
	{
		AUserCharacter* pOtherPlayer = Cast<AUserCharacter>(result.GetActor());
		if (IsValid(pOtherPlayer))
		{
			int32 OtherPlayerCharacterNumber = pOtherPlayer->GetCharacterNumber();

			pGameMode->GetGameWidget()->OpenPlayerInfoWidget(pOtherPlayer);

			//pGameMode->GetGameWidget()->GetPlayerInfoWidget()->SetLocationFromMouse();
		}
		else
		{
			pGameMode->GetGameWidget()->ClosePlayerInfoWidget();
		}
	}
	else
	{
		pGameMode->GetGameWidget()->ClosePlayerInfoWidget();
	}
}


