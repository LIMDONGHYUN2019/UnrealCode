// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentActor.h"

// Sets default values
AEnvironmentActor::AEnvironmentActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshRoot"));

	SetRootComponent(Body);

	MeshRoot->SetupAttachment(Body);
	Mesh->SetupAttachment(MeshRoot);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CellWidth = 1;
	CellHeight = 1;
}

// Called when the game starts or when spawned
void AEnvironmentActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnvironmentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

