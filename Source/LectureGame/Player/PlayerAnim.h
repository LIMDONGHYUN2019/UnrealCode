// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

UENUM(BlueprintType)
enum class EPlayerAnim : uint8
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
class LECTUREGAME_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnim();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPlayerAnim m_AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_Ground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_HitAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_Dir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_FallAdditive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_Attack;

	bool	m_AttackCombo;

public:
	bool GetAttackCombo()
	{
		return m_AttackCombo;
	}
	EPlayerAnim GetAnim()
	{
		return m_AnimType;
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

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	void ChangeAnimation(EPlayerAnim eAnim)
	{
		m_AnimType = eAnim;
	}

	UFUNCTION()
		void AnimNotify_AttackEnd();
	UFUNCTION()
		void AnimNotify_AttackCombo();
	UFUNCTION()
		void AnimNotify_Damage();
	UFUNCTION()
		void AnimNotify_Skill1Damage();
	UFUNCTION()
		void AnimNotify_Skill2Damage();
	UFUNCTION()
		void AnimNotify_Skill3Damage();
	UFUNCTION()
		void AnimNotify_Skill4Damage();
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
