// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class LECTUREGAME_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* Projectile;

	float	m_Dist;
	FVector	m_PrevPos;
	AActor* m_OwnerActor;

public:
	void SetOwnerActor(AActor* OwnerActor)
	{
		m_OwnerActor = OwnerActor;
	}

	void SetDist(float Dist)
	{
		m_Dist = Dist;
	}

	void SetSpeed(float Speed)
	{
		Projectile->InitialSpeed = Speed;
	}

	void SetDir(const FVector& Dir)
	{
		Projectile->Velocity = Dir;
		//InitialSpeed가 0일떄 Velocity값으로 처리함
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void StopProjectile(const FHitResult& result);
};
