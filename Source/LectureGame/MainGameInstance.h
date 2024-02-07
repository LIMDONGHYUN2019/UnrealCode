// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "Engine/GameInstance.h"
#include "Components/Image.h"
#include "MainGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FMonsterInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EMonsterType	eMonsterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iHPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iGold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			fMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			fTraceRange;	// 플레이어를 인식할 수 있는 반경

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			fAttackRange;	// 플레이어를 공격하기 위한 거리
};

//-----------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FIconPathInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	Path;
};

USTRUCT(BlueprintType)
struct FIconInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTexture2D*	Texture;
};

//-----------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FSkillIconData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* Image;
};

USTRUCT(BlueprintType)
struct FSkillIconJobData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EJob	Job;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FSkillIconData>	IconDataArray;
};

USTRUCT(BlueprintType)
struct FSkillIconImage : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSkillIconJobData	IconJobArray;
};

//-----------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FSkillAnimationInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Animation;
};

//-----------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FJobLevelStatus : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iHPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			iMPMax;
};

//-----------------------------------------------------------------------------------

UCLASS()
class LECTUREGAME_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMainGameInstance();
	~UMainGameInstance();

private:
	_tagCharacterInfo m_tCharacterInfo;
	bool		m_CharacterInfoSet;

	UPROPERTY()
		UDataTable* m_MonsterInfo;

	UPROPERTY()
		UDataTable* m_IconPathInfo;

	UPROPERTY()
		UDataTable* m_IconInfo;

	UPROPERTY()
		UDataTable* m_SkillIconTable;

	UPROPERTY()
		UDataTable* m_GreystoneAnimTable;
	UPROPERTY()
		UDataTable* m_TwinblastAnimTable;
	UPROPERTY()
		UDataTable* m_SerathAnimTable;

	UPROPERTY()
		UDataTable* m_GreystoneLevelInfo;
	UPROPERTY()
		UDataTable* m_TwinblastLevelInfo;
	UPROPERTY()
		UDataTable* m_SerathLevelInfo;

public:
	void SetCharacterInfo(const _tagCharacterInfo& Info)
	{
		m_CharacterInfoSet = true;
		m_tCharacterInfo = Info;
	}

	_tagCharacterInfo GetCharacterInfo()
	{
		return m_tCharacterInfo;
	}

	bool GetCharacterSet()
	{
		return m_CharacterInfoSet;
	}

	void ClearCharacterSet()
	{
		m_CharacterInfoSet = false;
	}

public:
	virtual void Init();

public:
	void SaveMonsterInfo();

	const FMonsterInfo* GetMonsterInfo(const FString& Name);

	const FIconPathInfo* GetIconPath(const FString& Name);

	const FIconInfo* GetIconImagePath(const FString& Name);

	const FSkillIconImage* GetSkillIcon(const FString& Name);

	const FSkillAnimationInfo* GetGreystoneAnim(const FString& Name);

	const FSkillAnimationInfo* GetTwinblastAnim(const FString& Name);

	const FSkillAnimationInfo* GetSerathAnim(const FString& Name);

	const FJobLevelStatus* GetGreystoneLevelInfo(const FString& Name);

	const FJobLevelStatus* GetTwinblastLevelInfo(const FString& Name);

	const FJobLevelStatus* GetSerathLevelInfo(const FString& Name);

public:
	UFUNCTION(BlueprintCallable, Category = "Socket")
	bool ConnectMainServer();

	//UFUNCTION(BlueprintCallable)
	//static void SaveEnvironment(const FString& FileName, const TArray<AActor*>& AllActors, FString& CountH, FString& CountV, FString& CellWidth, FString& CellHeight);

	UFUNCTION(BlueprintCallable)
	static void SaveEnvironment(const FString& FileName, FString NavName, const TArray<AActor*>& AllActors, int CountH, int CountV, float CellWidth, float CellHeight,
		FVector Min, FVector Max);

	UFUNCTION(BlueprintCallable)
	static void SaveMonster(const FString& FileName, FString NavName, const TArray<AActor*>& AllActors);
};
