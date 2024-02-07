// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Monster.h"
#include "Troll.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API ATroll : public AMonster
{
	GENERATED_BODY()
	

public:
	ATroll();


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SaveInfo(FArchive* pArchive);

};
