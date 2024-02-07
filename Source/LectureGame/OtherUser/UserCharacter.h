// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "UserCharacter.generated.h"

UCLASS()
class LECTUREGAME_API AUserCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUserCharacter();

protected:
	class UHPBarWidget* HPBar;

	class UUserAnim*	m_pAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HPWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*>	m_AttackMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*>	m_SkillMontageArray;

	FString				m_UserName;
	int32				m_AttackIndex;
	bool				m_AttackEnable;
	MOVE_TYPE			m_eMoveType;
	ATTACK_TYPE			m_eAttackType;
	bool				m_bMove;
	bool				m_bDeath;
	bool				m_MoveKey;
	FVector				m_vOtherPos;
	FVector				m_vMoveDir;
	int32				m_iUserNumber;
	int32				m_CharacterNumber;
	_tagCharacterInfo	m_tCharacterInfo;

	bool				m_IsGroup;
	bool				m_IsGroupAdmin;

	class UAnimMontage* m_FallRecoveryMontage;

	float				m_fViewDir;

public:
	float GetViewDir()	const
	{
		return m_fViewDir;
	}

	void SetViewMoveSpeed(float MoveSpeed);
	void SetViewDir(float Dir);
	bool AddHP(int32 damage);
	void SetHP();

public:
	UFUNCTION()
		void CallbackHPBarOtherPlayer();
public:
	int32 GetUserNumber()	const
	{
		return m_iUserNumber;
	}
	int32 GetCharacterNumber()	const
	{
		return m_CharacterNumber;
	}
	FString GetUserName()	const
	{
		return m_UserName;
	}
	_tagCharacterInfo GetChracterInfo()	const
	{
		return m_tCharacterInfo;
	}
	void SetCharacterInfo(_tagCharacterInfo* pCharacterInfo)
	{
		FMemory::Memcpy(&m_tCharacterInfo, pCharacterInfo, sizeof(_tagCharacterInfo));

		GetCharacterMovement()->MaxWalkSpeed = m_tCharacterInfo.fMoveSpeed;
	}

public:
	void SetUserNumber(int32 iNumber)
	{
		m_iUserNumber = iNumber;
	}

	void SetCharacterNumber(int32 iCharacterNumber)
	{
		m_CharacterNumber = iCharacterNumber;
	}

	void SetMove(bool bMove)
	{
		m_bMove = bMove;
	}

	void SetMoveDir(const FVector& vDir)
	{
		m_vMoveDir = vDir;
	}

	void SetUserName(const char* pName)
	{
		m_UserName = ANSI_TO_TCHAR(pName);
	}

	void SetDeath(bool bDeath)
	{
		m_bDeath = bDeath;
	}

	void SetAttackEnable(bool Enable)
	{
		m_AttackEnable = Enable;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(FVector vPos);
	void MoveEnd();
	virtual void NormalAttack(int32 AttackIndex);
	virtual void SkillAttack1();
	virtual void SkillAttack2();
	void JumpKey();
	void PlayFallRecovery();
	void Hit();
	void Death();
};