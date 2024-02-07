// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"
#include "../MainGameInstance.h"

// Sets default values
AMinion::AMinion()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_eMonsterType = EMonsterType::Minion;
}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();

	UMainGameInstance* pGameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	const FMonsterInfo* pInfo = pGameInst->GetMonsterInfo(TEXT("Minion"));

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

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMinion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AMinion::SaveInfo(FArchive* pArchive)
{
	AMonster::SaveInfo(pArchive);
}


