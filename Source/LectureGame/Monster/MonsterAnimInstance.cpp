// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"

UMonsterAnimInstance::UMonsterAnimInstance()
{
	m_AnimType = MonsterAnim::Idle;

	m_HitAlpha = 0.f;
}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UMonsterAnimInstance::AnimNotify_Attack()
{
	//PrintViewport(1.f, FColor::Red, TEXT("Monster Attack"));

	APawn* Pawn = TryGetPawnOwner();

	PrintViewport(1.f, FColor::Red, Pawn->GetActorLocation().ToString());
	//LOG(TEXT("Monster Location : %s"), *Pawn->GetActorLocation().ToString());
}

void UMonsterAnimInstance::AnimNotify_HitEnd()
{
	m_HitAlpha = 0.f;
}

void UMonsterAnimInstance::AnimNotify_DeathEnd()
{
	//APawn* pMonster = TryGetPawnOwner();
	//GetWorld()->DestroyActor(pMonster);
}
