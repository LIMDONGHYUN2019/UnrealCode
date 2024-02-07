// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PlayerCharacter.h"
#include "Serath.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API ASerath : public APlayerCharacter
{
	GENERATED_BODY()
public:
	ASerath();

protected:
	class UPlayerAnim*			m_pAnim;
	class AProjectileNiagara*	m_pMagic;
	int32						m_AttackCount;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	void MoveFront(float fScale);
	void MoveSide(float fScale);

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
