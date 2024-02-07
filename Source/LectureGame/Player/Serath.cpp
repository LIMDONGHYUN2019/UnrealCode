// Fill out your copyright notice in the Description page of Project Settings.


#include "Serath.h"
#include "PlayerAnim.h"
#include "../Projectile/ProjectileNiagara.h"
#include "../GameMode/LectureGameGameModeBase.h"
#include "../UI/GameWidget.h"
#include "../MainGameInstance.h"
#include "../UI/SlotListWidget.h"
#include "../NetSession.h"
#include "../PacketSession.h"
#include "../NetStream.h"
#include "../GameServerManager.h"

ASerath::ASerath()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack1Asset(TEXT("AnimMontage'/Game/Player/Serath/AMSerathAttack1.AMSerathAttack1'"));

	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack2Asset(TEXT("AnimMontage'/Game/Player/Serath/AMSerathAttack2.AMSerathAttack2'"));

	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	FallRecoveryAsset(TEXT("AnimMontage'/Game/Player/Serath/AMSerathFallRecovery.AMSerathFallRecovery'"));

	if (FallRecoveryAsset.Succeeded())
		m_FallRecoveryMontage = FallRecoveryAsset.Object;

	//static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/Player/Serath/AMSerathSkill1.AMSerathSkill1'"));

	//if (Skill1Asset.Succeeded())
	//	m_SkillMontageArray.Add(Skill1Asset.Object);

	//static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/Player/Serath/AMSerathSkill2.AMSerathSkill2'"));

	//if (Skill2Asset.Succeeded())
	//	m_SkillMontageArray.Add(Skill2Asset.Object);

	//static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill3Asset(TEXT("AnimMontage'/Game/Player/Serath/AMSerathSkill3.AMSerathSkill3'"));

	//if (Skill3Asset.Succeeded())
	//	m_SkillMontageArray.Add(Skill3Asset.Object);

	m_pMagic = CreateDefaultSubobject<AProjectileNiagara>(TEXT("Bullet"));
	m_AttackCount = 0;
}

void ASerath::BeginPlay()
{
	Super::BeginPlay();
	m_pAnim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
}

void ASerath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (m_pAnim->GetIdleEnable())
	//{
	//	//PrintViewport(0.1f, FColor::Blue, FString::Printf(TEXT("Dir : %.5f / %.5f"), m_vCurrentPos.X, m_vCurrentPos.Y));
	//	AddMovementInput(m_vTempoPos);
	//	FVector	vDir = GetCharacterMovement()->Velocity;
	//	if (vDir.Size() > 0.f)
	//	{
	//		vDir.Normalize();
	//		GetMesh()->SetRelativeRotation(vDir.Rotation());
	//	}
	//}
}

void ASerath::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAxis(TEXT("MoveFront"), this, &ASerath::MoveFront);
	//PlayerInputComponent->BindAxis(TEXT("MoveSide"), this, &ASerath::MoveSide);

	//PlayerInputComponent->BindAction(TEXT("NormalAttack"), EInputEvent::IE_Pressed,
	//	this, &ASerath::NormalAttack);
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed,
	//	this, &ASerath::Jump);
}

void ASerath::MoveFront(float fScale)
{
	/*if (m_pAnim->GetIdleEnable())
	{
		m_pAnim->ChangeAnimation(EPlayerAnim::Run);
		AddMovementInput(GetActorForwardVector(), fScale);
	}*/
}

void ASerath::MoveSide(float fScale)
{
	/*if (m_pAnim->GetIdleEnable())
	{
		m_pAnim->ChangeAnimation(EPlayerAnim::Run);
		AddMovementInput(GetActorRightVector(), fScale);
	}*/
}

void ASerath::NormalAttack()
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

void ASerath::Hit()
{
	m_pAnim->ChangeAnimation(EPlayerAnim::Hit);
}

void ASerath::Death()
{
	m_pAnim->ChangeAnimation(EPlayerAnim::Death);
	m_AttackEnable = false;
}

