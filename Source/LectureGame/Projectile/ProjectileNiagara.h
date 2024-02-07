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
		// 1������ : ������ System
		// 2������ : �ٿ��� ������Ʈ
		// 3������ : �ٿ��� ���� �̸�
		// 4������ : �ٿ��� ����� ��ġ
		// 5������ : �ٿ��� ����� ȸ��
		// 6������ : �ٿ��� ��ġ Ÿ��
		// 7������ : �ڵ����� �������� ����
		// ������Ʈ�� �����Ҷ� ���.
		/*Particle = UNiagaraFunctionLibrary::SpawnSystemAttached(Effect, Body, NAME_None, FVector(0.f), FRotator(0.f),
			EAttachLocation::KeepRelativeOffset, true);*/
		Particle->SetAsset(Effect);
	}

	void SetNiagaraSystem(const FString& Path)
	{
		UNiagaraSystem* Effect = LoadObject<UNiagaraSystem>(GetWorld(), *Path);

		// 1������ : ������ System
		// 2������ : �ٿ��� ������Ʈ
		// 3������ : �ٿ��� ���� �̸�
		// 4������ : �ٿ��� ����� ��ġ
		// 5������ : �ٿ��� ����� ȸ��
		// 6������ : �ٿ��� ��ġ Ÿ��
		// 7������ : �ڵ����� �������� ����
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
