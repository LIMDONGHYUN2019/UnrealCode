// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectileBase.h"
#include "particles/PxParticleSystem.h"
#include "ProjectilePaticle.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API AProjectilePaticle : public AProjectileBase
{
	GENERATED_BODY()

public:
	AProjectilePaticle();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* Particle;

public:
	void SetNiagaraSystem(UParticleSystem* Effect)
	{

	}

	void SetPaticleSystem(const FString& Path)
	{
		UParticleSystem* Effect = LoadObject<UParticleSystem>(GetWorld(), *Path);
		Particle->SetTemplate(Effect);
		Particle->bAutoActivate = false;

		//static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(*Path);
		//if (P_CHESTOPEN.Succeeded())
		//{
		//	Particle->SetTemplate(P_CHESTOPEN.Object);
		//	Particle->bAutoActivate = false;
		//}
	}
	UFUNCTION()
	void PaticleDestroy(UParticleSystemComponent* PSystem)
	{
		Destroy();
	}

	void PlayPaticle()
	{
		Particle->Activate(true);
		Particle->OnSystemFinished.AddDynamic(this, &AProjectilePaticle::PaticleDestroy);
	}

	void SetPaticleScale(FVector Scale)
	{
		Particle->SetRelativeScale3D(Scale);
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
