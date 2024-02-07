// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PlayerCharacter.h"
#include "Greyston.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API AGreystone : public APlayerCharacter
{
	GENERATED_BODY()
public:
	AGreystone();

protected:
	class UPlayerAnim* m_pAnim;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	virtual void NormalAttack();
	virtual void SkillAttack1();
	virtual void SkillAttack2();
	virtual void SkillAttack3();
	virtual void SkillAttack4();
	virtual void NormalAttackHitEffect();
	virtual void Hit();
	virtual void Death();
	virtual void AttackEnd();
	virtual void AttackDamage();
	virtual void Skill1Damage();
	virtual void Skill2Damage();
	virtual void Skill3Damage();
	virtual void Skill4Damage();
};
