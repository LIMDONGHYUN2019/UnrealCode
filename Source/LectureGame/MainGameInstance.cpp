// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "RecvThread.h"
#include "NetSession.h"
#include "NetworkManager.h"
#include "Etc/EnvironmentActor.h"
#include "Monster/Monster.h"

UMainGameInstance::UMainGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>	MonsterInfoAsset(TEXT("DataTable'/Game/Monster/MonsterInfoTable.MonsterInfoTable'"));
	if (MonsterInfoAsset.Succeeded())
		m_MonsterInfo = MonsterInfoAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	IconPathAsset(TEXT("DataTable'/Game/UI/DTIconPath.DTIconPath'"));
	if (IconPathAsset.Succeeded())
		m_IconPathInfo = IconPathAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	IconAsset(TEXT("DataTable'/Game/UI/DTIcon.DTIcon'"));
	if (IconAsset.Succeeded())
		m_IconInfo = IconAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	SkillIconAsset(TEXT("DataTable'/Game/UI/DTSkillIcon.DTSkillIcon'"));
	if (SkillIconAsset.Succeeded())
		m_SkillIconTable = SkillIconAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	GreystoneAnimAsset(TEXT("DataTable'/Game/Player/Greystone/DTGreystoneAnim.DTGreystoneAnim'"));
	if (GreystoneAnimAsset.Succeeded())
		m_GreystoneAnimTable = GreystoneAnimAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	TwinblastAnimAsset(TEXT("DataTable'/Game/Player/Twinblast/DTTwinblastAnim.DTTwinblastAnim'"));
	if (TwinblastAnimAsset.Succeeded())
		m_TwinblastAnimTable = TwinblastAnimAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	SerathAnimAsset(TEXT("DataTable'/Game/Player/Serath/DTSerathAnim.DTSerathAnim'"));
	if (SerathAnimAsset.Succeeded())
		m_SerathAnimTable = SerathAnimAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	GreystoneLevelAsset(TEXT("DataTable'/Game/Player/Greystone/DTGreystoneLevel.DTGreystoneLevel'"));
	if (GreystoneLevelAsset.Succeeded())
		m_GreystoneLevelInfo = GreystoneLevelAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	TwinblastLevelAsset(TEXT("DataTable'/Game/Player/Serath/DTSerathLevel.DTSerathLevel'"));
	if (TwinblastLevelAsset.Succeeded())
		m_TwinblastLevelInfo = TwinblastLevelAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	SerathLevelAsset(TEXT("DataTable'/Game/Player/Twinblast/DTTwinblastLevel.DTTwinblastLevel'"));
	if (SerathLevelAsset.Succeeded())
		m_SerathLevelInfo = SerathLevelAsset.Object;

	m_CharacterInfoSet = false;
}

UMainGameInstance::~UMainGameInstance()
{
	NetworkManager::DestroyInst();
}

void UMainGameInstance::Init()
{
	Super::Init();
}

bool UMainGameInstance::ConnectMainServer()
{
	return NetworkManager::GetInst()->Init();
}

void UMainGameInstance::SaveMonsterInfo()
{
	TArray<FMonsterInfo*>	Array;
	m_MonsterInfo->GetAllRows<FMonsterInfo>(TEXT(""), Array);

	FString	FullPath = FString::Printf(TEXT("%s%s"), *FPaths::ProjectSavedDir(), TEXT("MonsterInfo.mif"));

	PrintViewport(5.f, FColor::Red, FullPath);

	FArchive* pWriter = IFileManager::Get().CreateFileWriter(*FullPath, 0);

	int32	MonsterCount = Array.Num(); 

	PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("Count : %d"), MonsterCount));
	*pWriter << MonsterCount; // ���� ����

	for (auto& pInfo : Array)
	{
		*pWriter << pInfo->Name;//Minion
		int32	MonsterType = (int32)pInfo->eMonsterType;
		*pWriter << MonsterType;//0
		*pWriter << pInfo->iAttack;
		*pWriter << pInfo->iArmor;
		*pWriter << pInfo->iHPMax;
		*pWriter << pInfo->iLevel;
		*pWriter << pInfo->iExp;
		*pWriter << pInfo->iGold;
		*pWriter << pInfo->fMoveSpeed;
		*pWriter << pInfo->fTraceRange;
		*pWriter << pInfo->fAttackRange;
	}

	pWriter->Close();
	delete	pWriter;
}

const FMonsterInfo* UMainGameInstance::GetMonsterInfo(const FString& Name)
{
	return m_MonsterInfo->FindRow<FMonsterInfo>(*Name, TEXT(""));
}

