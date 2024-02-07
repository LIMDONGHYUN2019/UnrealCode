// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectAnimInstance.h"

USelectAnimInstance::USelectAnimInstance()
{
	bClick = false;
}

void USelectAnimInstance::NativeInitializeAnimation()
{
	// Super : 부모클래스를 의미한다.
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
