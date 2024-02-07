// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UserCharacter.h"
#include "../Projectile/ProjectileNiagara.h"
#include "UserArcherCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API AUserArcherCharacter : public AUserCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUserArcherCharacter();

	int32	m_AttackCount;

public:
	virtual void NormalAttack(int32 AttackIndex)
	{
		m_AttackIndex = AttackIndex;
		//m_AttackEnable = false;
		if (!m_pAnim->Montage_IsPlaying(m_AttackMontageArray[m_AttackIndex]))
		{
			m_AttackMontageArray[m_AttackIndex]->BlendIn.SetBlendTime(0.1f);
			m_AttackMontageArray[m_AttackIndex]->BlendOut.SetBlendTime(0.1f);

			m_pAnim->Montage_Play(m_AttackMontageArray[m_AttackIndex]);
			m_pAnim->SetAttackEnable(true);

			//m_AttackIndex = (m_AttackIndex + 1) % m_AttackMontageArray.Num();
		}

		m_pAnim->SetPlayerAnimationType(EPlayerAnimationType::Attack);


		FVector		MuzzleLoc;
		FRotator	MuzzleRot;
		if (m_AttackIndex == 1)
		{
			if (m_AttackCount == 0)
			{
				MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
				MuzzleRot = GetMesh()->GetSocketRotation(TEXT("Muzzle_01"));
			}

			else
			{
				MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_02"));
				MuzzleRot = GetMesh()->GetSocketRotation(TEXT("Muzzle_02"));
			}

			++m_AttackCount;
		}

		else
		{
			if (m_AttackCount == 0)
			{
				MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_02"));
				MuzzleRot = GetMesh()->GetSocketRotation(TEXT("Muzzle_02"));
			}

			else
			{
				MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
				MuzzleRot = GetMesh()->GetSocketRotation(TEXT("Muzzle_01"));
			}

			++m_AttackCount;
		}

		FActorSpawnParameters	param;

		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 머즐 위치에 투사체를 생성한다.
		AProjectileNiagara* Projectile = GetWorld()->SpawnActor<AProjectileNiagara>(AProjectileNiagara::StaticClass(),
			MuzzleLoc, GetActorRotation(), param);

		//Projectile->SetNiagaraSystem(TEXT("NiagaraSystem'/Game/sA_Megapack_v1/sA_SkillSet_1/Fx/NiagaraSystems/NS_Projectile.NS_Projectile'"));
		Projectile->SetNiagaraSystem(TEXT("NiagaraSystem'/Game/sA_Megapack_v1/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_ArrowAttack.NS_ArrowAttack'"));
		//Projectile->SetSpeed(1000.f);
		Projectile->SetDir(GetActorForwardVector() * 1500.f);
		Projectile->SetOwnerActor(this);
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
