// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Monster.h"
#include "FinalBoss.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API AFinalBoss : public AMonster
{
	GENERATED_BODY()

public:
	AFinalBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void SaveInfo(FArchive* pArchive);
	
};
