// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "DamageText.generated.h"

UCLASS()
class LECTUREGAME_API ADamageText : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageText();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* DamageTextWidgetCom;

	class UDamageTextWidget* DamageTextWidget;
	float	m_Damage;

	bool	m_SetDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetDamage(int32 Damage);
};
