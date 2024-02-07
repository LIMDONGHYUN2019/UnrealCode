// Fill out your copyright notice in the Description page of Project Settings.


#include "Greyston.h"
#include "PlayerAnim.h"
#include "../GameMode/LectureGameGameModeBase.h"
#include "../UI/GameWidget.h"
#include "../MainGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "../UI/SlotListWidget.h"
#include "../NetSession.h"
#include "../PacketSession.h"
#include "../NetStream.h"
#include "../GameServerManager.h"
#include "../Projectile/ProjectileNiagara.h"

AGreystone::AGreystone()
{
	PrimaryActorTick.bCanEverTick = true;

	//GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	//bUseControllerRotationYaw = false; // ĳ���Ͱ� ī�޶� ���� �̵������ʵ��� ����.

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack1Asset(TEXT("AnimMontage'/Game/Player/Greystone/AMGreystoneAttack1.AMGreystoneAttack1'"));

	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack2Asset(TEXT("AnimMontage'/Game/Player/Greystone/AMGreystoneAttack2.AMGreystoneAttack2'"));

	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack3Asset(TEXT("AnimMontage'/Game/Player/Greystone/AMGreystoneAttack3.AMGreystoneAttack3'"));

	if (Attack3Asset.Succeeded())
		m_AttackMontageArray.Add(Attack3Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	FallRecoveryAsset(TEXT("AnimMontage'/Game/Player/Greystone/AMGreystoneFallRecovery.AMGreystoneFallRecovery'"));

	if (FallRecoveryAsset.Succeeded())
		m_FallRecoveryMontage = FallRecoveryAsset.Object;

	//static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/Player/Greystone/AMGreystoneSkill1.AMGreystoneSkill1'"));

	//if (Skill1Asset.Succeeded())
	//	m_SkillMontageArray.Add(Skill1Asset.Object);

	//static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/Player/Greystone/AMGreystoneSkill2.AMGreystoneSkill2'"));

	//if (Skill2Asset.Succeeded())
	//	m_SkillMontageArray.Add(Skill2Asset.Object);

	//static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill3Asset(TEXT("AnimMontage'/Game/Player/Greystone/AMGreystoneSkill3.AMGreystoneSkill3'"));

	//if (Skill3Asset.Succeeded())
	//	m_SkillMontageArray.Add(Skill3Asset.Object);


}

void AGreystone::BeginPlay()
{
	Super::BeginPlay();
	//FVector	vDir = GetCharacterMovement()->Velocity; // �̰͋��� ������ ȸ���� �ϴ°� ����. ������.... �ӵ��� �������°��̱� ������ �̷��°� ����.
}

void AGreystone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector InputVector = fForward + fSide;
	//FRotator TargetRotation;

	//if (InputVector.SizeSquared() > 0.0f)
	//{
	//	TargetRotation = UKismetMathLibrary::MakeRotFromX(InputVector);
	//	GetController()->SetControlRotation(TargetRotation);
	//	UKismetMathLibrary::FindLookAtRotation();
	//}
}

void AGreystone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//void AGreystone::LookUp(float NewAxisValue)
//{
//	AddControllerPitchInput(NewAxisValue);
//}

void AGreystone::NormalAttack()
{
	if (m_AttackEnable)
	{
		// ������ ������ �����ش�.
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

			// ������ ������ �Ѵٰ� �����ش�. ������ �������� ���� �ε����� �Բ� �����ֵ��� �Ѵ�.
			// GP_ATTACK
			/*NetStream	stream;
			uint8	strPacket[PACKET_LENGTH] = {};
			stream.SetBuffer(strPacket);
			stream.Write(&vPos, sizeof(FVector));

			FRotator	Rot = GetActorRotation();
			stream.Write(&Rot, sizeof(FRotator));

			stream.Write(&vDir, sizeof(FVector));

			float Dir = m_Anim->GetDir();
			stream.Write(&Dir, sizeof(float));

			m_vPrevPos = vPos;

			PacketSession* Session = GameServerManager::GetInst()->GetSession();
			Session->Write(GP_ATTACK, stream.GetLength(), strPacket);*/
		}
	}
}

