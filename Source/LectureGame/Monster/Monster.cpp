// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "MonsterAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "../UI/HPBarWidget.h"
#include "Components/TextBlock.h"
#include "../Etc/DamageText.h"

// Sets default values
AMonster::AMonster()
{ 
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	//m_HitColor	= FVec

	m_HitEnable = false;
	m_HitTime = 0.f;
	m_HitTimeMax = 0.3f;

	m_HitColor = FVector(1.f, 0.f, 0.f);
	   
	m_PaperBurnEnable = false;
	m_PaperBurnValue = 0.f;
	m_PaperBurnFilter = 1.f;
	m_PaperBurnTemperature = 4500.f;

	m_PaperBurnTime = 10.f;

}

void AMonster::Move()
{
	m_pAnim->ChangeAnim(MonsterAnim::Run);
	//float fLength = FVector::Distance(m_vMonsterPos, GetActorLocation());
	//if (fLength > 505.f)
	//	SetActorLocation(m_vMonsterPos);
}

void AMonster::MoveEnd()
{
	m_pAnim->ChangeAnim(MonsterAnim::Idle);
}

void AMonster::Attack()
{
	m_pAnim->ChangeAnim(MonsterAnim::Attack);
}

void AMonster::Skill_1()
{
	m_pAnim->ChangeAnim(MonsterAnim::Skill_1);
}

void AMonster::Skill_2()
{
	m_pAnim->ChangeAnim(MonsterAnim::Skill_2);
}

void AMonster::Death()
{
	m_pAnim->ChangeAnim(MonsterAnim::Death);

	m_PaperBurnEnable = true;

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("PaperBurnEnable"), 3.f);

	HPBar->SetVisibility(ESlateVisibility::Collapsed);
	m_bMove = false;
}

void AMonster::Resurrection()
{
	m_pAnim->ChangeAnim(MonsterAnim::Idle);

	m_PaperBurnEnable = false;
	m_PaperBurnValue = 0.f;
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("PaperBurnEnable"), 0.f);
	HPBar->SetVisibility(ESlateVisibility::Visible);
	m_bMove = false;
}

void AMonster::SetHP(const int32 iHP)
{
	m_iHP = iHP;

	HPBar->SetHPPercent(iHP / (float)m_iHPMax);
}

void AMonster::Hit(int32 iDamage)
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADamageText* DmgText = GetWorld()->SpawnActor<ADamageText>(ADamageText::StaticClass(), GetActorLocation(),
		FRotator::ZeroRotator, param);

	DmgText->SetDamage(iDamage);

	m_HitEnable = true;

	m_HitTime = 0.f;

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("HitEnable"), 1.f);

	m_pAnim->Hit();
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	m_pAnim = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());

	HPBar = Cast<UHPBarWidget>(HPWidget->GetUserWidgetObject());

	HPBar->SetBeginCallback<AMonster>(this, &AMonster::BeginHPBarCallback);

	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("HitColor"), m_HitColor);

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("PaperBurnEnable"), 0.f);
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("PaperBurnTemperature"), m_PaperBurnTemperature);
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("PaperBurnFilter"), m_PaperBurnFilter);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 카메라 위치를 얻어와서 HPBar Widget이 카메라를 바라보게 회전을 시켜주어야 한다.
	//UAIBlueprintHelperLibrary::SimpleMoveToLocation();

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
		//RootComponent->AddRelativeLocation(m_vMoveDir * m_fMoveSpeed * DeltaTime);
		//FRotator rDir = vDir.Rotation();
		//GetMesh()->SetRelativeRotation(rDir);
		//SetActorRotation(rDir);
		//FVector	vLoc = GetActorLocation() + m_vMoveDir * m_fMoveSpeed * DeltaTime;
		//FVector	vLoc = m_vMonsterPos;
		// 아마 m_vMoveDir * m_fMoveSpeed * DeltaTime에서 z값이 추가되어 Set할떄 공중에 있는것이 아닌가
		//SetActorLocation(vLoc);
		AddMovementInput(m_vMoveDir);
	}

	if (m_HitEnable)	
	{
		m_HitTime += DeltaTime;

		if (m_HitTime >= m_HitTimeMax)
		{
			m_HitTime = 0.f;
			m_HitEnable = false;

			GetMesh()->SetScalarParameterValueOnMaterials(TEXT("HitEnable"), 0.f);
		}
	}

	if (m_PaperBurnEnable)
	{
		m_PaperBurnValue += DeltaTime / m_PaperBurnTime;
		GetMesh()->SetScalarParameterValueOnMaterials(TEXT("PaperBurn"), m_PaperBurnValue);
	}
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMonster::SaveInfo(FArchive* pArchive)
{
	*pArchive << m_eMonsterType;
	*pArchive << m_iAttack;
	*pArchive << m_iArmor;
	*pArchive << m_iHP;
	*pArchive << m_iHPMax;
	*pArchive << m_iLevel;
	*pArchive << m_iExp;
	*pArchive << m_iGold;
	*pArchive << m_fMoveSpeed;
	*pArchive << m_fTraceRange;
	*pArchive << m_fAttackRange;
}

void AMonster::BeginHPBarCallback()
{
	// 몬스터 이름 띄우기위해

	switch (m_eMonsterType)
	{
	case EMonsterType::Minion:
		HPBar->SetCharacterName(TEXT("Minion"));
		break;
	case EMonsterType::Golem:
		HPBar->SetCharacterName(TEXT("Golem"));
		break;
	case EMonsterType::Troll:
		HPBar->SetCharacterName(TEXT("Troll"));
		break;
	case EMonsterType::Boss:
		HPBar->SetCharacterName(TEXT("Boss"));
		break;
	case EMonsterType::FinalBoss:
		HPBar->SetCharacterName(TEXT("FinalBoss"));
		break;
	case EMonsterType::Sevarog:
		HPBar->SetCharacterName(TEXT("Sevarog"));
		break;
	}
}
