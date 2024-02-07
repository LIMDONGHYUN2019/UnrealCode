// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../GameMode/LectureGameGameModeBase.h"
#include "GameWidget.h"
#include"../UI/GameWidget.h"
#include"../UI/Inventory/InventoryWidget.h"
#include "../Chatting/ChattingWidget.h"

// Sets default values
AItemBox::AItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
    Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

    RootComponent = Trigger;
    Box->SetupAttachment(RootComponent);

    Trigger->SetBoxExtent(FVector(70.0f, 70.0f, 70.0f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BOX(TEXT("StaticMesh'/Game/Monster/ItemBox.ItemBox'"));
    if (BOX.Succeeded())
    {
        Box->SetStaticMesh(BOX.Object);
    }
    
    Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

    Trigger->SetCollisionProfileName(TEXT("ItemBox"));
    Box->SetCollisionProfileName(TEXT("NoCollision"));

    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnCharacterOverlap);

     // 박스 크기 130 정육면체

    bClick = false;
}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APlayerCharacter* pPlayer = Cast<APlayerCharacter>(OtherActor);
    //AMonster* pMonster = Cast<AMonster>(m_OwnerActor);

    if (IsValid(pPlayer))
    {
        ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
        UGameWidget* pGameWidget = pGameMode->GetGameWidget();
        UInventoryWidget* pInventoryWidget = pGameWidget->GetInventoryWidget();

        if (pInventoryWidget)
        {
            for (int32 i = 0; i < DropItemList.Num(); ++i)
            {
                FString ItemName = DropItemList[i].strName;

                pInventoryWidget->AddItem(ItemName, DropItemList[i].Grade, DropItemList[i].Type, DropItemList[i].Buy, DropItemList[i].Sell, DropItemList[i].tOption);

                pGameWidget->GetChatWidget()->AddChatting(ItemName + TEXT("을 획득했습니다."));
            }
        }
    }

   // 해당 아이템 상자에 아이템들을 가지고 있게 할까?
   // 아니면 지금 처럼 겹치는 상황이 생기면 패킷으로 보내고 아이템들을 서버에서 받아올까?

   /*
     아마 죽을때 템 드랍이 결정되지 않나요 ?
     그래야 아이템 떨굼 2배 템 먹고 잡아서 2배로 먹기 가능한거 아닌가 ?
     오히려 미리 템 줘놓고 넌 잡으면 이거 떨어짐
     이게 사양 더 많이 잡아먹을거 같은데
     마치 이건 유저들 인벤토리가 돌아다니는걸아 똑같은 격이라서
   */

    // 몬스터가 죽으면 상자에 아이템을 넣어둘까?
    // 일단 빈 상자를 생성하고 집으면 그떄 서버에서 유저에게 아이템들을 전해줄까?

    Destroy();
}

