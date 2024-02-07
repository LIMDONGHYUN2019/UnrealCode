// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Monster.h"
#include "Golem.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API AGolem : public AMonster
{
	GENERATED_BODY()
	

public:
	AGolem();

protected:
	virtual void BeginPlay()	override;

	virtual void Tick(float DeltaTime)	override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)	override;

public:
	virtual void SaveInfo(FArchive* pArchive);
};
