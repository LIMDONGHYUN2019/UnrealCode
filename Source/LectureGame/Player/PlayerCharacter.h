// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include"../MainGameInstance.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class LECTUREGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// �ʿ��� ������Ʈ�� �����
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* Arm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HPWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*>	m_AttackMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*>	m_SkillMontageArray;

	class UPlayerAnim* m_Anim;

	class UHPBarWidget* HPBar;

	int32		m_AttackIndex;
	int32		m_AttackSendIndex;
	bool		m_AttackEnable;
	bool		m_SkillEnable;
	// Player�� SlotWidget�ּҸ� �������ִٸ� CallBack�Լ��� ����ϰ�(�����ϰ� �÷��̾ ������ ������ �����Ҽ� ������)
	// ��� Slot�� ��������  GameWidget�� �޾ƿ��ʿ����.
	// 
	// Player�� SlotWidget�ּҸ� �Ȱ������ִٸ� SkillEnable�� ������ ������ ���������� ����� �����ؾ���.

	UAnimMontage* m_FallRecoveryMontage;

	bool		bMove;
	float		fMovePacketTime;
	float		fMovePacketTimeAcc;
	MOVE_TYPE	m_eMoveType;
	ATTACK_TYPE m_eAttackType;
	float		m_fMoveTimeCheck;
	float		m_fMoveDistanceCheck;

	_tagCharacterInfo tCharacterInfo;
	TArray<SkillInfo*>	m_SkillInfo; // ��ü ��ų ����

	FVector		m_vPrevPos;
	bool		m_IsGroup;
	bool		m_IsGroupAdmin;

	float	m_MonsterDetectTime;
	bool	m_MoveKey;

	bool	m_bDeath;

	FVector	m_PrevMoveDir;
	bool	m_ChangeMove;
	float	m_ChangeMoveDirTime;
	float	m_ChangeMoveDirTimeMax;

public:
	void AddSkillInfo(SkillInfo* Info)
	{
		m_SkillInfo.Add(Info);
	}

public: // �� �÷��̾��� ���� ���� �Լ���
	int32 GetGold()	const
	{
		return 	tCharacterInfo.iMoney;
	}
	int32 GetSkillPoint()	const
	{
		return tCharacterInfo.iSkillPoint;
	}
	_tagCharacterInfo GetCharacterInfo()	const
	{
		return tCharacterInfo;
	}
	bool GetIsGroup()
	{
		return m_IsGroup;
	}
	bool IsGroupAdmin()
	{
		return m_IsGroupAdmin;
	}
	// ========================================================
	void SetGold(int32 gold)
	{
		tCharacterInfo.iMoney += gold;
	}
	void SetExpMax(int32 ExpMax)
	{
		tCharacterInfo.iExpMax = ExpMax;
	}
	int32 AddExp(int32 Exp)
	{
		tCharacterInfo.iExp += Exp;

		if ( tCharacterInfo.iExp >= tCharacterInfo.iExpMax )
		{
			tCharacterInfo.iExp -= tCharacterInfo.iExpMax;
			tCharacterInfo.iLevel += 1;
		}
		else
		{
			return 0;
		}

		return tCharacterInfo.iLevel;
	}
	void SkillPointUp(int32 Point)
	{
		tCharacterInfo.iSkillPoint += Point;
	}
	void SetIsGroup(bool group)
	{
		m_IsGroup = group;
	}
	void SetIsGroupAdmin(bool Admin)
	{
		m_IsGroupAdmin = Admin;
	}
	void SetDeath(bool death)
	{
		m_bDeath = death;
	}
	void SetAttackEnable(bool enable)
	{
		m_AttackEnable = enable;
	}
	void SetSkillEnable(bool enable)
	{
		m_SkillEnable = enable;
	}
	void SetSkillPoint(int32 Point)
	{
		tCharacterInfo.iSkillPoint = Point;
	}
	void SetLevelUp(const FJobLevelStatus* Level)
	{
		tCharacterInfo.iLevel = Level->iLevel;
		tCharacterInfo.iExpMax = Level->iExp;
		tCharacterInfo.iAttack = Level->iAttack;
		tCharacterInfo.iArmor = Level->iArmor;
		tCharacterInfo.iHPMax = Level->iHPMax;
		tCharacterInfo.iMPMax = Level->iMPMax;
		tCharacterInfo.iSkillPoint++;

	}
	void SetCharacterInfo(_tagCharacterInfo* pCharacterInfo);

	bool AddHP(int32 damage);
	bool AddMP(int32 mana);
	void SetHP();
	void SetMP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void MoveFront(float fScale);
	void MoveSide(float fScale);

public:
	UFUNCTION()
		void CallbackHPBarPlayer();
public:
	virtual void NormalAttack();
	virtual void SkillAttack1();
	virtual void SkillAttack2();
	virtual void SkillAttack3();
	virtual void SkillAttack4();
	virtual void Levelup();
	virtual void PotionEffect();
	virtual void NormalAttackHitEffect();
	virtual void Item1();
	virtual void Item2();
	virtual void Turn(float NewAxisValue);
	virtual void LookUp(float NewAxisValue);
	virtual void Hit();
	virtual void Death();
	virtual void AttackEnd();
	virtual void AttackDamage();
	virtual void Skill1Damage();
	virtual void Skill2Damage();
	virtual void Skill3Damage();
	virtual void Skill4Damage();

	void JumpKey();
	void PlayFallRecovery();

	void Skill1Key();
	void Skill2Key();
	void Skill3Key();
	void Skill4Key();
	void Item1Key();
	void Item2Key();
};
