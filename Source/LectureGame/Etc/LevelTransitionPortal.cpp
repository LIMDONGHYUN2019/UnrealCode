// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionPortal.h"
#include "../GameMode/LectureGameGameModeBase.h"
#include "../UI/GameWidget.h"
#include "../UI/MainPortalWidget.h"

// Sets default values
ALevelTransitionPortal::ALevelTransitionPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	SetRootComponent(m_Body);

	m_Particle->SetupAttachment(m_Body);

	m_Body->SetCollisionProfileName(TEXT("PlayerTrigger"));
}

// Called when the game starts or when spawned
void ALevelTransitionPortal::BeginPlay()
{
	Super::BeginPlay();

	m_Body->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionPortal::PortalBeginOverlap);
	m_Body->OnComponentEndOverlap.AddDynamic(this, &ALevelTransitionPortal::PortalEndOverlap);
}

// Called every frame
void ALevelTransitionPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelTransitionPortal::OpenMainPortal()
{
	//UGameplayStatics::OpenLevel(GetWorld(), m_LevelName);
}

void ALevelTransitionPortal::PortalBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	if (pGameMode)
	{
		UMainPortalWidget* pMainPortal = pGameMode->GetGameWidget()->GetMainPortalWidget();

		pMainPortal->SetLevelType(m_LevelType);

		pGameMode->GetGameWidget()->OpenMainPortalWidget();

		// 여기서 다음 맵 타입을 이름으로 가지고 있고 여기서 위젯을 열어줄떄 그 이름별로
		// 타입을 전달해주자?
	}
}

void ALevelTransitionPortal::PortalEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


