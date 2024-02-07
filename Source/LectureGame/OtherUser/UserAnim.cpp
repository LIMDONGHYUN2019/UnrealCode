// Fill out your copyright notice in the Description page of Project Settings.


#include "UserAnim.h"
#include "UserCharacter.h"

UUserAnim::UUserAnim()
{
	//m_eAnimType = EUserAnim::Ground;

	/*m_Ground = false;

	m_HitAlpha = 0.f;
	m_Dir = 0.f;
	m_MoveSpeed = 0.f;
	m_FallAdditive = 0.f;
	m_Attack = false;
	m_AttackCombo = false;
	m_MoveSpeed = 600.0;*/

	m_AnimType = EPlayerAnimationType::Idle;
}

void UUserAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UUserAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	AUserCharacter* pUser = Cast<AUserCharacter>(TryGetPawnOwner());
	if (pUser)
	{
		UCharacterMovementComponent* pMovement = pUser->GetCharacterMovement();

		if (pMovement)
		{
			m_Ground = pMovement->IsMovingOnGround();

			m_MoveSpeed = pMovement->Velocity.Size();

			//if (!m_Ground && m_AnimType != EPlayerAnimationType::Jump)
			//{
			//	m_AnimType = EPlayerAnimationType::Fall;
			//}
		}
	}
}

void UUserAnim::AnimNotify_AttackEnd()
{
	//m_eAnimType = EUserAnim::Ground;
}

void UUserAnim::AnimNotify_AttackCombo()
{
	/*m_AttackCombo = true;
	AUserCharacter* pOtherUser = Cast<AUserCharacter>(TryGetPawnOwner());

	if (pOtherUser)
		pOtherUser->SetAttackEnable(true);*/
}

void UUserAnim::AnimNotify_Skill1Damage()
{
}

void UUserAnim::AnimNotify_Skill2Damage()
{
}

void UUserAnim::AnimNotify_HitEnd()
{
	m_HitAlpha = 0.f;
}

void UUserAnim::AnimNotify_DeathEnd()
{
	/*AUserCharacter* pOtherUser = Cast<AUserCharacter>(TryGetPawnOwner());
	if (pOtherUser)
		pOtherUser->SetDeath(true);*/
}

void UUserAnim::AnimNotify_JumpEnd()
{/*
	m_AnimType = EPlayerAnimationType::Fall;*/
}

void UUserAnim::AnimNotify_FallEnd()
{/*
	m_AnimType = EPlayerAnimationType::Ground;*/
	m_FallAdditive = 1.f;

	AUserCharacter* pOtherUser = Cast<AUserCharacter>(TryGetPawnOwner());
	if (pOtherUser)
		pOtherUser->PlayFallRecovery();
}

void UUserAnim::AnimNotify_FallRecoveryEnd()
{
	m_FallAdditive = 0.f;
}