void AGreystone::Hit()
{
	m_pAnim->ChangeAnimation(EPlayerAnim::Hit);
}

void AGreystone::Death()
{
	m_pAnim->ChangeAnimation(EPlayerAnim::Death);
}

void AGreystone::SkillAttack1()
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
		{
			return;
		}

		// ��Ÿ�ִ� �迭�� ���ְ� �� �迭�� �ε������� ���ִ� ��Ÿ�ֵ��� � ��ų�� ������� �̹�
		// ������ �ִ�. �׷��Ƿ� ��ų ������ Ȱ���Ͽ� � ��ų���� �Ǵ��ϰ� �����Ų��.

		const FSkillAnimationInfo* pGreystoneSkill = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetGreystoneAnim(SlotInfo->strName);

		if (!pGreystoneSkill)
		{
			return;
		}

		if (!m_Anim->Montage_IsPlaying(pGreystoneSkill->Animation))
		{
			m_eAttackType = ATTACK_TYPE::Skill1;
			pGreystoneSkill->Animation->BlendIn.SetBlendTime(0.1f);
			pGreystoneSkill->Animation->BlendOut.SetBlendTime(0.1f);
			m_Anim->SetAttackEnable(true);
			m_Anim->Montage_Play(pGreystoneSkill->Animation);

			m_SkillEnable = false;
			m_AttackEnable = false;
		}
	}
}

void AGreystone::SkillAttack2()
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

		const FSkillAnimationInfo* pGreystoneSkill = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetGreystoneAnim(SlotInfo->strName);

		if (!pGreystoneSkill)
			return;

		if (!m_Anim->Montage_IsPlaying(pGreystoneSkill->Animation))
		{
			m_eAttackType = ATTACK_TYPE::Skill2;
			pGreystoneSkill->Animation->BlendIn.SetBlendTime(0.1f);
			pGreystoneSkill->Animation->BlendOut.SetBlendTime(0.1f);
			m_Anim->SetAttackEnable(true);
			m_Anim->Montage_Play(pGreystoneSkill->Animation);

			m_SkillEnable = false;
			m_AttackEnable = false;
		}
	}
}

void AGreystone::SkillAttack3()
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

		const FSkillAnimationInfo* pGreystoneSkill = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetGreystoneAnim(SlotInfo->strName);

		if (!pGreystoneSkill)
			return;

		if (!m_Anim->Montage_IsPlaying(pGreystoneSkill->Animation))
		{
			m_eAttackType = ATTACK_TYPE::Skill3;
			pGreystoneSkill->Animation->BlendIn.SetBlendTime(0.1f);
			pGreystoneSkill->Animation->BlendOut.SetBlendTime(0.1f);
			m_Anim->SetAttackEnable(true);
			m_Anim->Montage_Play(pGreystoneSkill->Animation);

			m_SkillEnable = false;
			m_AttackEnable = false;
		}
	}
}

void AGreystone::SkillAttack4()
{
	if (m_bDeath)
		return;

	if (m_SkillEnable)
	{
		ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
		UGameWidget* pGameWidget = pGameMode->GetGameWidget();
		class USlotListWidget* pSlotList = pGameWidget->GetSlotWidget();
		SkillInfo* SlotInfo = pSlotList->GetSlotSkillInfo(3);

		if (!SlotInfo)
			return;

		const FSkillAnimationInfo* pGreystoneSkill = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetGreystoneAnim(SlotInfo->strName);

		if (!pGreystoneSkill)
			return;

		if (!m_Anim->Montage_IsPlaying(pGreystoneSkill->Animation))
		{
			m_eAttackType = ATTACK_TYPE::Skill4;
			pGreystoneSkill->Animation->BlendIn.SetBlendTime(0.1f);
			pGreystoneSkill->Animation->BlendOut.SetBlendTime(0.1f);
			m_Anim->SetAttackEnable(true);
			m_Anim->Montage_Play(pGreystoneSkill->Animation);

			m_SkillEnable = false;
			m_AttackEnable = false;
		}
	}
}

