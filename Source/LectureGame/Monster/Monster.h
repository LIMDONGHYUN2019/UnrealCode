// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "MonsterPatrolPoint.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UCLASS()
class LECTUREGAME_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EMonsterType	m_eMonsterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32			m_iAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32			m_iArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32			m_iHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32			m_iHPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32			m_iLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32			m_iExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32			m_iGold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float			m_fMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float			m_fTraceRange;	// 플레이어를 인식할 수 있는 반경

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float			m_fAttackRange;	// 플레이어를 공격하기 위한 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HPWidget;
	class UHPBarWidget* HPBar;

	int32			m_iMonsterNumber;
	class UMonsterAnimInstance* m_pAnim;
	bool			m_bMove;
	FVector			m_vMonsterPos;
	FVector			m_vMoveDir;

	//--------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector	m_HitColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	m_HitTimeMax;

	bool	m_HitEnable;
	float	m_HitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	m_PaperBurnTime;

	bool	m_PaperBurnEnable;
	float	m_PaperBurnValue;    
	float	m_PaperBurnFilter;
	float	m_PaperBurnTemperature;
	//--------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<AMonsterPatrolPoint*>	m_PatrolPointEditArray;

	TArray<FVector>					m_PatrolPointArray;

public:
	TArray<AMonsterPatrolPoint*>* GetPatrolPointEditArray()
	{
		return &m_PatrolPointEditArray;
	}

	EMonsterType GetMonsterType()	const
	{
		return m_eMonsterType;
	}

	int32 GetAttack()	const
	{
		return m_iAttack;
	}

	int32 GetArmor()	const
	{
		return m_iArmor;
	}

	int32 GetHP()	const
	{
		return m_iHP;
	}
	int32 GetHPMax()	const
	{
		return m_iHPMax;
	}
	int32 GetLevel()	const
	{
		return m_iLevel;
	}
	int32 GetExp()	const
	{
		return m_iExp;
	}
	int32 GetGold()	const
	{
		return m_iGold;
	}
	float GetMoveSpeed()	const
	{
		return m_fMoveSpeed;
	}
	float GetTraceRange()	const
	{
		return m_fTraceRange;
	}
	int32 GetMonsterNumber()	const
	{
		return m_iMonsterNumber;
	}
	FVector GetMonsterPos()	const
	{
		return m_vMonsterPos;
	}

	void SetMonsterNumber(int32 iMonsterNumber)
	{
		m_iMonsterNumber = iMonsterNumber;
	}
	void SetMove(bool bMove)
	{
		m_bMove = bMove;
	}
	void SetMoveDir(const FVector& vDir)
	{
		m_vMoveDir = vDir;

		FRotator rDir = vDir.ToOrientationRotator();
		SetActorRotation(rDir);	
	}
	void SetMonsterPos(const FVector& vPos)
	{
		m_vMonsterPos = vPos;
		 
		FVector	CurrentPos = GetActorLocation();
		CurrentPos.Z = m_vMonsterPos.Z;

		float Dist = FVector::Distance(m_vMonsterPos, CurrentPos);

		if (Dist > 175.f)
			SetActorLocation(m_vMonsterPos);
	}
	void SetHP(const int32 iHP);

	void Move();
	void MoveEnd();
	void Attack();
	void Skill_1();
	void Skill_2();
	void Death();
	void Hit(int32 iDamage);
	void Resurrection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void SaveInfo(FArchive* pArchive);

public:
	UFUNCTION()
	void BeginHPBarCallback();
};
