// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectPlayerController.h"
#include "SelectCharacter.h"
#include "CreateCharacterWidget.h"
#include "SelectStatusWidget.h"
#include "../GameMode/SelectGameMode.h"

ASelectPlayerController::ASelectPlayerController()
{
	bLButtonDown = false;
	bRButtonDown = false;

	bShowMouseCursor = true;

	bEnableClickEvents = false;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;
}

void ASelectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI	inputMode;
	SetInputMode(inputMode);

	InputPitchScale = 0.f;
	InputYawScale = 0.f;
	InputRollScale = 0.f;
}

void ASelectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("LButton"), EInputEvent::IE_Pressed, this, &ASelectPlayerController::LButtonClick);
	InputComponent->BindAction(TEXT("LButton"), EInputEvent::IE_Released, this, &ASelectPlayerController::LButtonRelease);

	InputComponent->BindAction(TEXT("RButton"), EInputEvent::IE_Pressed, this, &ASelectPlayerController::RButtonClick);
	InputComponent->BindAction(TEXT("RButton"), EInputEvent::IE_Released, this, &ASelectPlayerController::RButtonRelease);
}

void ASelectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bLButtonDown)
	{
		Picking();
	}
}

void ASelectPlayerController::LButtonClick()
{
	bLButtonDown = true;
}

void ASelectPlayerController::LButtonRelease()
{
	bLButtonDown = false;
}

void ASelectPlayerController::RButtonClick()
{
	bRButtonDown = true;
}

void ASelectPlayerController::RButtonRelease()
{
	bRButtonDown = false;
}

void ASelectPlayerController::Clear()
{
}

void ASelectPlayerController::Picking()
{
	FHitResult	result;
	bool	bHit = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, result);

	if (bHit)
	{
		// Cast를 이용해서 형변환을 했을때 IsValid를 해서 true가 나온다면 캐릭터를 선택했다는 것이 된다.
		pSelectCharacter = Cast<ASelectCharacter>(result.GetActor());

		ASelectGameMode* pGameMode = Cast<ASelectGameMode>(GetWorld()->GetAuthGameMode());
		UCreateCharacterWidget* pCharWidget = pGameMode->GetCreateCharacterWidget();
		USelectStatusWidget* pStatusWidget = pCharWidget->GetStatusWidget();

		if (IsValid(pSelectCharacter))
		{
			pSelectCharacter->CharacterClick(true);
			pStatusWidget->SetVisibility(ESlateVisibility::Visible);
			pStatusWidget->SetJob(pSelectCharacter->GetCharJob());
			pStatusWidget->SetAttack(pSelectCharacter->GetAttack());
			pStatusWidget->SetArmor(pSelectCharacter->GetArmor());
			pStatusWidget->SetAttackSpeed(pSelectCharacter->GetAttackSpeed());
			pStatusWidget->SetMoveSpeed(pSelectCharacter->GetMoveSpeed());
			pStatusWidget->SetHP(pSelectCharacter->GetHP());
			pStatusWidget->SetMP(pSelectCharacter->GetMP());
			pStatusWidget->SetLevel(pSelectCharacter->GetLevel());

			pCharWidget->EnableInputName(true);
		}

		else
		{
			pSelectCharacter = nullptr;
			pStatusWidget->SetVisibility(ESlateVisibility::Collapsed);
			pCharWidget->EnableInputName(false);
		}
	}

	else
	{
		ASelectGameMode* pGameMode = Cast<ASelectGameMode>(GetWorld()->GetAuthGameMode());

		UCreateCharacterWidget* pCharWidget = pGameMode->GetCreateCharacterWidget();

		USelectStatusWidget* pStatusWidget = pCharWidget->GetStatusWidget();

		pSelectCharacter = nullptr;
		pStatusWidget->SetVisibility(ESlateVisibility::Collapsed);

		pCharWidget->EnableInputName(false);
	}
}
