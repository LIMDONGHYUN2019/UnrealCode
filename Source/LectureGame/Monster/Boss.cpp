// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "../MainGameInstance.h"

ABoss::ABoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_eMonsterType = EMonsterType::Boss;
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	UMainGameInstance* pGameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	const FMonsterInfo* pInfo = pGameInst->GetMonsterInfo(TEXT("Boss"));

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

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABoss::SaveInfo(FArchive* pArchive)
{
	AMonster::SaveInfo(pArchive);
}
