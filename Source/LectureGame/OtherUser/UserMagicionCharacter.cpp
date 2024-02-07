// Fill out your copyright notice in the Description page of Project Settings.


#include "UserMagicionCharacter.h"
#include "UserAnim.h"



// Sets default values
AUserMagicionCharacter::AUserMagicionCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonSerath/Characters/Heroes/Serath/Skins/Tier1/Serath_Gold/Meshes/SerathGold.SerathGold'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/User/BPUserSerath/BPUserSerathAnim.BPUserSerathAnim_C'"));

	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack1Asset(TEXT("AnimMontage'/Game/User/BPUserSerath/AMSerathAttack1.AMSerathAttack1'"));

	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack2Asset(TEXT("AnimMontage'/Game/User/BPUserSerath/AMSerathAttack2.AMSerathAttack2'"));

	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	FallRecoveryAsset(TEXT("AnimMontage'/Game/User/BPUserSerath/AMSerathFallRecovery.AMSerathFallRecovery'"));

	if (FallRecoveryAsset.Succeeded())
		m_FallRecoveryMontage = FallRecoveryAsset.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/User/BPUserSerath/AMSerathSkill1.AMSerathSkill1'"));

	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/User/BPUserSerath/AMSerathSkill2.AMSerathSkill2'"));

	if (Skill2Asset.Succeeded())
		m_SkillMontageArray.Add(Skill2Asset.Object);
}
// Called when the game starts or when spawned
void AUserMagicionCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_pAnim = Cast<UUserAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AUserMagicionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AUserMagicionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}