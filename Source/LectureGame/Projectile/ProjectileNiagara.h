// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectileBase.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjectileNiagara.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API AProjectileNiagara : public AProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileNiagara();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* Particle;

public:
	void SetNiagaraSystem(UNiagaraSystem* Effect)
	{
		// 1번인자 : 생성할 System
		// 2번인자 : 붙여줄 컴포넌트
		// 3번인자 : 붙여줄 소켓 이름
		// 4번인자 : 붙여줄 상대적 위치
		// 5번인자 : 붙여줄 상대적 회전
		// 6번인자 : 붙여줄 위치 타입
		// 7번인자 : 자동으로 제거할지 여부
		// 컴포넌트를 생성할때 사용.
		/*Particle = UNiagaraFunctionLibrary::SpawnSystemAttached(Effect, Body, NAME_None, FVector(0.f), FRotator(0.f),
			EAttachLocation::KeepRelativeOffset, true);*/
		Particle->SetAsset(Effect);
	}

	void SetNiagaraSystem(const FString& Path)
	{
		UNiagaraSystem* Effect = LoadObject<UNiagaraSystem>(GetWorld(), *Path);

		// 1번인자 : 생성할 System
		// 2번인자 : 붙여줄 컴포넌트
		// 3번인자 : 붙여줄 소켓 이름
		// 4번인자 : 붙여줄 상대적 위치
		// 5번인자 : 붙여줄 상대적 회전
		// 6번인자 : 붙여줄 위치 타입
		// 7번인자 : 자동으로 제거할지 여부
		/*Particle = UNiagaraFunctionLibrary::SpawnSystemAttached(Effect, Body, NAME_None, FVector(0.f), FRotator(0.f),
			EAttachLocation::KeepRelativeOffset, true);*/
		Particle->SetAsset(Effect);
	}

	void SetNiagaraScale(FVector Scale)
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
