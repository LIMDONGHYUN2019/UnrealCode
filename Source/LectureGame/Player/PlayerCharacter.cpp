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

	// 게임 인스턴스에 정보를 저장해둔다.
	UMainGameInstance* GameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	// 그렇다면 Player와 게임인스턴스 두 곳에 저장이 되어있는데?
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
			// 오른쪽 위
			if (Dir == 0.f)
				m_Anim->SetDir(45.f);

			// 오른쪽 아래
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
			// 왼쪽 위
			if (Dir == 0.f)
				m_Anim->SetDir(-45.f);

			// 왼쪽 아래
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

	// 클라이언트가 보내는것은 3가지이다.
	// 1. 이동을 시작할때 위치와 방향을 보낸다.
	// 2. 이동 방향이 바뀔때 위치와 방향을 보낸다.
	// 3. 이동이 끝날때 위치와 방향을 보낸다.

	/*
	서버
	1. 이동을 시작할때 위치와 방향을 보낸다.
	2. 이동 방향이 바뀔때 위치와 방향을 보낸다.
	3. 이동이 끝날때 위치와 방향을 보낸다.
	4. 일정 시간에 한번씩 위치를 보내서 클라이언트의
	위치와 비교하여 너무 많은 차이가 날 경우 갱신할 수 있게 한다.
	*/

	// 방향이 바뀌는지를 찾아야 한다.


	// Velocity는 속도벡터이다.
	float MovementComponetSpeed = GetCharacterMovement()->Velocity.Size();

	if (MovementComponetSpeed > 0.f && !m_bDeath)
	{
		// 움직이기 시작할때

		//PrintViewport(2.f, FColor::Orange, FString::Printf(TEXT("MovementComponent Speed : %.5f"), MovementComponetSpeed));

		if (!bMove)
		{
			bMove = true;
			m_eMoveType = MOVE_TYPE::MoveStart;
			fMovePacketTimeAcc = 0.f;
			// 이동 시작은 패킷을 바로 보낸다.
			// 위치정보를 얻어온다.
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

		// 움직이고 있을때 방향을 전환했을 경우 패킷을 보낸다. 단, 너무 많이 보내면 안되기 때문에 0.1초에 1번 정도로 제한한다.
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

			// 방향이 바뀌었는지 판단한다.
			FVector	vDir = GetCharacterMovement()->Velocity;
			vDir.Normalize();

			if (m_ChangeMove)
			{

				//if (m_PrevMoveDir != vDir)
				{
					// 현재의 위치와 이동 방향을 패킷으로 만들어 보내준다.
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
		// 움직이다가 멈췄다는 것이다.
		if (bMove)
		{
			bMove = false;
			m_eMoveType = MOVE_TYPE::MoveEnd;

			// 이동 종료도 패킷을 바로 보낸다.
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
		// 계속 정지상태
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
	// 미니맵 체크를 하기위한 용도
	if (m_MonsterDetectTime >= 0.2f)
	{
		m_MonsterDetectTime -= 0.2f;

		ALectureGameGameModeBase* GameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
		UMinimapWidget* Minimap = GameMode->GetGameWidget()->GetMinimapWidget();

		Minimap->ClearMonster();

		// 주변에 몬스터가 있는지 탐색한다.
		// 충돌로 처리를 하니까 Remove / Destroy 하지 않는 이상 숨겨도 충돌처리 될것이다.
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
				// 플레이어를 중심으로 한 위치를 구한다.

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

	// 주변 일정 반경 안에 들어온 물체 충돌해보기.
	/*
	bool	Hit = GetWorld()->SweepMultiByChannel(resultArray, Loc, Loc, FQuat::Identity,
	ECollisionChannel::ECC_GameTraceChannel4, FCollisionShape::MakeSphere(AttackDist), param);
	*/

#if ENABLE_DRAW_DEBUG
	FColor	DrawColor = bHit ? FColor::Red : FColor::Green;
	FQuat	Qut = FRotationMatrix::MakeFromZ(Forward).ToQuat();
	// 캡슐은 기본적으로 서 있는 상태이기 때문에 이것을 Forward(X) 축 방향으로 회전시키고 싶다면 FRotationMatrix::MakeFromZ 함수를 사용합니다.
	DrawDebugCapsule(GetWorld(), Loc + Forward * AttackDist / 2, AttackDist, 100.f, Qut, DrawColor, false, 2.f);

	// 공격사거리는 DB에 있는 Player구조체에 넣어두는것이 어떤가? 평타말고는 쓸데가 없긴한데

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

	// 서버에 일반 공격으로 데메지를 전달할 수 있게 한다.
	// GP_PLAYERATTACKDAMAGE이 프로토콜로 패킷을 보내서 몬스터를 공격하게 하고
	// 몬스터가 공격받으면 각 클라이언트에 해당 몬스터 정보를 갱신하도록 패킷을 보낸다.
	// 먼저 여기서 충돌처리를 진행한다.
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
