// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectAnimInstance.h"

USelectAnimInstance::USelectAnimInstance()
{
	bClick = false;
}

void USelectAnimInstance::NativeInitializeAnimation()
{
	// Super : �θ�Ŭ������ �ǹ��Ѵ�.
	Super::NativeInitializeAnimation();
}

void USelectAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void USelectAnimInstance::AnimNotify_SelectEnd()
{
	bClick = false;
}
