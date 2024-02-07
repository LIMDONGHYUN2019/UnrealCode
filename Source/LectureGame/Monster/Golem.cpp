// Fill out your copyright notice in the Description page of Project Settings.


#include "Golem.h"
#include "../MainGameInstance.h"



AGolem::AGolem()
{
	PrimaryActorTick.bCanEverTick = true;

	m_eMonsterType = EMonsterType::Golem;
}

void AGolem::BeginPlay()
{
	Super::BeginPlay();

	UMainGameInstance* pGameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	const FMonsterInfo* pInfo = pGameInst->GetMonsterInfo(TEXT("Golem"));

	m_eMonsterType = pInfo->eMonsterType;
	m_iAttack = pInfo->iAttack;
	m_iArmor = pInfo->iArmor;
	m_iHPMax = pInfo->iHPMax;
	m_iHP = m_iHPMax;
	m_iLevel = pInfo->iLevel;
	m_iExp = pInfo->iExp;
	m_iGold = pInfo->iGold;
	m_fMoveSpeed = pInfo->fMoveSpeed;
	m_fTraceRange = pInfo->fTraceRange;
	m_fAttackRange = pInfo->fAttackRange;

	GetCharacterMovement()->MaxWalkSpeed = m_fMoveSpeed;
}

void AGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGolem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGolem::SaveInfo(FArchive* pArchive)
{
	AMonster::SaveInfo(pArchive);
}