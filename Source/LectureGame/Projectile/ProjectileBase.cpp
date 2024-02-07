// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "../NetworkManager.h"
#include "../NetSession.h"
#include "../PacketSession.h"
#include "../NetStream.h"
#include "../GameServerManager.h"
#include "../Monster/Monster.h"
#include "../Player/PlayerCharacter.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));

	SetRootComponent(Body);
	Body->SetCollisionProfileName(TEXT("PlayerProjectile"));

	Projectile->SetUpdatedComponent(Body);
	Projectile->ProjectileGravityScale = 0.f;
	Projectile->OnProjectileStop.AddDynamic(this, &AProjectileBase::StopProjectile);
	//발사체가 멈출 때 호출됩니다.


	m_Dist = 1500.f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	m_PrevPos = GetActorLocation();
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector	Loc = GetActorLocation();

	float Dist = FVector::Distance(m_PrevPos, Loc);

	m_PrevPos = Loc;

	m_Dist -= Dist;

	if (m_Dist <= 0.f)
		Destroy();
}

void AProjectileBase::StopProjectile(const FHitResult& result)
{
	Destroy();
	// 부딪힌 물체에 데미지를 준다.

	NetStream	stream;
	uint8	packet[PACKET_LENGTH] = {};
	stream.SetBuffer(packet);

	AMonster* Monster = Cast<AMonster>(result.GetActor());
	//APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(result.GetActor());
	APlayerCharacter* PlayerChar = nullptr;

	if (IsValid(Monster))
	{
		PlayerChar = Cast<APlayerCharacter>(m_OwnerActor);

		int32	iAttackDamage = 0;

		if (IsValid(PlayerChar))
			iAttackDamage = PlayerChar->GetCharacterInfo().iAttack;

		int32 iMonsterNumber = Monster->GetMonsterNumber();

		stream.Write(&iMonsterNumber, sizeof(int32));
		stream.Write(&iAttackDamage, sizeof(int32));

		PacketSession* Session = GameServerManager::GetInst()->GetSession();
		Session->Write(GP_PLAYERATTACKDAMAGE, stream.GetLength(), packet);
	}

	else if (IsValid(PlayerChar))
	{
		//Monster = Cast<AMonster>(m_OwnerActor);

		//int32	iAttackDamage = 0;

		//if (IsValid(Monster))
		//	iAttackDamage = Monster->GetAttack();

		//int32 iCharacterNumber = PlayerChar->GetCharacterInfo().;

		//stream.Write(&iCharacterNumber, sizeof(int32));
		//stream.Write(&iAttackDamage, sizeof(int32));

		//PacketSession* Session = GameServerManager::GetInst()->GetSession();
		//Session->Write(GP_PLAYERATTACKDAMAGE, stream.GetLength(), packet);
	}

	
}
