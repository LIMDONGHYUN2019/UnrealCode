// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageText.h"
#include "Components/WidgetComponent.h"
#include "../UI/DamageTextWidget.h"

// Sets default values
ADamageText::ADamageText()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageTextWidgetCom = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageTextWidget"));

	SetRootComponent(DamageTextWidgetCom);

	static ConstructorHelpers::FClassFinder<UDamageTextWidget>	DamageTextWidgetClass(TEXT("WidgetBlueprint'/Game/UI/UI_DamageText.UI_DamageText_C'"));

	if (DamageTextWidgetClass.Succeeded())
		DamageTextWidgetCom->SetWidgetClass(DamageTextWidgetClass.Class);

	DamageTextWidgetCom->SetWidgetSpace(EWidgetSpace::Screen);
	DamageTextWidgetCom->SetDrawSize(FVector2D(200.f, 50.f));
}

void ADamageText::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(1.5f);

	DamageTextWidget = Cast<UDamageTextWidget>(DamageTextWidgetCom->GetWidget());

	m_SetDamage = false;
}

// Called every frame
void ADamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DamageTextWidgetCom->AddRelativeLocation(FVector(-150.f * DeltaTime, 0.f, 200.f * DeltaTime));

	if (DamageTextWidget->IsStart() && !m_SetDamage)
	{
		m_SetDamage = true;
		DamageTextWidget->SetDamageText(m_Damage);
	}
}

void ADamageText::SetDamage(int32 Damage)
{
	m_Damage = Damage;
}