const FIconPathInfo* UMainGameInstance::GetIconPath(const FString& Name)
{
	return m_IconPathInfo->FindRow<FIconPathInfo>(*Name, TEXT(""));
}

const FIconInfo* UMainGameInstance::GetIconImagePath(const FString& Name)
{
	return m_IconInfo->FindRow<FIconInfo>(*Name, TEXT(""));
}

const FSkillIconImage* UMainGameInstance::GetSkillIcon(const FString& Name)
{ 
	return m_SkillIconTable->FindRow<FSkillIconImage>(*Name, TEXT(""));
}

const FSkillAnimationInfo* UMainGameInstance::GetGreystoneAnim(const FString& Name)
{
	return m_GreystoneAnimTable->FindRow<FSkillAnimationInfo>(*Name, TEXT(""));
}

const FSkillAnimationInfo* UMainGameInstance::GetTwinblastAnim(const FString& Name)
{
	return m_TwinblastAnimTable->FindRow<FSkillAnimationInfo>(*Name, TEXT(""));
}

const FSkillAnimationInfo* UMainGameInstance::GetSerathAnim(const FString& Name)
{
	return m_SerathAnimTable->FindRow<FSkillAnimationInfo>(*Name, TEXT(""));
}

const FJobLevelStatus* UMainGameInstance::GetGreystoneLevelInfo(const FString& Name)
{
	return m_GreystoneLevelInfo->FindRow<FJobLevelStatus>(*Name, TEXT(""));
}

const FJobLevelStatus* UMainGameInstance::GetTwinblastLevelInfo(const FString& Name)
{
	return m_TwinblastLevelInfo->FindRow<FJobLevelStatus>(*Name, TEXT(""));
}

const FJobLevelStatus* UMainGameInstance::GetSerathLevelInfo(const FString& Name)
{
	return m_SerathLevelInfo->FindRow<FJobLevelStatus>(*Name, TEXT(""));
}

void UMainGameInstance::SaveEnvironment(const FString& FileName, FString NavName, const TArray<AActor*>& AllActors,
	int Height, int Width, float CellWidth, float CellHeight, FVector Min, FVector Max)
{
	FString	FullPath = FString::Printf(TEXT("%s%s%s"), *FPaths::ProjectContentDir(), *FileName, TEXT(".nav"));
	FArchive* pWriter = IFileManager::Get().CreateFileWriter(*FullPath, 0);
	LOG(TEXT("Envirment Save File : %s"), *FullPath);
	int32	NameCount = NavName.Len();

	*pWriter << NameCount;
	*pWriter << NavName;
	int iCountV = Height / CellHeight;
	int iCountH = Width / CellWidth;
	// �׳� �� ��ü ���μ��� ũ�� �� ���� ���ϰ� ���� ���� ���μ��� ũ�⸸ �Է� �޾Ƽ� ������ ���� �߽��ϴ�.

	TArray<AEnvironmentActor*>	EnvironmentArray;
	for (auto& result : AllActors)
	{
		AEnvironmentActor* Environment = Cast<AEnvironmentActor>(result);

		if (Environment)
			EnvironmentArray.Add(Environment);
	}
	int32	Count = EnvironmentArray.Num();
	*pWriter << Count;
	*pWriter << iCountH;
	*pWriter << iCountV;
	*pWriter << CellWidth;
	*pWriter << CellHeight;
	*pWriter << Min;
	*pWriter << Max;
	float	fWidth = CellWidth * iCountH;
	float	fHeight = CellHeight * iCountV;
	float	fHalfWidth = Width / 2.f;
	float	fHalfHeight = Height / 2.f;
	int32 iIndexWidth;
	int32 iIndexHeight;
	int32	iCellWidth;
	int32	iCellHeight;

	for (auto& result : EnvironmentArray)
	{
		// x, y ���� 107000 �� ���ؼ� 0, 0 �������� ������ش�.
		FVector	Pos = result->GetActorLocation();

		// ���� ū ���� �����Ƿ� �������־ 0���� ������ ���ش�.
		Pos.X -= Min.X;
		Pos.X = Height - Pos.X;

		Pos.Y -= Min.Y;

		iIndexHeight = Pos.X / CellHeight;
		iIndexWidth = Pos.Y / CellWidth;

		iCellWidth = result->GetCellWidth();
		iCellHeight = result->GetCellHeight();

		// ���õ� �ε����� �߽����� ����, ���� ���� ������ �������� ������ �翷, ���Ʒ� ������ ������ ����
		// �����Ѵ�.
		// �� ������ ���Ͽ� �����ؾ� �Ѵ�.
		*pWriter << iIndexWidth;
		*pWriter << iIndexHeight;
	}

	pWriter->Close();
	delete	pWriter;

	//LOG(TEXT("SaveMonster Test : %d"), AllActors.Num());

	//FString	Path = FPaths::ProjectContentDir();

	//LOG(TEXT("%s"), *Path);

	//Path += FileName;

	//char* pFullPath = TCHAR_TO_ANSI(*Path);
	//

	////int	Length = WideCharToMultiByte

	//FILE* File = nullptr;

	//fopen_s(&File, pFullPath, "wb");

	//if (File)
	//{
	//	LOG(TEXT("Create File"));

	//	TArray<AEnvironmentActor*>	EnvironmentArray;

	//	for (auto& result : AllActors)
	//	{
	//		AEnvironmentActor* Environment = Cast<AEnvironmentActor>(result);

	//		if (Environment)
	//			EnvironmentArray.Add(Environment);
	//	}

	//	int32	Count = EnvironmentArray.Num();

	//	fwrite(&Count, sizeof(int32), 1, File);

	//	for (auto& result : EnvironmentArray)
	//	{
	//		// x, y ���� 107000 �� ���ؼ� 0, 0 �������� ������ش�.
	//		FVector	Pos = result->GetActorLocation();

	//		Pos.X += 107000.f;

	//		// ���� ū ���� �����Ƿ� �������־ 0���� ������ ���ش�.
	//		Pos.X = 214000.f - Pos.X;

	//		Pos.Y += 107000.f;

	//		int32 IndexWidth = Pos.Y / 1000;
	//		int32 IndexHeight = Pos.X / 1000;

	//		int32	CellWidth = result->GetCellWidth();
	//		int32	CellHeight = result->GetCellHeight();

	//		// ���õ� �ε����� �߽����� ����, ���� ���� ������ �������� ������ �翷, ���Ʒ� ������ ������ ����
	//		// �����Ѵ�.
	//		// �� ������ ���Ͽ� �����ؾ� �Ѵ�.
	//		fwrite(&IndexWidth, sizeof(int32), 1, File);
	//		fwrite(&IndexHeight, sizeof(int32), 1, File);
	//	}


	//	fclose(File);
	//}

	//else
	//	LOG(TEXT("Failed"));
}

