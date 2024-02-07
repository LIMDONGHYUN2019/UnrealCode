// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "SelectAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API USelectAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USelectAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool	bClick;

public:
	void CharacterClick(bool Click)
	{
		bClick = Click;
	}

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	UFUNCTION()
	void AnimNotify_SelectEnd();
};
