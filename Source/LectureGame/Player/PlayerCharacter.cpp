// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "PlayerAnim.h"
#include "../ChattingServerManager.h"
#include "../NetworkManager.h"
#include "../NetSession.h"
#include "../PacketSession.h"
#include "../NetStream.h"
#include "../GameMode/LectureGameGameModeBase.h"
#include "../GameServerManager.h"
#include "Components/WidgetComponent.h"
#include "../UI/HPBarWidget.h"
#include "DrawDebugHelpers.h"
#include "../Monster/Monster.h"
#include "../UI/GameWidget.h"
#include "../UI/MinimapWidget.h"
#include "../Etc/DamageText.h"
#include "../MainGameInstance.h"
#include "../UI/SlotListItemWidget.h"
#include "particles/PxParticleSystem.h"
#include "../Projectile/ProjectilePaticle.h"
#include "../UI/ItemBox.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));

	Arm->SetupAttachment(GetMesh());
	HPWidget->SetupAttachment(GetMesh());

	Arm->bInheritYaw = false;
	Camera->SetupAttachment(Arm);

	static ConstructorHelpers::FClassFinder<UHPBarWidget>	HPBarClass(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));

	if (HPBarClass.Succeeded())
		HPWidget->SetWidgetClass(HPBarClass.Class);

	HPWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HPWidget->SetDrawSize(FVector2D(200.f, 40.f));

	bMove = false;
	m_IsGroup = false;
	m_IsGroupAdmin = false;

	m_MoveKey = false;
	m_AttackIndex = 0;
	m_AttackSendIndex = 0;
	m_AttackEnable = true;
	m_SkillEnable = true;
	m_bDeath = false;

	m_eMoveType = MOVE_TYPE::None;
	m_eAttackType = ATTACK_TYPE::None;
	fMovePacketTime = 0.1f;
	fMovePacketTimeAcc = 0.f;

	m_fMoveTimeCheck = 0.f;
	m_fMoveDistanceCheck = 0.f;

	m_MonsterDetectTime = 0.f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_ChangeMoveDirTime = 0.f;
	m_ChangeMoveDirTimeMax = 0.1f;
	m_ChangeMove = true;

	m_PrevMoveDir = FVector(0.f, 0.f, 0.f);
}

void APlayerCharacter::SetCharacterInfo(_tagCharacterInfo* pCharacterInfo)
{
	FMemory::Memcpy(&tCharacterInfo, pCharacterInfo, sizeof(_tagCharacterInfo));

	GetCharacterMovement()->MaxWalkSpeed = tCharacterInfo.fMoveSpeed;
	//GetCharacterMovement()->MaxWalkSpeed = 2000;
	GetCharacterMovement()->JumpZVelocity = 650;

	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());

	if (pGameMode)
	{
		UGameWidget* GameWidget = pGameMode->GetGameWidget();

		GameWidget->SetExpPercent(tCharacterInfo.iExp / (float)tCharacterInfo.iExpMax);
	}
}

bool APlayerCharacter::AddHP(int32 damage)
{
	bool	result = false;

	tCharacterInfo.iHP -= damage;

	if (damage > 0)
	{
		/*FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ADamageText* DmgText = GetWorld()->SpawnActor<ADamageText>(ADamageText::StaticClass(), GetActorLocation(),
			FRotator::ZeroRotator, param);

		DmgText->SetDamage(damage);*/
	}

	if (tCharacterInfo.iHP <= 0)
	{
		tCharacterInfo.iHP = 0;
		result = true;
	}

	else
	{
		m_Anim->Hit();
	}

	HPBar->SetHPPercent(tCharacterInfo.iHP / (float)tCharacterInfo.iHPMax);

	return result;
}
bool APlayerCharacter::AddMP(int32 mana)
{
	tCharacterInfo.iMP -= mana;
	return true;
}

void APlayerCharacter::SetHP()
{
	HPBar->SetHPPercent(tCharacterInfo.iHP / (float)tCharacterInfo.iHPMax);
}

