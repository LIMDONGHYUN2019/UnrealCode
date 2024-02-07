// Fill out your copyright notice in the Description page of Project Settings.


#include "Twinblast.h"
#include "PlayerAnim.h"
#include "../Projectile/ProjectileNiagara.h"
#include "../GameMode/LectureGameGameModeBase.h"
#include "../NetSession.h"
#include "../PacketSession.h"
#include "../NetStream.h"
#include "../GameServerManager.h"
#include "../UI/GameWidget.h"
#include "../MainGameInstance.h"
#include "../UI/SlotListWidget.h"

// Sets default values
ATwinblast::ATwinblast()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack1Asset(TEXT("AnimMontage'/Game/Player/Twinblast/AMTwinblastAttack1.AMTwinblastAttack1'"));
	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack2Asset(TEXT("AnimMontage'/Game/Player/Twinblast/AMTwinblastAttack2.AMTwinblastAttack2'"));
	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	FallRecoveryAsset(TEXT("AnimMontage'/Game/Player/Twinblast/AMTwinblastFallRecovery.AMTwinblastFallRecovery'"));
	if (FallRecoveryAsset.Succeeded())
		m_FallRecoveryMontage = FallRecoveryAsset.Object;

	//static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/Player/Twinblast/AMTwinblastSkill1.AMTwinblastSkill1'"));
	//if (Skill1Asset.Succeeded())
	//	m_SkillMontageArray.Add(Skill1Asset.Object);

	//static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/Player/Twinblast/AMTwinblastSkill2.AMTwinblastSkill2'"));
	//if (Skill2Asset.Succeeded())
	//	m_SkillMontageArray.Add(Skill2Asset.Object);

	m_AttackCount = 0;

	m_pBullet = CreateDefaultSubobject<AProjectileNiagara>(TEXT("Bullet"));
}

void ATwinblast::BeginPlay()
{
	Super::BeginPlay();
	m_pAnim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame
void ATwinblast::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (m_pAnim->GetIdleEnable())
	//{
	//	FVector	vDir = GetCharacterMovement()->Velocity;

	//	if (vDir.Size() > 0.f)
	//	{
	//		vDir.Normalize();
	//		//SetActorRotation(vDir.Rotation());
	//		GetMesh()->SetRelativeRotation(vDir.Rotation());
	//	}
	//	AddMovementInput(m_vTempoPos);

	//	//FVector	vDir = GetCharacterMovement()->Velocity;

	//	//if (vDir.Size() > 0.f)
	//	//{
	//	//	vDir.Normalize();
	//	//	GetMesh()->SetRelativeRotation(vDir.Rotation());
	//	//}
	//}
}

// Called to bind functionality to input
void ATwinblast::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAxis(TEXT("MoveFront"), this, &ATwinblast::MoveFront);
	//PlayerInputComponent->BindAxis(TEXT("MoveSide"), this, &ATwinblast::MoveSide);
}

void ATwinblast::NormalAttack()
{
	if (m_AttackEnable)
	{
		// 각도를 서버로 보내준다.
		{
			float	Rot = GetActorRotation().Yaw;

			NetStream	stream;
			uint8	packet[PACKET_LENGTH] = {};
			stream.SetBuffer(packet);

			stream.Write(&Rot, sizeof(float));

			PacketSession* Session = GameServerManager::GetInst()->GetSession();
			Session->Write(GP_ROTATION, stream.GetLength(), packet);
		}

		if (!m_Anim->Montage_IsPlaying(m_AttackMontageArray[m_AttackIndex]))
		{
			m_AttackMontageArray[m_AttackIndex]->BlendIn.SetBlendTime(0.1f);
			m_AttackMontageArray[m_AttackIndex]->BlendOut.SetBlendTime(0.1f);
			m_Anim->SetAttackEnable(true);
			m_Anim->Montage_Play(m_AttackMontageArray[m_AttackIndex]);

			m_AttackSendIndex = m_AttackIndex;

			m_AttackIndex = (m_AttackIndex + 1) % m_AttackMontageArray.Num();

			m_eAttackType = ATTACK_TYPE::NormalAttack;

			m_SkillEnable = false;
			m_AttackEnable = false;
		}
	}
}

void ATwinblast::SkillAttack1()
{
	if (m_bDeath)
		return;
	
	if (m_SkillEnable)
	{
		ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
		UGameWidget* pGameWidget = pGameMode->GetGameWidget();
		class USlotListWidget* pSlotList = pGameWidget->GetSlotWidget();
		SkillInfo* SlotInfo = pSlotList->GetSlotSkillInfo(0);

		if (!SlotInfo)
			return;

		const FSkillAnimationInfo* pTwinblastSkill = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetTwinblastAnim(SlotInfo->strName);
		if (!pTwinblastSkill)
			return;

		if (!m_Anim->Montage_IsPlaying(pTwinblastSkill->Animation))
		{
			m_eAttackType = ATTACK_TYPE::Skill1;
			pTwinblastSkill->Animation->BlendIn.SetBlendTime(0.1f);
			pTwinblastSkill->Animation->BlendOut.SetBlendTime(0.1f);
			m_Anim->SetAttackEnable(true);
			m_Anim->Montage_Play(pTwinblastSkill->Animation);

			m_SkillEnable = false;
			m_AttackEnable = false;
		}
	}
}

