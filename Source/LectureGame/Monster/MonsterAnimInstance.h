// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"


UCLASS()
class LECTUREGAME_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMonsterAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	MonsterAnim		m_AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float			m_HitAlpha;

	bool bIdleEnable;

public:
	void Hit()
	{
		m_HitAlpha = 1.f;
	}

	void ChangeAnim(MonsterAnim Type)
	{
		m_AnimType = Type;
		switch (m_AnimType)
		{
		case MonsterAnim::Idle:
		case MonsterAnim::Walk:
		case MonsterAnim::Run:
			bIdleEnable = true;
			break;
		case MonsterAnim::Attack:
		case MonsterAnim::Skill_1:
		case MonsterAnim::Skill_2:
		case MonsterAnim::Death:
			bIdleEnable = false;
			break;
		}
	}
	bool GetIdleEnable()
	{
		return bIdleEnable;
	}

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_HitEnd();

	UFUNCTION()
	void AnimNotify_DeathEnd();
};
