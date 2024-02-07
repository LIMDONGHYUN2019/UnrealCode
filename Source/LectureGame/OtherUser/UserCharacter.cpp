// Fill out your copyright notice in the Description page of Project Settings.


#include "UserCharacter.h"
#include "UserAnim.h"
#include "Components/WidgetComponent.h"
#include "../NetworkManager.h"
#include "../UI/HPBarWidget.h"
#include "../MainGameInstance.h"
#include "Components/TextBlock.h"

// Sets default values
AUserCharacter::AUserCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Camera Arm의 부모로 플레이어 Mesh를 준다.
	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	//Camera Arm의 부모로 플레이어 Mesh를 준다.
	HPWidget->SetupAttachment(GetMesh());
	static ConstructorHelpers::FClassFinder<UHPBarWidget>	HPBarClass(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));
	if (HPBarClass.Succeeded())
	{
		HPWidget->SetWidgetClass(HPBarClass.Class);
	}
	HPWidget->SetWidgetSpace(EWidgetSpace::World);
	HPWidget->SetDrawSize(FVector2D(200.f, 40.f));

	m_bMove = false;
	m_MoveKey = false;
	m_IsGroup = false;
	m_IsGroupAdmin = false;
	m_AttackIndex = 0;
	m_AttackEnable = true;
	m_bDeath = false;
	m_vOtherPos = FVector::ZeroVector;
	m_vMoveDir = FVector::ZeroVector;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SetActorScale3D(FVector(1.5f, 1.5f, 1.5f));

	//HPWidget->SetRelativeLocation(FVector(0.f, 0.f, 250.f));

	m_fViewDir = 0.f;
}

void AUserCharacter::SetViewMoveSpeed(float MoveSpeed)
{
	m_pAnim->SetMoveSpeed(MoveSpeed);

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	m_pAnim->SetPlayerAnimationType(EPlayerAnimationType::Run);
}

void AUserCharacter::SetViewDir(float Dir)
{
	m_fViewDir = Dir;

	m_pAnim->SetDir(Dir);
}

void AUserCharacter::CallbackHPBarOtherPlayer()
{
	HPBar->SetCharacterName(m_UserName);
}

// Called when the game starts or when spawned
void AUserCharacter::BeginPlay()
{
	Super::BeginPlay();
	m_pAnim = Cast<UUserAnim>(GetMesh()->GetAnimInstance());

	HPBar = Cast<UHPBarWidget>(HPWidget->GetUserWidgetObject());
	HPBar->SetBeginCallback<AUserCharacter>(this, &AUserCharacter::CallbackHPBarOtherPlayer);
}

void AUserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerCameraManager* CameraMgr = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector	CameraLoc = CameraMgr->GetCameraLocation();

	FVector	CameraDir = CameraLoc - HPWidget->GetComponentLocation();
	CameraDir.Normalize();

	// GetComponentLocation : 월드위치를 반환한다.
	FRotator	HPRot = CameraDir.Rotation();
	HPRot.Roll = 0.f;
	HPWidget->SetWorldRotation(HPRot);


	if (m_bMove)
	{
		AddMovementInput(m_vMoveDir, 1.f, true);

		//SetActorLocation(GetActorLocation() + m_vMoveDir * GetCharacterMovement()->MaxWalkSpeed * DeltaTime);
		//PrintViewport(1.f, FColor::Red, m_vMoveDir.ToString());
		LOG(TEXT("User Move Dir : %s Speed : %.5f"), *m_vMoveDir.ToString(), GetCharacterMovement()->MaxWalkSpeed);
	}
}