void ATwinblast::SkillAttack2()
{
	if (m_bDeath)
		return;

	if (m_SkillEnable)
	{
		ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
		UGameWidget* pGameWidget = pGameMode->GetGameWidget();
		class USlotListWidget* pSlotList = pGameWidget->GetSlotWidget();
		SkillInfo* SlotInfo = pSlotList->GetSlotSkillInfo(1);

		if (!SlotInfo)
			return;

		const FSkillAnimationInfo* pTwinblastSkill = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetTwinblastAnim(SlotInfo->strName);
		if (!pTwinblastSkill)
			return;

		if (!m_Anim->Montage_IsPlaying(pTwinblastSkill->Animation))
		{
			m_eAttackType = ATTACK_TYPE::Skill2;
			pTwinblastSkill->Animation->BlendIn.SetBlendTime(0.1f);
			pTwinblastSkill->Animation->BlendOut.SetBlendTime(0.1f);
			m_Anim->SetAttackEnable(true);
			m_Anim->Montage_Play(pTwinblastSkill->Animation);

			m_SkillEnable = false;
			m_AttackEnable = false;
		}
	}
}

void ATwinblast::SkillAttack3()
{
	if (m_bDeath)
		return;

	if (m_SkillEnable)
	{
		ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
		UGameWidget* pGameWidget = pGameMode->GetGameWidget();
		class USlotListWidget* pSlotList = pGameWidget->GetSlotWidget();
		SkillInfo* SlotInfo = pSlotList->GetSlotSkillInfo(2);

		if (!SlotInfo)
			return;

		const FSkillAnimationInfo* pTwinblastSkill = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetTwinblastAnim(SlotInfo->strName);
		if (!pTwinblastSkill)
			return;

		if (!m_Anim->Montage_IsPlaying(pTwinblastSkill->Animation))
		{
			m_eAttackType = ATTACK_TYPE::Skill3;
			pTwinblastSkill->Animation->BlendIn.SetBlendTime(0.1f);
			pTwinblastSkill->Animation->BlendOut.SetBlendTime(0.1f);
			m_Anim->SetAttackEnable(true);
			m_Anim->Montage_Play(pTwinblastSkill->Animation);

			m_SkillEnable = false;
			m_AttackEnable = false;
		}
	}
}

void ATwinblast::Hit()
{
	//m_pAnim->ChangeAnimation(EPlayerAnim::Hit);
	m_pAnim->ChangeAnimation(EPlayerAnim::Hit);
}

void ATwinblast::Death()
{
	//m_pAnim->ChangeAnimation(EPlayerAnim::Death);
	m_pAnim->ChangeAnimation(EPlayerAnim::Death);
}

void ATwinblast::AttackEnd()
{
	Super::AttackEnd();
}

void ATwinblast::AttackDamage()
{
	// 이게 1일 경우는 공격1번, 0일 경우 공격2번이 나가게 된다.
	// 공격 1번은 오른쪽, 왼쪽으로 쏘고
	// 공격 2번은 왼쪽, 오른쪽으로 쏜다.
	FVector		MuzzleLoc;
	FRotator	MuzzleRot;
	if (m_AttackIndex == 1)
	{
		if (m_AttackCount == 0)
		{
			MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
			MuzzleRot = GetMesh()->GetSocketRotation(TEXT("Muzzle_01"));
		}

		else
		{
			MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_02"));
			MuzzleRot = GetMesh()->GetSocketRotation(TEXT("Muzzle_02"));
		}

		++m_AttackCount;
	}

	else
	{
		if (m_AttackCount == 0)
		{
			MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_02"));
			MuzzleRot = GetMesh()->GetSocketRotation(TEXT("Muzzle_02"));
		}

		else
		{
			MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
			MuzzleRot = GetMesh()->GetSocketRotation(TEXT("Muzzle_01"));
		}

		++m_AttackCount;
	}

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AProjectileNiagara* Projectile = GetWorld()->SpawnActor<AProjectileNiagara>(AProjectileNiagara::StaticClass(),
		MuzzleLoc, GetActorRotation(), param);
	
	//Projectile->SetNiagaraSystem(TEXT("NiagaraSystem'/Game/sA_Megapack_v1/sA_ShootingVfxPack/FX/NiagaraSystems/NS_BulletTrail_2.NS_BulletTrail_2'"));
	Projectile->SetNiagaraSystem(TEXT("NiagaraSystem'/Game/sA_Megapack_v1/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_ArrowAttack.NS_ArrowAttack'"));
	//Projectile->SetNiagaraScale(FVector(2.f, 2.f, 2.f));

	Projectile->SetSpeed(18000.f);
	Projectile->SetDir(GetActorForwardVector() * 1500.f);
	Projectile->SetDist(3000);
	Projectile->SetOwnerActor(this);


//	// PlayerAttack : ECC_GameTraceChannel4
//	// EnemyAttack : ECC_GameTraceChannel5
}

void ATwinblast::Skill1Damage()
{
}

void ATwinblast::Skill2Damage()
{
}

void ATwinblast::Skill3Damage()
{
}

void ATwinblast::Skill4Damage()
{
}
