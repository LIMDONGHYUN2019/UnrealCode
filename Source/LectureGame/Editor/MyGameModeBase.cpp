// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "../Monster/Monster.h"
#include "../MainGameInstance.h"

AMyGameModeBase::AMyGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	m_bSave = true;
}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bSave)
	{
		m_bSave = false;

		// ���� ������ �����Ѵ�.
		UMainGameInstance* pGameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

		pGameInst->SaveMonsterInfo();

		FString	FullPath = FString::Printf(TEXT("%s%s"), *FPaths::ProjectSavedDir(), TEXT("WorldMonster.mif"));

		PrintViewport(5.f, FColor::Red, FullPath);

		FArchive* pWriter = IFileManager::Get().CreateFileWriter(*FullPath, 0);
		// WriteFlag�� ���߿� 
		UWorld* pWorld = GetWorld();

		TArray<AMonster*>	MonsterArray;

		for (TActorIterator<AMonster> iter(pWorld); iter; ++iter)
		{
			MonsterArray.Add(*iter);
		}

		int32	MonsterCount = MonsterArray.Num();
		*pWriter << MonsterCount;

		for (auto& pMonster : MonsterArray)
		{
			// ���͸� �̾ƿ����Ƿ� �� �������� ��Ƽ� ���Ϸ� ���ش�.
			int32	MonsterType = (int32)pMonster->GetMonsterType();

			FVector	vPos = pMonster->GetActorLocation();
			FVector	vScale = pMonster->GetActorScale3D();
			FRotator vRot = pMonster->GetActorRotation();

			*pWriter << MonsterType;
			*pWriter << vPos;
			*pWriter << vScale;
			*pWriter << vRot;
		}

		pWriter->Close();
		delete	pWriter;
	}
}