void APlayerCharacter::SetMP()
{
	HPBar->SetMPPercent(tCharacterInfo.iMP / (float)tCharacterInfo.iMPMax);
}

void APlayerCharacter::CallbackHPBarPlayer()
{
	//FString UserName = ANSI_TO_TCHAR(NetworkManager::GetInst()->GetName());
	FString UserName = ANSI_TO_TCHAR(tCharacterInfo.strName);
	HPBar->SetCharacterName(UserName);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_Anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());

	bMove = false;
	fMovePacketTime = 0.1f;
	fMovePacketTimeAcc = 0.f;
	m_eMoveType = MOVE_TYPE::None;
	m_eAttackType = ATTACK_TYPE::None;

	HPBar = Cast<UHPBarWidget>(HPWidget->GetUserWidgetObject());
	HPBar->SetBeginCallback<APlayerCharacter>(this, &APlayerCharacter::CallbackHPBarPlayer);
	
	NetStream	stream;
	uint8	packet[PACKET_LENGTH] = {};
	stream.SetBuffer(packet);
	PacketSession* pSession = GameServerManager::GetInst()->GetSession();

	pSession->Write(GP_SkillList, stream.GetLength(), packet);
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// ���� �ν��Ͻ��� ������ �����صд�.
	UMainGameInstance* GameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	// �׷��ٸ� Player�� �����ν��Ͻ� �� ���� ������ �Ǿ��ִµ�?
	GameInst->SetCharacterInfo(tCharacterInfo);

	for (auto Info : m_SkillInfo)
	{
		delete Info;
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveFront"), this, &APlayerCharacter::MoveFront);
	PlayerInputComponent->BindAxis(TEXT("MoveSide"), this, &APlayerCharacter::MoveSide);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::JumpKey);

	PlayerInputComponent->BindAction(TEXT("Skill_1"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::Skill1Key);
	PlayerInputComponent->BindAction(TEXT("Skill_2"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::Skill2Key);
	PlayerInputComponent->BindAction(TEXT("Skill_3"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::Skill3Key);
	PlayerInputComponent->BindAction(TEXT("Skill_4"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::Skill4Key);
	PlayerInputComponent->BindAction(TEXT("Item_1"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::Item1Key);
	PlayerInputComponent->BindAction(TEXT("Item_2"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::Item2Key);
}

void APlayerCharacter::MoveFront(float fScale)
{
	FVector	Dir = GetActorForwardVector();

	AddMovementInput(Dir, fScale);

	if (fScale == 0.f)
	{
		m_MoveKey = false;
		m_Anim->SetDir(0.f);
	}

	else if (fScale == 1.f)
	{
		m_MoveKey = true;
		m_Anim->SetDir(0.f);
	}

	else
	{
		m_MoveKey = true;
		m_Anim->SetDir(180.f);
	}
}

void APlayerCharacter::MoveSide(float fScale)
{
	AddMovementInput(GetActorRightVector(), fScale);

	float	Dir = m_Anim->GetDir();

	if (fScale == 1.f)
	{
		if (m_MoveKey)
		{
			// ������ ��
			if (Dir == 0.f)
				m_Anim->SetDir(45.f);

			// ������ �Ʒ�
			else
				m_Anim->SetDir(135.f);
		}

		else
		{
			m_Anim->SetDir(90.f);
		}
		m_MoveKey = true;
	}

	else if (fScale == -1.f)
	{
		if (m_MoveKey)
		{
			// ���� ��
			if (Dir == 0.f)
				m_Anim->SetDir(-45.f);

			// ���� �Ʒ�
			else
				m_Anim->SetDir(-135.f);
		}

		else
		{
			m_Anim->SetDir(-90.f);
		}
		m_MoveKey = true;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Ŭ���̾�Ʈ�� �����°��� 3�����̴�.
	// 1. �̵��� �����Ҷ� ��ġ�� ������ ������.
	// 2. �̵� ������ �ٲ� ��ġ�� ������ ������.
	// 3. �̵��� ������ ��ġ�� ������ ������.

	/*
	����
	1. �̵��� �����Ҷ� ��ġ�� ������ ������.
	2. �̵� ������ �ٲ� ��ġ�� ������ ������.
	3. �̵��� ������ ��ġ�� ������ ������.
	4. ���� �ð��� �ѹ��� ��ġ�� ������ Ŭ���̾�Ʈ��
	��ġ�� ���Ͽ� �ʹ� ���� ���̰� �� ��� ������ �� �ְ� �Ѵ�.
	*/

	// ������ �ٲ������ ã�ƾ� �Ѵ�.


	// Velocity�� �ӵ������̴�.
	float MovementComponetSpeed = GetCharacterMovement()->Velocity.Size();

	if (MovementComponetSpeed > 0.f && !m_bDeath)
	{
		// �����̱� �����Ҷ�

		//PrintViewport(2.f, FColor::Orange, FString::Printf(TEXT("MovementComponent Speed : %.5f"), MovementComponetSpeed));

		if (!bMove)
		{
			bMove = true;
			m_eMoveType = MOVE_TYPE::MoveStart;
			fMovePacketTimeAcc = 0.f;
			// �̵� ������ ��Ŷ�� �ٷ� ������.
			// ��ġ������ ���´�.
			FVector	vPos = GetActorLocation();
			FVector	vDir = GetCharacterMovement()->Velocity;
			vDir.Normalize();
			
			NetStream	stream;
			uint8	strPacket[PACKET_LENGTH] = {};
			stream.SetBuffer(strPacket);
			stream.Write(&vPos, sizeof(FVector));

			FRotator	Rot = GetActorRotation();
			stream.Write(&Rot, sizeof(FRotator));

			stream.Write(&vDir, sizeof(FVector));

			float Dir = m_Anim->GetDir();
			stream.Write(&Dir, sizeof(float));


			m_PrevMoveDir	= vDir;
			m_vPrevPos		= vPos;

			PacketSession* Session = GameServerManager::GetInst()->GetSession();
			Session->Write(GP_MOVESTART, stream.GetLength(), strPacket);
		}

		// �����̰� ������ ������ ��ȯ���� ��� ��Ŷ�� ������. ��, �ʹ� ���� ������ �ȵǱ� ������ 0.1�ʿ� 1�� ������ �����Ѵ�.
		else
		{
			if (!m_ChangeMove)
			{
				m_ChangeMoveDirTime += DeltaTime;

				if (m_ChangeMoveDirTime >= m_ChangeMoveDirTimeMax)
				{
					m_ChangeMoveDirTime -= m_ChangeMoveDirTimeMax;

					m_ChangeMove = true;
				}
			}

			// ������ �ٲ������ �Ǵ��Ѵ�.
			FVector	vDir = GetCharacterMovement()->Velocity;
			vDir.Normalize();

			if (m_ChangeMove)
			{

				//if (m_PrevMoveDir != vDir)
				{
					// ������ ��ġ�� �̵� ������ ��Ŷ���� ����� �����ش�.
					FVector	vPos = GetActorLocation();

					NetStream	stream;
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
					Session->Write(GP_MOVE, stream.GetLength(), strPacket);

					m_ChangeMove = false;
				}
			}

			m_PrevMoveDir = vDir;
		}
	}

	else
	{
		// �����̴ٰ� ����ٴ� ���̴�.
		if (bMove)
		{
			bMove = false;
			m_eMoveType = MOVE_TYPE::MoveEnd;

			// �̵� ���ᵵ ��Ŷ�� �ٷ� ������.
			FVector	vPos = GetActorLocation();

			NetStream	stream;
			uint8	strPacket[PACKET_LENGTH] = {};
			stream.SetBuffer(strPacket);

			stream.Write(&vPos, sizeof(FVector));

			FRotator	Rot = GetActorRotation();
			stream.Write(&Rot, sizeof(FRotator));

			PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("MoveEnd Pos : %s"), *vPos.ToString()));

			PacketSession* Session = GameServerManager::GetInst()->GetSession();
			Session->Write(GP_MOVEEND, stream.GetLength(), strPacket);
			fMovePacketTimeAcc = 0.f;

			m_fMoveTimeCheck = 0.f;
			m_fMoveDistanceCheck = 0.f;

			m_PrevMoveDir = FVector(0.f, 0.f, 0.f);
			m_ChangeMove = false;
			m_ChangeMoveDirTime = 0.f;
		}
		// ��� ��������
		else
		{
			m_eMoveType = MOVE_TYPE::None;
			fMovePacketTimeAcc = 0.f;
		}
	}
	if (m_eAttackType != ATTACK_TYPE::None)
	{
		NetStream	stream;
		uint8	strPacket[PACKET_LENGTH] = {};
		stream.SetBuffer(strPacket);

		stream.Write(&m_eAttackType, sizeof(ATTACK_TYPE));

		switch (m_eAttackType)
		{
		case ATTACK_TYPE::NormalAttack:
			stream.Write(&m_AttackSendIndex, sizeof(int32));
			break;
		case ATTACK_TYPE::Skill1:
			break;
		case ATTACK_TYPE::Skill2:
			break;
		case ATTACK_TYPE::Skill3:
			break;
		}

		float Dir = m_Anim->GetDir();
		stream.Write(&Dir, sizeof(float));

		PacketSession* Session = GameServerManager::GetInst()->GetSession();
		Session->Write(GP_ATTACK, stream.GetLength(), strPacket);

		m_eAttackType = ATTACK_TYPE::None;
	}

	m_MonsterDetectTime += DeltaTime;
	// �̴ϸ� üũ�� �ϱ����� �뵵
	if (m_MonsterDetectTime >= 0.2f)
	{
		m_MonsterDetectTime -= 0.2f;

		ALectureGameGameModeBase* GameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
		UMinimapWidget* Minimap = GameMode->GetGameWidget()->GetMinimapWidget();

		Minimap->ClearMonster();

		// �ֺ��� ���Ͱ� �ִ��� Ž���Ѵ�.
		// �浹�� ó���� �ϴϱ� Remove / Destroy ���� �ʴ� �̻� ���ܵ� �浹ó�� �ɰ��̴�.
		TArray<FHitResult>	resultArray;
		FCollisionQueryParams	param(NAME_None, false, this);

		bool Collision = GetWorld()->SweepMultiByChannel(resultArray, GetActorLocation(), GetActorLocation(), FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel6, FCollisionShape::MakeSphere(15000.f), param);

		if (Collision)
		{
			FVector	Loc = GetActorLocation();
			Loc.Z = 0.f;

			float	Ratio = 300.f / 30000.f;

			for (auto& result : resultArray)
			{
				// �÷��̾ �߽����� �� ��ġ�� ���Ѵ�.

				FVector	Target = result.GetActor()->GetActorLocation();
				Target.Z = 0.f;

				FVector	Dist = Target - Loc;
				Dist *= Ratio;

				Dist.X += 150.f;
				Dist.Y += 150.f;

				Minimap->AddMonster(Dist);

			}
		}
	}
}

void APlayerCharacter::Hit()
{
}
void APlayerCharacter::Death()
{
}

void APlayerCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void APlayerCharacter::LookUp(float NewAxisValue)
{
	//PrintViewport(1.f, FColor::Silver, FString::Printf(TEXT("LookUp Access")));
}

void APlayerCharacter::AttackDamage()
{
	FVector	Loc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();

	// PlayerAttack : ECC_GameTraceChannel4
	// EnemyAttack : ECC_GameTraceChannel5
	FCollisionQueryParams	param(NAME_None, false, this);

	param.bReturnPhysicalMaterial = true;

	TArray<FHitResult>	resultArray;

	float	AttackDist = 350.f;

	bool	bHit = GetWorld()->SweepMultiByChannel(resultArray, Loc, Loc + Forward * AttackDist, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4, FCollisionShape::MakeSphere(100.f), param);

	// �ֺ� ���� �ݰ� �ȿ� ���� ��ü �浹�غ���.
	/*
	bool	Hit = GetWorld()->SweepMultiByChannel(resultArray, Loc, Loc, FQuat::Identity,
	ECollisionChannel::ECC_GameTraceChannel4, FCollisionShape::MakeSphere(AttackDist), param);
	*/

#if ENABLE_DRAW_DEBUG
	FColor	DrawColor = bHit ? FColor::Red : FColor::Green;
	FQuat	Qut = FRotationMatrix::MakeFromZ(Forward).ToQuat();
	// ĸ���� �⺻������ �� �ִ� �����̱� ������ �̰��� Forward(X) �� �������� ȸ����Ű�� �ʹٸ� FRotationMatrix::MakeFromZ �Լ��� ����մϴ�.
	DrawDebugCapsule(GetWorld(), Loc + Forward * AttackDist / 2, AttackDist, 100.f, Qut, DrawColor, false, 2.f);

	// ���ݻ�Ÿ��� DB�� �ִ� Player����ü�� �־�δ°��� ���? ��Ÿ����� ������ �����ѵ�

#endif
	if (bHit)   
	{    
		NetStream	stream;
		uint8	packet[PACKET_LENGTH] = {};
		stream.SetBuffer(packet);

		AMonster* Monster = (AMonster*)(*resultArray.begin()).GetActor();
		int32 iMonsterNumber = Monster->GetMonsterNumber();
		int32 iAttackDamage = tCharacterInfo.iAttack;

		stream.Write(&iMonsterNumber, sizeof(int32));
		stream.Write(&iAttackDamage, sizeof(int32));

		PacketSession* Session = GameServerManager::GetInst()->GetSession();
		Session->Write(GP_PLAYERATTACKDAMAGE, stream.GetLength(), packet);
	}

	// ������ �Ϲ� �������� �������� ������ �� �ְ� �Ѵ�.
	// GP_PLAYERATTACKDAMAGE�� �������ݷ� ��Ŷ�� ������ ���͸� �����ϰ� �ϰ�
	// ���Ͱ� ���ݹ����� �� Ŭ���̾�Ʈ�� �ش� ���� ������ �����ϵ��� ��Ŷ�� ������.
	// ���� ���⼭ �浹ó���� �����Ѵ�.
}

void APlayerCharacter::AttackEnd()
{
	m_AttackIndex = 0;
	m_AttackEnable = true;
	m_SkillEnable = true;
}

void APlayerCharacter::JumpKey()
{
	if (m_bDeath)
		return;

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		Jump();

		m_Anim->ChangeAnimation(EPlayerAnim::Jump);
	}
}

void APlayerCharacter::PlayFallRecovery()
{
	if (!m_FallRecoveryMontage)
		return;

	if (!m_Anim->Montage_IsPlaying(m_FallRecoveryMontage))
	{
		m_Anim->Montage_SetPosition(m_FallRecoveryMontage, 0.f);

		m_FallRecoveryMontage->BlendIn.SetBlendTime(0.1f);
		m_FallRecoveryMontage->BlendOut.SetBlendTime(0.1f);

		m_Anim->Montage_Play(m_FallRecoveryMontage);
	}
}

void APlayerCharacter::Skill1Key()
{
	SkillAttack1();
}

void APlayerCharacter::Skill2Key()
{
	SkillAttack2();
}

void APlayerCharacter::Skill3Key()
{
	SkillAttack3();
}

void APlayerCharacter::Skill4Key()
{
	SkillAttack4();
}

void APlayerCharacter::Item1Key()
{
	Item1();
}

void APlayerCharacter::Item2Key()
{
	Item2();
}

void APlayerCharacter::NormalAttack()
{
}
void APlayerCharacter::SkillAttack1()
{
}
void APlayerCharacter::SkillAttack2()
{
}
void APlayerCharacter::SkillAttack3()
{
}
void APlayerCharacter::SkillAttack4()
{
}
void APlayerCharacter::Levelup()
{
	FVector Loc = GetMesh()->GetSocketLocation(TEXT("FX_Root"));

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AProjectilePaticle* LevelUp1 = GetWorld()->SpawnActor<AProjectilePaticle>(AProjectilePaticle::StaticClass(),
		Loc, GetActorRotation(), param);

	LevelUp1->SetPaticleSystem(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/SharedGameplay/States/LevelUp/P_LevelUp.P_LevelUp'"));
	LevelUp1->SetPaticleScale(FVector(3.f, 3.f, 3.f));
	LevelUp1->PlayPaticle();

	AProjectilePaticle* LevelUp2 = GetWorld()->SpawnActor<AProjectilePaticle>(AProjectilePaticle::StaticClass(),
		Loc, GetActorRotation(), param);

	LevelUp2->SetPaticleSystem(TEXT("ParticleSystem'/Game/ParagonGreystone/FX/Particles/Greystone/Abilities/Ultimate/FX/P_Greystone_HToKill_Start.P_Greystone_HToKill_Start'"));
	LevelUp2->PlayPaticle();
}
void APlayerCharacter::PotionEffect()
{
	FVector Loc = GetMesh()->GetSocketLocation(TEXT("FX_Root"));

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AProjectilePaticle* Projectile = GetWorld()->SpawnActor<AProjectilePaticle>(AProjectilePaticle::StaticClass(),
		Loc, GetActorRotation(), param);

	Projectile->SetPaticleSystem(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ability/Heal/P_Heal_LongDistance_Target.P_Heal_LongDistance_Target'"));
	Projectile->SetPaticleScale(FVector(2.5f, 2.5f, 2.5f));
	Projectile->PlayPaticle();
}
void APlayerCharacter::NormalAttackHitEffect()
{
}
void APlayerCharacter::Skill1Damage()
{
}
void APlayerCharacter::Skill2Damage()
{
}
void APlayerCharacter::Skill3Damage()
{
}
void APlayerCharacter::Skill4Damage()
{
}
void APlayerCharacter::Item1()
{
	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	USlotListItemWidget* pSlotItemList = pGameMode->GetGameWidget()->GetSlotItemWidget();

	if (pSlotItemList)
	{
		UItemData* pItem = pSlotItemList->GetSlotItemInfo(0);
		if (pItem)
		{
			if (pItem->GetItemName().Compare(FString(TEXT("HP_Potion"))) == 0)
			{
				if (tCharacterInfo.iHP < tCharacterInfo.iHPMax)
				{
					int32 iOptionCount = pItem->GetOptionCount();
					for (int32 i = 0; i < iOptionCount; ++i)
					{
						FItemOptionData OptionData = pItem->GetOption(i);

						switch (OptionData.Type)
						{
						case IOT_ATTACK:
						{
							int iAttackOption = OptionData.Option;
							tCharacterInfo.iAttack += iAttackOption;
						}
							break;
						case IOT_ARMOR:
						{
							int iArmorOption = OptionData.Option;
							tCharacterInfo.iArmor += iArmorOption; 
						}
							break;
						case IOT_HPRECOVERY:
						{
							int iHpOption = OptionData.Option;
							tCharacterInfo.iHP += iHpOption;
							if (tCharacterInfo.iHP > tCharacterInfo.iHPMax)
								tCharacterInfo.iHP = tCharacterInfo.iHPMax;

							HPBar->SetHPPercent(tCharacterInfo.iHP / (float)tCharacterInfo.iHPMax);

							PotionEffect();
						}
							break;
						case IOT_MPRECOVERY:
						{	int iMpOption = OptionData.Option;
							tCharacterInfo.iMP += iMpOption;
							if (tCharacterInfo.iMP > tCharacterInfo.iMPMax)
								tCharacterInfo.iMP = tCharacterInfo.iMPMax;
						}
							break;
						case IOT_HP:
						{
							int iHpMaxOption = OptionData.Option;
							tCharacterInfo.iHPMax += iHpMaxOption;

							HPBar->SetHPPercent(tCharacterInfo.iHP / (float)tCharacterInfo.iHPMax);

							PotionEffect();
						}
							break;
						case IOT_MP:
						{	int iMpMaxOption = OptionData.Option;
							tCharacterInfo.iMPMax += iMpMaxOption;

							//HPBar->SetHPPercent(tCharacterInfo.iHP / (float)tCharacterInfo.iHPMax);
						}	
						break;
						case IOT_CRITICALRATIO:
							break;
						case IOT_CRITICALDAMAGE:
							break;
						case IOT_MOVESPEED:
						{
							int iSpeedOption = OptionData.Option;
							tCharacterInfo.fMoveSpeed += iSpeedOption; 
						}
							break;
						}
					}
				}
				else
				{
					PrintViewport(5.f, FColor::Red, TEXT("full HP"));
				}
			}
		}
	}
}
void APlayerCharacter::Item2()
{
	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	USlotListItemWidget* pSlotItemList = pGameMode->GetGameWidget()->GetSlotItemWidget();

	if (pSlotItemList)
	{
		UItemData* pItem = pSlotItemList->GetSlotItemInfo(1);
		if (pItem)
		{
			if (pItem->GetItemName().Compare(FString(TEXT("HP_Potion"))) == 0)
			{
				if (tCharacterInfo.iHP < tCharacterInfo.iHPMax)
				{
					int32 iOptionCount = pItem->GetOptionCount();
					for (int32 i = 0; i < iOptionCount; ++i)
					{
						FItemOptionData OptionData = pItem->GetOption(i);

						switch (OptionData.Type)
						{
						case IOT_ATTACK:
						{
							int iAttackOption = OptionData.Option;
							tCharacterInfo.iAttack += iAttackOption;
						}
						break;
						case IOT_ARMOR:
						{
							int iArmorOption = OptionData.Option;
							tCharacterInfo.iArmor += iArmorOption;
						}
						break;
						case IOT_HPRECOVERY:
						{
							int iHpOption = OptionData.Option;
							tCharacterInfo.iHP += iHpOption;
							if (tCharacterInfo.iHP > tCharacterInfo.iHPMax)
								tCharacterInfo.iHP = tCharacterInfo.iHPMax;

							HPBar->SetHPPercent(tCharacterInfo.iHP / (float)tCharacterInfo.iHPMax);

							PotionEffect();
						}
						break;
						case IOT_MPRECOVERY:
						{	int iMpOption = OptionData.Option;
						tCharacterInfo.iMP += iMpOption;
						if (tCharacterInfo.iMP > tCharacterInfo.iMPMax)
							tCharacterInfo.iMP = tCharacterInfo.iMPMax;
						}
						break;
						case IOT_HP:
						{
							int iHpMaxOption = OptionData.Option;
							tCharacterInfo.iHPMax += iHpMaxOption;

							HPBar->SetHPPercent(tCharacterInfo.iHP / (float)tCharacterInfo.iHPMax);

							PotionEffect();
						}
						break;
						case IOT_MP:
						{	int iMpMaxOption = OptionData.Option;
						tCharacterInfo.iMPMax += iMpMaxOption;

						//HPBar->SetHPPercent(tCharacterInfo.iHP / (float)tCharacterInfo.iHPMax);
						}
						break;
						case IOT_CRITICALRATIO:
							break;
						case IOT_CRITICALDAMAGE:
							break;
						case IOT_MOVESPEED:
						{
							int iSpeedOption = OptionData.Option;
							tCharacterInfo.fMoveSpeed += iSpeedOption;
						}
						break;
						}
					}
				}
				else
				{
					PrintViewport(5.f, FColor::Red, TEXT("full HP"));
				}
			}
		}
	}
}
