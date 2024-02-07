// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacter.h"
#include "SelectAnimInstance.h"
#include "../GameMode/SelectGameMode.h"

// Sets default values
ASelectCharacter::ASelectCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	memset(ServerCharacterName, 0, USERNAME_SIZE);
	memset(ServerJob, 0, JOBNAME_SIZE);
}

void ASelectCharacter::SetCharacterInfo(const _tagCharacterInfoTable* pInfo)
{ // Connect에서의 호출

	strcpy_s(ServerCharacterName, pInfo->strName);
	strcpy_s(ServerJob, pInfo->strJob);

	FString	strName(pInfo->strName);
	CharacterName	= strName;

	FString	strJob(pInfo->strJob);
	CharacterJob = pInfo->strJob;

	iMapLevel = pInfo->iMapLevel;

	iAttack = pInfo->iAttack;
	iArmor = pInfo->iArmor;
	fAttackSpeed = pInfo->fAttackSpeed;
	fMoveSpeed = pInfo->fMoveSpeed;
	fCritical = pInfo->fCritical;
	fCriticalDamage = pInfo->fCriticalDamage;
	iHP = pInfo->iHP;
	iHPMax = pInfo->iHPMax;
	iMP = pInfo->iMP;
	iMPMax = pInfo->iMPMax;
	iLevel = pInfo->iLevel;
	iMoney = pInfo->iMoney;
}

void ASelectCharacter::SetCharacterInfo(const _tagCharacterInfo* pInfo)
{ // SelectWidget에서 캐릭터 생성을 누르면 호출

	strcpy_s(ServerCharacterName, pInfo->strName);
	strcpy_s(ServerJob, pInfo->strJob);

	FString	strName(pInfo->strName);
	CharacterName = strName;

	FString	strJob(pInfo->strJob);
	CharacterJob = pInfo->strJob;

	iAttack = pInfo->iAttack;
	iArmor = pInfo->iArmor;
	fAttackSpeed = pInfo->fAttackSpeed;
	fMoveSpeed = pInfo->fMoveSpeed;
	fCritical = pInfo->fCritical;
	fCriticalDamage = pInfo->fCriticalDamage;
	iHP = pInfo->iHP;
	iHPMax = pInfo->iHPMax;
	iMP = pInfo->iMP;
	iMPMax = pInfo->iMPMax;
	iLevel = pInfo->iLevel;
	iMoney = pInfo->iMoney;
}

// Called when the game starts or when spawned
void ASelectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SelectAnim	= Cast<USelectAnimInstance>(GetMesh()->GetAnimInstance());
	// 해당 선택한 메쉬의 인스턴스 내용을 얻기위해

	if(eCharType == ESelectCharType::Create)
	{
		ASelectGameMode*	pGameMode = Cast<ASelectGameMode>(GetWorld()->GetAuthGameMode());

		if (pGameMode)
		{
			pGameMode->AddSelectCharacter(this);                                               
		}
	}
}

void ASelectCharacter::CharacterClick(bool bClick)
{
	SelectAnim->CharacterClick(bClick);

	// 이 캐릭터의 정보를 UI에 보여준다.
}

// Called every frame
void ASelectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASelectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

ASelectCharacter* ASelectCharacter::Clone(const FVector& vLoc, const FRotator& vRot)
{
	if (!IsValid(ClassType))
		return nullptr;

	ASelectCharacter* pClone = GetWorld()->SpawnActor<ASelectCharacter>(ClassType,vLoc, vRot);

	return pClone;
}
