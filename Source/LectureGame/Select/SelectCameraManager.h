// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SelectCamera.h"
#include "GameFramework/Actor.h"
#include "SelectCameraManager.generated.h"

UCLASS()
class LECTUREGAME_API ASelectCameraManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectCameraManager();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ASelectCamera*>	CameraArray;

	int32		ChangeCameraIndex;
	FTimerHandle	BlendTimerHandle;
	FTimerHandle	ChangeTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void ChangeCreateCharacter();
	void ChangeSelectCharacter();

private:
	void CameraBlendEnd();
	void EndEvent();

	void ReverseCameraBlendEnd();
	void ReverseEndEvent();
};