void AGreystone::NormalAttackHitEffect()
{

}

void AGreystone::AttackEnd()
{
	if (m_bDeath)
		return;
	Super::AttackEnd();
}

void AGreystone::AttackDamage()
{
	FVector		SwordTopLoc;
	FRotator	SwordTopRot;
	FVector		SwordTopScale = {2.0,2.0,2.0};

	SwordTopLoc = GetMesh()->GetSocketLocation(TEXT("spine_01"));

	FActorSpawnParameters	param;

	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Muzzle ��ġ�� ����ü�� �����Ѵ�.
	//AProjectileNiagara* Projectile = GetWorld()->SpawnActor<AProjectileNiagara>(AProjectileNiagara::StaticClass(),
	//	SwordTopLoc, GetActorRotation(), param);

	//if (m_AttackIndex == 1)
	//{
	//	Projectile->SetNiagaraSystem(TEXT("NiagaraSystem'/Game/sA_StylizedSwordSet/Fx/NS_Slash_UptoDown_Lv2.NS_Slash_UptoDown_Lv2'"));
	//}
	//else if (m_AttackIndex == 2)
	//{
	//	Projectile->SetNiagaraSystem(TEXT("NiagaraSystem'/Game/sA_StylizedSwordSet/Fx/NS_Slash_UptoDown_Lv2.NS_Slash_UptoDown_Lv2'"));
	//}
	//else if (m_AttackIndex == 0)
	//{
	//	SwordTopScale = { 4.0, 4.0, 4.0 };
	//	Projectile->SetNiagaraSystem(TEXT("NiagaraSystem'/Game/sA_StylizedSwordSet/Fx/NS_Buff_Lv2.NS_Buff_Lv2'"));
	//}
	////Projectile->SetSpeed(1000.f);
	//Projectile->SetDir(GetActorForwardVector() * 150.f);
	//Projectile->SetActorScale3D(SwordTopScale);
	//Projectile->SetDist(150);
	//Projectile->SetOwnerActor(this);

	Super::AttackDamage();
}

void AGreystone::Skill1Damage()
{
	FVector	Loc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();
	FCollisionQueryParams	param(NAME_None, false, this);
	param.bReturnPhysicalMaterial = true;
	TArray<FHitResult>	resultArray;

	float	fAttackDist = 400.f;

	bool	bHit = GetWorld()->SweepMultiByChannel(resultArray, Loc, Loc + Forward * fAttackDist, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4, FCollisionShape::MakeSphere(300.f), param);

#if ENABLE_DRAW_DEBUG
	FColor	DrawColor = bHit ? FColor::Red : FColor::Purple;
	FQuat Qut = FRotationMatrix::MakeFromZ(Forward).ToQuat();
	//ĸ���� �⺻������ �� �ִ� �����̱� ������ �̰��� Forward(X) �� �������� ȸ����Ű�� �ʹٸ� FRotationMatrix::MakeFromZ �Լ��� ����մϴ�.
	DrawDebugCapsule(GetWorld(), Loc + Forward * fAttackDist / 2.f, fAttackDist, 300.f, Qut, DrawColor, false, 5.f);
	//DrawDebugSphere(GetWorld(), Loc + Forward * AttackDist / 2.f, AttackDist, 300.f, DrawColor, false, 5.f);
	//DrawDebugCircle(GetWorld(), Loc + Forward * AttackDist / 2.f, 200, 50, DrawColor, true, 2.f, 0, 10);
	//DrawDebugBox(GetWorld(), Loc + Forward * fAttackDist / 2.f, FVector(200, 200, 200), DrawColor, true, 1.f, 0, 5);

#endif
	if (bHit)
	{
		int32 iMonsterNumber = 0;
		int32 iAttackDamage = 0;

		for (auto result : resultArray)
		{
			int32 MonsterCount = resultArray.Num();

			AMonster* monster = (AMonster*)result.GetActor();
			iMonsterNumber = monster->GetMonsterNumber();
			iAttackDamage = tCharacterInfo.iAttack;
			// �÷��̾� ��ų������ŭ ���ҽ�Ű��. AddMP(30);

			NetStream stream;
			uint8	packet[PACKET_LENGTH] = {};
			stream.SetBuffer(packet);
			stream.Write(&iMonsterNumber, sizeof(int32));
			stream.Write(&iAttackDamage, sizeof(int32));
			PacketSession* pSession = GameServerManager::GetInst()->GetSession();
			pSession->Write(GP_PLAYERATTACKDAMAGE, stream.GetLength(), packet);
		}
	}
}