void UMainGameInstance::SaveMonster(const FString& FileName, FString NavName, const TArray<AActor*>& AllActors)
{

	//FString Path = FPaths::ProjectContentDir();
	////FString Path = FPaths::ProjectSavedDir();
	//LOG(TEXT("%s"), *Path);
	//Path += FileName;
	//char* pFullPath = TCHAR_TO_ANSI(*Path);

	//FILE* pFile = nullptr;
	//fopen_s(&pFile, pFullPath, "wb");
	//if (pFile)
	//{
	//	
	//}
	//else
	//	LOG(TEXT("Monster Save Failed"));

	FString	FullPath = FString::Printf(TEXT("%s%s%s"), *FPaths::ProjectContentDir(), *FileName, TEXT(".mif"));
	//FString	FullPath = FString::Printf(TEXT("%s%s"), *FPaths::ProjectContentDir(), FileName);
	FArchive* pWriter = IFileManager::Get().CreateFileWriter(*FullPath, 0);
	LOG(TEXT("Monster Save File : %s"), *FullPath);

    //WriteFlag�� ���߿�
	//UWorld* pWorld = GetWorld();
	//
	//TArray<AMonster*>	MonsterArray;
	//
	//for (TActorIterator<AMonster> iter(pWorld); iter; ++iter)
	//{
	//	MonsterArray.Add(*iter);
	//}

	int32	NameCount = NavName.Len();

	*pWriter << NameCount;
	*pWriter << NavName;
	//string�� �����ҋ� �տ� ������ �ڵ����� �־��ִ°�

	TArray<AMonster*>	MonsterArray;
	for (auto& result : AllActors)
	{
		AMonster* pMonster = Cast<AMonster>(result);

		if (pMonster)
			MonsterArray.Add(pMonster);
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

		// ���� ��ġ ���� ����.
		TArray<AMonsterPatrolPoint*>* PatrolPointArray = pMonster->GetPatrolPointEditArray();

		int32	PatrolPointCount = PatrolPointArray->Num();
		*pWriter << PatrolPointCount;

		if (PatrolPointCount > 0)
		{
			for (auto& PatrolPoint : *PatrolPointArray)
			{
				FVector	PatrolPointPos = PatrolPoint->GetActorLocation();

				*pWriter << PatrolPointPos;
			}
		}
	}
	
	pWriter->Close();
	delete	pWriter;
}