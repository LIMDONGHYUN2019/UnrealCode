// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"

UPlayerAnim::UPlayerAnim()
{
	m_AnimType = EPlayerAnim::Ground;
	m_Ground = false;

	m_HitAlpha = 0.f;
	m_Dir = 0.f;
	m_MoveSpeed = 0.f;
	m_FallAdditive = 0.f;
	m_Attack = false;
	m_AttackCombo = false;
	m_MoveSpeed = 600.0;
}

void UPlayerAnim::NativeInitializeAnimation()
{
	// Super : 부모클래스를 의미한다.
	Super::NativeInitializeAnimation();
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (pPlayer)
	{
		UCharacterMovementComponent* pMovement = pPlayer->GetCharacterMovement();

		if (pMovement)
		{
			m_Ground = pMovement->IsMovingOnGround();

			m_MoveSpeed = pMovement->Velocity.Size();
			 
			if (!m_Ground && m_AnimType != EPlayerAnim::Jump)
			{
				m_AnimType = EPlayerAnim::Fall;
			}
		}
	}
}

void UPlayerAnim::AnimNotify_AttackEnd()
{
	m_Attack = false;
	m_AttackCombo = false;

	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (pPlayer)
		pPlayer->AttackEnd();
}

void UPlayerAnim::AnimNotify_AttackCombo()
{
	m_AttackCombo = true;

	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (pPlayer)
		pPlayer->SetAttackEnable(true);
}

void UPlayerAnim::AnimNotify_Damage()
{
	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (pPlayer)
		pPlayer->AttackDamage();
}

void UPlayerAnim::AnimNotify_Skill1Damage()
{
	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (pPlayer)
		pPlayer->Skill1Damage();
}

void UPlayerAnim::AnimNotify_Skill2Damage()
{
	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (pPlayer)
		pPlayer->Skill2Damage();
}

void UPlayerAnim::AnimNotify_Skill3Damage()
{
	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (pPlayer)
		pPlayer->Skill3Damage();
}

void UPlayerAnim::AnimNotify_Skill4Damage()
{
	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (pPlayer)
		pPlayer->Skill4Damage();
}

void UPlayerAnim::AnimNotify_HitEnd()
{
	m_HitAlpha = 0.f;
}

void UPlayerAnim::AnimNotify_DeathEnd()
{
	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (pPlayer)
		pPlayer->SetDeath(true);

	PrintViewport(5.f, FColor::Silver, FString::Printf(TEXT("Player Death.....")));
}

void UPlayerAnim::AnimNotify_JumpEnd()
{
	m_AnimType = EPlayerAnim::Fall;
}

void UPlayerAnim::AnimNotify_FallEnd()
{
	m_AnimType = EPlayerAnim::Ground;
	m_FallAdditive = 1.f;

	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (pPlayer)
		pPlayer->PlayFallRecovery();
}

void UPlayerAnim::AnimNotify_FallRecoveryEnd()
{
	m_FallAdditive = 0.f;
}