void ASerath::SkillAttack1()
{
	if (m_bDeath)
		return;

	if (m_SkillEnable)
	{
		ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
		UGameWidget* pGameWidget = pGameMode->GetGameWidget();
		class USlotListWidget* pSlotListWidget = pGameWidget->GetSlotWidget();
		SkillInfo* SlotInfo = pSlotListWidget->GetSlotSkillInfo(0);

		if (!SlotInfo)
		{
			return;
		}

		const FSkillAnimationInfo* pSerathSkill = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetSerathAnim(SlotInfo->strName);
		if (!pSerathSkill)
			return;

		if (!m_Anim->Montage_IsPlaying(pSerathSkill->Animation))
		{
			m_eAttackType = ATTACK_TYPE::Skill1;
			pSerathSkill->Animation->BlendIn.SetBlendTime(0.1f);
			pSerathSkill->Animation->BlendOut.SetBlendTime(0.1f);
			m_Anim->SetAttackEnable(true);
			m_Anim->Montage_Play(pSerathSkill->Animation);

			m_SkillEnable = false;
			m_AttackEnable = false;
		}
	}
}

void ASerath::SkillAttack2()
{
	if (m_bDeath)
		return;

	if (m_SkillEnable)
	{
		ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
		UGameWidget* pGameWidget = pGameMode->GetGameWidget();
		class USlotListWidget* pSlotListWidget = pGameWidget->GetSlotWidget();
		SkillInfo* pSlotInfo = pSlotListWidget->GetSlotSkillInfo(1);

		if (!pSlotInfo)
		{
			return;
		}

		const FSkillAnimationInfo* pSerathSkill = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetSerathAnim(pSlotInfo->strName);
		if (!pSerathSkill)
			return;

		if (!m_Anim->Montage_IsPlaying(pSerathSkill->Animation))
		{
			m_eAttackType = ATTACK_TYPE::Skill2;
			pSerathSkill->Animation->BlendIn.SetBlendTime(0.1f);
			pSerathSkill->Animation->BlendOut.SetBlendTime(0.1f);
			m_Anim->SetAttackEnable(true);
			m_Anim->Montage_Play(pSerathSkill->Animation);

			m_SkillEnable = false;
			m_AttackEnable = false;
		}
	}
}

void ASerath::SkillAttack3()
{
	if (m_bDeath)
		return;

	if (m_SkillEnable)
	{
		ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
		UGameWidget* pGameWidget = pGameMode->GetGameWidget();
		USlotListWidget* pSlotListWidget = pGameWidget->GetSlotWidget();
		SkillInfo* pSlotInfo = pSlotListWidget->GetSlotSkillInfo(2);

		if (!pSlotInfo)
			return;

		const FSkillAnimationInfo* pSerathSkill = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetSerathAnim(pSlotInfo->strName);

		if (!pSerathSkill)
			return;

		if (!m_Anim->Montage_IsPlaying(pSerathSkill->Animation))
		{
			m_eAttackType = ATTACK_TYPE::Skill3;
			pSerathSkill->Animation->BlendIn.SetBlendTime(0.1f);
			pSerathSkill->Animation->BlendOut.SetBlendTime(0.1f);
			m_Anim->SetAttackEnable(true);
			m_Anim->Montage_Play(pSerathSkill->Animation);
			m_SkillEnable = false;
			m_AttackEnable = false;
		}
	}
}

void ASerath::AttackEnd()
{
	Super::AttackEnd();
}

void ASerath::AttackDamage()
{
	FVector		MuzzleLoc;
	FRotator	MuzzleRot;

	MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_02"));

	FActorSpawnParameters	param;

	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Muzzle 위치에 투사체를 생성한다.
	AProjectileNiagara* Projectile = GetWorld()->SpawnActor<AProjectileNiagara>(AProjectileNiagara::StaticClass(),
		MuzzleLoc, GetActorRotation(), param);

	Projectile->SetNiagaraSystem(TEXT("NiagaraSystem'/Game/sA_Megapack_v1/sA_ShootingVfxPack/FX/NiagaraSystems/NS_ROCKET_Trail.NS_ROCKET_Trail'"));
	//Projectile->SetSpeed(1000.f);
	Projectile->SetDir(GetActorForwardVector() * 1500.f);
	Projectile->SetDist(3000);
	Projectile->SetOwnerActor(this);
}

void ASerath::Skill1Damage()
{
}

void ASerath::Skill2Damage()
{
}

void ASerath::Skill3Damage()
{
}

void ASerath::Skill4Damage()
{
}
