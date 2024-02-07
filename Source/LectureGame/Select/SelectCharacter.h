// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../GameHeader.h"
#include "GameFramework/Character.h"
#include "SelectCharacter.generated.h"

UENUM(BlueprintType)
enum class ESelectCharType : uint8
{
	Default,
	Create,
	Delete
};


UCLASS()
class LECTUREGAME_API ASelectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASelectCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USelectAnimInstance* SelectAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ESelectCharType	eCharType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ASelectCharacter>	ClassType;

	int32		iCharacterNumber;
	int32		iUserNumber;
	int32		iMapLevel;
	//------------------------------------
	char		ServerCharacterName[USERNAME_SIZE];
	char		ServerJob[JOBNAME_SIZE];
	FString		CharacterName;
	FString		CharacterJob;
	int32		iAttack;
	int32		iArmor;
	float		fAttackSpeed;
	float		fMoveSpeed;
	float		fCritical;
	float		fCriticalDamage;
	int32		iHP;
	int32		iHPMax;
	int32		iMP;
	int32		iMPMax;
	int32		iLevel;
	int32		iMoney;
public:
	ESelectCharType GetSelectCharacterType()	const
	{
		return eCharType;
	}

	int32 GetCharacterNumber()	const
	{
		return iCharacterNumber;
	}

	int32 GetUserNumber()	const
	{
		return iUserNumber;
	}

	int32 GetLevelType()	const
	{
		return iMapLevel;
	}

	const char* GetServerCharacterName()	const
	{
		return ServerCharacterName;
	}

	const char* GetServerJob()	const
	{
		return ServerJob;
	}

	FString GetCharName()	const
	{
		return CharacterName;
	}

	FString GetCharJob()	const
	{
		return CharacterJob;
	}

	int32 GetAttack()	const
	{
		return iAttack;
	}

	int32 GetArmor()	const
	{
		return iArmor;
	}

	float GetAttackSpeed()	const
	{
		return fAttackSpeed;
	}

	float GetMoveSpeed()	const
	{
		return fMoveSpeed;
	}

	float GetCritical()	const
	{
		return fCritical;
	}

	float GetCriticalDamage()	const
	{
		return fCriticalDamage;
	}

	int32 GetHP()	const
	{
		return iHP;
	}

	int32 GetHPMax()	const
	{
		return iHPMax;
	}

	int32 GetMP()	const
	{
		return iMP;
	}

	int32 GetMPMax()	const
	{
		return iMPMax;
	}

	int32 GetLevel()	const
	{
		return iLevel;
	}

	int32 GetMoney()	const
	{
		return iMoney;
	}

	//===========================================================

public:
	void CharacterClick(bool bClick);
	void SetCharacterInfo(const _tagCharacterInfoTable* pInfo);
	void SetCharacterInfo(const _tagCharacterInfo* pInfo);

	void SetCharacterNumber(int32 iNumber)
	{
		iCharacterNumber = iNumber;
	}

	void SetUserNumber(int32 iNumber)
	{
		iUserNumber = iNumber;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	ASelectCharacter* Clone(const FVector& vLoc, const FRotator& vRot);
};
