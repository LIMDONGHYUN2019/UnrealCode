// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PlayerCharacter.h"
#include "Twinblast.generated.h"


UCLASS()
class LECTUREGAME_API ATwinblast : public APlayerCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATwinblast();

protected:
	class UPlayerAnim*			m_pAnim;
	class AProjectileNiagara*	m_pBullet;
	int32						m_AttackCount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void NormalAttack();
	virtual void SkillAttack1();
	virtual void SkillAttack2();
	virtual void SkillAttack3();
	virtual void Hit();
	virtual void Death();
	virtual void AttackEnd();
	virtual void AttackDamage();
	virtual void Skill1Damage();
	virtual void Skill2Damage();
	virtual void Skill3Damage();
	virtual void Skill4Damage();
};
