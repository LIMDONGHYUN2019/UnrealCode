// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "UserAnim.generated.h"

UENUM(BlueprintType)
enum class EUserAnim : uint8
{
	Ground,
	Jump,
	Fall,
	Skill1,
	Skill2,
	Skill3,
	Hit,
	Death
};

UCLASS()
class LECTUREGAME_API UUserAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UUserAnim();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EPlayerAnimationType	m_AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool m_Ground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_HitAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_Dir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool m_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_FallAdditive;

	bool	m_AttackCombo;

public:
	EPlayerAnimationType GetPlayerAnimationType()	const
	{
		return m_AnimType;
	}

	void SetPlayerAnimationType(EPlayerAnimationType Type)
	{
		m_AnimType = Type;
	}

	bool GetAttackCombo()
	{
		return m_AttackCombo;
	}

	float GetDir()
	{
		return m_Dir;
	}
	void Hit()
	{
		m_HitAlpha = 0.5f;
	}

	void SetDir(float Dir)
	{
		m_Dir = Dir;
	}

	bool IsAttack()
	{
		return m_Attack;
	}
	void SetAttackEnable(bool Attack)
	{
		m_Attack = Attack;
	}

	void SetMoveSpeed(float Speed)
	{
		m_MoveSpeed = Speed;
	}

	void ChangeAnimation(EPlayerAnimationType eAnim)
	{
		m_AnimType = eAnim;
	}

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	UFUNCTION()
		void AnimNotify_AttackEnd();
	UFUNCTION()
		void AnimNotify_AttackCombo();
	UFUNCTION()
		void AnimNotify_Skill1Damage();
	UFUNCTION()
		void AnimNotify_Skill2Damage();
	UFUNCTION()
		void AnimNotify_HitEnd();
	UFUNCTION()
		void AnimNotify_DeathEnd();
	UFUNCTION()
		void AnimNotify_JumpEnd();// Apex³¡
	UFUNCTION()
		void AnimNotify_FallEnd(); // Fall³¡
	UFUNCTION()
		void AnimNotify_FallRecoveryEnd(); //ÂøÁö ³¡
};
