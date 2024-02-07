// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePaticle.h"

AProjectilePaticle::AProjectilePaticle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(RootComponent);
}

void AProjectilePaticle::BeginPlay()
{
}

void AProjectilePaticle::Tick(float DeltaTime)
{
}
