// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCameraManager.h"
#include "../GameMode/SelectGameMode.h"

// Sets default values
ASelectCameraManager::ASelectCameraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChangeCameraIndex	= 0;
}

// Called when the game starts or when spawned
void ASelectCameraManager::BeginPlay()
{
	Super::BeginPlay();
	
	ASelectGameMode* pGameMode = Cast<ASelectGameMode>(GetWorld()->GetAuthGameMode());
	// 게임 실행중 게임모드의 포인터를 가져올떄

	if (pGameMode)
		pGameMode->SetCameraManager(this);
}

void ASelectCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASelectCameraManager::ChangeCreateCharacter()// 정 진행
{
	ChangeCameraIndex = 0;

	APlayerController*	Controller = GetWorld()->GetFirstPlayerController();

	Controller->SetViewTargetWithBlend(CameraArray[0], 0.5f,
	EViewTargetBlendFunction::VTBlend_Linear, 0.f);

	// 블렌드가 종료될때 호출될 함수를 타이머를 이용하여 지정된 시간 뒤에 호출될 함수를 설정한다.
	GetWorldTimerManager().SetTimer(BlendTimerHandle, this, 
		&ASelectCameraManager::CameraBlendEnd, 0.5f, true);
}

void ASelectCameraManager::ChangeSelectCharacter() // 반대로 진행
{
	ChangeCameraIndex	= CameraArray.Num() - 2;

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	Controller->SetViewTargetWithBlend(CameraArray[ChangeCameraIndex], 0.5f,
		EViewTargetBlendFunction::VTBlend_Linear, 0.f);

	// 블렌드가 종료될때 호출될 함수를 타이머를 이용하여 지정된 시간 뒤에 호출될 함수를 설정한다.
	GetWorldTimerManager().SetTimer(BlendTimerHandle, this,
		&ASelectCameraManager::ReverseCameraBlendEnd, 0.5f, true);
}

// 그런데 여기서
void ASelectCameraManager::CameraBlendEnd()
{
	GetWorldTimerManager().ClearTimer(BlendTimerHandle);

	++ChangeCameraIndex;

	if(ChangeCameraIndex >= CameraArray.Num())
	{
		EndEvent();
		ASelectGameMode* pGameMode = Cast<ASelectGameMode>(GetWorld()->GetAuthGameMode());
		pGameMode->ChangeUMG(true);
	}

	else
	{
		APlayerController* Controller = GetWorld()->GetFirstPlayerController();

		Controller->SetViewTargetWithBlend(CameraArray[ChangeCameraIndex], 0.5f,
			EViewTargetBlendFunction::VTBlend_Linear, 0.f);

		// 블렌드가 종료될때 호출될 함수를 타이머를 이용하여 지정된 시간 뒤에 호출될 함수를 설정한다.
		GetWorldTimerManager().SetTimer(BlendTimerHandle, this,
			&ASelectCameraManager::CameraBlendEnd, 0.5f, true);
	}

}

void ASelectCameraManager::EndEvent()
{
	ChangeCameraIndex	= 0;
}

void ASelectCameraManager::ReverseCameraBlendEnd()
{
	GetWorldTimerManager().ClearTimer(BlendTimerHandle);

	--ChangeCameraIndex;

	if (ChangeCameraIndex < 0)
	{
		ReverseEndEvent();
		ASelectGameMode* pGameMode = Cast<ASelectGameMode>(GetWorld()->GetAuthGameMode());
		pGameMode->ChangeUMG(false);

		APlayerController* Controller = GetWorld()->GetFirstPlayerController();

		Controller->SetViewTargetWithBlend(Controller->GetPawn(), 0.2f,
			EViewTargetBlendFunction::VTBlend_Linear, 0.f);
	}

	else
	{
		APlayerController* Controller = GetWorld()->GetFirstPlayerController();

		Controller->SetViewTargetWithBlend(CameraArray[ChangeCameraIndex], 0.5f,
			EViewTargetBlendFunction::VTBlend_Linear, 0.f);

		// 블렌드가 종료될때 호출될 함수를 타이머를 이용하여 지정된 시간 뒤에 호출될 함수를 설정한다.
		GetWorldTimerManager().SetTimer(BlendTimerHandle, this,
			&ASelectCameraManager::ReverseCameraBlendEnd, 0.5f, true);
	}
}

void ASelectCameraManager::ReverseEndEvent()
{
	ChangeCameraIndex = 0;
}
