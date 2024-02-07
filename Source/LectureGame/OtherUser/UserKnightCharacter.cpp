// Fill out your copyright notice in the Description page of Project Settings.


#include "UserKnightCharacter.h"
#include "UserAnim.h"


// Sets default values
AUserKnightCharacter::AUserKnightCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/Novaborn/Meshes/Greystone_Novaborn.Greystone_Novaborn'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/User/BPUserGreyStone/BPUserGreystoneAnim.BPUserGreystoneAnim_C'"));

	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack1Asset(TEXT("AnimMontage'/Game/User/BPUserGreyStone/AMGreystoneAttack1.AMGreystoneAttack1'"));

	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack2Asset(TEXT("AnimMontage'/Game/User/BPUserGreyStone/AMGreystoneAttack2.AMGreystoneAttack2'"));

	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack3Asset(TEXT("AnimMontage'/Game/User/BPUserGreyStone/AMGreystoneAttack3.AMGreystoneAttack3'"));

	if (Attack3Asset.Succeeded())
		m_AttackMontageArray.Add(Attack3Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	FallRecoveryAsset(TEXT("AnimMontage'/Game/User/BPUserGreyStone/AMGreystoneFallRecovery.AMGreystoneFallRecovery'"));

	if (FallRecoveryAsset.Succeeded())
		m_FallRecoveryMontage = FallRecoveryAsset.Object;



}

void AUserKnightCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_pAnim = Cast<UUserAnim>(GetMesh()->GetAnimInstance());
}

void AUserKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUserKnightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUserKnightCharacter::SkillAttack1()
{
}

void AUserKnightCharacter::SkillAttack2()
{
}