// Called to bind functionality to input
void AUserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUserCharacter::Move(FVector vPos)
{
	m_vOtherPos = vPos;
	FVector	CurrentPos = GetActorLocation();

	// 위치를 비교하여 많이 차이가 날 경우 서버에서 보내준 정보로 보정한다.
	FVector	Pos1 = m_vOtherPos;
	FVector	Pos2 = CurrentPos;

	Pos1.Z = Pos2.Z;

	float fLength = FVector::Distance(Pos1, Pos2);

	if (fLength >= m_tCharacterInfo.fMoveSpeed * 0.1f)
	{
		SetActorLocation(m_vOtherPos);
	}


	//vPos.Z = CurrentPos.Z;

	//// 서버에서 정상적인 위치를 보내주었을때 현재의 위치와 비교하여 일정 범위 이상을 벗어났을 경우
	//// 클라이언트의 다른유저 위치를 이 정보로 보정해준다.
	//
	////SetActorLocation(vPos);
	//float fLength = FVector::Distance(vPos, CurrentPos);
	//if (fLength > 50.f)
	//{
	//	SetActorLocation(vPos);
	//}
}

void AUserCharacter::MoveEnd()
{
	m_pAnim->ChangeAnimation(EPlayerAnimationType::Idle);
	m_pAnim->SetPlayerAnimationType(EPlayerAnimationType::Idle);
}

void AUserCharacter::NormalAttack(int32 AttackIndex)
{
	//if (m_AttackEnable)
	{
		m_AttackIndex = AttackIndex;
		//m_AttackEnable = false;
		if (!m_pAnim->Montage_IsPlaying(m_AttackMontageArray[m_AttackIndex]))
		{
			PrintViewport(3.f, FColor::Turquoise, FString::Printf(TEXT("Attack Index %d..."), AttackIndex));

			m_AttackMontageArray[m_AttackIndex]->BlendIn.SetBlendTime(0.1f);
			m_AttackMontageArray[m_AttackIndex]->BlendOut.SetBlendTime(0.1f);

			m_pAnim->Montage_Play(m_AttackMontageArray[m_AttackIndex]);
			m_pAnim->SetAttackEnable(true);
		}

		m_pAnim->SetPlayerAnimationType(EPlayerAnimationType::Attack);
	}
}

void AUserCharacter::SkillAttack1()
{
	m_pAnim->ChangeAnimation(EPlayerAnimationType::Skill1);
}

void AUserCharacter::SkillAttack2()
{
	m_pAnim->ChangeAnimation(EPlayerAnimationType::Skill2);
}

void AUserCharacter::JumpKey()
{
	if (m_bDeath)
		return;

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		Jump();
	}
}

void AUserCharacter::PlayFallRecovery()
{
	if (!m_FallRecoveryMontage)
		return;

	if (!m_pAnim->Montage_IsPlaying(m_FallRecoveryMontage))
	{
		m_pAnim->Montage_SetPosition(m_FallRecoveryMontage, 0.f);

		m_FallRecoveryMontage->BlendIn.SetBlendTime(0.1f);
		m_FallRecoveryMontage->BlendOut.SetBlendTime(0.1f);

		m_pAnim->Montage_Play(m_FallRecoveryMontage);
	}
}

void AUserCharacter::Hit()
{
	m_pAnim->ChangeAnimation(EPlayerAnimationType::Hit);
}

void AUserCharacter::Death()
{
	m_pAnim->ChangeAnimation(EPlayerAnimationType::Death);
	m_pAnim->SetPlayerAnimationType(EPlayerAnimationType::Death);
}

bool AUserCharacter::AddHP(int32 Damage)
{
	bool	result = false;

	m_tCharacterInfo.iHP -= Damage;
	Hit();
	PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("OtherUser HP : %d"), m_tCharacterInfo.iHP));
	if (m_tCharacterInfo.iHP <= 0)
	{
		m_tCharacterInfo.iHP = 0;
		result = true;
	}

	HPBar->SetHPPercent(m_tCharacterInfo.iHP / (float)m_tCharacterInfo.iHPMax);

	return result;
}

void AUserCharacter::SetHP()
{
	HPBar->SetHPPercent(m_tCharacterInfo.iHP / (float)m_tCharacterInfo.iHPMax);
}
