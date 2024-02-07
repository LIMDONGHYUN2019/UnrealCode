#include "InventoryItemWidget.h"
#include "../../GameMode/LectureGameGameModeBase.h"
#include "../GameWidget.h"
#include "InventoryWidget.h"
#include "../Store/ItemData.h"

void UInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemIcon = Cast<UButton>(GetWidgetFromName(TEXT("Icon")));
	m_ItemImage = Cast<UBorder>(GetWidgetFromName(TEXT("Image")));

	m_ItemIcon->OnPressed.AddDynamic(this, &UInventoryItemWidget::ButtonDown);
	m_ItemIcon->OnReleased.AddDynamic(this, &UInventoryItemWidget::ButtonUp);


	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	m_pInventory = pGameMode->GetGameWidget()->GetInventoryWidget();
	m_pInventory->SetExist();
}

void UInventoryItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryItemWidget::SetData(UItemData* pData)
{
	m_Data = pData;

	if (!pData->GetIsEmpty())
	{
		UTexture2D* Test = pData->GetIconTexture();

		LOG(TEXT("Image Path : %s"), pData->GetIconTexture());

		SetIconImage(Test);
	}
}

void UInventoryItemWidget::ButtonDown()
{
	if (!m_Data->GetIsEmpty())
	{
		m_pInventory->SetPress();
		m_pInventory->SetSelectItemIconImage(m_Image);
		m_pInventory->SetSelectItemData(m_Data);
	}
}

void UInventoryItemWidget::ButtonUp()
{
	m_pInventory->SetRelease();
}
