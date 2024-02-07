// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileNiagara.h"

// Sets default values
AProjectileNiagara::AProjectileNiagara()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));

	Particle->SetupAttachment(Body);
}

// Called when the game starts or when spawned
void AProjectileNiagara::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProjectileNiagara::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


