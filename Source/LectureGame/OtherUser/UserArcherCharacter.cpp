// Fill out your copyright notice in the Description page of Project Settings.


#include "UserArcherCharacter.h"
#include "UserAnim.h"


// Sets default values
AUserArcherCharacter::AUserArcherCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Skins/Tier2/SummerTime/Meshes/TwinBlast_SummerTime.TwinBlast_SummerTime'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/User/BPUserTwinBlast/BPUserTwinBlastAnim.BPUserTwinblastAnim_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack1Asset(TEXT("AnimMontage'/Game/User/BPUserTwinBlast/AMTwinblastAttack1.AMTwinblastAttack1'"));
	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack2Asset(TEXT("AnimMontage'/Game/User/BPUserTwinBlast/AMTwinblastAttack2.AMTwinblastAttack2'"));
	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	FallRecoveryAsset(TEXT("AnimMontage'/Game/User/BPUserTwinBlast/AMTwinblastFallRecovery.AMTwinblastFallRecovery'"));
	if (FallRecoveryAsset.Succeeded())
		m_FallRecoveryMontage = FallRecoveryAsset.Object;/*

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/User/BPUserTwinBlast/AMTwinblastSkill1.AMTwinblastSkill1'"));
	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/User/BPUserTwinBlast/AMTwinblastSkill2.AMTwinblastSkill2'"));
	if (Skill2Asset.Succeeded())
		m_SkillMontageArray.Add(Skill2Asset.Object);*/

	m_AttackCount = 0;
}

// Called when the game starts or when spawned
void AUserArcherCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_pAnim = Cast<UUserAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AUserArcherCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUserArcherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}