void AGreystone::Skill2Damage()
{
	FVector	Loc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();
	FCollisionQueryParams	param(NAME_None, false, this);
	param.bReturnPhysicalMaterial = true;
	TArray<FHitResult>	resultArray;

	float	fAttackDist = 600.f;

	bool	bHit = GetWorld()->SweepMultiByChannel(resultArray, Loc, Loc + Forward * fAttackDist, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4, FCollisionShape::MakeSphere(600.f), param);

#if ENABLE_DRAW_DEBUG
	FColor	DrawColor = bHit ? FColor::Red : FColor::Purple;
	FQuat Qut = FRotationMatrix::MakeFromZ(Forward).ToQuat();
	//ĸ���� �⺻������ �� �ִ� �����̱� ������ �̰��� Forward(X) �� �������� ȸ����Ű�� �ʹٸ� FRotationMatrix::MakeFromZ �Լ��� ����մϴ�.
	// HalfHeight = ���� ����, Radius = ������
	DrawDebugCapsule(GetWorld(), Loc + Forward * fAttackDist / 2.f, 600, 600.f, Qut, DrawColor, false, 5.f);
	//DrawDebugSphere(GetWorld(), Loc + Forward * AttackDist / 2.f, AttackDist, 300.f, DrawColor, false, 5.f);
	//DrawDebugCircle(GetWorld(), Loc + Forward * AttackDist / 2.f, 200, 50, DrawColor, true, 2.f, 0, 10);
	//DrawDebugBox(GetWorld(), Loc + Forward * fAttackDist / 2.f, FVector(200, 200, 200), DrawColor, true, 1.f, 0, 5);

	// TakeDamage(������ ������, ������ ����, ���ݸ�� ������, ������ ������ ���� ����� ����) �� �̿��Ͽ� �α׿� ��½��Ѻ���.
	// TakeDamage();

#endif
	if (bHit)
	{
		int32 iMonsterNumber = 0;
		int32 iAttackDamage = 0;

		for (auto result : resultArray)
		{
			int32 MonsterCount = resultArray.Num();

			AMonster* monster = (AMonster*)result.GetActor();
			if (!monster)
				return;
			iMonsterNumber = monster->GetMonsterNumber();
			iAttackDamage = tCharacterInfo.iAttack;

			NetStream stream;
			uint8	packet[PACKET_LENGTH] = {};
			stream.SetBuffer(packet);
			stream.Write(&iMonsterNumber, sizeof(int32));
			stream.Write(&iAttackDamage, sizeof(int32));
			PacketSession* pSession = GameServerManager::GetInst()->GetSession();
			pSession->Write(GP_PLAYERATTACKDAMAGE, stream.GetLength(), packet);

		}
	}
}

void AGreystone::Skill3Damage()
{

}

void AGreystone::Skill4Damage()
{
}
