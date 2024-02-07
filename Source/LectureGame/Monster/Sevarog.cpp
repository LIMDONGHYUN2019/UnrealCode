// Fill out your copyright notice in the Description page of Project Settings.


#include "Sevarog.h"
#include "../MainGameInstance.h"

ASevarog::ASevarog()
{
	PrimaryActorTick.bCanEverTick = true;
	m_eMonsterType = EMonsterType::Sevarog;
}

void ASevarog::BeginPlay()
{
	Super::BeginPlay();

	UMainGameInstance* pGameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	const FMonsterInfo* pInfo = pGameInst->GetMonsterInfo(TEXT("Sevarog"));

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

void ASevarog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASevarog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASevarog::SaveInfo(FArchive* pArchive)
{
	AMonster::SaveInfo(pArchive);
}


